// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGItem.h"
#include "ARPGToken.generated.h"

/**
 * 
 */
UCLASS()
class LEARNARPG_API UARPGToken : public UARPGItem
{
	GENERATED_BODY()

	UARPGToken()
	{
		ItemType = FPrimaryAssetType("Token");
	}
};
