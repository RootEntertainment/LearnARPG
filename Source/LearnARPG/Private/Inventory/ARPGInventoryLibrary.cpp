// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ARPGInventoryLibrary.h"
#include "ARPGAssetManager.h"
#include "ARPGCharacterBase.h"


UARPGItem* UARPGInventoryLibrary::GetItemByGID(const FGuid& GID)
{
	if(!GID.IsValid()) return nullptr;
	return UARPGAssetManager::Get()->LoadItem(UARPGAssetManager::Get()->GetItemData(GID).ItemAssetId);
}

FARPGItemData UARPGInventoryLibrary::GetItemDataByGID(const FGuid& GID)
{
	return UARPGAssetManager::Get()->GetItemData(GID);
}

bool UARPGInventoryLibrary::GiveItemsToInventory(UARPGInventory* Inventory, const FGuid& GID, int Count)
{
	if(!Inventory || Inventory->IsPendingKill()) return false;
	if(Inventory->OwnedItems.Contains(GID)) return false;
	
	FARPGItemData& ItemData = UARPGAssetManager::Get()->AllItemData[GID];
	if(ItemData.Count < Count) return false;
	if(Count == 0) Count = ItemData.Count;
	
	UARPGItem* Item = UARPGAssetManager::Get()->LoadItem(ItemData.ItemAssetId);
	check(Item);

	FGuid InventoryGID;
	//可能合并
	if(Item->IsConsumable())
	{
		bool bFound = false;
		for(const FGuid& OtherGID : Inventory->ItemsMap[ItemData.ItemAssetId])
		{
			FARPGItemData& OtherItem = UARPGAssetManager::Get()->AllItemData[OtherGID];
			if(OtherItem.IsSame(ItemData))
			{
				bFound = true;
				InventoryGID = OtherGID;
				
				OtherItem.Count += Count;
				ItemData.Count -= Count;
				
				break;
			}
		}
		if(!bFound)
		{
			ItemData.Count -= Count;
			//这里有一个隐患ItemData引用可能会随着扩容失效，即使这里之后再没用过ItemData了，不应该这么写。
			InventoryGID = UARPGAssetManager::Get()->CreateItemData(ItemData.ItemAssetId, ItemData.Level, Count, Inventory);
			Inventory->AddItem(InventoryGID);
		}
		
		UARPGAssetManager::Get()->NotifyItemDataUpdate(GID);
		UARPGAssetManager::Get()->NotifyItemDataUpdate(InventoryGID);
	}
	else
	{
		Inventory->AddItem(ItemData);
		ItemData.Inventory = Inventory;
		InventoryGID = GID;
	}
	Inventory->NotifyInventoryItemUpdate(InventoryGID);
	return true;
}

bool UARPGInventoryLibrary::ConsumeItemsInInventory(UARPGInventory* Inventory, const FGuid& GID, int Count)
{
	if(!Inventory) return false;
	if(!Inventory->OwnedItems.Contains(GID)) return false;
	FARPGItemData& ItemData = UARPGAssetManager::Get()->AllItemData[GID];
	if(ItemData.Count < Count) return false;
	ItemData.Count -= Count;
	if(Count == 0 || ItemData.Count == Count)
	{
		Inventory->RemoveItem(ItemData);
		ItemData.Inventory = nullptr;
		//TODO: Remove Item Data
	}
	
	UARPGAssetManager::Get()->NotifyItemDataUpdate(GID);
	Inventory->NotifyInventoryItemUpdate(GID);
	return true;
}

bool UARPGInventoryLibrary::SetSlottedItem(UARPGInventory* Inventory, FName SlotName, const FGuid& GID)
{
	const FARPGItemSlot& ItemSlot = Inventory->GetSlot(SlotName);
	FARPGItemData& ItemData = UARPGAssetManager::Get()->AllItemData[GID];
	if(	!Inventory->OwnedItems.Contains(GID) ||
		!Inventory->SlotMap.Contains(SlotName) ||
		ItemSlot.SlotType != ItemData.ItemAssetId.PrimaryAssetType) return false;

	FGuid CurrentItemGID = Inventory->SlottedItems[SlotName];
	if (CurrentItemGID.IsValid())
	{
		bool bFound = false;
		FARPGItemData& CurrentItemData = UARPGAssetManager::Get()->AllItemData[CurrentItemGID];
		for(TPair<FName, FGuid>& Pair: Inventory->SlottedItems)
		{
			if(Pair.Value == GID)
			{
				Pair.Value = CurrentItemGID;
				CurrentItemData.CurrentSlot = Pair.Key;
				UARPGAssetManager::Get()->NotifyItemDataUpdate(CurrentItemGID);
				Inventory->NotifySlottedItemUpdate(Pair.Key);
				bFound = true;
				break;
			}
		}
		if(!bFound)
		{
			CurrentItemData.CurrentSlot = NAME_None;
			UARPGAssetManager::Get()->NotifyItemDataUpdate(CurrentItemGID);
		}
	}
	Inventory->SlottedItems[SlotName] = GID;
	ItemData.CurrentSlot = SlotName;
	UARPGAssetManager::Get()->NotifyItemDataUpdate(GID);
	Inventory->NotifySlottedItemUpdate(SlotName);
	return true;
}

bool UARPGInventoryLibrary::RemoveSlottedItem(UARPGInventory* Inventory, FName SlotName)
{
	if(!Inventory->SlotMap.Contains(SlotName)) return false;
	FGuid GID = Inventory->SlottedItems[SlotName];
	if(!GID.IsValid()) return false;
	
	FARPGItemData& ItemData = UARPGAssetManager::Get()->AllItemData[GID];
	ItemData.CurrentSlot = NAME_None;
	Inventory->SlottedItems[SlotName].Invalidate();
	UARPGAssetManager::Get()->NotifyItemDataUpdate(GID);
	Inventory->NotifySlottedItemUpdate(SlotName);
	return true;
}

UARPGItem* UARPGInventoryLibrary::GetSlottedItem(UARPGInventory* Inventory, FName SlotName)
{
	if(Inventory->SlottedItems.Contains(SlotName)) return GetItemByGID(Inventory->SlottedItems[SlotName]);
	else return nullptr;
}

void UARPGInventoryLibrary::GetSlottedPrimaryAbilityHandleAndInstanceByClass(AARPGCharacterBase* Character,
	FName SlotName, TSubclassOf<UARPGGameplayAbility> Class, FGameplayAbilitySpecHandle& OutHandle, UARPGGameplayAbility*& OutAbilityInstance)
{
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if(!Character->SlottedAbility.Contains(SlotName) || !ASC)
	{
		OutAbilityInstance = nullptr;
		return;
	}
	for(auto& AbilityHandle : Character->SlottedAbility[SlotName])
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(AbilityHandle);
		if(Spec && Spec->Ability->GetClass() == Class)
		{
			OutAbilityInstance = Cast<UARPGGameplayAbility>(Spec->GetPrimaryInstance());
			OutHandle = AbilityHandle;
			return;
		}
	}
	OutAbilityInstance = nullptr;
}

TArray<FGameplayAbilitySpecHandle> UARPGInventoryLibrary::GetAllSlottedAbilityHandles(AARPGCharacterBase* Character, FName SlotName)
{
	if(!Character->SlottedAbility.Contains(SlotName)) return TArray<FGameplayAbilitySpecHandle>();
	return Character->SlottedAbility[SlotName];
}

FGameplayAbilitySpecHandle UARPGInventoryLibrary::GetSlottedAbilityHandlesByClass(AARPGCharacterBase* Character, FName SlotName,
	TSubclassOf<UARPGGameplayAbility> Class)
{
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if(!Character->SlottedAbility.Contains(SlotName) || !ASC) return FGameplayAbilitySpecHandle();
	for(auto& AbilityHandle : Character->SlottedAbility[SlotName])
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(AbilityHandle);
		if(Spec && Spec->Ability->GetClass() == Class)
		{
			return AbilityHandle;
		}
	}
	return FGameplayAbilitySpecHandle();
}
