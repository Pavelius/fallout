#include "main.h"

caliber_info caliber_data[] = {{"NoAmmo", "���"},
{"CaliberRocket", "������", {ExplosiveRocket, RocketAP, RoboRocketAmmo}},
{"CaliberGas", "������� ��������", {FlamethrowerFuel, FlamethrowerFuelMKII}},
{"CaliberEnergyC", "��������� �", {SmallEnergyCell}},
{"CaliberEnergyD", "��������� �", {MicroFusionCell}},
{"Caliber233", ".223", {FMJ223}},
{"Caliber5mm", "5��", {JHP5mm, AP5mm}},
{"Caliber40", ".40 ���.", {}}, // ��� ����� ������� ��� �� ������ �� ��������
{"Caliber10mm", "10��", {JHP10mm, AP10mm}},
{"Caliber44", ".44 ���.", {MagnumJHP44, MagnumFMJ44}},
{"Caliber14mm", "14��", {AP14mm}},
{"CaliberFract", "�����", {GaShotgunShells12}},
{"Caliber9mm", "9��", {Ball9mm, Ammo9mm}},
{"CaliberBB", "��"}, // ��� ����� ������� ��� �� ������ �� ��������
{"Caliber45", ".45 ���.", {Ammo45}},
{"Caliber2mm", "2��", {EC2mm}},
{"Caliber47mm", "������ 4.7��", {Caseless47mm}},
{"CaliberXH", "�������� X�", {HNNeedlerCartridge, HNAPNeedlerCartridge}},
{"Caliber762", "7.62��", {Ammo762mm}},
};
assert_enum(caliber, Caliber762);
getstr_enum(caliber);
getsid_enum(caliber);