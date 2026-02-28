

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighLightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"

#include "EnemyCharacter.generated.h"



class UWidgetComponent;
class UBehaviorTree;
class AMyAIController;
/**
 * 
 */
UCLASS()
class MYAURA_API AEnemyCharacter : public ABaseCharacter,public IEnemyInterface,public IHighLightInterface
{
	GENERATED_BODY()
	AEnemyCharacter();
public:
	virtual void PossessedBy(AController* NewController) override;
	
	/** Enemy Interface*/

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	/** End Enemy Interface*/
	
	//Combat Interface
	virtual int32 GetLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	//End Combat Interface 
	
	//HighLight
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	//HighLight
	
	void SetCharacterClass (const ECharacterClass InClass){CharacterClass = InClass;};
	void SetLevel (const int32 InLevel){Level = InLevel;}
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSIgnature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSIgnature OnMaxHealthChanged;
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReacting = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;
	
	UPROPERTY(BlueprintReadWrite,Category="Combat")
	TObjectPtr<AActor> CombatTarget;
	
	
protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttribute() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat|Character Class Defaults" )
	int32 Level = 1;
	

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Widget")
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditAnywhere, Category = "Combat|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<AMyAIController> AIController;
	
	//课程外函数
	void PrintAttribute() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
};
