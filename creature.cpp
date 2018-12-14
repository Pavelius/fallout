#include "main.h"

struct drug_effect {
	unsigned			stamp;
	creature*			owner;
	item_s				drug;
};

creature					player;
adat<creature>				creature_data;
static adat<drug_effect>	drug_effect_data;
static datetime				today;

void creature::clear() {
	name = 0;
	level = 0;
	gender = Male;
	wounds = 0;
	ap = 0;
	skills_tag = 0;
	armor.clear();
	weapons[0].clear();
	weapons[1].clear();
	memset(stats, 0, sizeof(stats));
	memset(stats_boost, 0, sizeof(stats_boost));
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
	switch(value) {
	case FirstAid:
	case Doctor:
	case Speech:
	case Barter:
		if(is(GoodNatured))
			result += 15;
		break;
	case BigGuns:
	case SmallGuns:
	case EnergyWeapon:
	case Throwing:
	case MeleeWeapon:
	case Unarmed:
		if(is(GoodNatured))
			result -= 10;
		break;
	}
	return result;
}

int	creature::get(skill_s id) const {
	return getbase(id) + skills[id];
}

int	creature::getequipweight() const {
	auto result = armor.getweight();
	result += weapons[0].getweight();
	result += weapons[1].getweight();
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
		if(!weapons[0]) {
			weapons[0] = it;
			return true;
		}
		if(!weapons[1]) {
			weapons[1] = it;
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
	ability_s max_ability;
	switch(id.type) {
	case Abilities:
		if(ability_data[id.ability].percent)
			pr = "%1i%%";
		max_ability = getmaximum(id.ability);
		if(max_ability && show_maximum_only)
			id.ability = max_ability;
		if(max_ability && !show_maximum_only)
			szprint(result, result_maximum, "%1i/%2i", get(id.ability), get(max_ability));
		else
			szprint(result, result_maximum, pr, get(id.ability));
		break;
	case Skills:
		szprint(result, result_maximum, "%1.2i%%", get(id.skill));
		break;
	case Parameters:
		szprint(result, result_maximum, "%1i", get(id.parameter));
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

int	creature::getnextlevel() const {
	return 1000;
}

void creature::get(hit_info& hi, const item weapon, action_s id) const {
	hi.attack = weapon.getattack();
	hi.ammo = weapon.getammoinfo();
	hi.skill = weapon.getskill(id);
	if(weapon.ismelee()) {
		hi.attack.damage.max += get(DamageMelee);
		if(is(HeavyHanded))
			hi.critical.hit.effect -= 30;
	} else {
		if(is(Finesse))
			hi.ammo.dam_resist += 30;
		if(is(FastShoot)) {
			for(auto i = 0; i < 2; i++) {
				switch(hi.attack.actions[i]) {
				case FireBurst:
				case FireSingle:
				case FireContinuous:
				case Throwing:
					if(hi.attack.ap[0]>1)
						hi.attack.ap[0]--;
					break;
				}
			}
		}
	}
	hi.ammo.ac += get(hi.skill);
	if(is(WoundLeftHand) && weapon.istwohanded())
		hi.ammo.ac -= 30;
	if(is(WoundRightHand))
		hi.ammo.ac -= 20;
	if(is(WoundEye))
		hi.ammo.ac -= 25;
	if(is(OneHanded)) {
		if(!weapon.istwohanded())
			hi.ammo.ac += 20;
		else
			hi.ammo.ac -= 40;
	}
	hi.critical.miss.chance += getcriticalmiss();
	hi.critical.hit.chance += get(CriticalHit);
	hi.critical.hit.effect += get(CriticalHitTable);
}

int	creature::getcriticalmiss() const {
	auto result = 13 - get(Luck);
	if(is(Jinxed))
		result += 20;
	return result;
}

void creature::damage(const damage_info& di) {
	auto value = di.min + (rand() % (di.max - di.min));
}

const sprite* creature::getsprite() const {
	return draw::gres(armor.getdress(gender));
}

void weaponable::change_weapon() {
	iswap(weapons[0], weapons[1]);
	iswap(weapon_action[0], weapon_action[1]);
}