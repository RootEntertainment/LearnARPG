// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ARPGInventory.h"

#include "ARPGAssetManager.h"
#include "ARPGGlobalConfig.h"
#include "Item/ARPGItemDataType.h"

UARPGInventory::UARPGInventory()
{}

int UARPGInventory::CountItemsWithAssetId(FPrimaryAssetId AssetId)
{
	int Count = 0;
	if(ItemsMap.Contains(AssetId))
	{
		for(const FGuid& GID : ItemsMap[AssetId])
		{
			Count += UARPGAssetManager::Get()->GetItemData(GID).Count;
		}
	}
	return Count;
}

//TODO
template<typename ContainerType>
void UARPGInventory::UpdateItems(const ContainerType& GIDS)
{
	OwnedItems.Reset();
	ItemsMap.Reset();
	TypedItems.Reset();
		
	for(FGuid Gid : GIDS)
	{
		OwnedItems.Add(Gid);
		const FARPGItemData& ItemData = UARPGAssetManager::Get()->GetItemData(Gid);
		ItemsMap[ItemData.ItemAssetId].Add(Gid);
		TypedItems[ItemData.ItemAssetId.PrimaryAssetType].Add(Gid);
	}

	OnInventorySyncNative.Broadcast();
}

void UARPGInventory::LoadInventory()
{
	OwnedItems.Reset();
	TypedItems.Reset();
	ItemsMap.Reset();
	SlottedItems.Reset();

	for(auto& SlotConfig : UARPGGlobalConfig::Get()->NumOfSlots)
	{
		Slots.Add(SlotConfig);
		SlotMap.Add(SlotConfig.Name, Slots.Num()-1);
		SlottedItems.Add(SlotConfig.Name);
	}
	//LoadItems
	
	//SlotItems
	
	OnInventorySyncNative.Broadcast();
}

void UARPGInventory::AddItem(FGuid GID)
{
	const FARPGItemData& ItemData = UARPGAssetManager::Get()->GetItemData(GID);
	AddItem(ItemData);
}

void UARPGInventory::AddItem(const FARPGItemData& ARPGItemData)
{
	if(OwnedItems.Contains(ARPGItemData.GID)) return;
	// for(const FGuid& GID : ItemsMap[ARPGItemData.ItemAssetId])
	// {
	// 	const FARPGItemData& ItemData = UARPGAssetManager::Get()->GetItemData(GID);
	// 	if(ARPGItemData.IsSame(ItemData))
	// 	{
	// 		
	// 	}
	// }
	OwnedItems.Add(ARPGItemData.GID);
	ItemsMap.FindOrAdd(ARPGItemData.ItemAssetId).Add(ARPGItemData.GID);
	TypedItems.FindOrAdd(ARPGItemData.ItemAssetId.PrimaryAssetType).Add(ARPGItemData.GID);
	//OnInventoryItemUpdateNative.Broadcast(ARPGItemData.GID);
}

void UARPGInventory::RemoveItem(FGuid GID)
{
	const FARPGItemData& ItemData = UARPGAssetManager::Get()->GetItemData(GID);
	RemoveItem(ItemData);
}

void UARPGInventory::RemoveItem(const FARPGItemData& ARPGItemData)
{
	if(!OwnedItems.Contains(ARPGItemData.GID)) return;
	OwnedItems.Remove(ARPGItemData.GID);
	ItemsMap[ARPGItemData.ItemAssetId].Remove(ARPGItemData.GID);
	TypedItems[ARPGItemData.ItemAssetId.PrimaryAssetType].Remove(ARPGItemData.GID);
	//OnInventoryItemUpdateNative.Broadcast(ARPGItemData.GID);
}
