#include "main.h"

const unsigned short	Blocked = 0xFFFF;
const int				stx = tile_width / 5; // 16 - Ширина юнита тайла. Каждый тайл имеет размер в 5х3 юнита.
const int				sty = tile_height / 3; // 12 - Высота юнита тайла. Каждый тайл имеет размер в 5х3 юнита.
int						map::width;
int						map::height;
static short unsigned	floors[256 * 256];

static struct landscape {
	unsigned short		limits[2];
	unsigned short		sides[8];
	unsigned short		center[4];
	unsigned short		corners[4];
} landscapes[] = {{{2, 17}, {8, 6, 7, 4, 5, 3, 9, 10}, {2, 15, 16, 17}, {14, 11, 12, 13}}, // Внутренняя часть дома
{{117, 171}, {131, 129, 130, 124, 127, 117, 118, 126}, {132, 133, 134, 135}},
//
{{0, 0}, {136, 146, 137, 140, 142, 139, 144, 138}, {151, 149, 148}},
{{0, 0}, {1053, 1054, 1056, 1057, 1060, 1065, 1066, 1068}, {1062, 1063}},
{{0, 0}, {171, 122, 168, 127, 160, 170, 164, 169}, {1062, 1063}},
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

void map::create() {
	width = 128;
	height = 128;
	memset(floors, 0, sizeof(floors));
	for(int y = 0; y <= height; y++) {
		for(int x = 0; x < width; x++)
			floors[y*scanline + x] = 172 + rand() % (190 - 172);
	}
}

int get_type(short unsigned value) {
	for(int i = 0; i < sizeof(landscapes) / sizeof(landscapes[0]); i++) {
		for(auto m : landscapes[i].sides) {
			if(m == value)
				return i;
		}
		for(auto m : landscapes[i].corners) {
			if(m == value)
				return i;
		}
		for(auto m : landscapes[i].center) {
			if(m == value)
				return i;
		}
	}
	return 0;
}

short unsigned get_tile(int index) {
	return index;
}

void map::set(rect rc, int id) {
	if(rc.width() < 2 || rc.height() < 2)
		return;
	auto& e = landscapes[id];
	for(int y = rc.y1; y <= rc.y2; y++) {
		for(int x = rc.x1; x <= rc.x2; x++) {
			int i = m2i(x, y);
			if(y == rc.y1) {
				if(x == rc.x1)
					set(i, e.sides[0]);
				else if(x == rc.x2)
					set(i, e.sides[2]);
				else
					set(i, e.sides[1]);
			} else if(y == rc.y2) {
				if(x == rc.x1)
					set(i, e.sides[5]);
				else if(x == rc.x2)
					set(i, e.sides[7]);
				else
					set(i, e.sides[6]);
			} else {
				if(x == rc.x1)
					set(i, e.sides[3]);
				else if(x == rc.x2)
					set(i, e.sides[4]);
				else {
					if(e.center[1] && d100() < 30) {
						if(e.center[2] && d100() < 50) {
							int count = zlen(e.center + 2);
							set(i, e.center[2 + rand() % count]);
						} else
							set(i, e.center[1]);
					} else
						set(i, e.center[0]);
				}
			}
		}
	}
}

void map::set(short unsigned i, short unsigned fid) {
	if(i == Blocked)
		return;
	floors[i] = fid;
}

short unsigned map::get(short unsigned i) {
	if(i == Blocked)
		return 0;
	return floors[i];
}

short unsigned map::moveto(short unsigned index, direction_s d) {
	switch(d) {
	case Left:
		if(i2x(index) == 0)
			return Blocked;
		return index - 1;
	case Right:
		if(i2x(index + 1) == 0)
			return Blocked;
		return index + 1;
	case Up:
		if(i2y(index) == 0)
			return Blocked;
		return index - scanline;
	case Down:
		if((1 + i2y(index)) == height)
			return Blocked;
		return index + scanline;
	case RightDown:
		if(((index + 1) % scanline) == 0)
			return Blocked;
		if((1 + index / scanline) == width)
			return Blocked;
		return index + scanline + 1;
	case RightUp:
		if(((index + 1) % scanline) == 0)
			return Blocked;
		if((index / scanline) == 0)
			return Blocked;
		return index - scanline + 1;
	case LeftUp:
		if((index%scanline) == 0)
			return Blocked;
		if((index / scanline) == 0)
			return Blocked;
		return index - scanline - 1;
	case LeftDown:
		if((index%scanline) == 0)
			return Blocked;
		if((index / scanline) == 0)
			return Blocked;
		return index + scanline - 1;
	default:
		return Blocked;
	}
}