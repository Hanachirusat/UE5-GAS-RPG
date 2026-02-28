

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 需要在UeEngine.ini配置文件中设置AssetManager为自定义类
 * 
 *
 */
UCLASS()
class MYAURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UAuraAssetManager& Get();
protected:
	virtual void StartInitialLoading() override;
	
};
