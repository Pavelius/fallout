#include "main.h"

adat<creature, 128>	creature_data;
static datetime		today;

void creature::clear() {
	name = 0;
	level = 0;
	age = 16;
	gender = Male;
	wounds = 0;
	hp = 0;
	skills_tag = 0;
	armor.clear();
	weapon[0].clear();
	weapon[1].clear();
	current_weapon = 0;
	memset(illness, 0, sizeof(illness));
	memset(perks, 0, sizeof(perks));
	memset(stats, 5, sizeof(stats));
	memset(skills, 0, sizeof(skills));
}

const datetime& creature::getdate() {
	return today;
}

void creature::passtime(unsigned minutes) {
	today.add(minutes);
}

int creature::get(ability_s value) const {
	auto result = stats[value];
	if(is(Gifted))
		result++;
	switch(value) {
	case Agility:
		if(is(SmallFrame))
			result++;
		break;
	case Strenght:
		if(is(Bruiser))
			result += 2;
		break;
	}
	return result;
}

int	creature::getabilitypoints() const {
	auto result = 0;
	for(auto e : stats)
		result += e;
	return result;
}

int creature::getbase(skill_s value) const {
	auto& e = skill_data[value].base;
	auto result = e.modifier;
	if(e.a1.multiplier)
		result += e.a1.get(get(e.a1.ability));
	if(e.a2.multiplier)
		result += e.a2.get(get(e.a2.ability));
	if(is(Gifted))
		result -= 10;
	if(is(value))
		result += 20;
	return result;
}

int	creature::get(skill_s id) const {
	return getbase(id) + skills[id];
}

int creature::gethpmax() const {
	auto result = 15 + get(Strenght) + get(Endurance) * 2;
	if(level > 1)
		result += (level - 1)*get(Endurance) / 2;
	if(is(Lifegiver))
		result += 4;
	return result;
}

int creature::getapmax() const {
	auto result = 5 + get(Agility) / 2;
	if(is(ActionBoy))
		result++;
	if(is(Bruiser))
		result -= 2;
	return result;
}

int	creature::get(parameter_s id) const {
	return (this->*parameter_data[id].get)();
}

int	creature::getmax(parameter_s id) const {
	if(parameter_data[id].getmax)
		return (this->*parameter_data[id].getmax)();
	return 0;
}

int	creature::getequipweight() const {
	return 0;
}

int creature::getcarryweight() const {
	int result;
	if(is(SmallFrame))
		result = 25 + get(Strenght) * 15;
	else
		result = 25 + get(Strenght) * 25;
	if(is(PackRat))
		result += 50;
	if(is(StrongBack))
		result += 50;
	return result;
}

int creature::getcritical() const {
	auto result = get(Luck);
	if(is(Finesse))
		result += 10;
	if(is(MoreCriticals))
		result += 5;
	return result;
}

int creature::getthreshold(damage_s type) const {
	auto result = 0;
	result += armor.getthreshold(type);
	return result;
}

int creature::getresistance(damage_s type) const {
	auto result = 0;
	switch(type) {
	case PoisonResistance:
		result += get(Endurance) * 5;
		if(is(FastMetabolism))
			result = 0;
		if(is(SnakeEater))
			result += 25;
		if(is(VaultCityInoculation))
			result += 10;
		break;
	case RadiationResistance:
		result += get(Endurance) * 2;
		if(is(FastMetabolism))
			result = 0;
		break;
	}
	result += armor.getresistance(type);
	return result;
}

int creature::gethealrate() const {
	auto result = imax(1, get(Endurance) / 3);
	if(is(FastMetabolism))
		result += 2;
	if(is(FasterHealing))
		result += 2;
	return result;
}

int	creature::getmeleedamage() const {
	auto result = imax(1, get(Strenght) - 5);
	if(is(HeavyHanded))
		result += 4;
	if(is(BonusHtHDamage))
		result += 2;
	return result;
}

int	creature::getpartylimit() const {
	return get(Charisma) / 2;
}

int	creature::getperkrate() const {
	auto result = 3;
	if(is(Skilled))
		result++;
	return result;
}

int creature::getsequence() const {
	auto result = get(Perception) * 2;
	if(is(Kamikaze))
		result += 5;
	if(is(EarlierSequence))
		result += 2;
	return result;
}

int creature::getskillrate() const {
	auto result = 5 + get(Intellegence) * 2;
	if(is(Skilled))
		result += 5;
	return result;
}

int	creature::getac() const {
	auto result = get(Agility);
	if(is(Kamikaze))
		result = 0;
	return result;
}

bool creature::isplayer() const {
	return this == getplayer();
}

creature* creature::getplayer() {
	return creature_data.data;
}

bool creature::reload(item& target, bool run, bool interactive) {
	auto ammo_cap = target.getcapacity();
	if(!ammo_cap)
		return false;
	auto ammo_count = target.getammocount();
	auto delta = ammo_cap - ammo_count;
	if(delta<=0)
		return false;
	auto ammo_type = target.getammo();
	if(!ammo_type)
		ammo_type = target.getammo(0);
	if(!ammo_type)
		return false;
	auto pi = find(ammo_type);
	if(!pi) {
		if(run && interactive)
			act("У вас нет %1i.", getstr(ammo_type));
		return false;
	}
	if(delta > pi->getcount())
		delta = pi->getcount();
	if(run) {
		target.setammo(pi->get(), ammo_count + delta);
		pi->setcount(pi->getcount() - delta);
	}
	return true;
}

bool creature::equip(const item& it) {
	if(it.isweapon()) {
		if(!weapon[0]) {
			weapon[0] = it;
			return true;
		}
		if(!weapon[1]) {
			weapon[1] = it;
			return true;
		}
	} else if(it.isarmor()) {
		if(!armor) {
			armor = it;
			return true;
		}
	}
	return false;
}

void creature::add(const item& it) {
	if(equip(it))
		return;
	item it1 = it;
	additem(it1);
}

char* creature::get(char* result, const char* result_maximum, variant id, bool show_maximum_only, bool show_threshold) const {
	auto pr = "%1i";
	switch(id.type) {
	case Parameters:
		if(parameter_data[id.parameter].percent)
			pr = "%1i%%";
		if(parameter_data[id.parameter].maximum && !show_maximum_only)
			pr = "%1i/%2i";
		if(show_maximum_only)
			szprint(result, result_maximum, pr, parameter_data[id.parameter].getmax ? getmax(id.parameter) : get(id.parameter));
		else
			szprint(result, result_maximum, pr, get(id.parameter), getmax(id.parameter));
		break;
	case Damages:
		if(show_threshold)
			szprint(result, result_maximum, "%1i/%2i%%", getthreshold(id.damage), getresistance(id.damage));
		else
			szprint(result, result_maximum, "%1i%%", getresistance(id.damage));
		break;
	case Skills:
		szprint(result, result_maximum, "%1.2i%%", get(id.skill));
		break;
	default:
		result[0] = 0;
		break;
	}
	return result;
}