#include "main.h"

static_assert(sizeof(item) == sizeof(int), "Not valid items count");

struct item_info {
	const char*		name;
	unsigned short	frame[4];
	unsigned short	sound[2];
	unsigned short	weight;
	unsigned short	cost;
	attack_info		weapon;
	armor_info		armor;
	const char*		description;
};
static item_info item_data[] = {{},
{"Металлическая броня"},
{"Пистолет-пулемет 10мм", {}, {}, 2, 100, {{FireSingle, FireBurst}, SmallGuns, {5, 12}, Ammo10mm}},
};

const char*	item::getdescription() const {
	return item_data[type].description;
}

const char* item::getname() const {
	return item_data[type].name;
}

int item::getweaponindex() const {
	return 0;
}

int	item::getresistance(damage_s id) const {
	return item_data[type].armor.resistance[id];
}