


#include "GameMode/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "GameMode/AuraGameInstance.h"
#include "GameMode/LoadScreenSaveGame.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "MyAura/AuraLogChannels.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

class UAuraGameInstance;

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName,SlotIndex);
	}
}


void AAuraGameModeBase::SaveSlotDate(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	//如果存档存在则覆盖
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->SlotName,SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->SlotName,SlotIndex);
	}
	
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->SlotStatus = Taken;
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->SlotName, SlotIndex);
	
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotDate(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
	
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->SlotName;
	const int32 SlotIndex = Slot->SlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot,Maps.FindChecked(Slot->GetMapName()));
}

ULoadScreenSaveGame* AAuraGameModeBase::GetLoadSlotDate() const
{
	const UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	
	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	
	return GetSaveSlotDate(InGameLoadSlotName, InGameLoadSlotIndex);
	
}

void AAuraGameModeBase::SaveGameProgressDate(ULoadScreenSaveGame* SaveGame)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	
	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	AuraGameInstance->PlayerStartTag = SaveGame->PlayerStartTag; 
	
	UGameplayStatics::SaveGameToSlot(SaveGame,InGameLoadSlotName,InGameLoadSlotIndex);
}


void AAuraGameModeBase::SaveWorldState(UWorld* World ,const FString& InNextMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotDate(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		//该函数只会保存当前地图，这里的Save->MapName是为了方便下次登录游戏时候导航到正确的地图。
		if (InNextMapAssetName!=FString(""))
		{
			SaveGame->MapAssetName = InNextMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(InNextMapAssetName);
		}
		
		//如果没保存该地图
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}

		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty(); // clear it out, we'll fill it in with "actors"

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			//保存实现SaveActor接口的Actor
			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			//序列化需要保存的变量
			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);

			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
	}
}


void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	//是否存在该存档
	if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{

		//获取存档内容
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogAura, Error, TEXT("Failed to load slot"));
			return;
		}
		
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					//反向序列化保存的变量
					FMemoryReader MemoryReader(SavedActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive); // converts binary bytes back into variables

					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}


AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),OutActors);
	if (OutActors.Num() > 0)
	{
		AActor* SelectActor = OutActors[0];
		for (AActor* Actor : OutActors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectActor=PlayerStart;
					break;
				}
			}
		}
		return SelectActor;
	}
	return nullptr;
	
}

void AAuraGameModeBase::PlayerDied(ACharacter* Player)
{
	ULoadScreenSaveGame* SaveGame = GetLoadSlotDate();
	if (!IsValid(SaveGame)) return;
	
	//回溯到最近保存的地图
	 UGameplayStatics::OpenLevel(Player,FName(SaveGame->MapAssetName));
}
void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName,DefaultMap);
}
