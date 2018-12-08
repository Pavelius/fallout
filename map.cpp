#include "main.h"

const unsigned short	Blocked = 0xFFFF;
const int				stx = tile_width / 5; // 16 - Ширина юнита тайла. Каждый тайл имеет размер в 5х3 юнита.
const int				sty = tile_height / 3; // 12 - Высота юнита тайла. Каждый тайл имеет размер в 5х3 юнита.

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

short unsigned map_info::geti(int x, int y) const {
	if(((unsigned)x) >= width || ((unsigned)y) >= height)
		return Blocked;
	return y * width + x;
}

void map_info::settile(short unsigned index, short unsigned value) {
	if(index == Blocked)
		return;
	tiles[index] = value;
}

void map_info::setwall(unsigned char index, short unsigned value) {
	if(index == Blocked)
		return;
	walls[index] = value;
}

void map_info::clear() {
	memset(tiles, 0, sizeof(tiles));
	for(int y = 0; y <= height; y++) {
		for(int x = 0; x < width; x++)
			settile(geti(x, y), 172 + rand() % (190 - 172));
	}
}

short unsigned map_info::moveto(short unsigned index, direction_s d) {
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
		if(x >= (width-1))
			return Blocked;
		return index + 1;
	case Up:
		if(y == 0)
			return Blocked;
		return index - s;
	case Down:
		if(y >= (height-1))
			return Blocked;
		return index + s;
	default:
		return Blocked;
	}
}

void map_info::render_tiles(point screen, point camera) {
	auto ps = draw::gres(res::TILES);
	if(!ps)
		return;
	auto a = draw::getstamp() / 100;
	auto pm = s2m(camera);
	int x1 = pm.x - 8; int x2 = x1 + 8 + 11;
	int y1 = pm.y; int y2 = y1 + 18;
	for(auto y = y1; y < y2; y++) {
		if(y < 0 || y >= height)
			continue;
		for(int x = x1; x < x2; x++) {
			if(x < 0 || x >= width)
				continue;
			auto tv = geti(x, y);
			if(tv > 1) {
				point pt = m2s(x, y);
				point pz = pt + screen - camera;
				draw::image(pz.x, pz.y + tile_height / 2, ps, ps->ganim(tv, a), 0);
			}
		}
	}
}