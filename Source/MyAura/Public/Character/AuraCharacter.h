

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MYAURA_API AAuraCharacter : public ABaseCharacter,public IPlayerInterface
{
	GENERATED_BODY()
public:
	//Combat Interface 
	virtual int32 GetLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	//End Combat Intercate
	
	//Player Interface 
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) override;
	virtual int32 GetAttributePointReward_Implementation(int32 InLevel) const override;
	virtual int32 GetSpellPointReward_Implementation(int32 InLevel) const override;
	virtual void AddToLevel_Implementation(int32 InLevel) override;
	virtual void AddToSepllPoints_Implementation(int32 InSpellPoints) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& CheckPointTag) override;
	//End Player Intercate
	
	AAuraCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;
	
	void LoadProgress();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Effects")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 1.f;
	
	FTimerHandle DeathTimer;
private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
		
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	virtual void InitAbilityActorInfo() override;
	
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_LevelUpParaticles() const;
	
};
