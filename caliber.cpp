#include "main.h"

struct caliber_info {
	const char*		id;
	const char*		name;
	item_s			items[4];
};
caliber_info caliber_data[] = {{"NoAmmo", "���"},
{"CaliberRocket", "������"},
{"CaliberGas", "������� ��������"},
{"CaliberEnergyC", "��������� �"},
{"CaliberEnergyD", "��������� �"},
{"Caliber233", ".223", {FMJ223}},
{"Caliber5mm", "5��", {JHP5mm, AP5mm}},
{"Caliber40", ".40 ���.", {}},
{"Caliber10mm", "10��", {JHP10mm, AP10mm}},
{"Caliber44", ".44 ���.", {MagnumJHP44, MagnumFMJ44}},
{"Caliber14mm", "14��", {AP14mm}},
{"CaliberFract", "�����"},
{"Caliber9mm", "9��", {Ammo9mm}},
{"CaliberBB", "��"},
{"Caliber45", ".45 ���."},
{"Caliber2mm", "2��", {EC2mm}},
{"Caliber47mm", "������ 4.7��", {Caseless47mm}},
{"CaliberXH", "�������� X�"},
{"Caliber762", "7.62��", {Ammo762mm}},
};
assert_enum(caliber, Caliber762);
getstr_enum(caliber);
getsid_enum(caliber);