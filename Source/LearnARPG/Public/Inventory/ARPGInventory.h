// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGTypes.h"
#include "ARPGInventory.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LEARNARPG_API UARPGInventory : public UObject
{
	GENERATED_BODY()
	friend class UARPGInventoryLibrary;
public:
	UARPGInventory();
	
	UFUNCTION(BlueprintCallable, Category = ARPGInventory)
	const TSet<FGuid>& GetItemsWithType(FPrimaryAssetType Type)
	{
		return TypedItems[Type];
	}
	
	void GetItemsWithType(FPrimaryAssetType Type, TArray<FGuid>&OutArray)
	{
		OutArray = TypedItems[Type].Array();
	}
	UFUNCTION(BlueprintCallable, Category = ARPGInventory)
	int CountItemsWithAssetId(FPrimaryAssetId AssetId);

	void LoadInventory();
	
	//TODO
	template<typename ContainerType>
	void UpdateItems(const ContainerType& GIDS);
	
protected:
	//不应该直接调用，应该使用ARPGInventoryLibrary
	void AddItem(FGuid GID);
	void AddItem(const struct FARPGItemData& ARPGItemData);
	void RemoveItem(FGuid GID);
	void RemoveItem(const struct FARPGItemData& ARPGItemData);
	//Inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ARPGInventory)
	TSet<FGuid> OwnedItems;
	TMap<FPrimaryAssetId, TSet<FGuid>> ItemsMap;
	TMap<FPrimaryAssetType, TSet<FGuid>> TypedItems;
	
public:
	const FARPGItemSlot& GetSlot(FName SlotName)
	{
		return Slots[SlotMap[SlotName]];
	}
	const FGuid GetSlottedItem(FName SlotName)
	{
		return SlottedItems[SlotName];
	}
	//const void SetSlottedItem(FName SlotName, const FGuid& ItemGID);
protected:
	//Equips
	//优化蓝图遍历
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ARPGInventory)
	TArray<FARPGItemSlot> Slots;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ARPGInventory)
	TMap<FName, int32> SlotMap;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ARPGInventory)
	TMap<FName, FGuid> SlottedItems;

public:
	FOnInventoryItemUpdateNative OnInventoryItemUpdateNative;
	FOnInventorySyncNative OnInventorySyncNative;
	FOnSlottedItemUpdateNative OnSlottedItemUpdateNative;

	UPROPERTY(BlueprintAssignable, Category = ARPGInventory)
	FOnInventoryItemUpdate OnInventoryItemUpdate;
	UPROPERTY(BlueprintAssignable, Category = ARPGInventory)
	FOnSlottedItemUpdate OnSlottedItemUpdate;
	
	void NotifyInventoryItemUpdate(FGuid ItemGID) const
	{
		OnInventoryItemUpdateNative.Broadcast(ItemGID);
		OnInventoryItemUpdate.Broadcast(ItemGID);
	}
	void NotifySlottedItemUpdate(FName SlotName) const
	{
		OnSlottedItemUpdateNative.Broadcast(SlotName);
		OnSlottedItemUpdate.Broadcast(SlotName);
	}
	
};