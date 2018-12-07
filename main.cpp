#include "util.h"

using namespace draw;

void test_game() {
	map::create();
	draw::setcamera(i2s(m2i(14, 14)));
	//game::generate::scenery(8, m2i(20, 20));
	//game::generate::scenery(9, m2i(24, 28));
	//game::generate::scenery(34, m2i(10, 10));
	//game::generate::scenery(90, m2i(30, 30));
	//game::generate::scenery(128, m2i(40, 40));
	//game::generate::scenery(199, m2i(8, 8));
	//game::generate::scenery(230, m2i(18, 18));
	map::set({3, 3, 6, 6}, 0);
	map::set({8, 3, 16, 10}, 1);
	map::set({10, 5, 14, 8}, 2);
	map::set({3, 17, 6, 20}, 3);
	map::set({8, 12, 16, 20}, 1);
	map::set({10, 14, 14, 18}, 4);
}

static void new_game() {
	creature e;
	//e.choose_stats(2, 3, 10);
	e.newgame();
}

static void show_worldmap() {
	creature e;
	e.worldmap();
}

static void show_invertory() {
	creature e("narg");
	e.add(Pistol10mm);
	e.add(IceChest);
	e.add(IguanaOnAStick);
	e.add(Minigun);
	e.add(PowerArmor);
	e.add(AdvancedPowerArmor);
	e.add(CombatArmor);
	e.add(LeatherJacket);
	e.add(SmallStatuette);
	e.add(SmallPieceOfMachinery);
	e.add(Stimpak);
	e.add({Money, 2100});
	e.inventory();
}

static void redmark(point pt) {
	line(pt.x - 3, pt.y, pt.x + 3, pt.y, colors::red);
	line(pt.x, pt.y - 3, pt.x, pt.y + 3, colors::red);
}

static void test_tile() {
	static short map[] = {1, 2, 3, 4, 5,
		6,7,8,9,10,
		11,12,13,14,15
	};
	static short maw[] = {0, 0, 5, 0, 0, 0, 0, 0, 0,
		0, 0, 6, 0, 0, 0, 0, 0, 0,
		0, 0, 6, 0, 0, 0, 0, 0, 0,
		0, 0, 86, 87, 88, 89, 90, 91, 92,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	while(ismodal()) {
		rectf({0, 0, getwidth(), getheight()}, colors::gray);
		point camera = {50, 200};
		auto sr = gres(res::TILES);
		for(auto y = 0; y < 3; y++) {
			for(auto x = 0; x < 5; x++) {
				auto pt = m2s(x, y) + camera;
				draw::image(pt.x, pt.y, sr, tile_data[map[y*5+x]].fid, 0);
			}
		}
		sr = gres(res::WALLS);
		for(auto y = 0; y < 5; y++) {
			for(auto x = 9; x >= 0; x--) {
				auto pt = m2h(x, y) + camera;
				auto v = maw[y * 9 + x];
				if(v) {
					//pt = pt + wall_data[v].pos;
					draw::image(pt.x, pt.y, sr, wall_data[v].fid, 0);
					redmark(pt);
				}
			}
		}
		domodal();
		switch(hot.key) {
		case KeyEscape: breakmodal(0); break;
		}
	}
}

static void mainmenu() {
	while(ismodal()) {
		background(140);
		auto x = 29, y = 19;
		if(true) {
			draw::state push;
			draw::setfont(res::FONT4);
			field(x, y, 100, cmd(new_game), "�������"); y += 41;
			field(x, y, 100, cmd(show_worldmap), "���������"); y += 41;
			field(x, y, 100, cmd(show_invertory), "��������", Alpha + 'I'); y += 41;
			field(x, y, 100, cmd(test_animate), "��������", Alpha + 'A'); y += 41;
			field(x, y, 100, cmd(test_tile), "�����", Alpha + 'T'); y += 41;
		}
		domodal();
	}
}

int main(int argc, char* argv[]) {
	srand(clock());
	util_main();
	initialize();
	setfont(res::FONT1);
	setpause(false);
	test_animate();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}