// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ARPGAbilitySource.h"
#include "Inventory/ARPGInventoryLibrary.h"
#include "Item/ARPGItemDataType.h"

// Sets default values
AARPGCharacterBase::AARPGCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	CharacterLevel = 1;

	AttributeSet = CreateDefaultSubobject<UARPGCharacterAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AddStartupGameplayAbilities();
}

void AARPGCharacterBase::AddStartupGameplayAbilities()
{
	for(TPair<FName, TSubclassOf<UARPGGameplayAbility>>& Pair : DefaultAbility)
	{
		SlottedAbility.Add(Pair.Key, {AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Pair.Value, 1, static_cast<int>(Pair.Value.GetDefaultObject()->InputID)))});
		//AbilitySystemComponent->ability
	}
	for (TSubclassOf<UGameplayEffect>& GameplayEffect : DefaultEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

// Called every frame
void AARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UpdateDesiredRotation(DeltaTime);
}

void AARPGCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!bASCInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EARPGInputs")));

		bASCInputBound = true;
	}
}

UAbilitySystemComponent* AARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AARPGCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	TScriptInterface<IARPGInventorySource> InventorySource = NewController;
	if(InventorySource)
	{
		Inventory = InventorySource->GetInventory();
		InventoryUpdateHandle = Inventory->OnInventorySyncNative.AddUObject(this, &AARPGCharacterBase::HandleInventoryUpdate);
		SlottedItemUpdateHandle = Inventory->OnSlottedItemUpdateNative.AddUObject(this, &AARPGCharacterBase::HandleSlottedItemUpdate);
	}
	else
	{
		Inventory = nullptr;
	}
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	//自己刷新一下变化
	HandleInventoryUpdate();
}

void AARPGCharacterBase::UnPossessed()
{
	Super::UnPossessed();
	if(Inventory)
	{
		Inventory->OnInventorySyncNative.Remove(InventoryUpdateHandle);
		Inventory->OnSlottedItemUpdateNative.Remove(SlottedItemUpdateHandle);
	}
}

void AARPGCharacterBase::HandleInventoryUpdate()
{
	
}

void AARPGCharacterBase::HandleSlottedItemUpdate(FName SlotName)
{
	TArray<FGameplayAbilitySpecHandle>& AbilityHandles = SlottedAbility.FindOrAdd(SlotName);

	for(FGameplayAbilitySpecHandle& AbilityHandle : AbilityHandles)
	{
		//移除技能时如果技能在执行会直接endabilitywithmark会导致从spec中错误的移除，
		//onremoveability会在cdo中执行，但是如果不在cdo中执行就会在pendingkill的instance中执行，无法触发蓝图事件
		//所以使用自定义的事件。
		FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilityHandle);
		UARPGGameplayAbility* AbilityInstance = Cast<UARPGGameplayAbility>(Spec->GetPrimaryInstance());
		if(AbilityInstance) AbilityInstance->OnUnslottedAbility();
		AbilitySystemComponent->ClearAbility(AbilityHandle);
	}
	AbilityHandles.Reset();
	if(Inventory)
	{
		FGuid CurrentItemID = Inventory->GetSlottedItem(SlotName);
		if(CurrentItemID.IsValid())
		{
			FARPGItemData ItemData = UARPGAssetManager::Get()->GetItemData(CurrentItemID);
			UARPGItem* Item = UARPGAssetManager::Get()->LoadItem(ItemData.ItemAssetId);
			check(Item);
			for(auto& Ability:Item->ItemAbility)
			{
				EARPGInputs InputID = Ability.GetDefaultObject()->InputID;
				if(Ability.GetDefaultObject()->bUseSlotInput) InputID = Inventory->GetSlot(SlotName).InputAction;
				UARPGAbilitySource* AbilitySource = NewObject<UARPGAbilitySource>();
				AbilitySource->SourceItemGID = CurrentItemID;
				FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, ItemData.Level, static_cast<int32>(InputID), AbilitySource));
				AbilityHandles.Add(Handle);
			}
			for(FGameplayAbilitySpecHandle& AbilityHandle : AbilityHandles)
			{
				FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilityHandle);
				UARPGGameplayAbility* AbilityInstance = Cast<UARPGGameplayAbility>(Spec->GetPrimaryInstance());
				if(AbilityInstance) AbilityInstance->OnSlottedAbility();
			}
		}
	}
}

bool AARPGCharacterBase::ActivateAbilityWithSlot(FName SlotName)
{
	/*
	if(SlottedAbility.Contains(SlotName))
	{
		return AbilitySystemComponent->TryActivateAbility(SlottedAbility[SlotName]);
	}
	*/
	UE_LOG(LogTemp, Warning, TEXT("UselessActivateFunction! Please use binding input to activate abilities"));
	return false;
}

bool AARPGCharacterBase::CancelAllAbilityWithSlot(FName SlotName)
{
	
	if(!SlottedAbility.Contains(SlotName)) return false;
	for(auto& Ability : SlottedAbility[SlotName])
	{
		AbilitySystemComponent->CancelAbilityHandle(Ability);
	}
	
	return true;
}

float AARPGCharacterBase::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AARPGCharacterBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AARPGCharacterBase::GetShield() const
{
	return AttributeSet->GetShield();
}

float AARPGCharacterBase::GetMaxShield() const
{
	return AttributeSet->GetMaxShield();
}

int32 AARPGCharacterBase::GetLevel() const
{
	return CharacterLevel;
}

