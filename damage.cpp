#include "main.h"

resist_info damage_data[] = {{"Phisycal", "����", "�� ��������� ������� ����������� ��� ����������� �� ����������� ���������, ��������, �� ����� ����� ��� ����. �������� � ����������� �� ������ �����."},
{"Laser", "�����"},
{"Fire", "�����"},
{"Plasma", "������"},
{"Electrical", "�������"},
{"EMP", "���"},
{"Explosive", "�����"},
};
assert_enum(damage, Explosive);
getstr_enum(damage);
getsid_enum(damage);