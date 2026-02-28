


#include "Character/EnemyCharacter.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/BlueFuncLibrary.h"
#include "Components/WidgetComponent.h"
#include "MyAura/MyAura.h"
#include "UI/Widget/AuraWidget.h"
#include "AuraGameplayTags.h"
#include "AI/MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet= CreateDefaultSubobject<UAuraAttributeSet>("AuraAttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
	bUseControllerRotationPitch=false;
	bUseControllerRotationRoll=false;
	bUseControllerRotationYaw=false;
	GetCharacterMovement()->bUseControllerDesiredRotation=true;
	
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DPETH_RED);
	GetMesh()->MarkRenderStateDirty();
	Weapon->SetCustomDepthStencilValue(CUSTOM_DPETH_RED);
	GetMesh()->MarkRenderStateDirty();
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority()) return;
	AIController = Cast<AMyAIController>(NewController);
	
	AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIController->RunBehaviorTree(BehaviorTree);
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);

}

void AEnemyCharacter::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);

}

void AEnemyCharacter::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AEnemyCharacter::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	//Do Nothing!!
}

void AEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;	
}

AActor* AEnemyCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

int32 AEnemyCharacter::GetLevel_Implementation()
{
	return Level;
}

//在服务端调用
void AEnemyCharacter::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	SpawnLoot();
	Super::Die(DeathImpulse);
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0 : BaseWalkSpeed;
	
	check(AbilitySystemComponent);
	InitAbilityActorInfo();
	
	//设置WIdget的Controller为Enemy Character自身
	if (UAuraWidget* Widget=Cast <UAuraWidget>(HealthBar->GetUserWidgetObject()))
	{
		Widget->SetWidgetController(this);
	}
	
	//Controller绑定ASC的委托
	if (const UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHPAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}	
		);	
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHPAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}	
		);	
		
		//绑定ASC的Tag变化委托
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FAuraGameplayTags::Get().Abilities_HitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AEnemyCharacter::HitReactTagChanged);
		//Controller广播初始值
		OnHealthChanged.Broadcast(AS->GetHP());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHP());
	}
	
}

void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	//在服务端和客户端都会调用而AIController只存在服务端
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0 : BaseWalkSpeed;
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),bHitReacting);
	}
}


void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->InitAbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Lightning,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AEnemyCharacter::StunTagChanged);
	
	if (HasAuthority())
	{
		InitializeDefaultAttribute();
		//赋予初始能力
		UBlueFuncLibrary::GiveStartupAbilities(this,AbilitySystemComponent,CharacterClass);
	}
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void AEnemyCharacter::InitializeDefaultAttribute() const
{
	UBlueFuncLibrary::InitializedDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
	PrintAttribute();
}

void AEnemyCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("Stun"),bIsStunned);
	}
}


void AEnemyCharacter::PrintAttribute() const
{
	// 获取拥有者信息
	FString OwnerName = Owner ? Owner->GetName() : TEXT("None");
    
	UE_LOG(LogTemp, Display, TEXT("========================================"));
	UE_LOG(LogTemp, Display, TEXT("AttributeSet 属性初始化结果 - 拥有者: %s"), *OwnerName);
	UE_LOG(LogTemp, Display, TEXT("========================================"));
    
	
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	// 打印基础属性
	UE_LOG(LogTemp, Display, TEXT("基础属性:"));
	UE_LOG(LogTemp, Display, TEXT("  HP: %.1f / %.1f"), AS->GetHP(), AS->GetMaxHP());
	UE_LOG(LogTemp, Display, TEXT("  Mana: %.1f / %.1f"), AS->GetMana(), AS->GetMaxMana());
    
	// 打印主要属性
	UE_LOG(LogTemp, Display, TEXT("主要属性:"));
	UE_LOG(LogTemp, Display, TEXT("  Strength: %.1f"), AS->GetStrength());
	UE_LOG(LogTemp, Display, TEXT("  Intelligence: %.1f"), AS->GetIntelligence());
	UE_LOG(LogTemp, Display, TEXT("  Resilience: %.1f"), AS->GetResilience());
	UE_LOG(LogTemp, Display, TEXT("  Vigor: %.1f"), AS->GetVigor());
    
	// 打印次要属性
	UE_LOG(LogTemp, Display, TEXT("次要属性:"));
	UE_LOG(LogTemp, Display, TEXT("  Armor: %.1f"), AS->GetArmor());
	UE_LOG(LogTemp, Display, TEXT("  ArmorPenetration: %.1f"), AS->GetArmorPenetration());
	UE_LOG(LogTemp, Display, TEXT("  BlockChance: %.1f%%"), AS->GetBlockChance());
	UE_LOG(LogTemp, Display, TEXT("  CriticalHitChance: %.1f%%"), AS->GetCriticalHitChance());
	UE_LOG(LogTemp, Display, TEXT("  CriticalHitDamage: %.1f%%"), AS->GetCriticalHitDamage());
	UE_LOG(LogTemp, Display, TEXT("  CriticalHitResistance: %.1f%%"), AS->GetCriticalHitResistance());
	UE_LOG(LogTemp, Display, TEXT("  HealthRegeneration: %.1f/秒"), AS->GetHealthRegeneration());
	UE_LOG(LogTemp, Display, TEXT("  ManaRegeneration: %.1f/秒"), AS->GetManaRegeneration());
	
	UE_LOG(LogTemp, Display, TEXT("  FireResistance: %.1f%%"), AS->GetFireResistance());
	UE_LOG(LogTemp, Display, TEXT("  LightningResistance: %.1f%%"), AS->GetLightningResistance());
	UE_LOG(LogTemp, Display, TEXT("  ArcaneResistance: %.1f%%"), AS->GetArcaneResistance());
	UE_LOG(LogTemp, Display, TEXT("  PhysicalResistance: %.1f%%"), AS->GetPhysicalResistance());
	
}


