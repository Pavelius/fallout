#include "main.h"

using namespace draw;

void util_main();

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
	creature e;
	e.equip(ArmorMetal);
	e.inventory();
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
		}
		domodal();
	}
}

int main(int argc, char* argv[]) {
	srand(clock());
	//util_main();
	initialize();
	setfont(res::FONT1);
	setpause(false);
	setlayout(mainmenu);
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}