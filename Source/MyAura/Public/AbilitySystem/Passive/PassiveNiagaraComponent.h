

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYAURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	
	UPassiveNiagaraComponent();
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;
	
protected:
	virtual void BeginPlay();
	
	void OnPassiveActivete(const FGameplayTag& Tag, bool bActivate);
};
