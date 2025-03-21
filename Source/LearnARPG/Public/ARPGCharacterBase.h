// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "ARPGTypes.h"
#include "ARPGPlayerControllerBase.h"
#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "AbilitySystem/ARPGCharacterAttributeSet.h"
#include "ARPGCharacterBase.generated.h"

UCLASS()
class LEARNARPG_API AARPGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	friend class UARPGInventoryLibrary;
public:
	// Sets default values for this character's properties
	AARPGCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	
	void HandleInventoryUpdate();
	void HandleSlottedItemUpdate(FName SlotName);

	UFUNCTION(BlueprintCallable)
	bool ActivateAbilityWithSlot(FName SlotName);
	UFUNCTION(BlueprintCallable)
	bool CancelAllAbilityWithSlot(FName SlotName);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, TSubclassOf<UARPGGameplayAbility>> DefaultAbility;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffect;


	/** Returns current health, will be 0 if dead */
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetShield() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxShield() const;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetLevel() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UARPGAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	UARPGCharacterAttributeSet* AttributeSet;

	bool bASCInputBound;

	void AddStartupGameplayAbilities();

	UPROPERTY(EditAnywhere, Category = Abilities)
	int32 CharacterLevel;
	
	//Equipment
	UPROPERTY(BlueprintReadOnly)
	UARPGInventory* Inventory;
	
	TMap<FName, TArray<FGameplayAbilitySpecHandle>> SlottedAbility;

	FDelegateHandle InventoryUpdateHandle;
	FDelegateHandle SlottedItemUpdateHandle;

	TMap<FName, FName> InputMap;
};
