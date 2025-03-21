// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ARPGItemActorBase.h"

#include "ARPGAssetManager.h"

// Sets default values
AARPGItemActorBase::AARPGItemActorBase() : bInitialized(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AARPGItemActorBase::InitializeFromItemGID(FGuid InGID)
{
	if(bInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Actor already initialized"));
		return;
	}
	ItemGID = InGID;
	const FARPGItemData ItemData = UARPGAssetManager::Get()->GetItemData(ItemGID);
	Level = ItemData.Level;
	Count = ItemData.Count;
	bInitialized = true;
}

// Called when the game starts or when spawned
void AARPGItemActorBase::BeginPlay()
{
	Super::BeginPlay();

	if(!bInitialized) ItemGID = UARPGAssetManager::Get()->CreateItemData(AssetId, Level, Count);
}

void AARPGItemActorBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}

