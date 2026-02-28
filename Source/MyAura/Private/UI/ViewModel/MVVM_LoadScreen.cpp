


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "GameMode/AuraGameInstance.h"
#include "GameMode/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializedLoadSLots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_0->SlotName = FString("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0,LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_1->SlotName = FString("LoadSlot_1");
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1,LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this,LoadSlotViewModelClass);
	LoadSlot_2->SlotName = FString("LoadSlot_2");
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2,LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotNameButtonPressed(int32 SlotIndex, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameModeBase)) return;    //只有在服务端才能创建角色
	
	LoadSlots[SlotIndex]->SetMapName(AuraGameModeBase->DefaultMapName);
	LoadSlots[SlotIndex]->SetPlayerName(EnteredName);
	LoadSlots[SlotIndex]->SetPlayerLevel(1);
	LoadSlots[SlotIndex]->SlotStatus = Taken;
	LoadSlots[SlotIndex]->PlayerStartTag = AuraGameModeBase->DefaultPlayerStartTag;;
	LoadSlots[SlotIndex]->MapAssetName = AuraGameModeBase->DefaultMap.ToSoftObjectPath().GetAssetName();
	
	AuraGameModeBase->SaveSlotDate(LoadSlots[SlotIndex],SlotIndex);
	LoadSlots[SlotIndex]->InitializeSlot();
	
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameModeBase->GetGameInstance());
	AuraGameInstance->LoadSlotName = LoadSlots[SlotIndex]->SlotName;
	AuraGameInstance->LoadSlotIndex = LoadSlots[SlotIndex]->SlotIndex;
	AuraGameInstance->PlayerStartTag = LoadSlots[SlotIndex]->PlayerStartTag;

}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 SlotIndex)
{
	LoadSlots[SlotIndex]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 SlotIndex)
{
	SlotSelected.Broadcast();
	for (const TTuple<int32,UMVVM_LoadSlot*> Pair : LoadSlots)
	{
		//仅禁用已经点击的Button，启用其他Button
		if (Pair.Key == SlotIndex)
		{
			Pair.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			Pair.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	//保存当前选择的slotVM指针
	SelectSlot = LoadSlots[SlotIndex];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectSlot->SlotName,SelectSlot->SlotIndex);
		SelectSlot->SlotStatus = Vacant;
		SelectSlot->InitializeSlot();
		SelectSlot->EnableSelectSlotButton.Broadcast(true);
		SelectSlot = nullptr;  //删除后清空SelectSlot
	}

}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());

	if (IsValid(SelectSlot))
	{
		AuraGameInstance->PlayerStartTag = SelectSlot->PlayerStartTag; 
		AuraGameInstance->LoadSlotName = SelectSlot->SlotName;
		AuraGameInstance->LoadSlotIndex = SelectSlot->SlotIndex;

		AuraGameMode->TravelToMap(SelectSlot);
	}
	
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameMode)) return; //只有在服务端才能的读取存档
	
	for (const TTuple<int32,UMVVM_LoadSlot*> Pair : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotDate(Pair.Value->SlotName,Pair.Key);
		
		Pair.Value->SlotStatus = SaveObject->SlotStatus;
		Pair.Value->SetPlayerName(SaveObject->PlayerName);
		Pair.Value->SetMapName(SaveObject->MapName);
		Pair.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		Pair.Value->SetPlayerLevel(SaveObject->PlayerLevel);
		Pair.Value->InitializeSlot();  //广播Slot自己的状态以便选择正确的Widget显示
		
	}
}
