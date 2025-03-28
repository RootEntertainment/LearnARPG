// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGInventory.h"
#include "UObject/Interface.h"
#include "ARPGInventorySource.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UARPGInventorySource : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEARNARPG_API IARPGInventorySource
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UARPGInventory* GetInventory() = 0;
};
