


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		//LevelUpInformation[1] = level 1 Information
		if (LevelUpInformation.Num()-1<=level) return level;
		
		if (XP>=LevelUpInformation[level].LevelUpRequirement)
		{
			++level;
		}
		else
		{
			bSearching = false;
		}
	}
	return level;
}
