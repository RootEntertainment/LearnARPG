// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "LearnARPG/LearnARPG.h"
#include "Item/ARPGItemDataType.h"

void UARPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
	ItemDataUpdateHandle = OnItemDataUpdateNative.AddUObject(this, &UARPGAssetManager::OnItemDataUpdateFunc);

	UDataTable* AssetTypeTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(),nullptr,
		TEXT("DataTable'/Game/LearnARPG/Blueprints/Data/DT_AssetType.DT_AssetType'")));


	if(AssetTypeTable)
	{
		TArray<FARPGAssetType*> Rows;
		AssetTypeTable->GetAllRows(TEXT(""), Rows);
		for(auto Row : Rows)
		{
			ScanPathForPrimaryAssets(FName(Row->Type), FPaths::Combine(TEXT("/Game/LearnARPG/Item/"), Row->Type), Row->BaseClass, false);
		}
	}
	
}

FGuid UARPGAssetManager::CreateItemData(const FPrimaryAssetId& AssetId, int Level, int Count, UARPGInventory* InInventory, FName InSlotName)
{
	FGuid ItemId = FGuid::NewGuid();
	FARPGItemData ItemData(ItemId, Level, AssetId, Count, InInventory, InSlotName);
	AddItemData(ItemData);
	return ItemId;
}

UARPGItem* UARPGAssetManager::LoadItem(const FPrimaryAssetId& PrimaryAssetId)
{
	if(!LoadedItems.Contains(PrimaryAssetId))
	{
		LoadedItems.Add(PrimaryAssetId, ForceLoadItem(PrimaryAssetId));
	}
	return LoadedItems[PrimaryAssetId];
}

void UARPGAssetManager::AddItemData(const struct FARPGItemData& ItemData)
{
	check(!AllItemData.Contains(ItemData.GID));
	AllItemData.Add(ItemData.GID, ItemData);
}

UARPGItem* UARPGAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UARPGItem* LoadedItem = Cast<UARPGItem>(ItemPath.TryLoad());

	if (LoadedItem == nullptr)
	{
		UE_LOG(LogARPG, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}

void UARPGAssetManager::OnItemDataUpdateFunc(FGuid GID)
{
	if(AllItemData[GID].Count == 0) AllItemData.Remove(GID);
}
