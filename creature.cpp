#include "main.h"

adat<creature, 128>	creature_data;
static datetime		today;

void creature::clear() {
	name = 0;
	level = 0;
	gender = Male;
	wounds = 0;
	ap = 0;
	skills_tag = 0;
	armor.clear();
	weapon[0].clear();
	weapon[1].clear();
	current_weapon = 0;
	memset(stats, 0, sizeof(stats));
	memset(perks, 0, sizeof(perks));
	memset(skills, 0, sizeof(skills));
	for(auto i = Strenght; i<=Luck; i = (ability_s)(i+1))
		stats[i] = 5;
}

const datetime& creature::getdate() {
	return today;
}

void creature::passtime(unsigned minutes) {
	today.add(minutes);
}

int creature::get(ability_s value) const {
	auto result = stats[value];
	switch(value) {
	case Strenght:
		if(is(Gifted))
			result++;
		if(is(Bruiser))
			result += 2;
		break;
	case Perception:
		if(is(Gifted))
			result++;
		break;
	case Endurance:
		if(is(Gifted))
			result++;
		break;
	case Charisma:
		if(is(Gifted))
			result++;
		break;
	case Intellegence:
		if(is(Gifted))
			result++;
		break;
	case Agility:
		if(is(Gifted))
			result++;
		if(is(SmallFrame))
			result++;
		break;
	case Luck:
		if(is(Gifted))
			result++;
		break;
	case HPMax:
		result += 15 + get(Strenght) + get(Endurance) * 2;
		if(level > 1)
			result += (level - 1)*get(Endurance) / 2;
		if(is(Lifegiver))
			result += 4;
		break;
	case APMax:
		result += 5 + get(Agility) / 2;
		if(is(ActionBoy))
			result++;
		if(is(Bruiser))
			result -= 2;
		break;
	case CarryWeight:
		if(is(SmallFrame))
			result += 25 + get(Strenght) * 15;
		else
			result += 25 + get(Strenght) * 25;
		if(is(PackRat))
			result += 50;
		if(is(StrongBack))
			result += 50;
		break;
	case CriticalHit:
		result += get(Luck);
		if(is(Finesse))
			result += 10;
		if(is(MoreCriticals))
			result += 5;
		break;
	case HealingRate:
		result += imax(1, get(Endurance) / 3);
		if(is(FastMetabolism))
			result += 2;
		if(is(FasterHealing))
			result += 2;
		break;
	case Sequence:
		result += get(Perception) * 2;
		if(is(Kamikaze))
			result += 5;
		if(is(EarlierSequence))
			result += 2;
		break;
	case AC:
		result = get(Agility);
		if(is(Kamikaze))
			result = 0;
		break;
	case DamageUnarmed:
		result = imax(1, get(Strenght) - 5);
		if(is(HeavyHanded))
			result += 4;
		if(is(BonusHtHDamage))
			result += 2;
		break;
	case DamageMelee:
		result = imax(1, get(Strenght) - 5);
		if(is(HeavyHanded))
			result += 4;
		if(is(BonusHtHDamage))
			result += 2;
		break;
	case PhisycalThreshold:
	case LaserThreshold:
	case FireThreshold:
	case PlasmaThreshold:
	case ElectricalThreshold:
	case EMPThreshold:
	case ExplosiveThreshold:
		result += armor.getthreshold(damage_s(value - PhisycalThreshold));
		break;
	case PhisycalResistance:
	case LaserResistance:
	case FireResistance:
	case PlasmaResistance:
	case ElectricalResistance:
	case EMPResistance:
	case ExplosiveResistance:
		result += armor.getresistance(damage_s(value - PhisycalResistance));
		break;
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
	result += stats_boost[value];
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

int	creature::getequipweight() const {
	auto result = armor.getweight();
	result += weapon[0].getweight();
	result += weapon[1].getweight();
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
	if(delta <= 0)
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

char* creature::get(char* result, const char* result_maximum, variant id, bool show_maximum_only) const {
	auto pr = "%1i";
	switch(id.type) {
	case Abilities:
		if(ability_data[id.ability].percent)
			pr = "%1i%%";
		if(id.ability == HP && show_maximum_only)
			id.ability = HPMax;
		if(id.ability==HP)
			szprint(result, result_maximum, "%1i/%2i", get(HP), get(HPMax));
		else
			szprint(result, result_maximum, pr, get(id.ability));
		break;
	case Skills:
		szprint(result, result_maximum, "%1.2i%%", get(id.skill));
		break;
	case Damages:
		szprint(result, result_maximum, "%1i/%2i%%", get((ability_s)(PhisycalThreshold + id.damage)), get((ability_s)(PhisycalResistance + id.damage)));
		break;
	default:
		result[0] = 0;
		break;
	}
	return result;
}