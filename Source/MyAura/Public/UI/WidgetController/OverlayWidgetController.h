

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FAuraAbilityInfo;
class UAuraAbilitySystemComponent;
class UAbilityInfo;
class UAuraWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraWidget> MessageWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSIgnature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

UCLASS(BlueprintType,Blueprintable)
class MYAURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")  //蓝图绑定这个委托对象
	FOnAttributeChangedSIgnature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")  //蓝图绑定这个委托对象
	FOnAttributeChangedSIgnature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")  //蓝图绑定这个委托对象
	FOnAttributeChangedSIgnature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attribute")  //蓝图绑定这个委托对象
	FOnAttributeChangedSIgnature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Message") 
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|XP")  //蓝图绑定这个委托对象
	FOnAttributeChangedSIgnature OnXPPercentChangedDelegate;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Level")  //蓝图绑定这个委托对象
	FOnPlayerStateChangedSIgnature OnLevelChangedDelegate;
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable , const FGameplayTag Tag);
	
	void OnXPChanged(int32 NewXP);
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag,const FGameplayTag& Status, const FGameplayTag& Slot,const FGameplayTag& PrevSlot) const;
	
	
	
private:
	
};

//模板函数的定义和声明应在一个文件中
template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
