// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGAssetManager.h"
#include "Inventory/ARPGInventory.h"
#include "Inventory/ARPGInventorySource.h"
#include "GameFramework/PlayerController.h"
#include "ARPGPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class LEARNARPG_API AARPGPlayerControllerBase : public APlayerController, public IARPGInventorySource
{
	GENERATED_BODY()
public:
	AARPGPlayerControllerBase();
	
	virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintPure)
	virtual UARPGInventory* GetInventory() override
	{
		return Inventory;
	}
protected:
	virtual void SetupInputComponent() override;
	void BindInputActions();
	void HandleRotateCamera(float Val);
	void HandlePitchCamera(float Val);
	void HandleMoveForward(float Val);
	void HandleMoveLeft(float Val);

	//Inventory
	UPROPERTY()
	UARPGAssetManager* AssetManager;
	UPROPERTY()
	UARPGInventory* Inventory;
};
