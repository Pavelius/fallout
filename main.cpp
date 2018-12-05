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
	e.add({Money, 2100});
	e.inventory();
}

static void mmax(int& v, int min, int max) {
	if(v > max)
		v = max;
	if(v < 0)
		v = 0;
}

static void test_animate() {
	static int offset_lt[] = {0, 0, -1, -3, 8, -1, 8, 3, -1, -1};
	static int offset_am[] = {4, 6, 4, 4};
	static animation_s actions[] = {AnimateStand};
	static item weapons[] = {NoItem, Club, Pistol10mm, SMG10mm, Flamer, RocketLauncher, Minigun, Shotgun, Spear, Knife, Sledgehammer};
	static item armors[] = {LeatherArmor, PowerArmor, CombatArmor, LeatherJacket};
	int weapon = 0, armor = 0, action = 0, orientation = 2;
	while(ismodal()) {
		mmax(orientation, 0, 5);
		mmax(action, 0, sizeof(actions) / sizeof(actions[0]) - 1);
		mmax(weapon, 0, sizeof(weapons) / sizeof(weapons[0]) - 1);
		mmax(armor, 0, sizeof(armors) / sizeof(armors[0]) - 1);
		rectf({0, 0, getwidth(), getheight()}, colors::gray);
		short x = 100, y = 100;
		point pt = {x, y};
		point pz = {-1, 4};
		pt.x += offset_lt[weapon];
		if(weapon) {
			pt.y += offset_am[armor];
			switch(weapons[weapon].get()) {
			//case Club: pt.y++; break;
			case Minigun: pt.y += offset_am[armor] + 1; break;
			}
		}
		pt = pt + pz;
		actor::preview(pt.x, pt.y, Male, armors[armor], weapons[weapon], orientation % 6, actions[action], getstamp() / 200, {-140, -200, 140, 60});
		line(x - 4, y, x + 4, y, colors::red);
		line(x, y - 4, x, y + 4, colors::red);
		domodal();
		switch(hot.key) {
		case KeyEscape: breakmodal(0); break;
		case Alpha + '1': armor--; break;
		case Alpha + '2': armor++; break;
		case Alpha + 'A': action--; break;
		case Alpha + 'S': action++; break;
		case KeyUp: weapon++; break;
		case KeyDown: weapon--; break;
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
	setlayout(test_animate);
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}