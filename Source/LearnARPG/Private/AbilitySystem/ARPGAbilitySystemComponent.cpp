// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ARPGAbilitySystemComponent.h"

UARPGAbilitySystemComponent::UARPGAbilitySystemComponent()
{
	
}

bool UARPGAbilitySystemComponent::TryActivateAbilityByHandle(FGameplayAbilitySpecHandle Handle)
{
	return TryActivateAbility(Handle);
}

