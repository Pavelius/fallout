#include "main.h"

using namespace draw;

static bool			game_running;
static unsigned		gamestamp;
static unsigned		timestamp;

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
		if(weapons.weapon_action[0] >= (int)actions.count)
			weapons.weapon_action[0] = 0;
	}
	int getindex() const {
		return weapons.weapon_action[0];
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
	player.weapon_action[0]++;
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

static void render_tiles(point screen, point camera) {
	auto ps = draw::gres(res::TILES);
	if(!ps)
		return;
	auto a = draw::getstamp() / 100;
	auto pm = s2m(camera);
	int x1 = pm.x - 8; int x2 = x1 + 8 + 11;
	int y1 = pm.y; int y2 = y1 + 18;
	for(auto y = y1; y < y2; y++) {
		if(y < 0 || y >= map.height)
			continue;
		for(int x = x1; x < x2; x++) {
			if(x < 0 || x >= map.width)
				continue;
			auto tv = map.gettile(map.getm(x, y));
			if(tv > 1) {
				point pt = m2s(x, y);
				point pz = pt + screen - camera;
				draw::image(pz.x, pz.y + tile_height / 2, ps, ps->ganim(tv, a), 0);
			}
		}
	}
}

static void render_area(point screen, point camera) {
	const rect rc = {0, 0, 640, 480};
	map_render result;
	rect rcscreen;
	rcscreen.x1 = camera.x - screen.x;
	rcscreen.y1 = camera.y - screen.y;
	rcscreen.x2 = rcscreen.x1 + 640;
	rcscreen.y2 = rcscreen.y1 + 480;
	for(auto& e : map.getscenes()) {
		if(e.getrect().intersect(rcscreen))
			result.add(&e);
	}
	point pc = screen - camera;
	auto pm = s2m(camera);
	int x1 = pm.x - 8; int x2 = x1 + 8 + 10;
	int y1 = pm.y; int y2 = y1 + 18;
	x1 *= 2; y1 *= 2; x2 *= 2; y2 *= 2;
	auto psw = draw::gres(res::WALLS);
	auto pss = draw::gres(res::SCENERY);
	auto a = draw::getstamp() / 120;
	for(auto y = y1; y < y2; y++) {
		if(y < 0 || y >= map.height)
			continue;
		for(int x = x2; x >= x1; x--) {
			if(x < 0 || x >= map.width)
				continue;
			auto tv = map.getwall(map.geth(x, y));
			if(tv > 0) {
				point pz = m2h(x, y);
				result.add(pz.x, pz.y, psw, psw->ganim(wall_data[tv].fid, a));
			}
		}
	}
	result.add(&player);
	qsort(result.data, result.count, sizeof(result.data[0]), compare_drawable);
	for(auto p : result)
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
	textf(rc.x1, rc.y1, rc.width(), "Крутизна позволяет вам не [-замечать] мелкие жизненный неприятности. Вы получаете 10 процентов устойчивости к повреждениям.");
}

static void update_logic() {
	auto tm = clock();
	if(!timestamp)
		timestamp = tm;
	gamestamp += (tm - timestamp);
	timestamp = tm;
	player.update();
}

static void resume_game() {
	timestamp = clock();
}

static void open_invertory() {
	player.inventory();
	resume_game();
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

static void render_screen(point& hilite_hex) {
	char temp[260];
	auto hotspot = camera + hot.mouse;
	auto mapspot = s2m(hotspot);
	point screen = {0, 0};
	hilite_hex = h2m(hotspot);
	render_tiles(screen, camera);
	draw::hexagon(hilite_hex.y*(map_info::width * 2) + hilite_hex.x, camera);
	render_area(screen, camera);
	render_actions();
	szprint(temp, zendof(temp), "mouse(%1i, %2i), hex(%3i, %4i)", hotspot.x, hotspot.y, hilite_hex.x, hilite_hex.y);
	draw::text(10, 10, temp);
}

extern unsigned wall_count;

struct wall_place_info {
	const char*			name;
	short unsigned		corners[8][4];
	void lefttoright(int x, int y, int c) const {
		auto i = map.geth(x, y);
		map.setwall(i, corners[0][0]);
		for(auto k = 0; k < 3; k++) {
			for(auto v : corners[1]) {
				if(!v)
					break;
				i = map.to(i, Right);
				map.setwall(i, v);
			}
		}
		map.setwall(i, corners[2][0]);
	}
};
static wall_place_info wall_place[] = {{"Деревянная ограда", {{64}, {66, 65}, {67}, {62, 63}, {61}, {57, 58}, {56}, {59, 60}}}};

static short unsigned choose_wall(int id) {
	const int col = 3;
	const auto dx = 640 / col;
	const auto dy = 480 / col;
	auto ps = gres(res::WALLS);
	auto push_id = id;
	int origin = (id / col) * col - col;
	while(ismodal()) {
		draw::rectf({0, 0, getwidth(), getheight()}, colors::gray);
		if(origin < 0)
			origin = 0;
		for(auto y = 0; y < col; y++) {
			for(auto x = 0; x < col; x++) {
				auto x1 = x * dx;
				auto y1 = y * dy;
				auto ii = origin + y * 3 + x;
				auto fi = wall_data[ii].fid;
				if(fi!=-1)
					image(x1 + dx/2, y1 + dy/2 + 20, ps, ps->ganim(fi, getstamp() / 100), 0);
				auto pn = wall_data[ii].name;
				char temp[32]; szprint(temp, zendof(temp), "%1i", ii);
				text(x1 + (dx - textw(pn)) / 2, y1 + dy - 32, pn);
				text(x1 + (dx - textw(temp)) / 2, y1 + dy-48, temp);
				if(ii == id)
					rectx({x1 + 4, y1 + 4, x1 + 210 - 4, y1 + 150 - 4}, colors::black);
			}
		}
		domodal();
		auto prev_scenery = id;
		switch(hot.key) {
		case KeyLeft: id--; break;
		case KeyRight: id++; break;
		case KeyUp:
			if(id >= col)
				id -= col;
			break;
		case KeyDown: id += col; break;
		case KeyEnter: breakmodal(1); break;
		case KeyEscape: breakmodal(0); break;
		}
		if(id < 0)
			id = 0;
		if(id >= (int)wall_count-1)
			id = wall_count - 2;
		if(id < origin)
			origin -= col;
		if(id > origin + 8)
			origin += 3;
	}
	if(getresult())
		return id;
	return push_id;
}

static short unsigned choose_scenery(int id_scenery) {
	auto ps = gres(res::SCENERY);
	auto push_scenery = id_scenery;
	int origin = (id_scenery/3)*3 - 3;
	while(ismodal()) {
		draw::rectf({0, 0, getwidth(), getheight()}, colors::gray);
		if(origin < 0)
			origin = 0;
		for(auto y = 0; y < 3; y++) {
			for(auto x = 0; x < 3; x++) {
				auto x1 = x * 210;
				auto y1 = y * 150;
				auto fi = origin + y * 3 + x;
				image(x1 + 100, y1 + 75, ps, ps->ganim(fi, getstamp()/100), 0);
				if(fi==id_scenery)
					rectx({x1 + 4, y1 + 4, x1 + 210 - 4, y1 + 150 - 4}, colors::black);
			}
		}
		domodal();
		auto prev_scenery = id_scenery;
		switch(hot.key) {
		case KeyLeft: id_scenery--; break;
		case KeyRight: id_scenery++; break;
		case KeyUp:
			if(id_scenery >= 3)
				id_scenery -= 3;
			break;
		case KeyDown: id_scenery += 3; break;
		case KeyEnter: breakmodal(1); break;
		}
		if(id_scenery < 0)
			id_scenery = 0;
		if(id_scenery < origin)
			origin -= 3;
		if(id_scenery > origin + 8)
			origin += 3;
	}
	if(getresult())
		return id_scenery;
	return push_scenery;
}

void creature::adventure() {
	cursorset cursor;
	unsigned short current_scenery = 4;
	unsigned short current_wall = 4;
	camera = {400, -100};
	point current_hex;
	while(ismodal() && player.isalive()) {
		render_screen(current_hex);
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
		case Alpha + 'Q':
			player.setaction(AnimateWalk);
			break;
		case Ctrl + Alpha + 'S':
			map.serialize(true);
			break;
		case Ctrl + Alpha + 'L':
			map.serialize(false);
			break;
		case Alpha + 'U':
			current_scenery = choose_scenery(current_scenery);
			resume_game();
			break;
		case Alpha + 'W':
			current_wall = choose_wall(current_wall);
			resume_game();
			break;
		case Alpha + 'P':
			map.setscene(current_hex.x, current_hex.y, current_scenery);
			break;
		case Alpha + 'O':
			map.setwall(map.geth(current_hex.x, current_hex.y), current_wall);
			break;
		case Alpha + 'M':
			wall_place[0].lefttoright(current_hex.x, current_hex.y, 4);
			break;
		}
	}
}

void actor::wait() {
	cursorset cursor;
	point current_hex;
	cursor.set(res::INTRFACE, 295);
	auto current_action = action;
	auto stop_frame = frame_maximum;
	while(ismodal()) {
		if(stop_frame == frame && next_stamp >= gametick())
			break;
		update_logic();
		if(current_action != action)
			break;
		render_screen(current_hex);
		domodal();
	}
}