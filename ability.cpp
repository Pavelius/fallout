#include "main.h"

ability_info ability_data[] = {{"NoStat", -1, "��� ��������"},
{"Strenght", 0, "����", "����", "��", false, "������ ���������� ����. X����� �������� ��� ������ � �� ��������. ��������� ������� �� ���� ������, ��������� ����������� � ��������� �����."},
{"Perception", 1, "��������", "���.", "��", false, "����������� ����� ������, �������, �������� � ��������������� �������. ����������� �������� ��� ������� ��������. ��������� ������� �� ��������� �������� � ������� ��������."},
{"Endurance", 2, "������������", "������.", "��", false, "������������ �������� ���������� ������ � ������� ������ ����� � ���������� ������������. ��������� ������� �� ���� ������, ������������ � ��� � ��������, ������� ������� � ��."},
{"Charisma", 3, "�������", "�������", "��", false, "������� ������������ ����� ���������� ��������� � �����. ������� ������� ���������� ��� ��� ����������, ������� ������ ����� �������� �������. ������ �� ������� ����� � ���� �������."},
{"Intellegence", 4, "���������", "�����.", "��", false, "��������� �������� �� ������, �������� ������� � ����������� ��������� �������. ������� ��������� ����� ��� ����. ������ �� ���������� ����� ������ �� �������, ������ ������� � ������ �����������."},
{"Agility", 5, "��������", "����.", "��", false, "�������� � ����������� �������� �� ��� ������ ��� �� ������ ������. ������ �������� ����� ��������� � ��������. ������ �� ���� ��������, ����� �����, ������� �������� � ��."},
{"Luck", 6, "�����", "�����", "��", false, "������� ����������� ������ ������������ �� ���� ������. ������� ��� ������ ����������� ����� ����������, ��� ��� ������� � ����, ��� ����� ����������� ���������� �������."},
{"HPMax", 10, "���� ������", "������", "��", false, "���������� �����������, ���������� ������ ��������� ����� ���, ��� ��� �����. ���� ����� ��� ������ �������� ������."},
{"APMax", 19, "���� ��������", "���� ��������", "��", false, "���������� ����� ��������, ������� �������� ����� ������������ �� ����� ������ ���."},
{"AC", 18, "����� �����", "����� �����", "��", true, "������ �� ����������� �������� ������� ����������� ���������."},
{"DamageUnarmed", 21, "����������� � ���������� ���", "���. ����", "��", false, "���������� �����������, ����������� ����� ���������� � ���������� ���."},
{"DamageMelee", 21, "����������� � ������� ���", "���. ����", "��", false, "���������� �����������, ����������� ����� ���������� � ������� ���."},
{"CarryWeight", 20, "������������ ����", "����. ����", "��", false, "������������ ��� �������, ��������� ����� ����������, � ������ (373 ��. �� ��.)"},
{"Sequence", 24, "������� ����", "�������", "��", false, "���������� ������� ������ �������� �������� ������ ���������."},
{"HealingRate", 10, "������� �������", "������� ���.", "��", false, "� ����� ���, ��� �������� ����� ��������� �� 1 ����� ��� ������� ���� ������ �������. �� ����� ������ �� �������� ������ ����� �����."},
{"CriticalHit", 26, "���� ������������ �����", "���� ����.", "��", true, "������ ������� ��������� ����� ���� ������ ����� ���������� ����� �����������."},
{"CriticalHitTable", 26, "���� ������������ �����", "���� ����.", "��", true, "���� �������� ���������� ��������� ��������� ��� ���������� ����� ���� ����������� �����."},
{"PhisycalThreshold", -1, "���������� �����������", "���. �����", "��", false, "�� ������� ����������� ����� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"LaserThreshold", -1, "���������� ����������� �����", "�����", "�����", false, "�� ������� ����������� ����� ������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"FireThreshold", -1, "���������� ����������� �����", "�����", "�����", false, "�� ������� ����������� ����� ������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"PlasmaThreshold", -1, "���������� ����������� ������", "������", "������", false, "�� ������� ����������� ����� ������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"ElectricalThreshold", -1, "���������� ����������� �������", "�������", "�������", false, "�� ������� ����������� ����� �������������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"EMPThreshold", -1, "���������� ����������� ���", "���", "���", false, "�� ������� ����������� ����� ��� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"ExplosiveThreshold", -1, "���������� ����������� �����", "�����", "�����", false, "�� ������� ����������� ����� ������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"PhisycalResistance", -1, "������������ �����������", "���. �����", "��", true, "�� ������� ����������� ����� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"LaserResistance", -1, "������������ ������", "���. �����", "�����", true, "�� ������� ����������� ����� ������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"FireResistance", 168, "������������ �����", "���. ����", "�����", true, "�� ������� ����������� ����� ������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"PlasmaResistance", -1, "������������ ������", "���. ������", "������", true, "�� ������� ����������� ����� ������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"ElectricalResistance", -1, "������������ �������������", "���. ������.", "�������", true, "�� ������� ����������� ����� �������������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"EMPResistance", -1, "������������ ���", "���. ���", "���", true, "�� ������� ����������� ����� ��� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"ExplosiveResistance", -1, "������������ ������", "���. ������", "�����", true, "�� ������� ����������� ����� ������� ����� ���������� ��������� �����. ������� �� ������ �����."},
{"RadiationResistance", 83, "������������ ��������", "���. ���.", "���", true, "���������� ������������� ���������, �������� �� �������������, ������� �� ���� ����������. �� ��� ������ ��� ������ ���� ����� � ����� ����-������������ ����������."},
{"PoisonResistance", 23, "������������ ���", "���. ���", "��", true, "��������� ����������� �� ����������."},
{"Age", -1, "�������", "�������", "����", false, ""},
{"Gender", -1, "���", "���", "���", false, ""},
{"HP", 10, "���� �����", "������", "��", false, "���������� �����������, ���������� ������ ��������� ����� ���, ��� ��� �����. ���� ����� ��� ������ �������� ������."},
{"PoisonLevel", 11, "������� ���", "��������", "��", false, "������� ������� ���"},
{"RadiationLevel", 12, "������� ��������", "�������", "��", false, "������� ������� ��������"},
{"AP", 19, "���� ��������", "���� ��������", "��", false, "���������� ����� ��������, ������� �������� ����� ������������ �� ����� ������ ���."},
{"Random", -1, "��������", "����.", "���", false, "������ ������������ ��������� �������"},
};
assert_enum(ability, Random);
getstr_enum(ability);
getsid_enum(ability);
getfid_enum(ability);

const char* ability_values[11] = {"�����",
"�����", "�����", "�����", "������", "������", "X�����", "������", "�����.", "�����", "�����!"};

ability_s creature::getmaximum(ability_s id) {
	switch(id) {
	case HP: return HPMax;
	case AP: return APMax;
	default: return NoStat;
	}
}