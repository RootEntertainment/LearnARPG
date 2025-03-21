// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/ARPGCharacterAttributeSet.h"

#include "ARPGCharacterBase.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UARPGCharacterAttributeSet::UARPGCharacterAttributeSet():
	Health(1),
	MaxHealth(1),
	Shield(1),
	MaxShield(1){}

void UARPGCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
}

void UARPGCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	AActor* TargetActor = nullptr;
	AARPGCharacterBase* TargetCharacter = nullptr;
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<AARPGCharacterBase>(TargetActor);
	}
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		
	}
	else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		// Clamp mana
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield()));
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