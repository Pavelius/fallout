#include "main.h"

struct critical_table_info {
	unsigned char		dam_mul;
	unsigned			effect;
	ability_s			ability;
	char				ability_modifier;
	unsigned			effect_failed_check;
	const char*			text;
	const char*			text_failed_check;
};
static critical_table_info critical_table[] = {{3, 0, NoStat, 0, 0, " после одного сильного удара"},
{3, BypassArmor, NoStat, 0, 0, ", броня насквозь пробита"},
{4, KnockDown|BypassArmor, NoStat, 0, 0, " без защиты брони, боец рухнул на землю"},
{4, KnockDown | BypassArmor, NoStat, 0, 0, " без защиты брони, боец рухнул на землю"},
{6, KnockDown | BypassArmor, NoStat, 0, 0, " у него перехватило дыхание, после чего он рухнул на землю"},
{6, InstantDeath, NoStat, 0, 0, ". К сожалению, его позвоночник теперь ясно виден со стороны живота"},
};