#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "NiagaraSystem.h"
#include "Interaction/CombatInterface.h"
#include "BaseCharacter.generated.h"



class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;

UCLASS(Abstract) //Abstract表示该类是抽象类无法在编辑器中创建该类的实例
class MYAURA_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const{ return AttributeSet;	}
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	//**Combat Interface
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;   //服务端处理角色死亡后的逻辑
	virtual TArray<FTaggedMontage> GetTaggedMontages_Implementation() override; 
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncreateMinionCount_Implementation(int32 Count) override;
	virtual void SetMaster_Implementation(AActor* InMaster) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual FOnDeathSignature& GetOnDeathDelegate2() override;
	virtual void SetIsBeingShock_Implementation(bool bInShock) override;
	virtual bool GetIsBeingShock_Implementation() const override;
	virtual FOnTakeDamageSignature& GetOnTakeDamageSignature() override;
 	//**End Combat Interface 
	

	
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);  //客户端和服务端都会调用
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackMontages;
	
	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;
	FOnDeathSignature OnDeathDelegate;
	FOnTakeDamageSignature OnTakeDamageDelegate;

	
	

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandTipSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandTipSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailTipSocketName;
	
	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	virtual void InitAbilityActorInfo();
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Combat|Init Effect")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributeEffect;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Combat|Init Effect")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributeEffect;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Combat|Init Effect")
	TSubclassOf<UGameplayEffect> DefaultBaseAttributeEffect;
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float level) const;
	virtual void InitializeDefaultAttribute() const;
	
	void AddCharacterAbilities();
	
	//Dissolve Effects
	void Dissolove();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	UNiagaraSystem* BloodEffect;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	USoundBase* DeathSound;
	
	//Minions
	int32 MinionCount = 0.f;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat|Character Class Defaults" )
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> Master = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> FireDebuffNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> LightningDebuffNiagaraComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed = 250.f;
	
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_bInShockLoop, BlueprintReadOnly)
	bool bBingShockLoop = false;

	UFUNCTION()
	virtual void OnRep_Stunned();
	
	UFUNCTION()
	virtual void OnRep_Burned();
	
	UFUNCTION()
	virtual void OnRep_bInShockLoop();
private:
	
	UPROPERTY(EditAnywhere,Category="Combat|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere,Category="Combat|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;	
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;	
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;	

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
};
