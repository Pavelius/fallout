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
static critical_table_info critical_table[] = {{3, 0, NoStat, 0, 0, " ����� ������ �������� �����"},
{3, BypassArmor, NoStat, 0, 0, ", ����� �������� �������"},
{4, KnockDown|BypassArmor, NoStat, 0, 0, " ��� ������ �����, ���� ������ �� �����"},
{4, KnockDown | BypassArmor, NoStat, 0, 0, " ��� ������ �����, ���� ������ �� �����"},
{6, KnockDown | BypassArmor, NoStat, 0, 0, " � ���� ����������� �������, ����� ���� �� ������ �� �����"},
{6, InstantDeath, NoStat, 0, 0, ". � ���������, ��� ����������� ������ ���� ����� �� ������� ������"},
};