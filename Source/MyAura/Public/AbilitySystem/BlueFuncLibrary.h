

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "BlueFuncLibrary.generated.h"


class ULootTiers;
class ULoadScreenSaveGame;
struct FDamageEffectParams;
class AAuraHUD;
class USpellMenuWidgetController;
class UAbilitySystemComponent;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class MYAURA_API UBlueFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/* Widget Controller*/ 
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject,FWidgetControllerParams& OutWCParams,AAuraHUD*& OutAuraHUD );
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	/* ASC class Default*/ 
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|CharacterClassDefault")
	static void InitializedDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float level,UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|CharacterClassDefault")
	static void InitializedDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,ULoadScreenSaveGame* SaveGame);
	
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|CharacterClassDefault")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|CharacterClassDefault")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|CharacterClassDefault",meta = (DefaultToSelf = "WorldContextObject"))
	static ULootTiers* GetLootTiers(const UObject* WorldContextObject);
	
	
	/* Gameplay  Effect Context Getter*/ 
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static float GetDebuffDamage(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static float GetDebuffDuration(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static FVector GetKnockBackForce(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static bool IsRadialdamage(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle&  EffectContextHandle);
	
	/* Gameplay  Effect Context Setter*/ 
	//UARAM(ref)表示这个参数是输入，在UE蓝图中，如果函数参数是非const引用，通常代表的是输出。
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bInIsBlockedHit);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bInIsCriticalHit);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bSuccessfulDebuff);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,float DebuffDamage);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,float DebuffDuration);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,float DebuffFrequency);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FGameplayTag& InDamageType);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FVector& InDeathImpulse);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetKnockBackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FVector& InForce);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bInIsRadialDamage);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FVector& Origin);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,float Radius);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|GameplayEffect")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,float Radius);
	
	
	/* HelpFunc*/ 
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|HelpFunc")
	static void GetLivePlayerWithRadius(const UObject* WorldContextObject,TArray<AActor*>& OutOverlappingActors,const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin); 
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|HelpFunc")
	static void GetClosestTarget(int32 MaxTarget, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTarget,const FVector& Origin);
	
	UFUNCTION(BlueprintPure,Category="ASC-BlueprintFunctionLibrary|HelpFunc")
	static bool IsNotFriend(AActor* FirstActor,AActor* SecondActor);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|ApplyDamage")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& Params);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|ApplyDamage")
	static TArray<FRotator> EvenlyRotators(const FVector& Forward, const FVector& Axis,float Spread,int32 Num);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|ApplyDamage")
	static TArray<FVector> EvenlyVectors(const FVector& Forward, const FVector& Axis,float Spread,int32 Num);
	
	static int32 GetXpRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass,int32 Level);
	
	/*
	 * Damage Effect Params
	 */
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|DamageEffectParams")
	static void SetIsRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& Params,bool bIsRadial,float InnerRadius,float OuterRadius,FVector Origin);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|DamageEffectParams")
	static void SetKnockBackDirection(UPARAM(ref) FDamageEffectParams& Params,FVector KnockBackDirection,float Magnitude = 0.f);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|DamageEffectParams")
	static void SetDeathImpulse_DamageEffectParams(UPARAM(ref) FDamageEffectParams& Params,FVector DeathImpulseDirection,float Magnitude = 0.f);
	
	UFUNCTION(BlueprintCallable,Category="ASC-BlueprintFunctionLibrary|DamageEffectParams")
	static void SetEffectParmasASC(UPARAM(ref) FDamageEffectParams& Params,UAbilitySystemComponent* InASC);
	
};


