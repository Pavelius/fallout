#include "util.h"

using namespace draw;

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
		0, 0, 86, 87, 86, 87, 90, 91, 92,
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

static void test_adventure() {
	player.create("narg");
	player.add(LeatherArmor);
	player.adventure();
}

static void mainmenu() {
	while(ismodal()) {
		background(140);
		auto x = 29, y = 19;
		if(true) {
			draw::state push;
			draw::setfont(res::FONT4);
			field(x, y, 100, cmd(new_game), "История"); y += 41;
			field(x, y, 100, cmd(show_worldmap), "Загрузить"); y += 41;
			field(x, y, 100, cmd(show_invertory), "Предметы", Alpha + 'I'); y += 41;
			field(x, y, 100, cmd(test_animate), "Анимация", Alpha + 'A'); y += 41;
			field(x, y, 100, cmd(test_tile), "Тайлы", Alpha + 'T'); y += 41;
			field(x, y, 100, cmd(test_adventure), "Приключения", Alpha + 'G'); y += 41;
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
	setlayout(mainmenu);
	//stest_tile();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}