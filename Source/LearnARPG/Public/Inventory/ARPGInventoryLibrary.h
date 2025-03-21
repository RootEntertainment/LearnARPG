// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGInventory.h"
#include "AbilitySystem/ARPGGameplayAbility.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Item/ARPGItemDataType.h"

#include "ARPGInventoryLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LEARNARPG_API UARPGInventoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static class UARPGItem* GetItemByGID(const FGuid& GID);

	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static FARPGItemData GetItemDataByGID(const FGuid& GID);
	
	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static bool GiveItemsToInventory(UARPGInventory* Inventory, const FGuid& GID, int Count = 0);
	
	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static bool ConsumeItemsInInventory(UARPGInventory* Inventory, const FGuid& GID, int Count = 0);
	
	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static bool SetSlottedItem(UARPGInventory* Inventory, FName SlotName, const FGuid& GID);

	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static bool RemoveSlottedItem(UARPGInventory* Inventory, FName SlotName);

	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static class UARPGItem* GetSlottedItem(UARPGInventory* Inventory, FName SlotName);

	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static TArray<FGameplayAbilitySpecHandle> GetAllSlottedAbilityHandles(AARPGCharacterBase* Character, FName SlotName);
	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static FGameplayAbilitySpecHandle GetSlottedAbilityHandlesByClass(AARPGCharacterBase* Character, FName SlotName, TSubclassOf<UARPGGameplayAbility> Class);
	UFUNCTION(BlueprintCallable, Category = "ARPGInventory")
	static void GetSlottedPrimaryAbilityHandleAndInstanceByClass(AARPGCharacterBase* Character, FName SlotName, TSubclassOf<UARPGGameplayAbility> Class, FGameplayAbilitySpecHandle& OutHandle, UARPGGameplayAbility*& OutAbilityInstance);
};
