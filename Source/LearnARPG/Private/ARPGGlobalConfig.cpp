// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPGGlobalConfig.h"

const UARPGGlobalConfig* UARPGGlobalConfig::Get()
{
	static UARPGGlobalConfig* obj = nullptr;
	if(!obj)
	{
		obj = LoadObject<UARPGGlobalConfig>(nullptr, TEXT("/Game/LearnARPG/Blueprints/Data/DA_GlobalConfig.DA_GlobalConfig"));
		obj->AddToRoot();
	}
	return obj;
}
