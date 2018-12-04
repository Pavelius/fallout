#include "main.h"

caliber_info caliber_data[] = {{"NoAmmo", "Нет"},
{"CaliberRocket", "Ракета", {ExplosiveRocket, RocketAP, RoboRocketAmmo}},
{"CaliberGas", "Топливо огнемета", {FlamethrowerFuel, FlamethrowerFuelMKII}},
{"CaliberEnergyC", "Энергопак С", {SmallEnergyCell}},
{"CaliberEnergyD", "Энергопак Д", {MicroFusionCell}},
{"Caliber233", ".223", {FMJ223}},
{"Caliber5mm", "5мм", {JHP5mm, AP5mm}},
{"Caliber40", ".40 кал.", {}}, // Для этого калибра нет ни оружия ни патронов
{"Caliber10mm", "10мм", {JHP10mm, AP10mm}},
{"Caliber44", ".44 кал.", {MagnumJHP44, MagnumFMJ44}},
{"Caliber14mm", "14мм", {AP14mm}},
{"CaliberFract", "Дробь", {GaShotgunShells12}},
{"Caliber9mm", "9мм", {Ball9mm, Ammo9mm}},
{"CaliberBB", "ББ"}, // Для этого калибра нет ни оружия ни патронов
{"Caliber45", ".45 кал.", {Ammo45}},
{"Caliber2mm", "2мм", {EC2mm}},
{"Caliber47mm", "Обойма 4.7мм", {Caseless47mm}},
{"CaliberXH", "Основные XН", {HNNeedlerCartridge, HNAPNeedlerCartridge}},
{"Caliber762", "7.62мм", {Ammo762mm}},
};
assert_enum(caliber, Caliber762);
getstr_enum(caliber);
getsid_enum(caliber);