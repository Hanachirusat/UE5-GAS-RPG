


#include "UI/Widget/AuraWidget.h"

void UAuraWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();  //蓝图函数，在蓝图中为每个小widget设置相同的widgetController
}
