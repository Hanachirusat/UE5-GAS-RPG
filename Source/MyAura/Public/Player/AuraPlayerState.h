

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, int32 /**/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateStart, int32 /**/);


class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class MYAURA_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UAttributeSet* GetAttributeSet() const{ return AttributeSet;	}
	FORCEINLINE int32 GetLevel() const{ return Level; }
	FORCEINLINE int32 GetXP() const{ return XP; }
	FORCEINLINE int32 GetAttributePoints() const{ return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const{ return SpellPoints; }
	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InAttributePoints);
	void AddToSpellPoints(int32 InSpellPoints);
	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void SetAttributePoints(int32 InAttributePoints);
	void SetSpellPoints(int32 InSpellPoints);
	
	FOnPlayerStateChanged OnXPChangedDelegate;
	FOnPlayerStateChanged OnLevelChangedDelegate;
	FOnPlayerStateChanged OnAttributePointsChangedDelegate;
	FOnPlayerStateChanged OnSpellPointsChangedDelegate;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo>  LevelUpInfo;
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	
private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_XP)
	int32 XP = 1;
	
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 100;
	
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 100;
	
	UFUNCTION()
	void OnRep_Level(int32& OldLevel);
	
	UFUNCTION()
	void OnRep_XP(int32& OldXP);
	
	UFUNCTION()
	void OnRep_AttributePoints(int32& OldAttributePoints);
	
	UFUNCTION()
	void OnRep_SpellPoints(int32& OldSpellPoints);
	
};


