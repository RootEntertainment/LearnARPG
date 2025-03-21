// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGItem.h"
#include "ARPGSkill.generated.h"

/**
 * 
 */
UCLASS()
class LEARNARPG_API UARPGSkill : public UARPGItem
{
	GENERATED_BODY()
public:
	UARPGSkill()
	{
		ItemType = FPrimaryAssetType("Skill");
	}
};
