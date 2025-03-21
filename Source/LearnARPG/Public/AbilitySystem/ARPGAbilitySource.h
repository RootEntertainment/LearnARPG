// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ARPGAbilitySource.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LEARNARPG_API UARPGAbilitySource : public UObject
{
	GENERATED_BODY()
public:
	FGuid SourceItemGID;
	UFUNCTION(BlueprintPure)
	FGuid GetSourceItemGID()
	{
		return SourceItemGID;
	}
};
