// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ARPGGameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "ARPGCharacterBase.h"
//#include "Item/ARPGWeapon.h"


UARPGGameplayAbility::UARPGGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

// void UARPGGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
// {
// 	Super::OnGiveAbility(ActorInfo, Spec);
// 	AARPGCharacterBase* Actor = Cast<AARPGCharacterBase>(ActorInfo->AvatarActor.Get());
// 	if (Actor)
// 	{
// 		OnGiveAbility(Actor, Spec);
// 	}
// }
//这个函数会在Pendingkill的情况下执行所以没法执行蓝图事件
/*
void UARPGGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
	AARPGCharacterBase* Actor = Cast<AARPGCharacterBase>(ActorInfo->AvatarActor.Get());
	if (Actor)
	{
		OnRemoveAbility(Actor, Spec);
	}
}
*/
bool UARPGGameplayAbility::IsActive()
{
	return Super::IsActive();
}

//deperated
bool UARPGGameplayAbility::MontageJumpToNextSection()
{
	check(CurrentActorInfo);

	UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo ? CurrentActorInfo->AbilitySystemComponent.Get() : nullptr;
	UAnimInstance* AnimInstance = CurrentActorInfo ? CurrentActorInfo->GetAnimInstance() : nullptr;
	UAnimMontage* Montage = GetCurrentMontage();
	if(AbilitySystemComponent && AnimInstance && Montage)
	{
		float CurrentPosition = AnimInstance->Montage_GetPosition(Montage);
		int32 CurrentSection = Montage->GetSectionIndexFromPosition(CurrentPosition);
		CurrentSection++;
		FName NextSection = Montage->GetSectionName(CurrentSection);
		if(NextSection != NAME_None)
		{
			AbilitySystemComponent->CurrentMontageJumpToSection(NextSection);
			return true;
		}
	}
	return false;
}

bool UARPGGameplayAbility::MontageSetNextSectionWithPrefixAndSuffix(FString ToPrefix, FString ToSuffix)
{
	check(CurrentActorInfo);

	UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo ? CurrentActorInfo->AbilitySystemComponent.Get() : nullptr;
	UAnimInstance* AnimInstance = CurrentActorInfo ? CurrentActorInfo->GetAnimInstance() : nullptr;
	UAnimMontage* Montage = GetCurrentMontage();
	if(AbilitySystemComponent && AnimInstance && Montage)
	{
		float CurrentPosition = AnimInstance->Montage_GetPosition(Montage);
		int32 CurrentSection = Montage->GetSectionIndexFromPosition(CurrentPosition);
		FName CurrentSectionName = Montage->GetSectionName(CurrentSection);
		if(CurrentSectionName != NAME_None)
		{
			int32 CurrentNumber = GetSuffixNumber(CurrentSectionName.ToString());
			CurrentNumber++;
			FName NextSection(ToPrefix + "_" + ToSuffix + "_" + FString::FromInt(CurrentNumber));
			AbilitySystemComponent->CurrentMontageSetNextSectionName(CurrentSectionName, NextSection);
			return true;
		}
	}
	return false;
}

int32 UARPGGameplayAbility::GetSuffixNumber(const FString& InString)
{
	int32 LastUnderscoreIndex = 0;
	InString.FindLastChar('_', LastUnderscoreIndex);
	if (LastUnderscoreIndex != INDEX_NONE)
	{
		FString Substring = InString.Mid(LastUnderscoreIndex + 1);
		if (Substring.IsNumeric())
		{
			return FCString::Atoi(*Substring);
		}
	}
	return -1;
}

TArray<FActiveGameplayEffectHandle> UARPGGameplayAbility::ApplyGameplayEffects(const FGameplayEventData& EventData,
	const FARPGGameplayEffectContainer& EffectContainer, const TArray<FARPGSetByCaller> SetByCallers)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;
	
	UAbilitySystemComponent* TargetComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EventData.Target);

	if(TargetComponent)
	{
		for (auto& GameplayEffectClass : EffectContainer.TargetGameplayEffectClasses)
		{
			//这里会将Ability的SetByCaller复制给GameplayEffect
			FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(GameplayEffectClass, GetAbilityLevel());
			FGameplayEffectSpec* Spec = EffectSpec.Data.Get();
			for(auto& SetByCaller : SetByCallers)
			{
				Spec->SetSetByCallerMagnitude(SetByCaller.Key, SetByCaller.Value);
			}
			
			AllEffects.Add(GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), TargetComponent));
		}
	}
	// Iterate list of effect specs and apply them to their target data
	
	return AllEffects;
}
