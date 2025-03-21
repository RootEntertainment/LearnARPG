// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ARPGTypes.h"
#include "Item/ARPGItem.h"
#include "Item/ARPGItemDataType.h"
#include "ARPGAssetManager.generated.h"

UCLASS()
class LEARNARPG_API UARPGAssetManager : public UAssetManager
{
	GENERATED_BODY()
	friend class UARPGInventoryLibrary;
public:
	virtual void StartInitialLoading() override;

	/** Returns the current AssetManager object */
	UFUNCTION(BlueprintPure)
	static UARPGAssetManager* Get()
	{
		check(GEngine);
		UARPGAssetManager* This = Cast<UARPGAssetManager>(GEngine->AssetManager);

		check(This);
		return This;
	}

	FGuid CreateItemData(const FPrimaryAssetId& AssetId, int Level = 1, int Count = 1, class UARPGInventory* InInventory = nullptr, FName InSlotName = NAME_None);
	
	UARPGItem* LoadItem(const FPrimaryAssetId& PrimaryAssetId);
	
	const struct FARPGItemData& GetItemData(const FGuid& GID)
	{
		check(AllItemData.Contains(GID));
		return AllItemData[GID];
	}
	void AddItemData(const struct FARPGItemData& ItemData);

	void RemoveItemData(const FGuid& GID)
	{
		AllItemData.Remove(GID);
	}
	
	FOnItemDataUpdateNative OnItemDataUpdateNative;
	FOnItemDataSyncNative OnItemDataSyncNative;

	UPROPERTY(BlueprintAssignable, Category = ARPGInventory)
	FOnItemDataUpdate OnItemDataUpdate;
	
	void NotifyItemDataUpdate(FGuid ItemGID)
	{
		OnItemDataUpdateNative.Broadcast(ItemGID);
		OnItemDataUpdate.Broadcast(ItemGID);
	}
	
protected:
	TMap<FGuid, FARPGItemData> AllItemData;
	
	UPROPERTY()
	TMap<FPrimaryAssetId, UARPGItem*> LoadedItems;
	UARPGItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId);

	void OnItemDataUpdateFunc(FGuid GID);
	FDelegateHandle ItemDataUpdateHandle;
};
