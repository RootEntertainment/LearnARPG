// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGItem.h"
#include "ARPGProp.generated.h"

/**
 * 
 */
UCLASS()
class LEARNARPG_API UARPGProp : public UARPGItem
{
	GENERATED_BODY()
	UARPGProp()
	{
		ItemType = FPrimaryAssetType(TEXT("Prop"));
	}
};
