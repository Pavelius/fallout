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

static void mmax(int& v, int min, int max) {
	if(v > max)
		v = max;
	if(v < 0)
		v = 0;
}

static void test_animate() {
	char temp[260];
	anm_info ai = {};
	static animation_s actions[] = {AnimateStand, AnimatePickup, AnimateUse, AnimateDodge,
		AnimateDamaged, AnimateDamagedRear,
		AnimateUnarmed1, AnimateUnarmed2, AnimateThrown, AnimateRun,
		AnimateKnockOutBack, AnimateKnockOutForward,
		AnimatePistol, AnimateSMG, AnimateClub, AnimateRifle, AnimateWeaponStand, AnimateHammer, AnimateSpear,
		AnimateHeavyGun, AnimateMachineGun, AnimateRocketLauncher};
	static res::tokens resources[] = {res::HMLTHR, res::HMCMBT};
	int resource = 0, action = 0, orientation = 2;
	bool fast_stand = false;
	bool freezy_frame = false;
	res::tokens last_id = res::NoRes;
	while(ismodal()) {
		mmax(orientation, 0, 5);
		mmax(action, 0, sizeof(actions) / sizeof(actions[0]) - 1);
		mmax(resource, 0, sizeof(resources) / sizeof(resources[0]) - 1);
		if(last_id != resources[resource]) {
			ai.serialize(last_id, true);
			last_id = resources[resource];
			ai.serialize(last_id, false);
		}
		rectf({0, 0, getwidth(), getheight()}, colors::gray);
		short x = 100, y = 100;
		point pt = {x, y};
		auto a = actions[action];
		if(fast_stand)
			a = AnimateStand;
		auto* pa = ai.points + (a * 6 + orientation);
		pt = pt + *pa;
		szprint(temp, zendof(temp), "{%1i, %2i}", pa->x, pa->y);
		if(fast_stand)
			szprint(zend(temp), zendof(temp), " stand");
		if(freezy_frame)
			szprint(zend(temp), zendof(temp), " freezy");
		text(10, 10, temp);
		auto ps = gres(resources[resource]);
		auto tick = getstamp() / 200;
		auto c1 = a * 6 + orientation;
		auto fr = ps->ganim(c1, freezy_frame ? 0 : tick);
		image(pt.x, pt.y, ps, fr, 0);
		line(x - 4, y, x + 4, y, colors::red);
		line(x, y - 4, x, y + 4, colors::red);
		domodal();
		switch(hot.key) {
		case KeyEscape: breakmodal(0); break;
		case Alpha + 'Q': resource--; break;
		case Alpha + 'W': resource++; break;
		case Alpha + 'D': orientation--; break;
		case Alpha + 'F': orientation++; break;
		case Alpha + 'A': action--; ai.serialize(last_id, true); break;
		case Alpha + 'S': action++; ai.serialize(last_id, true); break;
		case Alpha + 'Z': fast_stand = !fast_stand; break;
		case Alpha + 'X': freezy_frame = !freezy_frame; break;
		case KeyLeft: pa->x--; break;
		case KeyRight: pa->x++; break;
		case KeyUp: pa->y--; break;
		case KeyDown: pa->y++; break;
		}
	}
	ai.serialize(last_id, true);
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
			field(x, y, 100, cmd(new_game), "История"); y += 41;
			field(x, y, 100, cmd(show_worldmap), "Загрузить"); y += 41;
			field(x, y, 100, cmd(show_invertory), "Предметы", Alpha + 'I'); y += 41;
			field(x, y, 100, cmd(test_animate), "Анимация", Alpha + 'A'); y += 41;
			field(x, y, 100, cmd(test_tile), "Тайлы", Alpha + 'T'); y += 41;
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