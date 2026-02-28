


#include "AbilitySystem/Data/AttributeInfo.h"

#include "MyAura/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound)
{
	for (const FAuraAttributeInfo& AttrInfo : AttributeInfos)
	{
		if (AttrInfo.AttributeTag.MatchesTag(AttributeTag))
		{
			return AttrInfo;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura,Error,
			TEXT("Can't find Info for AttributeTag [%s] on this UAttributeInfo [%s]. ") ,
			*AttributeTag.ToString(),
			*GetNameSafe(this)
			);
	}
	return FAuraAttributeInfo();
}
