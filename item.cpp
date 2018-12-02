#include "main.h"

using namespace res;

static_assert(sizeof(item) == sizeof(int), "Not valid items count");

struct armor_info {
	unsigned char	ac;
	unsigned char	threshold[Explosive + 1];
	unsigned char	resistance[Explosive + 1];
	res::tokens		dress[2];
};
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
{"Металлическая броня", {1}, {}, 50, 3000, {}, {30, {}, {}, {HMMETL, HFMETL}}},
{"Пистолет-пулемет 10мм", {2}, {}, 2, 100, {{FireSingle, FireBurst}, SmallGuns, {5, 12}, 2, 30, {Ammo10mm}}},
};
getstr_enum(item);

item::item(item_s type, int count) : type(type), count(0) {
	setcount(count);
	auto ammo_cap = getcapacity();
	auto ammo_type = getammo(0);
	if(ammo_cap)
		setammo(ammo_type, ammo_cap);
}

void item::clear() {
	*((int*)this) = 0;
}

unsigned short item::get(item_sprite_s i) const {
	return item_data[type].frame[i];
}

bool item::isweapon() const {
	return item_data[type].weapon.damage.max != 0;
}

bool item::isarmor() const {
	return item_data[type].armor.dress[0] != res::NoRes;
}

int item::getcapacity() const {
	return item_data[type].weapon.capacity;
}

int	item::getweight() const {
	return item_data[type].weight;
}

res::tokens	item::getdress(gender_s gender) const {
	return item_data[type].armor.dress[gender];
}

int	item::getminst() const {
	return item_data[type].weapon.minst;
}

bool item::ismatch(const item& it) const {
	if(type != it.type)
		return false;
	return isweapon() ?
		(ammo_count == it.ammo_count && ammo_index == it.ammo_index) : (count == it.count);
}

void item::join(item& it) {
	if(!ismatch(it))
		return;
	auto src_count = it.getcount();
	auto old_count = getcount();
	setcount(old_count + src_count);
	auto new_count = getcount();
	it.setcount(src_count - (new_count - old_count));
}

int item::getcount() const {
	if(isweapon())
		return weapon_count + 1;
	return count + 1;
}

item_s item::getammo() const {
	if(ammo_count == 0)
		return NoItem;
	return item_data[type].weapon.ammo[ammo_index];
}

bool item::setammo(item_s type, int count) {
	if(type == NoItem || count <= 0)
		return false;
	auto result_ammo_index = getammoindex(type);
	if(result_ammo_index == -1)
		return false;
	if(count > 255)
		count = 255;
	ammo_index = (unsigned char)result_ammo_index;
	ammo_count = (unsigned char)count;
	return true;
}

item_s item::getammo(int index) const {
	return item_data[type].weapon.ammo[index];
}

int	item::getammocount() const {
	if(!isweapon())
		return 0;
	return ammo_count;
}

void item::setammocount(int value) {
	if(!isweapon())
		return;
	if(value < 0)
		value = 0;
	if(value > 255) // TODO: Использовать размер магазина
		value = 255;
	ammo_count = value;
}

void item::setcount(int value) {
	if(value <= 0)
		clear();
	else if(isweapon()) {
		if(value > 32)
			value = 32;
		weapon_count = value - 1;
	} else {
		if(value > 256 * 256)
			value = 256 * 256;
		count = value - 1;
	}
}

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

int	item::getammoindex(item_s ammo_type) const {
	const auto im = sizeof(item_data[type].weapon.ammo) / sizeof(item_data[type].weapon.ammo[0]);
	for(unsigned i = 0; i < im; i++) {
		if(item_data[type].weapon.ammo[i] == ammo_type)
			return i;
	}
	return -1;
}