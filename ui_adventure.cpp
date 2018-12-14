#include "main.h"

using namespace draw;

static bool			game_running;
static unsigned		gamestamp;

runstate::runstate(bool new_value) : value(game_running) {
	game_running = new_value;
}

runstate::~runstate() {
	game_running = value;
}

void draw::setpause(bool value) {
	game_running = !value;
}

bool draw::ispause() {
	return !game_running;
}

unsigned draw::gametick() {
	return gamestamp;
}

//static void game_map(rect& rc, cursorset& cursor) {
//	static bool info_mode;
//	game_index = 0;
//	game_objects = 0;
//	draw::state push;
//	draw::setclip(rc);
//	point screen = {(short)rc.x1, (short)rc.y1};
//	drawable* drawables[2048];
//	draw::tiles(screen);
//	dwselect(drawables);
//	dwclipping(drawables, rc, camera);
//	dworder(drawables, zlen(drawables));
//	dwpaint(drawables, rc, camera);
//	auto a = draw::area(rc);
//	if(a == AreaHilited || a == AreaHilitedPressed) {
//		if(hot.key == MouseRight && hot.pressed)
//			info_mode = !info_mode;
//		if(info_mode) {
//			cursor.set(res::INTRFACE, 250);
//			auto p = dwfind(drawables, rc, camera, hot.mouse);
//			if(p) {
//				int actions[] = {Look, 0};
//				draw::setaction(actions);
//				game_objects = p->getid();
//			}
//		} else {
//			// Movement mode. Each command is move to.
//			cursor.set(res::NoRes);
//			point pm = h2m(hot.mouse + camera - screen);
//			game_index = m2i(pm);
//			draw::hexagon(game_index, camera - screen);
//			if(hot.key == MouseLeft && hot.pressed)
//				draw::execute(MoveTo);
//		}
//	}
//}

static void open_invertory() {
	player.inventory();
}

static void use_action() {
	animate(608, 477, gres(res::INTRFACE), 104);
	animate(608, 477, gres(res::INTRFACE), 104, 6, -2, -1);
}

struct weaponwear {
	weaponable&			weapons;
	adat<action_s, 8>	actions;
	weaponwear(weaponable& source, const item& it) : weapons(source) {
		for(auto a : it.getattack().actions) {
			if(!a)
				continue;
			actions.add(a);
		}
		correct();
	}
	void correct() {
		if(weapons.weapon_action[weapons.weapon] >= (int)actions.count)
			weapons.weapon_action[weapons.weapon] = 0;
	}
	int getindex() const {
		return weapons.weapon_action[weapons.weapon];
	}
	action_s getaction() const {
		if(!actions)
			return NoAction;
		return actions[getindex()];
	}
};

static void correct_weapon_action() {
	weaponwear ww(player, player.getweapon());
	ww.correct();
}

static void change_weapon_action() {
	player.weapon_action[player.weapon]++;
	correct_weapon_action();
}

static int compare_drawable(const void* p1, const void* p2) {
	auto d1 = *((drawable**)p1);
	auto d2 = *((drawable**)p2);
	auto z1 = d1->getposition();
	auto z2 = d2->getposition();
	z1.y += d1->getzorder();
	z2.y += d2->getzorder();
	if(z1.y < z2.y)
		return -1;
	if(z1.y > z2.y)
		return 1;
	if(z1.x < z2.x)
		return -1;
	if(z1.x > z2.x)
		return 1;
	return 0;
}

static void render_area() {
	adat<drawable*, 512> source;
	source.add(&player);
	for(auto& e : map.getscenes())
		source.add(&e);
	qsort(source.data, source.count, sizeof(source.data[0]), compare_drawable);
	for(auto p : source)
		p->painting(camera);
}

static void render_item(int x, int y) {
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	auto fr = ps->ganim(32, 0);
	auto pf = ps->get(fr);
	rect rc = {x, y, x + pf.sx - 4, y + pf.sy - 4};
	if(draw::buttonf(rc.x1, rc.y1, 32, 31, 0, false))
		execute(use_action);
	auto wp = player.getweapon();
	auto a = area(rc);
	if(hot.mouse.in(rc) && hot.pressed) {
		rc.y1 -= 2;
		rc.y2 -= 2;
		rc.x1 += 1;
		rc.x2 += 1;
	}
	draw::iteminv(rc.x1, rc.y1, rc.width(), rc.height(), wp, false);
	weaponwear ww(player, wp);
	auto ac = ww.getaction();
	if(ac) {
		draw::image(rc.x1 + rc.width() - 30, rc.y1 + 16,
			ps, ps->ganim(action_data[ac].fid, getstamp()), 0);
		if((a == AreaHilited || a == AreaHilitedPressed) && hot.key == MouseRight && !hot.pressed)
			execute(change_weapon_action);
		hit_info hi = {}; player.get(hi, wp, ac);
		draw::image(rc.x1 + 16, rc.y1 + 56, ps, ps->ganim(289, 0), 0);
		draw::numberap(rc.x1 + 30, rc.y1 + 45, hi.attack.getap(ac));
	}
}

static void open_charsheet() {
	player.choose_stats(0, 0, 0, true, player.getskillrate());
}

static void open_pipboy() {}

static void change_weapon_proc() {
	player.setaction(AnimateWeaponHide);
	player.wait();
	player.change_weapon();
	correct_weapon_action();
	player.setaction(AnimateWeaponHide, true);
	player.wait();
}

static void render_console(const rect& rc) {
	draw::state push;
	//rectb(rc, colors::red);
	setclip(rc);
	textf(rc.x1, rc.y1, rc.width(), " рутизна позвол€ет вам не [-замечать] мелкие жизненный непри€тности. ¬ы получаете 10 процентов устойчивости к повреждени€м.");
}

static void render_actions() {
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	auto fr = ps->ganim(16, 0);
	auto dy = ps->get(fr).sy;
	int x = (draw::getwidth() - ps->get(fr).sx) / 2;
	int y = draw::getheight() - dy;
	draw::image(x, y, ps, fr, ImageNoOffset);
	if(draw::buttonf(x + 210, y + 40, 47, 46, 0) || (hot.key == Alpha + 'I'))
		draw::execute(open_invertory);
	if(draw::buttonf(x + 210, y + 60, 18, 17, 0) || (hot.key == KeyEscape)) {
		//draw::execute(Options);
	}
	render_item(x + 265, y + 29);
	if(draw::buttonf(x + 526, y + 38, 13, 10, 0)) {
		//draw::execute(Map);
	}
	if(buttonf(x + 526, y + 58, 57, 56, 0) || (hot.key == Alpha + 'C'))
		execute(open_charsheet);
	if(buttonf(x + 526, y + 78, 59, 58, 0))
		execute(open_pipboy);
	if(buttonf(x + 218, y + 6, 6, 7, 0))
		execute(change_weapon_proc);
	if(buttonf(x + 523, y + 6, 6, 7, 0)) {
		//Sprites: 119, 120, 121
		//draw::execute(Skills);
	}
	numbersm(x + 472, y + 38, 4, player.get(HP));
	numbersm(x + 472, y + 76, 4, player.get(AC));
	render_console({x + 28, y + 32, x + 186, y + 82});
}

static void update_logic() {
	static unsigned last;
	auto timestamp = clock();
	if(!last)
		last = timestamp;
	gamestamp += (timestamp - last);
	last = timestamp;
	player.update();
}

static void render_screen() {
	char temp[260];
	auto hotspot = camera + hot.mouse;
	auto mapspot = s2m(hotspot);
	auto hexpos = h2m(hotspot);
	map.render_tiles({0, 0}, camera);
	render_area();
	render_actions();
	draw::hexagon(2, camera);
	draw::hexagon(3, camera);
	draw::hexagon(4, camera);
	draw::hexagon(5, camera);
	szprint(temp, zendof(temp), "mouse(%1i, %2i), hex(%3i, %4i)", hotspot.x, hotspot.y, hexpos.x, hexpos.y);
	draw::text(10, 10, temp);
}

void creature::adventure() {
	cursorset cursor;
	camera = {400, -100};
	while(ismodal() && player.isalive()) {
		render_screen();
		update_logic();
		domodal();
		switch(hot.key) {
		case KeyLeft: camera.x -= tile_width / 2; break;
		case KeyRight: camera.x += tile_width / 2; break;
		case KeyUp: camera.y -= tile_width / 2; break;
		case KeyDown: camera.y += tile_width / 2; break;
		case Alpha + '-':
			if(player.getorientation())
				player.setorientation(player.getorientation() - 1);
			break;
		case Alpha + '+':
			if(player.getorientation() < 5)
				player.setorientation(player.getorientation() + 1);
			break;
		case Alpha + 'T':
			player.setaction(AnimateDodge);
			player.wait();
			break;
		case Alpha + 'D':
			player.setaction(AnimateKnockOutBack);
			player.wait();
			player.setaction(AnimateStandUpBack);
			player.wait();
			break;
		case Alpha + 'S':
			player.setaction(AnimateWeaponAim);
			player.wait();
			player.setaction(AnimateWeaponSingle);
			player.wait();
			player.setaction(AnimateWeaponBurst);
			player.wait();
			player.setaction(AnimateWeaponAttack);
			player.wait();
			break;
		case Alpha + 'H':
			player.setaction(AnimateWeaponHide);
			player.wait();
			player.setaction(AnimateWeaponHide, true);
			player.wait();
			break;
		case Alpha + 'W':
			player.setaction(AnimateWalk);
			break;
		}
	}
}

void actor::wait() {
	cursorset cursor;
	cursor.set(res::INTRFACE, 295);
	auto current_action = action;
	auto stop_frame = frame_maximum;
	while(ismodal()) {
		if(stop_frame == frame && next_stamp >= gametick())
			break;
		update_logic();
		if(current_action != action)
			break;
		render_screen();
		domodal();
	}
}