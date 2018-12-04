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
	creature e("narg");
	e.add(Pistol10mm);
	e.add(IceChest);
	e.add(IguanaOnAStick);
	e.add(Minigun);
	e.add(PowerArmor);
	e.add(SmallStatuette);
	e.add(SmallPieceOfMachinery);
	e.add(Stimpak);
	e.inventory();
}

static void test_animate() {
	static item weapons[] = {NoItem, Pistol10mm, Flamer, Shotgun, SMG10mm, Knife, Club};
	auto weapon = 0;
	auto orientation = 2;
	item armor = LeatherArmor;
	while(ismodal()) {
		rectf({0, 0, getwidth(), getheight()}, colors::gray);
		auto x = 100, y = 100;
		actor::preview(100, 100, Male, armor, weapons[weapon], orientation%6);
		line(x - 4, y, x + 4, y, colors::red);
		line(x, y - 4, x, y + 4, colors::red);
		domodal();
		switch(hot.key) {
		case KeyEscape: breakmodal(0); break;
		case KeyUp:
			weapon++;
			if(weapon >= sizeof(weapons) / sizeof(weapons[0]))
				weapon = 0;
			break;
		case KeyDown:
			weapon--;
			if(weapon < 0)
				weapon = sizeof(weapons) / sizeof(weapons[0]) - 1;
			break;
		case KeyLeft: orientation--; break;
		case KeyRight: orientation++; break;
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
			field(x, y, 100, cmd(new_game), "История"); y += 41;
			field(x, y, 100, cmd(show_worldmap), "Загрузить"); y += 41;
			field(x, y, 100, cmd(show_invertory), "Предметы", Alpha + 'I'); y += 41;
			field(x, y, 100, cmd(test_animate), "Анимация", Alpha + 'A'); y += 41;
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
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}