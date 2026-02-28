


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::SetPlayerName(const FString& InPlayerName)
{
	//会向绑定该变量的Widget广播内容
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName,InPlayerName);
}

void UMVVM_LoadSlot::SetMapName(const FString& InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName,InMapName);
}

void UMVVM_LoadSlot::SetPlayerLevel(int32 InPlayerlevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel,InPlayerlevel);
}

void UMVVM_LoadSlot::InitializeSlot()
{
	//根据自身的状态确定显示哪一个子控件
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}
