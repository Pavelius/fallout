#include "main.h"

struct caliber_info {
	const char*		id;
	const char*		name;
	item_s			items[4];
};
caliber_info caliber_data[] = {{"NoAmmo", "Нет"},
{"CaliberRocket", "Ракета"},
{"CaliberGas", "Топливо огнемета"},
{"CaliberEnergyC", "Энергопак С"},
{"CaliberEnergyD", "Энергопак Д"},
{"Caliber233", ".223", {FMJ223}},
{"Caliber5mm", "5мм", {JHP5mm, AP5mm}},
{"Caliber40", ".40 кал.", {}},
{"Caliber10mm", "10мм", {JHP10mm, AP10mm}},
{"Caliber44", ".44 кал.", {MagnumJHP44, MagnumFMJ44}},
{"Caliber14mm", "14мм", {AP14mm}},
{"CaliberFract", "Дробь"},
{"Caliber9mm", "9мм", {Ammo9mm}},
{"CaliberBB", "ББ"},
{"Caliber45", ".45 кал."},
{"Caliber2mm", "2мм", {EC2mm}},
{"Caliber47mm", "Обойма 4.7мм", {Caseless47mm}},
{"CaliberXH", "Основные XН"},
{"Caliber762", "7.62мм", {Ammo762mm}},
};
assert_enum(caliber, Caliber762);
getstr_enum(caliber);
getsid_enum(caliber);