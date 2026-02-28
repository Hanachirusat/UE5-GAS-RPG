

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath,AActor*,DeathActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature,AActor*,DeathActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeDamageSignature,float /*DamageAmount*/)

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;
/**
 * 
 */
class MYAURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)  //蓝图可调用，c++可覆写
	int32 GetLevel();
	
	//避免将实例转化为接口然后调用函数
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)  //蓝图可调用，c++可覆写
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable )
	void UpdateFacingTarget(const FVector& Target);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)  //蓝图可调用，c++可覆写
	UAnimMontage* GetHitReactMontage();
	
	virtual void Die(const FVector& DeathImpulse) = 0;

	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)  //蓝图可调用，c++可覆写
	bool IsDead() const;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)  //蓝图可调用，c++可覆写
	AActor* GetAvatar();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)  //蓝图可调用，c++可覆写
	TArray<FTaggedMontage> GetTaggedMontages();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetMinionCount();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void IncreateMinionCount(int32 Count);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SetMaster(AActor* InMaster);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	ECharacterClass GetCharacterClass();
	
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;
	virtual FOnDeath& GetOnDeathDelegate() = 0;
	virtual FOnDeathSignature& GetOnDeathDelegate2() = 0;
	virtual FOnTakeDamageSignature& GetOnTakeDamageSignature() = 0;
	
	//设置我方角色是否在持续发射激光
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable )
	void SetInShockLoop(bool bInLoop);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	USkeletalMeshComponent*  GetWeapon();
	
	//敌方是否被眩晕
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool GetIsBeingShock() const;
	
	//设置敌方是否被眩晕
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SetIsBeingShock(bool bInShock);
};
