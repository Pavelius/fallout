#include "main.h"

using namespace draw;

settlement settlement_data[] = {{"Аройо", {184, 133}, 1},
{"Яма", {473, 272}, 2},
{"Кламат", {373, 122}, 2},
};

const int		world_map_tile_width = 50; // Width of world map tile
const int		world_map_tile_height = 50; // Height of world map tile
const int		world_map_width = 28; // Width of world map int tiles
const int		world_map_height = 30; // Height of world map int tiles
const int		dx = 350;
const int		dy = 300;

static unsigned char worldmap[world_map_height][world_map_width];

static int getwm(int x, int y) {
	if(x < 0 || x >= world_map_width)
		return 0;
	if(y < 0 || y >= world_map_height)
		return 0;
	return worldmap[y][x];
}

static void setwm(int x, int y, char value) {
	if(x < 0 || x >= world_map_width)
		return;
	if(y < 0 || y >= world_map_height)
		return;
	if(getwm(x, y) > value)
		return;
	worldmap[y][x] = value;
}

inline point m2w(int x, int y) {
	return{(short)(x * world_map_tile_width), (short)(y * world_map_tile_height)};
}

struct adventurer : drawable {
	point		pos;
	point		start, target;
	unsigned	timestart;
	bool		car;

	adventurer(point pos, bool car) : pos(pos), start(pos), target(pos), car(car) {
		explore();
	}

	void explore() {
		int x = pos.x / world_map_tile_width;
		int y = pos.y / world_map_tile_height;
		setwm(x, y, 2);
		setwm(x - 1, y - 1, 1);
		setwm(x - 0, y - 1, 1);
		setwm(x + 1, y - 1, 1);
		setwm(x - 1, y, 1);
		setwm(x + 1, y, 1);
		setwm(x - 1, y + 1, 1);
		setwm(x - 0, y + 1, 1);
		setwm(x + 1, y + 1, 1);
	}

	bool canmove() const {
		return pos != target;
	}

	void stop() {
		target = pos;
		start = pos;
	}

	int getspeed() const {
		return car ? 10 : 2;
	}

	rect getrect() const override {
		return {pos.x - 4, pos.y - 4, pos.x + 4, pos.y + 4};
	}

	point getposition() const override {
		return pos;
	}

	void update() override {
		unsigned hours = (creature::getdate() - timestart) / 60;
		int dx = target.x - start.x;
		int dy = target.y - start.y;
		int lenght_total = isqrt(dx*dx + dy * dy);
		int lenght_passed = getspeed()*hours;
		if(lenght_passed >= lenght_total) {
			pos = target;
			explore();
			stop();
		} else {
			pos.x = start.x + dx * lenght_passed / lenght_total;
			pos.y = start.y + dy * lenght_passed / lenght_total;
			explore();
		}
	}

	void painting(point camera) const override {
		auto ps = gres(res::INTRFACE);
		if(!ps)
			return;
		if(target != pos) {
			auto pt = pos - camera;
			draw::image(pt.x, pt.y, ps, ps->ganim(138, 0), 0);
			pt = target - camera;
			draw::image(pt.x - 5, pt.y - 5, ps, ps->ganim(139, 0), ImageNoOffset);
		} else {
			auto fr = ps->ganim(168, 0);
			auto pf = ps->get(fr);
			auto pt = pos - camera;
			draw::image(pt.x - pf.sx / 2, pt.y - pf.sy, ps, fr, ImageNoOffset);
		}
	}

	void moveto(point pos) {
		start = this->pos;
		target = pos;
		timestart = creature::getdate();
	}

};

// Вся карта состоит из тайлов 50 на 50 пикселей.
// Тайлы группируются блоками - один рисунок 7 тайлов на 6 тайлов.
static void draw_map(const rect& screen, point camera, adventurer& player, sprite* ps, bool show_settlement) {
	auto fr = ps->ganim(339, 0);
	draw::state push;
	draw::setclip(screen);
	point origin = {(short)screen.x1, (short)screen.y1};
	// Нарисуем фоновые текстуры
	for(int iy = 0; iy < 5; iy++) {
		int y1 = dy * iy + screen.y1 - camera.y;
		for(int ix = 0; ix < 4; ix++) {
			int x1 = dx * ix + screen.x1 - camera.x;
			draw::image(x1, y1, ps, fr + iy * 4 + ix, ImageNoOffset);
		}
	}
	// Нарисуем поселения и локации
	if(show_settlement) {
		color pallette[256];
		memcpy(pallette, draw::palt, sizeof(pallette));
		draw::modify(pallette, 0, ColorText, 16);
		draw::state push;
		draw::palt = pallette;
		for(auto& e : settlement_data) {
			if(!e.position)
				continue;
			if(getwm(e.position.x / world_map_tile_width, e.position.y / world_map_tile_height) < 1)
				continue;
			auto pt = e.position - camera + origin;
			auto fr = ps->ganim(336 + e.size, 0);
			auto pf = ps->get(fr);
			draw::image(pt.x - pf.sx / 2, pt.y - pf.sy / 2, ps, fr, ImageNoOffset, 128);
			const char* name = e.name;
			draw::text(pt.x - draw::textw(name) / 2, pt.y + pf.sy / 2 + 1, name);
		}
	}
	// Нарисуем туман войны
	int y2 = (camera.y + screen.height()) / world_map_tile_height + 1;
	for(int y = camera.y / world_map_tile_height; y < y2; y++) {
		if(y < 0 || y >= world_map_height)
			continue;
		int x2 = (camera.x + screen.width()) / world_map_tile_width + 1;
		for(int x = camera.x / world_map_tile_width; x < x2; x++) {
			auto pt = m2w(x, y) - camera + origin;
			auto value = getwm(x, y);
			if(!value)
				draw::rectf({pt.x, pt.y, pt.x + world_map_tile_width, pt.y + world_map_tile_height}, colors::black);
			else if(value == 1)
				draw::rectf({pt.x, pt.y, pt.x + world_map_tile_width, pt.y + world_map_tile_height}, colors::black, 128);
		}
	}
	// Нарисуем партию игроков
	player.painting(camera - origin);
	// Проверим рекцию мышки
	if(draw::areb(screen)) {
		if(hot.key == MouseLeft && hot.pressed)
			player.moveto(hot.mouse + camera - origin);
	}
}

static void draw_cities(int x, int y, sprite* ps) {
	rect rc = {x, y, x + 122, y + 184};
	draw::state push;
	draw::setclip(rc);
	draw::rectb(rc, colors::red);
	draw::image(rc.x1 - 12, rc.y1, ps, ps->ganim(364, 0), ImageNoOffset);
}

void creature::worldmap() {
	point camera = {0, 0};
	unsigned timestart = 0;
	adventurer party({125, 125}, true);
	while(ismodal()) {
		auto ps = gres(res::INTRFACE);
		if(!ps)
			return;
		if(party.canmove()) {
			unsigned tm = getstamp();
			if(!timestart)
				timestart = tm;
			if(tm >= timestart) {
				timestart += 1000 / 8;;
				passtime(60);
				party.update();
				point pt = {451 / 2, 448 / 2};
				camera = party.pos - pt;
			}
		} else
			timestart = 0;
		// Откорректируем позицию камеры
		if(camera.x < 0)
			camera.x = 0;
		if(camera.x > dx * 4 - 451)
			camera.x = dx * 4 - 451;
		if(camera.y < 0)
			camera.y = 0;
		if(camera.y > dy * 5 - 446)
			camera.y = dy * 5 - 446;
		// Расчитаем дату
		auto date = getdate();
		auto year = date.year();
		auto month = date.month();
		auto day = date.day();
		// Нарисуем экран
		draw_map({21, 21, 21 + 451, 21 + 446}, camera, party, ps, true);
		draw_cities(502, 132, ps);
		// Рамка
		draw::image(0, 0, ps, ps->ganim(136, 0), ImageNoOffset);
		// Смена дня и ночи
		draw::image(532, 48, ps, ps->ganim(365, date.hour()), ImageNoOffset);
		// Способ путишествия
		if(party.car) {
			auto x = 500, y = 330;
			draw::image(x + 16, y + 10, ps, ps->ganim(433, date / 60), ImageNoOffset);
			draw::image(x, y, ps, ps->ganim(363, 0), ImageNoOffset);
		} else
			draw::image(494, 331, ps, ps->ganim(366, 0), ImageNoOffset);
		domodal();
		switch(hot.key) {
		case KeyEscape: return;
		case KeyRight: camera.x += 8; break;
		case KeyLeft: camera.x -= 8; break;
		case KeyUp: camera.y -= 8; break;
		case KeyDown: camera.y += 8; break;
		}
	}
}