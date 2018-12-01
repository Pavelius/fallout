#include "main.h"

// TODO: Удача может влиять на обыск объектов сцены.
// Например, есть объект сцены бутылки. Время от времени в бутылках можно найти колу. Шанс зависит от удачи.
// В машинах можно попытаться найти какую-то запчасть к машине.
// Каждый объект сцены должен иметь встроенный таймер, который показывает когда наступило то самое время.
// Может быть от недели до месяца, когда объект сцены можно снова обыскать.
// TODO: У объекта сцены может быть статус поврежден. Время от времени его будут чинить в городе местные люди.

const int		start_year = 2240;
unsigned		game::datetime = mkdate(start_year, 1, 1, 0, 0);
static bool		game_pause = false;

static struct skillinfo {
	int		base;
	int		multiplier;
	int		abilitites[2];
} skills[LastSkill - FirstSkill + 1] = {{5, 4, {Agility}},
{0, 2, {Agility}},
{0, 2, {Agility}},
{30, 2, {Strenght, Agility}},
{20, 2, {Strenght, Agility}},
{0, 4, {Agility}},
{0, 2, {Perception, Intellegence}},
{5, 1, {Perception, Intellegence}},
{5, 3, {Agility}},
{10, 1, {Perception, Agility}},
{10, 1, {Perception, Agility}},
{0, 3, {Agility}},
{0, 4, {Intellegence}},
{0, 3, {Intellegence}},
{0, 5, {Charisma}},
{0, 4, {Charisma}},
{0, 5, {Luck}},
{0, 2, {Endurance, Intellegence}},
};

int game::summ(unsigned char* data, int i1, int i2) {
	int r = 0;
	for(int i = i1; i <= i2; i++)
		r += data[i];
	return r;
}

int game::summnz(unsigned char* data, int i1, int i2) {
	int r = 0;
	for(int i = i1; i <= i2; i++) {
		if(data[i])
			r++;
	}
	return r;
}

const char*	game::getnameshort(int id) {
	switch(id) {
	case MaxAP: return szt("", "Очки действия");
	case CarryWeight: return szt("", "Макс. груз");
	case DamageResistanceNormal: return szt("", "Уст. к повр.");
	case PoisonResistance: return szt("", "Уст. к яду");
	case RadiationResistance: return szt("", "Уст. к рад.");
	case MeleeDamage: return szt("", "Xол. оружие");
	case HealingRate: return szt("", "Уров. лечен.");
	case CriticalHitChance: return szt("", "Шанс на крит.");
	case Male: return szt("", "Муж.");
	case Female: return szt("", "Жен.");
	case DamageNormal: return szt("", "Норма");
	case DamageLaser: return szt("", "Лазер");
	case DamageFire: return szt("", "Огонь");
	case DamagePlasma: return szt("", "Плазма");
	case DamageExplosive: return szt("", "Взрыв");
	case Strenght: return szt("", "СЛ");
	case Perception: return szt("", "МД");
	case Endurance: return szt("", "ВН");
	case Charisma: return szt("", "ОБ");
	case Intellegence: return szt("", "ИН");
	case Agility: return szt("", "ЛВ");
	case Luck: return szt("", "УД");
	case HP: case MaxHP: return szt("Hits", "Жизней");
	default: return bsgets(id, Name);
	}
}

char* game::getformula(char* result, int id) {
	result[0] = 0;
	if(id >= FirstSkill && id <= LastSkill) {
		auto& si = skills[id - FirstSkill];
		if(si.base)
			szprint(zend(result), "%1i%% + ", si.base);
		if(si.multiplier > 1)
			szprint(zend(result), "%1ix", si.multiplier);
		if(si.abilitites[1])
			szprint(zend(result), "(");
		for(int i = 0; i < sizeof(si.abilitites) / sizeof(si.abilitites[0]); i++) {
			if(!si.abilitites[i])
				break;
			if(i != 0)
				zcat(result, "+");
			zcat(result, getnameshort(si.abilitites[i]));
		}
		if(si.abilitites[1])
			szprint(zend(result), ")");
	}
	return result;
}

char* game::getname(char* result, unsigned char* data) {
	result[0] = 0;
	zcpy(result, getnamepart(data[FirstNamePart]));
	return result;
}

char* game::getname(char* result, int rec) {
	if(rec >= FirstPlayer && rec <= LastPlayer) {
		unsigned char* attr = (unsigned char*)bsptr(rec, 0);
		return getname(result, attr);
	}
	result[0] = 0;
	return result;
}

int game::getlongest(point from, point to) {
	return imax(iabs(from.x - to.x), iabs(from.y - to.y));
}

int game::getorientation(point from, point to) {
	static const char orientations[25] =
	{
		5, 5, 0, 0, 0,
		5, 5, 0, 0, 0,
		4, 4, 2, 1, 1,
		3, 3, 2, 2, 2,
		3, 3, 3, 2, 2
	};
	int dx = to.x - from.x;
	int dy = to.y - from.y;
	int div = imax(iabs(dx), iabs(dy));
	if(!div)
		return 2; //default
	if(div > 3)
		div /= 2;
	int ax = dx / div;
	int ay = dy / div;
	return orientations[(ay + 2) * 5 + ax + 2];
}

int game::getdistance(point p1, point p2) {
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return isqrt(dx*dx + dy * dy);
}

short unsigned game::getnextindex(short unsigned index, int orientation) {
	// 0 - Right Up
	// 1 - Right
	// 2 - Right Down
	// 3 - Left Down
	// 4 - Left
	// 5 - Left 
	int nx = i2x(index);
	switch(orientation) {
	case 0:
		if(nx & 1)
			return index - 256 + 1;
		return index + 1;
	case 1:
		if(nx & 1)
			return index + 1;
		return index + 256 + 1;
	case 2:
		return index + 1 * 256;
	case 3:
		if(nx & 1)
			return index - 1;
		return index + 256 - 1;
	case 4:
		if(nx & 1)
			return index - 256 - 1;
		return index - 1;
	case 5:
		return index - 1 * 256;
	}
	return index;
}

int game::find(int id, unsigned char* data, int i1, int i2) {
	for(int i = i1; i <= i2; i++) {
		if(data[i] == id)
			return i;
	}
	return 0;
}

int game::get(int id, unsigned char* data) {
	if(id >= FirstSkill && id <= LastSkill) {
		int r = data[id];
		// Apply tagged skill
		if(data[Tag1] == id || data[Tag2] == id || data[Tag3] == id || data[Tag4] == id)
			r += 20;
		// Add formula
		auto& si = skills[id - FirstSkill];
		int s = 0;
		for(auto e : si.abilitites) {
			if(!e)
				continue;
			s += get(e, data);
		}
		s *= si.multiplier;
		s += si.base;
		r += s;
		if(get(GoodNatured, data)) {
			switch(id) {
			case SkillFirstAid:
			case SkillDoctor:
			case SkillSpeech:
			case SkillBarter:
				r += 15;
				break;
			case SkillBigGuns:
			case SkillSmallGuns:
			case SkillEnergyWeapon:
			case SkillThrowing:
			case SkillUnarmed:
			case SkillMeleeWeapon:
				r -= 10;
				break;
			}
		}
		if(get(Gifted, data))
			r -= 10;
		if(r < 0)
			r = 0;
		return r;
	} else if(id >= FirstTraits && id <= LastTraits) {
		if(data[Trait1] == id || data[Trait2] == id)
			return 1;
		return 0;
	} else if(id >= FirstStat && id <= LastStat) {
		int r = data[id];
		if(get(Gifted, data))
			r++;
		if(get(SmallFrame, data) && id == Agility)
			r++;
		if(get(Bruiser, data) && id == Strenght)
			r += 2;
		if(r < 1)
			r = 1;
		else if(r > 10)
			r = 10;
		return r;
	}
	int r;
	switch(id) {
	case SkillsPerLevel:
		return 2 * get(Intellegence, data) + 5 + get(Skilled, data) * 5 - get(Gifted, data) * 5 + get(Educated, data) * 2;
	case MaxHP:
		return 15 + 2 * get(Endurance, data) + get(Strenght, data)
			+ (2 + get(Endurance, data) / 2)*(get(Level, data) - 1)
			+ get(Lifegiver, data) * 4;
	case MaxAP:
		return 5 + get(Agility, data) / 2 - get(Bruiser, data) * 2 + get(ActionBoy, data);
	case AC:
		if(get(Kamikaze, data))
			r = 0;
		else
			r = get(Agility, data);
		return r + get(Dodger, data);
	case CarryWeight:
		if(get(SmallFrame, data))
			return 25 + 15 * get(Strenght, data) + get(StrongBack, data) * 50;
		return 25 + 25 * get(Strenght, data) + get(StrongBack, data) * 50;
	case MeleeDamage:
		return get(Strenght, data) - 5 + get(HeavyHanded, data) * 4 + get(BonusHtHDamage, data) * 2;
	case PoisonResistance:
		if(get(FastMetabolism, data))
			r = 0;
		else
			r = get(Endurance, data) * 5;
		return r + get(SnakeEater, data) * 25;
	case RadiationResistance:
		if(get(FastMetabolism, data))
			return 0;
		return get(Endurance, data) * 2 + get(RadResistance, data) * 15;
	case Sequence:
		return get(Perception, data) * 2 + get(Kamikaze, data) * 5 + get(EarlierSequence, data) * 2;
	case HealingRate:
		return get(Endurance, data) / 3 + get(FastMetabolism, data) * 2 + get(FasterHealing, data) * 2;
	case CriticalHitChance:
		return get(Luck, data) / 3 + get(MoreCriticals, data) * 5;
	default:
		if(id > LastToken)
			return 0;
		return data[id];
	}
}

int game::get(int player, int id) {
	unsigned char* data = (unsigned char*)bsptr(player, 0);
	if(!data)
		return 0;
	if(id == HP)
		return bsget(player, HP);
	if(id == AC
		|| (id >= FirstDamageType && id <= LastDamageType)
		|| (id >= FirstDamageResistance && id <= LastDamageResistance)) {
		int r = get(id, data);
		r += item::get(bsget(player, Armor), id);
		return r;
	} else
		return get(id, data);
}

void game::generate::creature(unsigned char* data, int premade) {
	memset(data, 0, LastToken + 1);
	for(int i = FirstStat; i <= LastStat; i++)
		data[i] = bsget(premade, i);
	for(int i = Tag1; i <= Tag4; i++)
		data[i] = bsget(premade, i);
	for(int i = Trait1; i <= Trait2; i++)
		data[i] = bsget(premade, i);
	static int other[] = {Level, Gender, Age};
	for(auto i : other)
		data[i] = bsget(premade, i);
	const char* name = bsgets(premade, Name);
	data[FirstNamePart] = game::findname(name);
}

int game::generate::scenery(int type, int index) {
	int rec = bscreate(FirstScenery);
	if(!rec)
		return 0;
	point pt = i2h(index);
	bsset(rec, Sprite, res::SCENERY);
	bsset(rec, Animation, type);
	bsset(rec, Position, *((int*)&pt));
	return rec;
}

int game::generate::player(int type, int index, int orientation) {
	unsigned char data[LastToken + 2];
	game::generate::creature(data, type);
	return game::generate::player(data, index, orientation);
}

struct itemwear {
	int			owner;
	int			object;
};
static aref<itemwear> wears;

void game::additem(int player, int rec) {
	int item_type = item::gettype(rec);
	int c2 = item::get(rec, Count);
	for(auto& e : wears) {
		if(e.owner != player || item::gettype(e.object) != item_type)
			continue;
		if(item::add(e.object, rec))
			return;
	}
	for(auto& e : wears) {
		if(e.owner)
			continue;
		e.owner = player;
		e.object = rec;
		return;
	}
	wears.reserve();
	auto& e = wears.add();
	e.owner = player;
	e.object = rec;
}

int game::getwears(int** result, int player) {
	auto pb = result;
	auto pe = pb + 1024; // Maximum 1024 items per creature
	for(auto& e : wears) {
		if(e.owner != player || !e.object)
			continue;
		*pb++ = &e.object;
		if(pb >= pe)
			break;
	}
	*pb = 0;
	return pb - result;
}

bool game::getdamage(int player, int id, damagei& result) {
	result.min = 1;
	result.max = 1;
	result.type = DamageNormal;
	int w = bsget(player, id);
	if(w) {
		result.min = item::get(w, DamageMinimum);
		result.max = item::get(w, DamageMaximum);
		result.type = (tokens)item::get(w, DamageType);
	}
	return true;
}

int game::getwearsweight(int player) {
	int* wear[1024 + 2];
	int result = 0;
	result += item::get(bsget(player, Weapon), CarryWeight);
	result += item::get(bsget(player, WeaponSecondonary), CarryWeight);
	result += item::get(bsget(player, Armor), CarryWeight);
	int count = getwears(wear, player);
	for(int i = 0; i < count; i++)
		result += item::get(*wear[i], CarryWeight);
	return result;
}

void game::passtime(int count) {
	datetime += count;
}

static unsigned char worldmap[world_map_height][world_map_width];

int game::getwm(int x, int y) {
	if(x < 0 || x >= world_map_width)
		return 0;
	if(y < 0 || y >= world_map_height)
		return 0;
	return worldmap[y][x];
}

void game::setwm(int x, int y, int value) {
	if(x < 0 || x >= world_map_width)
		return;
	if(y < 0 || y >= world_map_height)
		return;
	if(getwm(x, y) > value)
		return;
	worldmap[y][x] = value;
}