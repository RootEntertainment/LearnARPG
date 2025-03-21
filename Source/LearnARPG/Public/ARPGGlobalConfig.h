// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGTypes.h"
#include "Engine/DataAsset.h"
#include "ARPGGlobalConfig.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class LEARNARPG_API UARPGGlobalConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, DisplayName="ARPGGlobalConfig")
	static const UARPGGlobalConfig* Get();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FARPGItemSlot> NumOfSlots;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementSetting MovementSetting;
	
};
