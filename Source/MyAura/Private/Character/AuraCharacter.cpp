
#include "Character/AuraCharacter.h"

#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/BlueFuncLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/AuraGameModeBase.h"
#include "GameMode/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

int32 AAuraCharacter::GetLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetLevel();
}

void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);
	
	//展示信息
	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this](){
		AAuraGameModeBase* AuraGM=Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (AuraGM)
		{
			AuraGM->PlayerDied(this);
		}
	});
	//启动计时器
	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate,DeathTime,false);
	CameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);  //固定相机
	
	
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	Multicast_LevelUpParaticles();
}
void AAuraCharacter::Multicast_LevelUpParaticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		//设置朝向为相机朝向
		const FVector CameraLocation  = CameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator  ToCameraRotation = (CameraLocation-NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AAuraCharacter::GetXP_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetAttributePointReward_Implementation(int32 InLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointReward_Implementation(int32 InLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].SpellPointAward;
}

void AAuraCharacter::AddToLevel_Implementation(int32 InLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InLevel);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatues(AuraPlayerState->GetLevel());
	}
}

void AAuraCharacter::AddToSepllPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints(); 
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckPointTag)
{
	if (!HasAuthority()) return;
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameModeBase)
	{
		ULoadScreenSaveGame* SaveDate = AuraGameModeBase->GetLoadSlotDate();
		if (SaveDate == nullptr) return;
		
		SaveDate->PlayerStartTag = CheckPointTag;
		AAuraPlayerState* PlayerStat = Cast<AAuraPlayerState>(GetPlayerState());
		if (PlayerStat)
		{
			SaveDate->PlayerLevel = PlayerStat->GetLevel();
			SaveDate->XP = PlayerStat->GetXP();
			SaveDate->AttributePoints = PlayerStat->GetAttributePoints();
			SaveDate->SpellPoints = PlayerStat->GetSpellPoints();
		}
		SaveDate->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveDate->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveDate->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveDate->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		SaveDate->bNewGame = false;

		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		
		SaveDate->SavedAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this,AuraASC,&SaveDate](const FGameplayAbilitySpec& Spec)
		{
			const FGameplayTag& AbilityTag = AuraASC->GetAbilityTagFromSpec(Spec);
			UAbilityInfo* AbilityInfo = UBlueFuncLibrary::GetAbilityInfo(this);
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			
			FSavedAbility SavedAbility;
			SavedAbility.GA = Info.Ability;
			SavedAbility.AbilityLevel = Spec.Level;
			//会发生变化的Tag添加在Spec中，不会发生变化的Tag添加在GAInfo数据资产中。所以Slot,Status用ASC获取，Type用Info获取。
			SavedAbility.AbilitySlot = AuraASC->GetInputTagFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = AuraASC->GetStatusFromAbilityTag(AbilityTag); 
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityType;
			
			SaveDate->SavedAbilities.AddUnique(SavedAbility);
		});
		AuraASC->ForEachAbility(SaveAbilityDelegate);
		
		AuraGameModeBase->SaveGameProgressDate(SaveDate);
	}
}

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	
	CameraComponent  = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;
	
	//顶视角游戏常见操作
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	CharacterClass = ECharacterClass::Elementalist;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	LoadProgress();
	if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		AuraGameModeBase->LoadWorldState(GetWorld());
	}
}


void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}


void AAuraCharacter::LoadProgress()
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameModeBase)
	{
		ULoadScreenSaveGame* SaveDate = AuraGameModeBase->GetLoadSlotDate();
		if (SaveDate == nullptr) return;
		
		if (SaveDate ->bNewGame)
		{
			InitializeDefaultAttribute();
			AddCharacterAbilities();
		}
		else
		{
			//从磁盘加载Abilities
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
			{
				AuraASC->AddCharacterAbilitiesFromSaveData(SaveDate);
			}
			if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
			{
				AuraPlayerState->SetLevel(SaveDate->PlayerLevel);
				AuraPlayerState->SetXP(SaveDate->XP);
				AuraPlayerState->SetAttributePoints(SaveDate->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveDate->SpellPoints);
			}
			UBlueFuncLibrary::InitializedDefaultAttributesFromSaveData(this,AbilitySystemComponent,SaveDate);
		}
	}
}

void AAuraCharacter::OnRep_Stunned()
{
	 if (UAuraAbilitySystemComponent* AuraASC =  Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	 {	
	 	FGameplayTagContainer NewTagContainer;
	 	NewTagContainer.AddTag(FAuraGameplayTags::Get().Player_Block_CursorTrace);
	 	NewTagContainer.AddTag(FAuraGameplayTags::Get().Player_Block_InputHeld);
	 	NewTagContainer.AddTag(FAuraGameplayTags::Get().Player_Block_InputPressed);
	 	NewTagContainer.AddTag(FAuraGameplayTags::Get().Player_Block_InputReleased);
	 	if (bIsStunned)
	 	{
	 		AuraASC->AddLooseGameplayTags(NewTagContainer);
	 		// LightningDebuffNiagaraComponent->Activate();
	 	}
	    else
	    {
		    AuraASC->RemoveLooseGameplayTags(NewTagContainer);
	    	// LightningDebuffNiagaraComponent->Deactivate();
	    }
	 }
}

void AAuraCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		FireDebuffNiagaraComponent->Activate();
	}
	else
	{
		FireDebuffNiagaraComponent->Deactivate();
	}
}



void AAuraCharacter::InitAbilityActorInfo()
{
	
	//通过在GameMode中设置PlayerState把PlayerState和Character联系起来。
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->InitAbilityActorInfoSet();
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);//广播ASC已经初始化完毕，可以绑定ASC上的委托
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Lightning,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AAuraCharacter::StunTagChanged);
	
	
	//在多人情况下，客户端只有本地角色有PlayerController，其他角色没有。
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
	// InitializeDefaultAttribute();
	//从磁盘加载
	

}


