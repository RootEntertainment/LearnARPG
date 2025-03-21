// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ARPGAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class LEARNARPG_API UARPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UARPGAbilitySystemComponent();

	UFUNCTION(BlueprintCallable, Category = "ARPGAbilitySystemComponent")
	bool TryActivateAbilityByHandle(FGameplayAbilitySpecHandle Handle);
};
