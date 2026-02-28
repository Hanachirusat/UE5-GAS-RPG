

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYAURA_API UAuraWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BluePrintReadWrite)
	TObjectPtr<UObject> WidgetController;
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
