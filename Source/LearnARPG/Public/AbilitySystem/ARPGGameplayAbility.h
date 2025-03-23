// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGTypes.h"
#include "Abilities/GameplayAbility.h"
#include "ARPGAbilityTypes.h"
#include "ARPGGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct FARPGSetByCaller
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
};

class AARPGCharacterBase;
/**
 * 
 */
UCLASS()
class LEARNARPG_API UARPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UARPGGameplayAbility();
	//virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	//virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintPure)
	bool IsActive();

	UFUNCTION(BlueprintCallable)
	bool MontageJumpToNextSection();

	UFUNCTION(BlueprintCallable)
	bool MontageSetNextSectionWithPrefixAndSuffix(FString ToPrefix, FString ToSuffix);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bind Input")
	EARPGInputs InputID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bind Input")
	bool bUseSlotInput;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSlottedAbility();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnslottedAbility();
protected:
	int32 GetSuffixNumber(const FString& InString);

	UFUNCTION(BlueprintCallable,Category="ARPGAbility")
	TArray<FActiveGameplayEffectHandle> ApplyGameplayEffects(const FGameplayEventData& EventData, const FARPGGameplayEffectContainer& EffectContainer, const TArray<FARPGSetByCaller> SetByCallers);
};
