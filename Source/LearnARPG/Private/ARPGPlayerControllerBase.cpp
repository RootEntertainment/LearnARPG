// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGPlayerControllerBase.h"

#include "ARPGAssetManager.h"
#include "ARPGCharacterBase.h"
#include "ARPGGlobalConfig.h"

AARPGPlayerControllerBase::AARPGPlayerControllerBase() {
	if(IsTemplate())
	{
		AssetManager = nullptr;
	}
	else
	{
		AssetManager = UARPGAssetManager::Get();
		Inventory = CreateDefaultSubobject<UARPGInventory>(TEXT("Inventory"));
	}
}

void AARPGPlayerControllerBase::BeginPlay()
{
	if(Inventory) Inventory->LoadInventory();
	Super::BeginPlay();
}

void AARPGPlayerControllerBase::SetupInputComponent() {
	Super::SetupInputComponent();
	
	BindInputActions();
	
}

void AARPGPlayerControllerBase::BindInputActions() {
	InputComponent->BindAxis("PitchCamera", this, &AARPGPlayerControllerBase::HandlePitchCamera);
	InputComponent->BindAxis("RotateCamera", this, &AARPGPlayerControllerBase::HandleRotateCamera);
	InputComponent->BindAxis("MoveForward", this, &AARPGPlayerControllerBase::HandleMoveForward);
	InputComponent->BindAxis("MoveLeft", this, &AARPGPlayerControllerBase::HandleMoveLeft);
}

void AARPGPlayerControllerBase::HandlePitchCamera(float Val) {
	AddPitchInput(Val);
}

void AARPGPlayerControllerBase::HandleRotateCamera(float Val) {
	AddYawInput(Val);
}

void AARPGPlayerControllerBase::HandleMoveForward(float Val) {
	GetPawn<APawn>()->AddMovementInput(PlayerCameraManager->GetActorForwardVector().GetSafeNormal2D(), Val);
}

void AARPGPlayerControllerBase::HandleMoveLeft(float Val) {
	GetPawn<AARPGCharacterBase>()->AddMovementInput(PlayerCameraManager->GetActorRightVector().GetSafeNormal2D(), -Val);
}

