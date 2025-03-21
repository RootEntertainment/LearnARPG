// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ARPGTypes.generated.h"

UENUM(BlueprintType)
enum class EARPGInputs : uint8
{
	None,
	Jump,
	Interact,
	Run,
	Crouch,
	PrimaryAttack,
	PrimarySkill,
	SecondarySkill,
	Prop1,
	Prop2,
	Prop3,
	Table,
};

UENUM(BlueprintType)
enum class EARPGWeapons : uint8
{
	None,
	Rifle,
	Light,
	Heavy,
};

USTRUCT(BlueprintType)
struct FMovementSetting
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed;
};


USTRUCT(BlueprintType)
struct LEARNARPG_API FARPGItemSlot
{
	GENERATED_BODY()
	
	FARPGItemSlot():MaxCount(-1)
	{}

	FARPGItemSlot(const FName& InName, const FPrimaryAssetType& InSlotType, const FText& Title, EARPGInputs InputAction)
		: Name(InName), MaxCount(0), SlotType(InSlotType), Title(Title), InputAction(InputAction)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EARPGInputs InputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPrimaryAssetType SlotType;
	
	/** Equality operators */
	bool operator==(const FARPGItemSlot& Other) const
	{
		return Name == Other.Name;
	}
	bool operator!=(const FARPGItemSlot& Other) const
	{
		return !(*this == Other);
	}

	/** Implemented so it can be used in Maps/Sets */
	friend inline uint32 GetTypeHash(const FARPGItemSlot& Key)
	{
		return GetTypeHash(Key.Name);
	}

	/** Returns true if slot is valid */
	bool IsValid() const
	{
		return SlotType.IsValid() && MaxCount >= 0;
	}
};

USTRUCT(BlueprintType)
struct FARPGAssetType :public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Type;

	UPROPERTY(EditAnywhere)
	UClass* BaseClass;
};

//物品数据更新
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemDataUpdateNative, FGuid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDataUpdate, FGuid, ItemGID);
//物品数据同步
DECLARE_MULTICAST_DELEGATE(FOnItemDataSyncNative);

//库存物品数据更新（add/remove/update）
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemUpdateNative, FGuid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemUpdate, FGuid, ItemGID);
//库存刷新
DECLARE_MULTICAST_DELEGATE(FOnInventorySyncNative);
//装备切换
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSlottedItemUpdateNative, FName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlottedItemUpdate, FName, SlotName);

// USTRUCT(BlueprintType)
// struct FARPGWeaponStruct :public FTableRowBase
// {
// 	GENERATED_BODY()
// public:
// 	UPROPERTY(EditAnywhere)
// 	FPrimaryAssetId ItemAsset;
// 	
// 	UPROPERTY(EditAnywhere)
// 	FString Name;
//
// 	UPROPERTY(EditAnywhere)
// 	int32 Damage;
// };


//DECLARE_MULTICAST_DELEGATE(FOnItemDataSync);


// USTRUCT(BlueprintType)
// struct LEARNARPG_API FARPGItemSlot
// {
// 	GENERATED_BODY()
// 	
// 	FARPGItemSlot()
// 		: ItemData(nullptr)
// 	{}
//
// 	FARPGItemSlot(const UARPGItemData* InItem, const TArray<uint32>& InGIDs)
// 		: ItemData(InItem), ItemGIDs(InGIDs)
// 	{}
// 	
// 	//装入的物品
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
// 	const UARPGItemData* ItemData;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
// 	TArray<uint32> ItemGIDs;
// 	
// 	/** Returns true if slot is valid */
// 	UFUNCTION(BlueprintCallable)
// 	bool IsEmpty() const
// 	{
// 		return ItemData == nullptr;
// 	}
// };