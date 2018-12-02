#include "main.h"

perk_info perk_data[] = {{72, "������������", "��� �������������, ����� ���������� ���������� � ����� ��'����, ������� �� ��������. �� ������ �� ����� �������� �����, � ��� �� ��������� ���������� �� ��������� � ��� ������. "},
{73, "���. ��������. �����", "�� ������� � ������� ������� ����������� �������, ��� �������� ���� �����. ��� �� ��� �� ����, ���� ���������� ����� �������� �� 1 �� ������."},
{74, "���. ��������. �������.", "������� ���� ���������� ������� ������ ��� ������� �������� ������ ����� ���������� �����������. ��� ����� ����� ��� �������� ������� ���� ������ �� 2 ���� ������ ������."},
{75, "����� ��������", "�� ������ ������� ������� ������ �� ��������� 2 ��, ������� ������������ ������ ��� �����������. ����� �������, � ������ ������� ������ �� ��������� 2 �������������� ����."},
{76, "����� ��������������", "���� ���������� � ��������� �� ���������� ������� ������� ���� ����. �� ������ ������� ������� ������ ��������� ���� ��� �������� ����������� ������������� �� 2."},
{77, "����� ��������", "������ �� ��������� �� ����� ������� �������, �� ��������� ��-�������� �����. �����, ����������� �� �����, ����������� �� 1 ���� ��������."},
{78, "������� �������", "����� ������ ����� ���������, ���� ���� ���, ��� ��������� ������, � ��� ������� �������� ������������� �� 2 � ������ �������."},
{79, "������� �������", "� ������ ������� ����� ������, �� ������������ �� 2 ���� ������� �������. ��� ���������� �������� �������, ����������� ��� ������� ��������������."},
{80, "������ ����. ������", "���� � ��� ���� ���� �����, ����������� ��������� ����������� ����������� ���������� ������������� �� 5 ���������. ����� ������� ��� ��������� - ���� �������, ���� ����."},
{81, "������ ������", "� ������� ������ ������� ������ �� ������� ����� ������ � �������. ��� ������� ����� ������ ��������� �� 20%."},
{82, "������ ���", "�� ���������� (�����������) ��������, ������ ����� � �������. �������������� ������� ������� �������� ���������� �� 10% ��� ������� �� ������� ����� ������."},
{83, "���. � ��������", "�� ����� ���������� �������� ����������� �������� �� �������� � �� �������. ������ ������� ����� ������ ����������� ���� ������������ � �������� �� 15%."},
{84, "��������", "�������� ��������� ��� �� �������� ������ ��������� ������������. �� ��������� 10 ��������� ������������ � ������������."},
{85, "���������", "����������� ������� ������� ����� - ������ �� ������ � �������� �������. �������� 50 ������ �����."},
{86, "��������", "��������� �������� � ���� �� ������� ����������. ��� ����������� ��������� �� ��������� ����� 2 � �������� �� ������ ������. ������ ������� �������� ����� ������� ����� � ��������."},
{87, "��������� ���", "� ������� ����� ������ �� ������ ������������� ������� �������, ��������� �� �������. �� ������ �������� � ������ ������������. � ��� ���� �� ������������� ����������� �������� �� ����� ����."},
{88, "���������", "�������� � ������ ����� � ������� ���������� � ��� ����������� ���������. ���� ����� ���� ��� ����������� �������� � ��������������� ��������, ���������� �� 25% ����������� ���������."},
{89, "��������", "�� ��������� ������� ��������� ��������: ������� �������� ������ ������� �������, ��� ��� ��������� ���������. � ���� ������� �� ��������� 25% ������ �� ������� ������� � �������� ��� � ���������."},
{90, "�����������", "������ ������� ����������� ����� ���������� ��� 2 ���� ��� ��������� ������ ������ ���������. ������ ����� ����� ��� ������� �� ������ ������ ����."},
{91, "������", "��������� ���������� ����� ������ ��� � ���� �������. �� ������ ������ �� ������ �������� 4-10 ����� �����, �������� ����������� ������ ��� ������ ������."},
{92, "��������", "� ��� ���� ������ ������������ ������ � ����� ����������� ������, ���� �� ����� ��������� ������ � �������."},
{93, "������ ����. �����", "����������� ������ ���������� ����������� ����� ����� �����������. �� ��������� 20 ���������� ����� ��� ��������� �����������, ���� ����������� ��������� �������� �� �������������."},
{94, "�������", "�� ������ ������� ������ ������������ ������� � ������ �� ������� �� ���� �������. ��� ��������� �� ���������� ������������� ������� ����������� � ������ ������� ��� ���������� ��� ������� � ����."},
{95, "������", "������ ����� �� �����! �������� ����� ����� ������� � ��� ������ ����� ��������� ��������� ����������� �����������, ����� ���������� � ���� �� ����� ����."},
{96, "�������", "�� � ������������ ��������� ������� ���������� �������. � ������ ������� ��� ���������� �������� ����� ����� ������ ������ ��������� ����� ��������� ����������� ��� ������ ����������."},
{97, "������", "���� �� ������������ � ���������� � ����� ��� ����� ������� ��� � �����, �� �������� ��� ������� �����������. ����� ���� �������� ����� - ����� ������."},
{98, "������� ��������", "������� �������� ��� �������� ��������� �����, ��������� ����������. ��� ������� ��� �������������� ���� ��������, ������� ����� ������������ � ����� �����."},
{99, "������ ��������", "���������� ���� - ��� ����������� ��������� ������ ������� ��������� �������������. �� ������ ����  ���������� ���� ����������� �� ����������� ����, ��� �� ����� ������� ������ ������."},
{100, "���� �����", "� ������ ������� ������� ������ �� �������� �������������� 4 ���� �����. X��� ��� � �������, ������ ���������, � ����� ����������. ��������-�� ����!"},
{101, "������������", "�� ����� ��� � ��� ����� ������� ������� �������. ���������� �� ������ �� ��� � ���� ������ ��������, �� ��������� ����� � 5 ��������� � ������ �����."},
{102, "��������", "����� ����������� �������� ��������� �� ��� ������ ����������. ���� ������������ � ��� ���������� �� 25 ���������."},
{103, "����������", "��� ������ ���� ��� ����� � +10% � ������� ������� � �����."},
{104, "��������", "�������� ��������� ����� � 10% ������� ��������� � ������. ������ ����������� ���� ������� �� ���� ������ � ���."},
{105, "��� ������������", "��� ��������������� ������ �� ������ ������ � ������ ����� ��������. ��� ������ ��������� 15% � ������� ��������� � �����. "},
{106, "������", "������� ������ ������ ��������� 20% � ������ ������� ���������. ����� ����� ������������."},
{107, "�������", "������ ������� ������ ������� ��������� 2 �������������� ���� � ���� ��� ������� ���������� �� ����. ��� ������ �� ��������� �������� ������������ ��������������."},
{108, "ERROR -- UNIMPED", "ERROR.UNIMPED!"},
{109, "���������", "�� ����� ������ � ��������� ��� ������� ���������. ��������� ����� ������ ������� � ������� �� ������ � ������ ��������� �������� ��� �� ����� ��������."},
{110, "����������", "����� ������� ������ ��� �� ���������� � ��������������� �����, �� ������ ������������� � ���� ��� ����������. ���� ����������� ������������� ��������� ������������� �� 20%."},
{111, "����� ��������", "���� ��������� ������ ���������. ��� ����� ������ ���������� ��������� ����� �� ������������� ������� �� ��������������� � ������. �������, ���� ����� ���� ��������� ����� ������� ������ \"�����-�������\" ����� �� ������ ��� ������� ����� ������."},
{112, "����������", "��� ������ ��������� �������� � ��� ���� ������ �������� ��� ������."},
{113, "��������", "� ���� ������� � ��� ������ ������ �������� �������� � ������ ����������� ��������� �����."},
{114, "���� ������", "� ���� ������� �� � ��� ���� ������ ���������� � ���. ���������� � � ��� ���� ������� �������� �� �� �����������."},
{115, "��������", "�������� ���������� ����� �������� ����, ������ ����� ������ ������������ �� ����� �� 25%."},
{116, "���� ��������", "����� �� ����� ��������� ������ ����� ���� �� ��� �� �� ��������."},
{117, "�����", "� ��� ���������� ������, ������������� ����������� �������� �����, ���������� ���� ������� � ��������� ��������."},
{118, "������������ ��������", "��������� ����� ������ �� ��������� ��������������� ������������� ���������, ������������� �� ����� ���. ���� �� �������� - �� ���������, ��� �� ��������."},
{119, "��������", "��� ������ ��������� 15% �� ����������� ��������� �� �������. ��� ����� ������ ����� � ���������� �������� ���� � �����."},
{120, "����������", "�� ����� ������������ ���� �������. ������ � ������� �� ����� ��� �������� �� 4 ��, � 2."},
{121, "������������ ����", "�� ������ ����������� �������� ������ � ��������� ������ �� ������� � ��� �� ������."},
{122, "��������", "�� ������� ������ � ��� ��������� ��� �������� �� 5% ������������� � ������ ����������� ��� ��������� ������."},
{123, "�������������� �����!", "���� ����������� ������� �� ������, ����� �� ������ ������� �������������� ������, ������� ����� ����� � ��� ���� �������."},
{124, "�������!", "�������� �������� ���! ���� �� ����� ���� ����������� ��������."},
{142, "���������� � ����-����", "�� ���������� �� ������ ������� ����-����."},
{126, "���������� � ��������", "��� ���������."},
{-1, "���������� � �������", "��� �����."},
{144, "���������� � �����", "�� ����������� �������� � ������."},
{145, "���������� � �������", "��� ������."},
{-1, "������� ��������������", "������������ ������."},
{-1, "������� ��������", "����������� ������."},
{-1, "������� ��������", "����������� ������."},
{-1, "������ ������", "������."},
{-1, "�������������", "�������������� �����."},
{-1, "������ �������", "������ �����."},
{-1, "�������� � ��������", "��������� ����������� �������."},
{-1, "������� ����������� ������", "������� ���������� ������."},
{-1, "������ ������� �������", "������������ ������."},
{-1, "��������� ��������", "������������ ������� ��������."},
{-1, "��������� ����� I", "���������� ����� I"},
{-1, "��������� ����� II", "���������� ����� II"},
{136, "���������� � ����", "��� ��� �������� ���������. ����� ��� �����."},
{-1, "������������ ����������", "� ������ �� ������� ������� ��� ���� ����� ����."},
{-1, "���������� �������", "���������� �������"},
{-1, "������ ����� � ��������", "� ��� ������� ������ ������������ �������� ��� ������ �� ����."},
{-1, "��������� �����", "�� ������ ���������� ������ �� ����������� �����������."},
{-1, "��������� ��������� �����", "�� ������ �������������� ���������� ������ �� ����������� �����������."},
{-1, "�������� ����������� ������", "�� ������ ���������� ������ �� �������������� ����."},
{-1, "��������� ����������� ������", "�� ������ �������������� ���������� ������ �� �������������� ����."},
{-1, "�������� ������ �������", "�� ��������� 10% ������������� ��� � ��������."},
{-1, "������ ����������", "� ������� ����� ������ �� ��������� �������������� ���� ���� ��� ������ ���������� ����� ��� ������� ���������� ���� ��������."},
{-1, "������������������ �������", "�� ����� ����� ��� ��������� � � ������ ������ ������ ����������� � ��� �������������. ��� ������� ����� ������ �� ��������� +3 ���� �������� ��� ��������� �������."},
{-1, "���������", "�� ��������� ������ �������� �� �������� ������ ��� ������. ��� �� �� ��������� �� 50% ������ ����� ��� �������� ����-���� ����� ������ ����."},
{-1, "�������-���������", "�� ��������� ������� �� ����� ���������� ����. �����, ������������� ���� ������ ���������� � ������ ����� � ������� �������������� �����."},
{-1, "�����", "�� ������ ���������� ����� ������� �������. �� ��������� �� +20% � ���������� � ����� ������������ ������."},
{-1, "���������� ����", "��� ������� ����� ������ �� ��������� +1 � ����� ����."},
{-1, "���������� ��������", "��� ������� ����� ������ �� ��������� +1 � ����� ��������."},
{-1, "���������� ������������", "��� ������� ����� ������ �� ��������� +1 � ����� ������������."},
{-1, "���������� �������", "��� ������� ����� ������ �� ��������� +1 � ������ �������."},
{-1, "���������� ����������", "��� ������� ����� ������ �� ��������� +1 � ������ ����������."},
{-1, "���������� ��������", "��� ������� ����� ������ �� ��������� +1 � ����� ��������."},
{-1, "���������� �����", "��� ������� ����� ������ �� ��������� +1 � ����� �����."},
{-1, "����������", "��� �� ����� ������� ������ �������� ��� ������������ ������ �� �������� ����� � ��������� +20% � ����� ����������� ��������� ������."},
{-1, "����� � ������", "� ���� ������� �� ����� �� ��������� �������������� ������� �����."},
{-1, "�����������", "���� ��� ����� ��� ������ �����, ������ ���������������� �� ���� ��� +2 ������ � ��������� +1/12 ����� ����������� ���."},
{-1, "����-������ ���� �����", "��� �������� ����������� �����������, �� ��� ��� ������ �� ������� �� ���� ����."},
{-1, "������ �����", "���� ����� ������ ��������� � ��������� ����� ���������� �� �������������� � �������."},
{-1, "������ ���", "�� ��������, ��������� � ��� ����������� �����. ��� ������ ��� �� ������� ����� ��������� � �������."},
{-1, "������ �������� ", "�� ������ ������� ������������� � �������� ����� ������� � ������ �� ������� � ������ �������. ��� ������ ����� ��� �������������� ���������� ������ ������ �� 10%, � ����� �������������� +5% ����� �� ���� � ����� � ������ ����������."},
{-1, "������������� ��������", "�� ������ ����� ���� � ������� ��� ������ ����������, �� �������, ��� � ���� ������� - ��� �����."},
{-1, "������� �����������", "�� ���������� ������� � ������� �����������. �� �� ������ ������ ������� ����� ����, �� � ��������� ���� ������ ������� ��, ��� ��� ���������."},
{-1, "���������", "�� ����� ������������ ���� ����. ��� ��������� ������ ��� ��������� ������ ����� ��� ������."},
{-1, "�������", "�� �������� �������������� ����������, � ����� ������ ������� � ����� ������ �������� �� ������."},
{-1, "������� ��������������", "�� ������ ��������� � ���� ����� ��������."},
{-1, "��������", "�� ������������ ��������. ��������� ����� ������ �� ��������� +20% � ������ ������� ������."},
{-1, "�������� �����", "��� ������� ����� ������ ����������� ������� � ��� �����������."},
{-1, "���", "� ����� ����� ����� ����� ����������������� ����. ��� ������ ��������� +10% � �����, ������, ������ �������� � �������������� �������."},
{173, "��������� � �������", "��������� � ������� ������ ��� ������� ����� ��� ��������� � ��������� +3 � ����."},
{-1, "�������� � ������ �������", "�� ������ ����������� �������� � ������ �������."},
{-1, "���������� ��������� ���� �����", "���� ���� ����� ������� ����������� � ���������� ���������������� �����."},
{-1, "���������� ��������� ���� ����� II", "���� ���� ����� ��� ������� ����������� � ���������� ���������������� �����."},
{-1, "���������� ��������� ���� �����", "���� ���� ����� ������� ����������� � ���������� ���������������� �����."},
{-1, "���������� ��������� ���� ����� II", "���� ���� ����� ��� ������ ����������� � ���������� ���������������� �����."},
{-1, "���������� ����-�������� ���-������", ""},
{-1, "���������� ����-�������� ���-������ II", ""},
{-1, "���������� ����-�������� ���-������", ""},
{-1, "���������� ����-�������� ���-������ II", ""},
{-1, "������� ���������� ������������", "�� ������ ���������� ����� ������ �� ���."},
{-1, "��������� ������ ������", "������ �� ����� �������."},
{-1, "���������� ���������", "������ ��, ��� � ���� ������ ����� ����������� ������ � ���. �� ������ ��, ��� � � ���� ����."},
// Traits
{55, "������� ����������", "��� ������� ����������� ����� ��������� �����. ��� ��������, ��� �� ������ ����� ��������� � ����������� �������� � ����, �� �������� ������ �������."},
{56, "�������", "��������� �����������, �� ���� ��������������. ���� �������, �� ���� ���, ����� �� ��� ����� ���������! ���������� ����� ����� �������� �����������, �� ���� ���� �������������."},
{57, "���������", "�� �� ��� ������, ��� ������ ������ �������, ���� ��� ������� ��� ����� �� ������. �� �� � ��������� ������� ������� �����, ���� ����� ��������."},
{58, "���������", "�� ������ ��������� ������ ����� �� ���. ��������� ����� �� ������� �������� ������� �������, �� � ��������� ���������� ��������."},
{59, "��������", "��� ���� ����� ����� �����. ����������� ��������� ��������, �� ��� ���� ������ ����������� ������������ �����."},
{60, "���������", "� ��� �� �� ��������� �������� �� ���������� ���������� � ���������� ��������. ��� ������� ���������� ����� ����� ����� ������ �� �������� � �������� ��."},
{61, "����� �� ����", "�� ����� ������, �� �� ����� �����. �������� ����� ��������� �������� ����������� ������, ������ ����� ������� �� ����������� ������������ ��� �� ���������."},
{62, "������� �������", "� ��� ��� ������� ��� ����������� ������������, ������� ��������� ������, ���� �� ���������. ��� ���������� ������ ��� ��������� �� ���� ���� �������� ������."},
{63, "�������", "�� �������� �����������, ���� ������ ��� ������� ��������, ����������� �������. ��� ��� �� �������� ������ ������� �������."},
{64, "������ ����", "X������ ������� ���� ����������� � ���, ��� ���� ����� ��������� ���������. ����� ��, ��� �� ���������� �� ������ ��!"},
{65, "�����������", "� ������� �� ���������� ������, ������������ ��������� � �� ������ �������. ���� ������ ������ ������, ��� � ������, �� ���� ������ ������, ������, ������ � �������� ������� �����."},
{66, "�����. � �����", "�� ����� ��������� � ����������� �� ���������. ����������� ���������� � ������ ��������� ����� ������, ��� �����, ������ �� ����� ���������� �� ������."},
{67, "���. � �����", "X�������� �������� ��� ������ �������� ����������� �������, �� ����������� ���������� ����� ������� �� 50% �� �����."},
{68, "��������������", "�� ������� \"����������\" �����������. ����� ���������������� ���� ������������ ����, � ������ �� ��� �� - �������� ���������."},
{69, "������", "��� ��� �� ��������� �������� ��� ����� � ��������� �������� ���� �����������, �� ������� ���� � ����������� �����������. � ������ �������, �� ��������� ������ ���. ������ - ���� �� ������ 4 ������."},
{70, "������", "�� ����� ������� ���������� ������������� � ������������ ������� �������. ��� ���� ��������� ������ �� �������, �� �� ������� 10% ��� ������ � ��������� �� 5 ����� ������ �� �������."},
};
assert_enum(perk, LastTraits);
getstr_enum(perk);