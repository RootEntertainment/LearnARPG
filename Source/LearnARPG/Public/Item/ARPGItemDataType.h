// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Inventory/ARPGInventory.h"
#include "ARPGItemDataType.generated.h"


USTRUCT(BlueprintType)
struct LEARNARPG_API FARPGItemData
{
	GENERATED_BODY()

	FARPGItemData():Level(0), Count(0)
	{}
	
	FARPGItemData(const FGuid& InGID, int InLevel, const FPrimaryAssetId& InAssetId, int InCount = 1, UARPGInventory* InInventory = nullptr, FName InSlotName = NAME_None)
		: ItemAssetId(InAssetId), GID(InGID), Level(InLevel), Count(InCount), Inventory(InInventory), CurrentSlot(InSlotName)
	{}
	
	UPROPERTY(BlueprintReadWrite)
	FPrimaryAssetId ItemAssetId;
	UPROPERTY(BlueprintReadWrite)
	FGuid GID;
	
	//动态数据
	UPROPERTY(BlueprintReadWrite)
	int Level;
	UPROPERTY(BlueprintReadWrite)
	int Count;
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UARPGInventory> Inventory;
	UPROPERTY(BlueprintReadWrite)
	FName CurrentSlot;
	
	bool IsValid() const
	{
		return GID.IsValid() && Count != 0 && ItemAssetId.IsValid();
	}

	/** Equality operators */
	bool operator==(const FARPGItemData& Other) const
	{
		return GID == Other.GID;
	}
	bool operator!=(const FARPGItemData& Other) const
	{
		return !(*this == Other);
	}

	bool IsSame(const FARPGItemData& Other) const
	{
		return ItemAssetId == Other.ItemAssetId && Level == Other.Level;
	}

	/** Implemented so it can be used in Maps/Sets */
	friend inline uint32 GetTypeHash(const FARPGItemData& Key)
	{
		return GetTypeHash(Key.GID);
	}
};

