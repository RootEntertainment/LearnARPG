// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/ARPGCharacterAttributeSet.h"

#include "ARPGCharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"


UARPGCharacterAttributeSet::UARPGCharacterAttributeSet():
	Health(1),
	MaxHealth(1),
	Shield(1),
	MaxShield(1){}

void UARPGCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetShieldAttribute())
	{
		// Clamp mana
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
}

void UARPGCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	//const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	AActor* TargetActor = nullptr;
	AARPGCharacterBase* TargetCharacter = nullptr;
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<AARPGCharacterBase>(TargetActor);
	}
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the Source actor
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		AARPGCharacterBase* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			// Use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<AARPGCharacterBase>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<AARPGCharacterBase>(SourceActor);
			}
			
		}

		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHealth = GetHealth();
			const float OldShield = GetShield();
			if(LocalDamageDone <= OldShield)
			{
				SetShield(OldShield - LocalDamageDone);
			}
			else
			{
				SetShield(0);
				SetHealth(OldHealth + OldShield - LocalDamageDone);
			}
			//SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			if (TargetCharacter)
			{
				// This is proper damage
				TargetCharacter->HandleDamage(LocalDamageDone, SourceCharacter);
			}
		}
	}
	
}

void UARPGCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UARPGCharacterAttributeSet, Health);
	DOREPLIFETIME(UARPGCharacterAttributeSet, MaxHealth);
	
}

void UARPGCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGCharacterAttributeSet, Health, OldValue);
}

void UARPGCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGCharacterAttributeSet, MaxHealth, OldValue);
}

void UARPGCharacterAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGCharacterAttributeSet, Shield, OldValue);
}

void UARPGCharacterAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGCharacterAttributeSet, MaxShield, OldValue);
}

void UARPGCharacterAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGCharacterAttributeSet, Attack, OldValue);
}

void UARPGCharacterAttributeSet::OnRep_Defence(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGCharacterAttributeSet, Defence, OldValue);
}

