// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPGCharacterBase.h"
#include "GameFramework/Actor.h"
#include "ARPGItemActorBase.generated.h"

UCLASS()
class LEARNARPG_API AARPGItemActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARPGItemActorBase();

	UPROPERTY(EditAnywhere)
	FPrimaryAssetId AssetId;
	UPROPERTY(EditAnywhere)
	int Count = 1;
	UPROPERTY(EditAnywhere)
	int Level = 1;
	
	UFUNCTION(BlueprintCallable)
	void InitializeFromItemGID(FGuid InGID);
protected:
	bool bInitialized;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(BlueprintReadOnly)
	FGuid ItemGID;
};
