#include "archive.h"
#include "main.h"

const unsigned short	Blocked = 0xFFFF;
map_info				map;
const int				stx = tile_width / 5; // 16 - ������ ����� �����. ������ ���� ����� ������ � 5�3 �����.
const int				sty = tile_height / 3; // 12 - ������ ����� �����. ������ ���� ����� ������ � 5�3 �����.

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

short unsigned map_info::getwall(short unsigned index) const {
	if(index == Blocked)
		return 0;
	return walls[index];
}

void map_info::setscene(int x, int y, short unsigned value) {
	scenery* ps = 0;
	auto pt = m2h(x, y);
	for(auto& e : scenes) {
		if(pt.x == e.x && pt.y == e.y) {
			ps = &e;
			break;
		}
	}
	if(!ps)
		ps = scenes.add();
	ps->x = pt.x;
	ps->y = pt.y;
	ps->type = value;
}

void map_info::settile(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	tiles[index] = value;
}

void map_info::setwall(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	walls[index] = value;
}

void map_info::clear() {
	memset(tiles, 0, sizeof(tiles));
	for(int y = 0; y <= height; y++) {
		for(int x = 0; x < width; x++)
			settile(getm(x, y), 172 + rand() % (190 - 172));
	}
}

short unsigned map_info::to(short unsigned index, direction_s d) {
	if(index == Blocked)
		return index;
	auto x = index % (width * 2);
	auto y = index / (width * 2);
	auto s = (width * 2);
	switch(d) {
	case Left:
		if(x == 0)
			return Blocked;
		return index - 1;
	case Right:
		if(x >= (width - 1))
			return Blocked;
		return index + 1;
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

template<> void archive::set<map_info>(map_info& e) {
	set(e.tiles, sizeof(e.tiles));
	set(e.walls, sizeof(e.walls));
	set(e.scenes);
}

void map_info::serialize(bool write_mode) {
	char temp[32]; szprint(temp, zendof(temp), "temp/AR%1.5i.map", 200);
	io::file file(temp, write_mode ? StreamWrite : StreamRead);
	if(!file)
		return;
	archive a(file, write_mode);
	a.set(*this);
}