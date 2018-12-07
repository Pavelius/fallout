#include "main.h"

using namespace draw;

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

//static void show_combat_animation(bool finish = false) {
//	game::state push;
//	auto ps = gres(res::INTRFACE);
//	if(!ps)
//		return;
//	game::setpause(true);
//	draw::screenshoot screen;
//	cursorset cursor;
//	cursor.set(res::INTRFACE, 295);
//	auto cl = ps->gcicle(104);
//	unsigned timestart = clock();
//	int fr = finish ? cl->count - 1 : 0;
//	while(true) {
//		screen.restore();
//		draw::image(608, 477, ps, cl->start + fr, 0);
//		draw::sysredraw();
//		if(timestart <= clock()) {
//			timestart += animation_speed;
//			if(finish) {
//				if(--fr < 0)
//					break;
//			} else {
//				if(++fr >= cl->count)
//					break;
//			}
//		}
//	}
//}

//static void wait(int player) {
//	draw::state push;
//	draw::mouseinput = false;
//	int a = bsget(player, Animation);
//	if(a == ActionStand)
//		return;
//	cursorset cursor(res::INTRFACE, 295);
//	while(true) {
//		dwupdate();
//		int a = bsget(player, Animation);
//		if(a == ActionStand)
//			return;
//		rect rc = {0, 0, draw::getwidth(), draw::getheight()};
//		game_panel(rc, player);
//		game_map(rc, cursor);
//		draw::input(true);
//	}
//}

static void open_invertory() {
	player.inventory();
}

static void render_item(int x, int y) {
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	auto fr = ps->ganim(32, 0);
	auto pf = ps->get(fr);
	rect rc = {x, y, x + pf.sx - 4, y + pf.sy - 4};
	if(draw::buttonf(rc.x1, rc.y1, 32, 31, 0, false)) {
		//draw::execute(id);
	}
	if(hot.mouse.in(rc) && hot.pressed)
		draw::iteminv(rc.x1, rc.y1 - 2, rc.width(), rc.height(), player.getweapon(), false);
	else
		draw::iteminv(rc.x1, rc.y1, rc.width(), rc.height(), player.getweapon(), false);
}

void creature::render_actions() {
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	auto fr = ps->ganim(16, 0);
	auto dy = ps->get(fr).sy;
	int x = (draw::getwidth() - ps->get(fr).sx) / 2;
	int y = draw::getheight() - dy;
	draw::image(x, y, ps, fr, ImageNoOffset);
	if(draw::buttonf(x + 210, y + 40, 47, 46, 0) || (hot.key==Alpha + 'I'))
		draw::execute(open_invertory);
	if(draw::buttonf(x + 210, y + 60, 18, 17, 0) || (hot.key == KeyEscape)) {
		//draw::execute(Options);
	}
	render_item(x + 265, y + 29);
	if(draw::buttonf(x + 526, y + 38, 13, 10, 0)) {
		//draw::execute(Map);
	}
	if(draw::buttonf(x + 526, y + 58, 57, 56, 0)) {
		//draw::execute(Charsheet);
	}
	if(draw::buttonf(x + 526, y + 78, 59, 58, 0)) {
		//draw::execute(Charsheet);
	}
	if(draw::buttonf(x + 218, y + 6, 6, 7, 0)) {
		//draw::execute(ChangeWeapon);
	}
	if(draw::buttonf(x + 523, y + 6, 6, 7, 0)) {
		//draw::execute(Skills);
	}
	draw::numbersm(x + 472, y + 38, 4, get(HP));
	draw::numbersm(x + 472, y + 76, 4, get(AC));
	//console(x1 + 28, y1 + 35, 155, 50, "Ёто длинна€ строка по идее должна разбитьс€ на несколько коротких строчек с переносом. „тобы еще добавить? ќна должна скроллитьс€. »нтеллект отвечает за пам€ть, скорость реакции и способность оценивать событи€. ¬ысокий »нтеллект важен дл€ всех. ¬ли€ет на количество очков умений на уровень, режимы диалога и многие способности.");
}

void creature::adventure() {
	cursorset cursor;
	while(ismodal() && player.isalive()) {
		rect rc = {0, 0, draw::getwidth(), draw::getheight()};
		rectf(rc, colors::gray);
		player.render_actions();
		domodal();
		switch(hot.key) {
		case KeyLeft: camera.x -= tile_width / 2; break;
		case KeyRight: camera.x += tile_width / 2; break;
		case KeyUp: camera.y -= tile_width / 2; break;
		case KeyDown: camera.y += tile_width / 2; break;
		case Alpha + '<':
			break;
		case Alpha + '>':
			break;
		}
	}
}