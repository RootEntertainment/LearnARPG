#include "Item/ARPGItem.h"

#include "AbilitySystemComponent.h"

bool UARPGItem::IsConsumable()
{
	if (MaxCount <= 0)
	{
		return false;
	}
	return true;
}

FPrimaryAssetId UARPGItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}

