#include "main.h"

skill_info skill_data[] = {{"SmallGuns", 28, "������ ������", {5, {Agility, 4}}, "�������������, ������� � ����� ������ �� ����������� ������. ����������, ��������� � ���������."},
{"BigGuns", 29, "������� ������", {0, {Agility, 2}}, "������ �������� � ��������� ������� �����������. ���������������� ��������, �����������, �������� � ���� ��������."},
{"EnergyWeapon", 30, "������ ������", {0, {Agility, 2}}, "����������� ���������� ��������� �������������� ������. ��� ����� ����� �� ���������� ��������� � ����������� ���������."},
{"Unarmed", 31, "����������", {30, {Agility, 2}, {Strenght, 2}}, "��������� ������ ��������, ������ ����������� ��� � ������� ����������� ��������� ��� ������� ������ ������ � ������."},
{"MeleeWeapon", 32, "X������� ������", {25, {Agility, 2}, {Strenght, 2}}, "������������� ������ ��������� ������� � ������� ���. ����, �������, �����, ����, �������, ���� � ���� �������� ����."},
{"Throwing", 33, "�����������", {0, {Agility, 4}}, "����������� ������� ��������� ����������� ��������. �����, ������� � ������ � ��� �� ����."},
//
{"FirstAid", 34, "������ ������", {0, {Perception, 2}, {Intellegence, 2}}, "����� �������� � ������� ������ ����������� ������. ����������� �������� �� ������� �������, ������� � �������. �� ���� ����������� ��������������� ������ ������, ��� ������ �����."},
{"Doctor", 35, "������", {5, {Perception, 1}, {Intellegence, 1}}, "��������� ������� ������� � ������ ���������� � ������������� ������� ����. ��� ���� ����������� ��� ��������� ����� ����������� ����� ����� �������."},
{"Sneak", 36, "��������", {0, {Agility, 3}}, "����� ������������ � ����������� ���������� ������������. � ������ ������ ��� ����� ������� ������� ����������. �� �� ������ ������������ ������ � ��������."},
{"Lockpick", 37, "����������", {10, {Perception, 1}, {Agility, 1}}, "������ ��������� �����, ���� �� ���� ����������� �����. ������������� ������� ��� ����������� �������� ������ �������� ��������� �����������."},
{"Steal", 38, "��������", {20, {Agility, 1}}, "����� �� ������������ � ��������� �������������� ����� ���� ��� ������ ���������. ������� ��� ��������� �������."},
{"Traps", 39, "�������", {20, {Perception, 1, 2}, {Agility, 1, 2}}, "����������� ������������ � ������������� �������. ����� ����, ��������� ������� ����������� ��������."},
{"Science", 40, "�����", {25, {Intellegence, 2}}, "������� ���������� �������� ������, ������� ������ � �����������, ��������, ������ � ��������."},
{"Repair", 41, "�������", {20, {Intellegence, 1}}, "������������ ���������� ������� ������. ������ ���������� ����������, ������������� ���������� � �����������."},
//
{"Speech", 42, "��������", {0, {Charisma, 5}}, "����������� �������� � ������ � ������������ �������� �������� �� ��������. ������������ � ����� �������, ������������ ������������. ��������, ����������� �������� ���� �� ������."},
{"Barter", 43, "��������", {0, {Charisma, 4}}, "����� ��������� � ���������� ��������� ������. ��������� ������� ��� �� �������� ����� � ������� �� ������� ������� ����� ������� � �����."},
{"Gambling", 44, "����", {0, {Luck, 5}}, "������ � ���������� �� �������� ����� ����������� �������� ���. �� ������ ����������� � ����� � �����."},
{"Outdoorsman", 45, "��������", {0, {Intellegence, 2}, {Endurance, 2}}, "�� �� ��������� ������� � �������� ���������, � ������ � ������. �� ������ ������ � ������, ������, ��������� � �������."},
};
assert_enum(skill, LastSkill);
getstr_enum(skill);