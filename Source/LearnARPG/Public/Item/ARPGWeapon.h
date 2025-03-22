// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/ARPGAbilityTypes.h"
#include "Item/ARPGItem.h"
#include "ARPGWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LEARNARPG_API UARPGWeapon : public UARPGItem
{
	GENERATED_BODY()
public:
	UARPGWeapon()
	{
		ItemType = FPrimaryAssetType("Weapon");
	}
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	// TSubclassOf<AActor> WeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	TMap<FGameplayTag, FARPGGameplayEffectContainer> WeaponEffects;
	
};
