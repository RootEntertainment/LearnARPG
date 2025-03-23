// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ARPGDamageExecution.h"
#include "AbilitySystem/ARPGCharacterAttributeSet.h"
struct ARPGDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	//DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	ARPGDamageStatics()
	{
		// Capture the Target's DefensePower attribute. Do not snapshot it, because we want to use the health value at the moment we apply the execution.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARPGCharacterAttributeSet, Defence, Target, false);

		// Capture the Source's AttackPower. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		// the projectile was launched, not when it hits).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UARPGCharacterAttributeSet, Attack, Source, true);

		// Also capture the source's raw Damage, which is normally passed in directly via the execution
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UARPGCharacterAttributeSet, Damage, Source, true);
	}
};

static const ARPGDamageStatics& DamageStatics()
{
	static ARPGDamageStatics DmgStatics;
	return DmgStatics;
}

UARPGDamageExecution::UARPGDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().DefenceDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
	//RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UARPGDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	//float EffectBaseDamage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, 1.0f);

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// --------------------------------------
	//	Damage Done = Damage * AttackPower / DefensePower
	//	If DefensePower is 0, it is treated as 1.0
	// --------------------------------------

	float DefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenceDef, EvaluationParameters, DefensePower);
	if (DefensePower == 0.0f)
	{
		DefensePower = 1.0f;
	}

	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDef, EvaluationParameters, AttackPower);
	
	float DamageDone = AttackPower / DefensePower;
	if (DamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UARPGCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
	}
}
