#include "archive.h"
#include "main.h"

using namespace map;

static unsigned short	floor[width*height];
static unsigned short	roof[width*height];
static unsigned short	objects[width*height * 4];
static unsigned char	flags[width*height * 4];
static unsigned short	path_stack[256 * 256];
static unsigned short	path_cost[256 * 256];
static map::node		path_nodes[1024 * 8];
const int				stx = tile_width / 5; // 16 - ������ ����� �����. ������ ���� ����� ������ � 5�3 �����.
const int				sty = tile_height / 3; // 12 - ������ ����� �����. ������ ���� ����� ������ � 5�3 �����.
static direction_s		land_directions[] = {Up, Right, Down, Left};
static direction_s		land_directions_all[] = {Up, Right, Down, Left, LeftDown, LeftUp, RightDown, RightUp};
static direction_s		land_directions_corners[] = {LeftUp, RightUp, RightDown, LeftDown};
static direction_s		hex_directions[] = {Left, Right, LeftUp, RightUp, RightDown, LeftDown};
static direction_s		land_decode[16] = {Center, Left, Down, LeftDown,
Right, Center, RightDown, RightDown,
Up, LeftUp, Center, LeftUp,
RightUp, Center, Center, Center};

// LeftUp, Up, RightUp, Right, RightDown, Down, LeftDown, Left,
// LeftUp, RightUp, RightDown, LeftDown
land_info land_data[] = {{"����������"},
{"�������", 15, {132, 133, 134, 135}, {131, 129, 130, 127, 126, 118, 117, 124}, {169, 170, 171, 168}},
{"���", 30, {470, 471, 472, 473}, {481, 474, 478, 475, 479, 476, 480, 477}, {403, 402, 404, 403}},
{"�������� ���", 15, {2, 16, 17, 15}, {8, 6, 7, 5, 10, 9, 3, 4}, {12, 13, 14, 11}},
{"�����", 40, {1058, 1059, 1062, 1063}, {1053, 1054, 1056, 1060, 1068, 1066, 1065, 1061}, {1080, 1077, 1069, 1072}},
{"������", 15, {2261, 2274, 2274, 2274}, {2267, 2265, 2266, 2264, 2269, 2268, 2262, 2263}, {2271, 2272, 2273, 2270}},
{"�������� ���", 80, {2237, 2238, 2239, 2240}, {}, {}},
{"������� ���", 40, {1892, 1892, 1892, 1892}, {1899, 1893, 1897, 1895, 1900, 1894, 1898, 1896}, {1903, 1901, 1902, 1892}},
{"��� ������", 50, {21, 22, 23, 24}, {}, {}},
{"����� ������", 40, {57, 63, 57, 63}, {65, 61, 68, 55, 66, 51, 67, 45}, {}},
{"������", 15, {716, 717, 718}, {715, 719}, {}},
};
struct group_info {
	struct element : point {
		constexpr element(short x, short y, int shift = 0) : point{x, y}, shift(shift) {}
		int				shift;
	};
	const char*			name;
	point				size;
	short unsigned		start;
	short unsigned		count;
	aref<element>		additional;
};
static group_info::element add_poseidon[] = {{2, 1},
{4, 1}, {6, 1}, {8, 1}, {10, 1}, {11, 1}, {11, 1},
{10, 2}, {9, 1}, {8, 2}, {7, 1}, {6, 2}, {5, 1},
{4, 2}, {3, 1}, {2, 2}};
static group_info::element add_alien[] = {{7, 4, -1}, {6, 1}, {5, 1}};
static group_info::element add_wall[] = {{4, 1, -1}};
static group_info::element add_building[] = {{4, 1}, {6, 1}, {8, 1}, {7, 1}, {6, 1}, {5, 2}, {4, 1}, {3, 2}, {2, 1}, {1, 1}};
static group_info::element add_some[] = {{9, 1}, {10, 1, -1}, {9, 1}, {9, 2, -1}, {8, 1}, {6, 1, 1}, {4, 1, 1}, {3, 1, 1}};
static group_info::element add_entrance[] = {{8, 2, -1}, {9, 1, -1}, {10, 2, -1}, {8, 1, 2}, {7, 1, 1}};
static group_info::element add_entrance2[] = {{9, 1}, {8, 1}, {5, 4}, {2, 1, 1}, };
static group_info group_data[] = {{"���������� � ������", {2, 3}, 118},
{"���� � ������", {2, 2}, 124},
{"����� � �������", {3, 6}, 474},
{"�����", {2, 5}, 492},
{"����� � ������������", {3, 2}, 500},
{"��������", {0, 0}, 508, 0, add_poseidon},
{"���������", {6, 1}, 757, 0, add_alien},
{"�����", {3, 1}, 812, 0, add_wall},
{"�����", {7, 7}, 826},
{"������ � ������������", {2, 1}, 914, 0, add_building},
{"����� � �����", {10, 1}, 1101, 0, add_some},
{"����", {3, 3}, 1415},
{"�������� �������", {8, 4}, 1425},
{"�����", {9, 3}, 1527, 26},
{"�����", {9, 2}, 1554},
{"����", {3, 2}, 1749},
{"�������", {8, 8}, 1827},
{"������������ �������", {8, 8}, 2028},
{"����� ����� �����", {7, 1}, 2120, 0, add_entrance},
{"������ ����� �����", {10, 1}, 2187, 0, add_entrance2},
};
short unsigned get_group_frame(short unsigned i) {
	return tile_data[group_data[i].start].fid;
}
const char* get_group_name(short unsigned i) {
	return group_data[i].name;
}
unsigned get_group_last() {
	return sizeof(group_data) / sizeof(group_data[0]) - 1;
}
unsigned get_land_last() {
	return sizeof(land_data) / sizeof(land_data[0]) - 1;
}

// ��������� ���������� �����(x,y) �� ������
point m2s(int x, int y) {
	return{(short)(2 * stx*y + 3 * stx*x), (short)(2 * sty*y - sty * x)};
}

// ����������� ������� ����� �� �����������(x,y) �� ������
point s2m(point pt) {
	int x = pt.x + tile_width / 2 - 2;
	int y = pt.y + tile_height / 4 - 2;
	return{(short)((x - 4 * y / 3) / 64), (short)((x + 4 * y) / 128)};
}

// ��������� ���������� ���������(x,y) �� ������
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

short unsigned map::getm(int x, int y) {
	if(((unsigned)x) >= width || ((unsigned)y) >= height)
		return Blocked;
	return y * width + x;
}

short unsigned map::geth(int x, int y) {
	if(((unsigned)x) >= (width * 2) || ((unsigned)y) >= (height * 2))
		return Blocked;
	return y * width * 2 + x;
}

short unsigned map::gettile(short unsigned index) {
	if(index == Blocked)
		return 0;
	return floor[index];
}

short unsigned map::getroof(short unsigned index) {
	if(index == Blocked)
		return 0;
	return roof[index];
}

short unsigned map::getobject(short unsigned index) {
	if(index == Blocked)
		return 0;
	return objects[index];
}

void map::setscene(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	objects[index] = value + FirstScenery;
}

void map::settile(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	floor[index] = value;
}

void map::setroof(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	roof[index] = value;
}

void map::setnone(short unsigned index) {
	if(index == Blocked)
		return;
	objects[index] = 0;
}

void map::setwall(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	objects[index] = value + FirstWall;
}

void map::clear() {
	memset(floor, 0, sizeof(floor));
	memset(roof, 0, sizeof(roof));
	for(int y = 0; y <= height; y++) {
		for(int x = 0; x < width; x++)
			settile(getm(x, y), 172 + rand() % (190 - 172));
	}
	memset(objects, 0, sizeof(objects));
}

short unsigned map::to(short unsigned index, direction_s d) {
	if(index == Blocked)
		return index;
	const auto s = width * 2;
	auto x = index % s;
	auto y = index / s;
	switch(d) {
	case Left:
		if(x == 0)
			return Blocked;
		if((x & 1) == 0)
			return index - 1;
		if(y == 0)
			return Blocked;
		return index - 1 - s;
	case LeftUp:
		if(y == 0)
			return Blocked;
		return index - s;
	case LeftDown:
		if(x == 0)
			return Blocked;
		if((x & 1) != 0)
			return index - 1;
		if(y >= (height * 2 - 1))
			return Blocked;
		return index - 1 + s;
	case Right:
		if(x >= (width * 2 - 1))
			return Blocked;
		if(x & 1)
			return index + 1;
		if(y >= (height * 2 - 1))
			return Blocked;
		return index + 1 + s;
	case RightUp:
		if(x >= (width * 2 - 1))
			return Blocked;
		if((x & 1) == 0)
			return index + 1;
		if(y == 0)
			return Blocked;
		return index + 1 - s;
	case RightDown:
		if(y >= (height * 2 - 1))
			return Blocked;
		return index + s;
	default:
		return Blocked;
	}
}

short unsigned map::tot(short unsigned index, direction_s d) {
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
	case Right:
		if(x >= (width - 1))
			return Blocked;
		return index + 1;
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

short unsigned map::getland(short unsigned tile) {
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

unsigned short map::getlandtile(short unsigned index, short unsigned value) {
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

static void updateland() {
	for(auto index = 0; index < width*height; index++) {
		auto value = getland(floor[index]);
		if(value)
			floor[index] = getlandtile(index, value);
	}
}

void map::setgroup(short unsigned index, short unsigned start, short unsigned width, short unsigned height, int count) {
	auto pb = 0;
	if(!count)
		count = width * height;
	for(auto y = height; y > 0; y--) {
		auto i1 = index + width;
		auto pi = pb;
		for(auto i = index; i < i1; i++) {
			if(i == Blocked)
				break;
			if(pi >= count)
				break;
			floor[i] = tile_data[start + pi].fid;
			pi++;
		}
		pb += width;
		index = tot(index, Down);
		if(index == Blocked)
			break;
	}
}

void map::setgroup(short unsigned index, short unsigned group) {
	setgroup(index, group_data[group].start,
		group_data[group].size.x, group_data[group].size.y, group_data[group].count);
	auto m = group_data[group].start + group_data[group].size.x * group_data[group].size.y;
	index += group_data[group].size.y * width;
	for(auto& e : group_data[group].additional) {
		index += e.shift;
		setgroup(index, m, e.x, e.y, 0);
		m += e.x * e.y;
		index += e.y * width;
	}
}

void map::setland(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	floor[index] = getlandtile(index, value);
	updateland();
}

void map::setlandx(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	if(land_data[value].tiles[0]) {
		floor[index] = getlandtile(index, value);
		for(auto e : land_directions_all) {
			auto i = tot(index, e);
			if(i != Blocked)
				floor[i] = getlandtile(i, value);
		}
		updateland();
	} else
		floor[index] = land_data[value].random();
}

void map::serialize(bool write_mode) {
	char temp[32]; szprint(temp, zendof(temp), "temp/AR%1.5i.map", 200);
	io::file file(temp, write_mode ? StreamWrite : StreamRead);
	if(!file)
		return;
	archive a(file, write_mode);
	a.set(floor, sizeof(floor));
	a.set(roof, sizeof(roof));
	a.set(objects, sizeof(objects));
}

void map::blockimpassable(short unsigned free_state) {
	auto my = height * 2;
	auto mx = width * 2;
	for(short unsigned y = 0; y < my; y++) {
		short unsigned i = y * mx;
		short unsigned i2 = i + mx;
		while(i < i2) {
			path_cost[i] = isblocked(i) ? Blocked : free_state;
			i++;
		}
	}
}

map::node* map::addnode() {
	for(auto& e : path_nodes) {
		if(!e.index) {
			e.next = 0;
			return &e;
		}
	}
	path_nodes[0].index = 0;
	path_nodes[0].next = 0;
	return path_nodes;
}

int map::getnodecount() {
	int result = 0;
	for(auto& e : path_nodes) {
		if(e.index)
			result++;
	}
	return result;
}

unsigned short map::getcost(unsigned short index) {
	return path_cost[index];
}

void map::setcost(short unsigned index, short unsigned value) {
	path_cost[index] = value;
}

map::node* map::removeall(map::node* p) {
	while(p) {
		p->index = 0;
		p = p->next;
	}
	return 0;
}

map::node* map::remove(map::node* p) {
	auto p1 = p->next;
	p->index = 0;
	p->next = 0;
	return p1;
}

map::node* map::removeback(node* p) {
	auto start = p;
	node* result = 0;
	while(p->next) {
		result = p;
		p = p->next;
	}
	result->next = 0;
	p->index = 0;
	p->next = 0;
	return start;
}

void map::createwave(short unsigned start, short unsigned max_cost) {
	short unsigned path_push = 0;
	short unsigned path_pop = 0;
	path_stack[path_push++] = start;
	path_cost[start] = 1;
	while(path_push != path_pop) {
		auto n = path_stack[path_pop++];
		auto w = path_cost[n] + 1;
		if(w >= max_cost)
			continue;
		for(auto d : hex_directions) {
			auto i = to(n, d);
			if(i == Blocked || path_cost[i] >= Blocked)
				continue;
			if(!path_cost[i] || path_cost[i] > w) {
				path_cost[i] = w;
				path_stack[path_push++] = i;
			}
		}
	}
}

short unsigned map::stepto(short unsigned index) {
	auto current_index = Blocked;
	auto current_value = Blocked - 1;
	for(auto d : hex_directions) {
		auto i = to(index, d);
		if(i >= Blocked - 1)
			continue;
		if(path_cost[i] < current_value) {
			current_value = path_cost[i];
			current_index = i;
		}
	}
	return current_index;
}

short unsigned map::stepfrom(short unsigned index) {
	auto current_index = Blocked;
	auto current_value = 0;
	for(auto d : hex_directions) {
		auto i = to(index, d);
		if(i >= Blocked - 1)
			continue;
		if(path_cost[i] > current_value) {
			current_value = path_cost[i];
			current_index = i;
		}
	}
	return current_index;
}

// Calculate path step by step to any cell on map analizing create_wave result.
// Go form goal to start and get lowest weight.
// When function return 'path_stack' has step by step path and 'path_push' is top of this path.
map::node* map::route(short unsigned start, short unsigned(*proc)(short unsigned index), short unsigned maximum_range, short unsigned minimal_reach) {
	node* result = 0;
	node* p = 0;
	auto w = 0;
	minimal_reach += 1; // Base cost is one
	for(auto n = proc(start); n != Blocked && path_cost[n] >= 1; n = proc(n)) {
		if(!p) {
			result = addnode();
			p = result;
		} else {
			p->next = addnode();
			p = p->next;
		}
		p->index = n;
		w += 1;
		if(minimal_reach >= path_cost[n])
			break;
		if(maximum_range && w >= maximum_range)
			break;
	}
	return result;
}

bool map::isblocked(short unsigned index) {
	return objects[index]!=0;
}

short unsigned creature::getindex() const {
	auto pt = h2m(*this);
	return geth(pt.x, pt.y);
}