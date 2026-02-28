


#include "AbilitySystem/BlueFuncLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "GameMode/AuraGameModeBase.h"
#include "GameMode/LoadScreenSaveGame.h"
#include "Interaction/CombatInterface.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


bool UBlueFuncLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
	FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		OutAuraHUD = Cast<AAuraHUD>(PC->GetHUD());
		if (OutAuraHUD)
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent=	ASC;
			OutWCParams.PlayerController= PC;
			OutWCParams.PlayerState = PS;;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UBlueFuncLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (const bool bSuccessfulParams = MakeWidgetControllerParams(WorldContextObject,WCParams,AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UBlueFuncLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (const bool bSuccessfulParams = MakeWidgetControllerParams(WorldContextObject,WCParams,AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UBlueFuncLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (const bool bSuccessfulParams = MakeWidgetControllerParams(WorldContextObject,WCParams,AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UBlueFuncLibrary::InitializedDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float level,UAbilitySystemComponent* ASC)
{
	// AAuraGameModeBase* GameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	// if (GameModeBase==nullptr) return;
	
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	// if (CharacterClassInfo == nullptr) return;  //在下面 函数中解决
	FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryContext = ASC->MakeEffectContext();
	PrimaryContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(Info.PrimaryAttribute,level,PrimaryContext);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle SecondaryContext = ASC->MakeEffectContext();
	SecondaryContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttribute,level,SecondaryContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle BaseContext = ASC->MakeEffectContext();
	BaseContext.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle BaseAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->BaseAttribute,level,BaseContext);
	ASC->ApplyGameplayEffectSpecToSelf(*BaseAttributeSpecHandle.Data.Get());
	
}


void UBlueFuncLibrary::InitializedDefaultAttributesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttribute_SetByCaller,1.f,EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Attribute_Primary_Strength,SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Attribute_Primary_Resilience,SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Attribute_Primary_Intelligence,SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Attribute_Primary_Vigor,SaveGame->Strength);
	
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	
	FGameplayEffectContextHandle SecondaryContext = ASC->MakeEffectContext();
	SecondaryContext.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttribute_Infinite,1.f,SecondaryContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle BaseContext = ASC->MakeEffectContext();
	BaseContext.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle BaseAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->BaseAttribute,1.f,BaseContext);
	ASC->ApplyGameplayEffectSpecToSelf(*BaseAttributeSpecHandle.Data.Get());
}

void UBlueFuncLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo =GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		//通用能力一般不会随着等级提升而提升。
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())  
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,ICombatInterface::Execute_GetLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

int32 UBlueFuncLibrary::GetXpRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass,
	int32 Level)
{
	UCharacterClassInfo* CharacterClassInfo =GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;
	
	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	return static_cast<int32>(Info.XPReward.GetValueAtLevel(Level));
}

void UBlueFuncLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& Params, bool bIsRadial, float InnerRadius,
	float OuterRadius, FVector Origin)
{
	Params.bIsRadialDamage = bIsRadial;
	Params.RadialDamageInnerRadius = InnerRadius;
	Params.RadialDamageOuterRadius = OuterRadius;
	Params.RadialDamageOrigin = Origin;
}

void UBlueFuncLibrary::SetKnockBackDirection(FDamageEffectParams& Params, FVector KnockBackDirection,float Magnitude )
{
	KnockBackDirection.Normalize();
	if (Magnitude == 0.f)
		Params.KnockBackForce = KnockBackDirection * Params.KnockBackForceMagnitude;
	else
		Params.KnockBackForce = KnockBackDirection * Magnitude;
}

void UBlueFuncLibrary::SetDeathImpulse_DamageEffectParams(FDamageEffectParams& Params, FVector DeathImpulseDirection,float Magnitude )
{
	DeathImpulseDirection.Normalize();
	if (Magnitude == 0.f)
		Params.DeathImpulse = DeathImpulseDirection * Params.DeathImpulseMagnitude;
	else
		Params.DeathImpulse = DeathImpulseDirection * Magnitude;
}

void UBlueFuncLibrary::SetEffectParmasASC(FDamageEffectParams& Params, UAbilitySystemComponent* InASC)
{
	Params.TargetASC = InASC;
}


UCharacterClassInfo* UBlueFuncLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* GameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameModeBase==nullptr) return nullptr;
	
	return GameModeBase->CharacterClass;
	
}

UAbilityInfo* UBlueFuncLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* GameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameModeBase==nullptr) return nullptr;
	return GameModeBase->AbilityInfo;
}

ULootTiers* UBlueFuncLibrary::GetLootTiers(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* GameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (GameModeBase==nullptr) return nullptr;
	return GameModeBase->LootTiers;
}

bool UBlueFuncLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsBlockedHit();
	}
	else
	{
		return false;
	}
}

bool UBlueFuncLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsCriticalHit();
	}
	else
	{
		return false;
	}
}

bool UBlueFuncLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsSuccessfulDebuff();
	}
	return false;
	
}


float UBlueFuncLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetDebuffDamage();
	}
	return 0.f;
}

float UBlueFuncLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetDebuffDuration();
	}
	return 0.f;
}

float UBlueFuncLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UBlueFuncLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (AuraContext->GetDamageType().IsValid())
		{
			return *AuraContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UBlueFuncLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UBlueFuncLibrary::GetKnockBackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetKnockBackForce();
	}
	return FVector::ZeroVector;
}

bool UBlueFuncLibrary::IsRadialdamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetIsRadialDamage();
	}
	return false;
}

float UBlueFuncLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UBlueFuncLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UBlueFuncLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UBlueFuncLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetIsBlockedHit(bInIsBlockedHit);
	}
		
}

void UBlueFuncLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UBlueFuncLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bSuccessfulDebuff)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetIsSuccessfulDebuff(bSuccessfulDebuff);
	}
}

void UBlueFuncLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float DebuffDamage)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetDebuffDamage(DebuffDamage);
	}
}

void UBlueFuncLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float DebuffDuration)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetDebuffDuration(DebuffDuration);
	}
}

void UBlueFuncLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float DebuffFrequency)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetDebuffFrequency(DebuffFrequency);
	}
}

void UBlueFuncLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetDamageType(MakeShared<FGameplayTag>(InDamageType));
	}
}

void UBlueFuncLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UBlueFuncLibrary::SetKnockBackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetKnockBackForce(InForce);
	}
}

void UBlueFuncLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsRadialDamage)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UBlueFuncLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& Origin)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetRadialDamageOrigin(Origin);
	}
}

void UBlueFuncLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, const float Radius)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetRadialDamageInnerRadius(Radius);
	}
}

void UBlueFuncLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, const float Radius)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetRadialDamageOuterRadius(Radius);
	}
}

void UBlueFuncLibrary::GetLivePlayerWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
                                               const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps; 
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		
		// 使用TSet来去重
		TSet<AActor*> UniqueActors;
		
		for (FOverlapResult& Overlap : Overlaps)
		{
			//这个模板函数 用U而不是I
			const bool ImplementsCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			//只有实现接口才判断是否死亡
			if (ImplementsCombatInterface && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				// 获取Avatar
				AActor* Avatar = ICombatInterface::Execute_GetAvatar(Overlap.GetActor());
				if (Avatar && !UniqueActors.Contains(Avatar))
				{
					UniqueActors.Add(Avatar);
					OutOverlappingActors.Add(Avatar);
				}
			}
		}
	}
}

void UBlueFuncLibrary::GetClosestTarget(int32 MaxTarget, const TArray<AActor*>& Actors,
	TArray<AActor*>& OutClosestTarget, const FVector& Origin)
{
	if (Actors.Num() <= MaxTarget)
	{
		OutClosestTarget = Actors;
		return;
	}
	// TArray<AActor*> ActorsToCheck =  Actors;
	// int32 NumTargetsFound = 0;
	// while (NumTargetsFound < MaxTarget)
	// {
	// 	if (ActorsToCheck.Num()==0) break;
	// 	double ClosestDistance = TNumericLimits<double>::Max();
	// 	AActor* ClosestActor = nullptr;
	// 	for (AActor* PotentialTarget : ActorsToCheck)
	// 	{
	// 		const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
	// 		if (Distance < ClosestDistance)
	// 		{
	// 			ClosestDistance = Distance;
	// 			ClosestActor = PotentialTarget;
	// 		}
	// 	}
	// 	ActorsToCheck.Remove(ClosestActor);
	// 	OutClosestTarget.Add(ClosestActor);
	// 	++NumTargetsFound;
	// }
	// 创建一个最大堆来维护最近的MaxTarget个Actor
	// 堆顶是这MaxTarget个中最远的
	TArray<AActor*> MaxHeap;
	MaxHeap.Reserve(MaxTarget + 1);
    
	// 比较函数：距离大的在堆顶
	auto MaxHeapCompare = [Origin](const AActor& A, const AActor& B) -> bool
	{
		float DistA = FVector::DistSquared(Origin, A.GetActorLocation());
		float DistB = FVector::DistSquared(Origin, B.GetActorLocation());
		return DistA > DistB;  // 最大堆
	};
    
	// 初始化堆
	for (int32 i = 0; i < MaxTarget; ++i)
	{
		if (Actors[i])
		{
			MaxHeap.HeapPush(Actors[i], MaxHeapCompare);
		}
	}
    
	// 处理剩余Actor
	for (int32 i = MaxTarget; i < Actors.Num(); ++i)
	{
		AActor* CurrentActor = Actors[i];
		if (!CurrentActor) continue;
        
		float CurrentDist = FVector::DistSquared(Origin, CurrentActor->GetActorLocation());
		float MaxDist = FVector::DistSquared(Origin, MaxHeap.HeapTop()->GetActorLocation());
        
		// 如果当前Actor比堆顶（当前最远的）更近，替换堆顶
		if (CurrentDist < MaxDist)
		{
			// 弹出堆顶（最远的）
			AActor* TopActor;
			MaxHeap.HeapPop(TopActor, MaxHeapCompare);
            
			// 插入当前Actor
			MaxHeap.HeapPush(CurrentActor, MaxHeapCompare);
		}
	}
    
	// 将堆中的元素转移到输出数组
	OutClosestTarget.Reserve(MaxTarget);
	while (MaxHeap.Num() > 0)
	{
		AActor* Actor;
		MaxHeap.HeapPop(Actor, MaxHeapCompare);
		OutClosestTarget.Add(Actor);
	}

}


bool UBlueFuncLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool BothArePlayers= FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool BothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	return !(BothArePlayers || BothAreEnemies);
}

FGameplayEffectContextHandle UBlueFuncLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
	const AActor* SourceAvatarActor = Params.SourceASC->GetAvatarActor();
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		
	FGameplayEffectContextHandle EffectContextHandle = Params.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle,Params.DeathImpulse);
	SetKnockBackForce(EffectContextHandle,Params.KnockBackForce);
	
	SetIsRadialDamage(EffectContextHandle,Params.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle,Params.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle,Params.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle,Params.RadialDamageOrigin);
	
	
	
	const FGameplayEffectSpecHandle SpecHandle = Params.SourceASC->MakeOutgoingSpec(Params.DamageGameplayEffectClass,Params.AbilityLevel,EffectContextHandle);
	
	//Set By Caller Magnitude
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Params.DamageType,Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Debuff_Chance,Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Debuff_Damage,Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Debuff_Duration,Params.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Debuff_Frequency,Params.DebuffFrequency);
	
	Params.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

TArray<FRotator> UBlueFuncLibrary::EvenlyRotators(const FVector& Forward, const FVector& Axis,float Spread,int32 Num)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread/2.f,Axis);
	
	if (Num>1)
	{
		const float DeltaSpread =  Spread/(Num-1);  //有三个物体的时候才会中分
		for (int32 i =0;i<Num;i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i,FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UBlueFuncLibrary::EvenlyVectors(const FVector& Forward, const FVector& Axis,float Spread,int Num)
{
	TArray<FVector> Vectors;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread/2.f,Axis);
	
	if (Num>1)
	{
		const float DeltaSpread =  Spread/(Num-1);  //有三个物体的时候才会中分
		for (int32 i =0;i<Num;i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i,FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}


