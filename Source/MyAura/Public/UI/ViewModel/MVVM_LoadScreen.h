

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);


class UMVVM_LoadSlot;

UCLASS()
class MYAURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
	

public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;
	
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	
	void InitializedLoadSLots();
	
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;
	
	//这里的按钮是输入玩家名称后按下的按钮，严格来说这里是PlayerName而不是SlotName
	UFUNCTION(BlueprintCallable)
	void NewSlotNameButtonPressed(int32 SlotIndex,const FString& EnteredName);
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();
	
	void LoadData();
	
	
protected:
	
private:
	
	UPROPERTY()
	TMap<int32,UMVVM_LoadSlot*> LoadSlots;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
	UPROPERTY()
	UMVVM_LoadSlot* SelectSlot;
};
