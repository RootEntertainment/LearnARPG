// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ARPGDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class LEARNARPG_API UARPGDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UARPGDamageExecution();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
