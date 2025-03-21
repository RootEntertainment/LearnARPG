// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/ARPGGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "ARPGItem.generated.h"

/**
 * 一种Item的基本数据，名称、UI图标等
 */
UCLASS()
class LEARNARPG_API UARPGItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FSlateBrush ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Count)
	int MaxCount;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Count)
	bool IsConsumable();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UARPGGameplayAbility>> ItemAbility;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
};
