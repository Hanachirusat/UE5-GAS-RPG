

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameMode/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);



UCLASS()
class MYAURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;
	
	UPROPERTY()
	FString SlotName;  
	 
	UPROPERTY()
	int32 SlotIndex;  //这是第几个插槽
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus>  SlotStatus;
	
	UPROPERTY()
	FName PlayerStartTag;
	
		
	UPROPERTY()
	FString MapAssetName;
	
	void InitializeSlot();
	void SetPlayerName(const FString& InPlayerName);
	void SetMapName(const FString& InMapName);
	void SetPlayerLevel(int32 InPlayerlevel);
	FString GetPlayerName() const {return PlayerName;}
	FString GetMapName() const {return MapName;}
	int32 GetPlayerLevel() const {return PlayerLevel;}
	

	
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta = (AllowPrivateAccess="true"));
	FString PlayerName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta = (AllowPrivateAccess="true"));
	FString MapName;
	
		
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta = (AllowPrivateAccess="true"));
	int32 PlayerLevel;
};
