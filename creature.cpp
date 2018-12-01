#include "main.h"

void creature::clear() {
	name = 0;
	level = 0;
	age = 16;
	gender = Male;
	wounds = 0;
	hp = 0;
	skills_tag = 0;
	memset(illness, 0, sizeof(illness));
	memset(perks, 0, sizeof(perks));
	memset(stats, 5, sizeof(stats));
	memset(skills, 0, sizeof(skills));
	memset(wears, 0, sizeof(wears));
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
	result += wears[Armor].getresistance(type);
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