

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYAURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;
	
	UDebuffNiagaraComponent();
	
protected:
	virtual void BeginPlay() override;
	
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UFUNCTION()
	void OnOwnerDeath(AActor* DeathActor);
};
