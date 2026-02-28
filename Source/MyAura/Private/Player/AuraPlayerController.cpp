#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighLightInterface.h"
#include "MyAura/MyAura.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraInput);	// 添加输入映射上下文
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());  //单例
	if(Subsystem)
	{
		//考虑多人游戏，只有本地有Subsystem，因此不能用check。
		Subsystem->AddMappingContext(AuraInput,0);   // 优先级最高
	}
	
	bShowMouseCursor = true;   // 设置光标可见
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UAuraEnhancedInputComponent* AuraEnhancedInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);
	AuraEnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraEnhancedInputComponent->BindAction(ShiftAction,ETriggerEvent::Started,this,&AAuraPlayerController::ShiftPressed);
	AuraEnhancedInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AAuraPlayerController::ShiftReleased);
	
	
	AuraEnhancedInputComponent->BindAbilityActions(
		AuraInputConfig,
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}


void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);

	}
	if (DecalMaterial)
	{
		MagicCircle->MagicCircleComponent->SetMaterial(0,DecalMaterial);
	}
	bShowMouseCursor = false;
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();

	}
	bShowMouseCursor = true;
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float Damage,ACharacter* Target,bool bBlockedHit, bool bCriticalHit)
{
	//IsValid除检查指针是否为空，还会检查指针所指对象是否待销毁.
	//只在本地执行
	if (IsValid(Target) && DamageTextComponent && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(Target,DamageTextComponent);
		DamageText->RegisterComponent();  //动态创建的组件，自己注册
		DamageText->AttachToComponent(Target->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(Damage,bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputXY = InputActionValue.Get<FVector2D>();  //我们设置的是一个二维输入
	//获得控制器在水平面上的旋转（绕z轴旋转）
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
	//获得向前向量（单位向量）
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);  
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControllerPawn = GetPawn<APawn>())
	{
		ControllerPawn->AddMovementInput(ForwardDirection,InputXY.Y); 
		ControllerPawn->AddMovementInput(RightDirection,InputXY.X);
	}
}


void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag tag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	//GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Red,*tag.ToString());
	if (tag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UEnemyInterface>()? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNotEnemy;

		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
	}
	bAutoRunning = false;  //按下的时候停止自动寻路，松开的时候判定自动寻路。
	if (GetASC())
	{
		GetASC()->AbilityInputTagPressed(tag);
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag tag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	
	if (!tag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) 
			GetASC()->AbilityInputTagHeld(tag);
		return ;
	}
	
	if (TargetingStatus==ETargetingStatus::TargetingEnemy || bShiftPressed)  //按住shift进行攻击
	{
		if (GetASC()) 
			GetASC()->AbilityInputTagHeld(tag);
	}
	//如果没有锁定目标就开始移动
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (CursorHitResult.bBlockingHit)
			CachedDestination = CursorHitResult.Location;
		if (APawn* ControllerPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag tag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (!tag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) 
			GetASC()->AbilityInputTagRelease(tag);
		return ;
	}
	
	if (GetASC()) 
		GetASC()->AbilityInputTagRelease(tag);
	
	if (TargetingStatus!=ETargetingStatus::TargetingEnemy && !bShiftPressed)
	{
		APawn* ControllerPawn=GetPawn();
		//短按
		if (FollowTime <= ShortPressThreshold && ControllerPawn)
		{
			//移动到场景中的特定位置不会显示特效
			if (IsValid(ThisActor) && ThisActor->Implements<UHighLightInterface>())
			{
				IHighLightInterface::Execute_SetMoveToLocation(ThisActor,CachedDestination);
			}
			else if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ClickEffect,CachedDestination);
			}
			
			//自动寻路
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControllerPawn->GetActorLocation(),CachedDestination))
			{
				if (NavPath->PathPoints.Num()>0)
				{
					Spline->ClearSplinePoints();
					for (const FVector& PointLoc:NavPath->PathPoints)
					{
						Spline->AddSplinePoint(PointLoc,ESplineCoordinateSpace::World);
					}
					//有可能玩家点击的地点无法到达，此时将Spline生成的最后一个点作为目的地

					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num()-1];
					bAutoRunning = true; 
				}
			}

		}
		FollowTime =0.f;
		TargetingStatus = ETargetingStatus::NotTargeting;
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent==nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}


void AAuraPlayerController::HighLightActor(AActor* Actor)
{
	if (Actor && Actor->Implements<UHighLightInterface>()) IHighLightInterface::Execute_HighlightActor(Actor);
	
}

void AAuraPlayerController::UnHighLightActor(AActor* Actor)
{
	if (Actor && Actor->Implements<UHighLightInterface>()) IHighLightInterface::Execute_UnHighlightActor(Actor);
}

void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		//如果ASC中有禁用CurceTrace的标签，则禁用
		UnHighLightActor(LastActor);
		HighLightActor(ThisActor);
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	const ECollisionChannel TraceChannel = IsValid(MagicCircle)? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel,false,CursorHitResult);
	if (!CursorHitResult.bBlockingHit) return;
	
	//更新ThisActor和LastActor
	LastActor = ThisActor;
	if (IsValid(CursorHitResult.GetActor()) &&CursorHitResult.GetActor()->Implements<UHighLightInterface>())
	{
		ThisActor = CursorHitResult.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}
	
	if (LastActor!=ThisActor)
	{
		UnHighLightActor(LastActor);
		HighLightActor(ThisActor);
	}
	
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControllerPawn = GetPawn<APawn>())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControllerPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(ControllerPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		ControllerPawn->AddMovementInput(Direction);
		
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHitResult.Location);
	}
}
