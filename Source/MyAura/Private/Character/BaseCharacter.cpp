#include "Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyAura/MyAura.h"
#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	FireDebuffNiagaraComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("FireDebuffNiagaraComponent");
	FireDebuffNiagaraComponent->SetupAttachment(GetRootComponent());
	FireDebuffNiagaraComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Fire;
	
	LightningDebuffNiagaraComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("LightningDebuffNiagaraComponent");
	LightningDebuffNiagaraComponent->SetupAttachment(GetRootComponent());
	LightningDebuffNiagaraComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Lightning;
	
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon= CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),"WeaponHandSocket");
	Weapon->SetCollisionEnabled((ECollisionEnabled::NoCollision));
	
	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("ScentComponent");
	EffectAttachComponent ->SetupAttachment(GetRootComponent());
	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloOfProtectionComp");
	HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);
	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonComp");
	LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphonComp");
	ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABaseCharacter,bIsStunned);
	DOREPLIFETIME(ABaseCharacter,bIsBurned);
	DOREPLIFETIME(ABaseCharacter,bBingShockLoop);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ABaseCharacter::Die(const FVector& DeathImpulse)
{
	//武器分离，自动同步复制
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	MulticastHandleDeath(DeathImpulse);
}

void ABaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this,DeathSound,GetActorLocation(),GetActorRotation());
	
	//设置角色死亡后的一些物理状态
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse*0.1f,NAME_None,true);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse,NAME_None,true);  //忽略质量 
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolove();
	FireDebuffNiagaraComponent->Deactivate();
	LightningDebuffNiagaraComponent->Deactivate();
	bDead=true;
	OnDeath.Broadcast(this);
	OnDeathDelegate.Broadcast(this);
}

TArray<FTaggedMontage> ABaseCharacter::GetTaggedMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* ABaseCharacter::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage ABaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTage == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 ABaseCharacter::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ABaseCharacter::IncreateMinionCount_Implementation(int32 Count)
{
	MinionCount+=Count;
}

void ABaseCharacter::SetMaster_Implementation(AActor* InMaster)
{
	Master= InMaster;
}

ECharacterClass ABaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered& ABaseCharacter::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeath& ABaseCharacter::GetOnDeathDelegate()
{
	return OnDeath;
}

USkeletalMeshComponent* ABaseCharacter::GetWeapon_Implementation()
{
	return Weapon;
}

FOnDeathSignature& ABaseCharacter::GetOnDeathDelegate2()
{
	return OnDeathDelegate;
}

void ABaseCharacter::SetIsBeingShock_Implementation(bool bInShock)
{
	bBingShockLoop = bInShock;
}

bool ABaseCharacter::GetIsBeingShock_Implementation() const
{
	return bBingShockLoop;
}

FOnTakeDamageSignature& ABaseCharacter::GetOnTakeDamageSignature()
{
	return OnTakeDamageDelegate;
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

float ABaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnTakeDamageDelegate.Broadcast(Damage);
	return Damage;
}

FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	//可以用TMap
	const FAuraGameplayTags& GameplayTag = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTag.CombatSocket_Weapon) && IsValid(Weapon))
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	
	if (MontageTag.MatchesTagExact(GameplayTag.CombatSocket_LeftHand) )
		return GetMesh()->GetSocketLocation(LeftHandTipSocketName);
	
	if (MontageTag.MatchesTagExact(GameplayTag.CombatSocket_RightHand))
		return GetMesh()->GetSocketLocation(RightHandTipSocketName);
	
	if (MontageTag.MatchesTagExact(GameplayTag.CombatSocket_Tail))
		return GetMesh()->GetSocketLocation(TailTipSocketName);
	return FVector();
}

bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* ABaseCharacter::GetAvatar_Implementation()
{
	return this;
}

void ABaseCharacter::InitAbilityActorInfo()
{
	
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffect)
	//Get Effect Spec
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this); //这里的Source是Aura or Enemy
	const FGameplayEffectSpecHandle SpecHandle =  GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect,level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void ABaseCharacter::InitializeDefaultAttribute() const 
{
	ApplyEffectToSelf(DefaultPrimaryAttributeEffect,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributeEffect,1.f);
	ApplyEffectToSelf(DefaultBaseAttributeEffect,1.f);
}

void ABaseCharacter::AddCharacterAbilities()
{
	//只能在服务端添加Ability
	if (!HasAuthority()) return;
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	
	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
	
} 

void ABaseCharacter::Dissolove()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0, DynamicMaterialInstance);
		StartDissolveTimeLine(DynamicMaterialInstance);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		Weapon->SetMaterial(0, DynamicMaterialInstance);
		StartWeaponDissolveTimeLine(DynamicMaterialInstance);
	}
}

void ABaseCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount> 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f: BaseWalkSpeed;
	
}

void ABaseCharacter::OnRep_Stunned()
{
}

void ABaseCharacter::OnRep_Burned()
{
}

void ABaseCharacter::OnRep_bInShockLoop()
{
	
}





