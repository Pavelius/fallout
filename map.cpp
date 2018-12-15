#include "archive.h"
#include "main.h"

const unsigned short	Blocked = 0xFFFF;
map_info				map;
const int				stx = tile_width / 5; // 16 - Ширина юнита тайла. Каждый тайл имеет размер в 5х3 юнита.
const int				sty = tile_height / 3; // 12 - Высота юнита тайла. Каждый тайл имеет размер в 5х3 юнита.
static direction_s		land_directions[] = {Up, Right, Down, Left};
static direction_s		land_directions_all[] = {Up, Right, Down, Left, LeftDown, LeftUp, RightDown, RightUp};
static direction_s		land_directions_corners[] = {LeftUp, RightUp, RightDown, LeftDown};
static direction_s		land_decode[16] = {Center, Left, Down, LeftDown,
Right, Center, RightDown, RightDown,
Up, LeftUp, Center, LeftUp,
RightUp, Center, Center, Center};

// LeftUp, Up, RightUp, Right, RightDown, Down, LeftDown, Left,
// LeftUp, RightUp, RightDown, LeftDown
land_info land_data[] = {{"Недоступно"},
{"Тротуар", 15, {132, 133, 134, 135}, {131, 129, 130, 127, 126, 118, 117, 124}, {169, 170, 171, 168}},
{"Пол", 30, {470, 471, 472, 473}, {481, 474, 478, 475, 479, 476, 480, 477}, {403, 402, 404, 403}},
{"Кипичный пол", 15, {2, 16, 17, 15}, {8, 6, 7, 5, 10, 9, 3, 4}, {12, 13, 14, 11}},
{"Слизь", 40, {1058, 1059, 1062, 1063}, {1053, 1054, 1056, 1060, 1068, 1066, 1065, 1061}, {1080, 1077, 1069, 1072}},
{"Плитка", 15, {2261, 2274, 2274, 2274}, {2267, 2265, 2266, 2264, 2269, 2268, 2262, 2263}, {2271, 2272, 2273, 2270}},
{"Каменный пол", 80, {2237, 2238, 2239, 2240}, {}, {}},
//
{"Огород", 15, {716, 717, 718}, {715, 719}, {}},
};
struct group_info {
	const char*			name;
	point				size;
	short unsigned		start;
	short unsigned		count;
};
static group_info group_data[] = {{"", {3, 3}, 1415},
{"Каменная впадина", {8, 4}, 1425},
{"Скалы", {9, 3}, 1527, 26},
{"Скалы", {9, 2}, 1554},
{"Мост", {3, 2}, 1749},
{"Реактор", {8, 8}, 1827},
{"Неработающий реактор", {8, 8}, 2028},
};

// Получение координаты тайла(x,y) на экране
point m2s(int x, int y) {
	return{(short)(2 * stx*y + 3 * stx*x), (short)(2 * sty*y - sty * x)};
}

// Определение индекса тайла по координатам(x,y) на экране
point s2m(point pt) {
	int x = pt.x + tile_width / 2 - 2;
	int y = pt.y + tile_height / 4 - 2;
	return{(short)((x - 4 * y / 3) / 64), (short)((x + 4 * y) / 128)};
}

// Получение координаты хексагона(x,y) на экране
point m2h(int x, int y) {
	int x1 = stx * (y + x + x / 2) - stx / 2;
	int y1 = sty * (y - x + x / 2) - sty / 2;
	return{(short)x1, (short)y1};
}

point h2m(point pt) {
	int x1 = pt.x + stx / 2;
	int y1 = pt.y;
	int x0 = 4800;
	int nx;
	if(x1 - x0 < 0)
		nx = (x1 - x0 + 1) / stx - 1;
	else
		nx = (x1 - x0) / stx;
	int ny;
	if(y1 < 0)
		ny = (y1 + 1) / sty - 1;
	else
		ny = y1 / sty;
	if(iabs(nx) % 2 != iabs(ny) % 2)
		nx--;
	long xhBase = x0 + stx * nx;
	long yhBase = sty * ny;
	int x = (4 * yhBase - 3 * (xhBase - x0)) / 96;
	int y = yhBase / 12 - x / 2;
	x = 150 - x;
	y = y + 76;
	return{(short)x, (short)y};
}

short unsigned map_info::getm(int x, int y) const {
	if(((unsigned)x) >= width || ((unsigned)y) >= height)
		return Blocked;
	return y * width + x;
}

short unsigned map_info::geth(int x, int y) const {
	if(((unsigned)x) >= (width * 2) || ((unsigned)y) >= (height * 2))
		return Blocked;
	return y * width * 2 + x;
}

short unsigned map_info::gettile(short unsigned index) const {
	if(index == Blocked)
		return 0;
	return tiles[index];
}

short unsigned map_info::getobject(short unsigned index) const {
	if(index == Blocked)
		return 0;
	return objects[index];
}

void map_info::setscene(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	objects[index] = value + FirstScenery;
}

void map_info::settile(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	tiles[index] = value;
}

void map_info::setnone(short unsigned index) {
	if(index == Blocked)
		return;
	objects[index] = 0;
}

void map_info::setwall(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	objects[index] = value + FirstWall;
}

void map_info::clear() {
	memset(tiles, 0, sizeof(tiles));
	for(int y = 0; y <= height; y++) {
		for(int x = 0; x < width; x++)
			settile(getm(x, y), 172 + rand() % (190 - 172));
	}
	memset(objects, 0, sizeof(objects));
}

short unsigned map_info::to(short unsigned index, direction_s d) const {
	if(index == Blocked)
		return index;
	auto x = index % (width * 2);
	auto y = index / (width * 2);
	const auto s = (width * 2);
	switch(d) {
	case Left:
		if(x == 0)
			return Blocked;
		return index - 1;
	case Right:
		if(x >= (width * 2 - 1))
			return Blocked;
		return index + 1;
	case Up:
		if(y == 0)
			return Blocked;
		return index - s;
	case Down:
		if(y >= (height * 2 - 1))
			return Blocked;
		return index + s;
	default:
		return Blocked;
	}
}

short unsigned map_info::tot(short unsigned index, direction_s d) const {
	if(index == Blocked)
		return index;
	auto x = index % width;
	auto y = index / width;
	auto s = width;
	switch(d) {
	case Left:
		if(x == 0)
			return Blocked;
		return index - 1;
	case LeftUp:
		if(x == 0)
			return Blocked;
		if(y == 0)
			return Blocked;
		return index - 1 - s;
	case LeftDown:
		if(x == 0)
			return Blocked;
		if(y >= (height - 1))
			return Blocked;
		return index - 1 + s;
	case Right:
		if(x >= (width - 1))
			return Blocked;
		return index + 1;
	case RightUp:
		if(x >= (width - 1))
			return Blocked;
		if(y == 0)
			return Blocked;
		return index + 1 - s;
	case RightDown:
		if(x >= (width - 1))
			return Blocked;
		if(y >= (height - 1))
			return Blocked;
		return index + 1 + s;
	case Up:
		if(y == 0)
			return Blocked;
		return index - s;
	case Down:
		if(y >= (height - 1))
			return Blocked;
		return index + s;
	default:
		return Blocked;
	}
}

short unsigned map_info::getland(short unsigned tile) {
	if(!tile)
		return 0;
	for(short unsigned i = 1; i < sizeof(land_data) / sizeof(land_data[0]); i++) {
		for(auto e : land_data[i].tiles) {
			if(e == tile)
				return i;
		}
		for(auto e : land_data[i].central) {
			if(e == tile)
				return i;
		}
		for(auto e : land_data[i].corners) {
			if(e == tile)
				return i;
		}
	}
	return 0;
}

unsigned short map_info::getlandtile(short unsigned index, short unsigned value) const {
	if(!value)
		return 0;
	if(index == Blocked)
		return 0;
	unsigned char ri = 0;
	for(auto e : land_directions) {
		ri <<= 1;
		auto v = getland(gettile(tot(index, e)));
		if(v != value)
			ri |= 1;
	}
	auto& e = land_data[value];
	auto result = land_decode[ri];
	if(result == Center) {
		if(e.corners[0] && !ri) {
			for(auto i = 0; i < 4; i++) {
				auto v = getland(gettile(tot(index, land_directions_corners[i])));
				if(v != value)
					return e.corners[i];
			}
		}
		return e.random();
	}
	return land_data[value].tiles[result];
}

void map_info::updateland() {
	for(auto index = 0; index < width*height; index++) {
		auto value = getland(tiles[index]);
		if(value)
			tiles[index] = getlandtile(index, value);
	}
}

void map_info::setgroup(short unsigned index, short unsigned group) {
	auto& g = group_data[group];
	auto pb = 0;
	auto count = g.count;
	if(!count)
		count = g.size.x* g.size.y;
	for(auto y = g.size.y; y > 0; y--) {
		auto i1 = index + g.size.x;
		auto pi = pb;
		for(auto i = index; i < i1; i++) {
			if(i == Blocked)
				break;
			if(pi >= count)
				break;
			tiles[i] = g.start + pi;
			pi++;
		}
		pb += g.size.x;
		index = tot(index, Down);
		if(index == Blocked)
			break;
	}
}

short unsigned land_info::getlast() {
	return sizeof(land_data) / sizeof(land_data[0]) - 1;
}

void map_info::setland(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	tiles[index] = getlandtile(index, value);
	updateland();
}

void map_info::setlandx(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	if(land_data[value].tiles[0]) {
		tiles[index] = getlandtile(index, value);
		for(auto e : land_directions_all) {
			auto i = tot(index, e);
			if(i != Blocked)
				tiles[i] = getlandtile(i, value);
		}
		updateland();
	} else
		tiles[index] = land_data[value].random();
}

template<> void archive::set<map_info>(map_info& e) {
	set(e.tiles, sizeof(e.tiles));
	set(e.objects, sizeof(e.objects));
}

void map_info::serialize(bool write_mode) {
	char temp[32]; szprint(temp, zendof(temp), "temp/AR%1.5i.map", 200);
	io::file file(temp, write_mode ? StreamWrite : StreamRead);
	if(!file)
		return;
	archive a(file, write_mode);
	a.set(*this);
}