#include "main.h"

void draw::radio(int x, int y, const runable& ev, int cicle, unsigned key) {
	auto ps = gres(res::INTRFACE);
	int frame = ps->ganim(cicle, 0);
	rect rc = {x, y,
		x + ps->get(frame).sx - 1,
		y + ps->get(frame).sy - 1};
	areas a = area(rc);
	if((hot.key == MouseLeft && !hot.pressed
		&& (a == AreaHilited || a == AreaHilitedPressed))
		|| (key && hot.key == key))
		ev.execute();
	if(ev.ischecked())
		a = AreaHilitedPressed;
	switch(a) {
	case AreaHilitedPressed:
		image(x, y, ps, frame + 1, ImageNoOffset);
		break;
	default:
		image(x, y, ps, frame, ImageNoOffset);
		break;
	}
}

bool draw::buttonf(int x, int y, int cicles_normal, int cicle_pressed, bool checked, bool need_pressed, areas* ra, rect* rrc) {
	bool result = false;
	auto ps = gres(res::INTRFACE);
	int normal = ps->ganim(cicles_normal, 0);
	int pressed = ps->ganim(cicle_pressed, 0);
	rect rc = {x, y,
		x + ps->get(normal).sx - 1,
		y + ps->get(normal).sy - 1};
	auto a = draw::area(rc);
	if(hot.key == MouseLeft
		&& (hot.pressed == need_pressed)
		&& (a == AreaHilited || a == AreaHilitedPressed))
		result = true;
	if(checked)
		a = AreaHilitedPressed;
	if(ra)
		*ra = a;
	if(rrc)
		*rrc = rc;
	switch(a) {
	case AreaHilitedPressed: draw::image(x, y, ps, pressed, ImageNoOffset); break;
	default: draw::image(x, y, ps, normal, ImageNoOffset); break;
	}
	return result;
}

int draw::button(int x, int y, int width, const runable& ev, const char* string, unsigned key) {
	draw::state push;
	draw::setcolor(ColorButton);
	draw::setfont(res::FONT3);
	auto ps = gres(res::INTRFACE);
	auto frame = ps->ganim(8, 0);
	int sx = ps->get(frame).sx;
	int sy = draw::texth();
	int w1 = width - sx;
	text(x + sx + (w1 - textw(string)) / 2, y, string);
	rect rc = {x, y, x + width, y + sy};
	areas a = area(rc);
	if((hot.key == MouseLeft && !hot.pressed && a == AreaHilited)
		|| (key && hot.key == key))
		ev.execute();
	if(ev.ischecked())
		a = AreaHilitedPressed;
	switch(a) {
	case AreaHilitedPressed:
		image(x, y + 1, ps, frame + 1, ImageNoOffset);
		break;
	default:
		image(x, y + 1, ps, frame, ImageNoOffset);
		break;
	}
	//rectb(rc.x1, rc.y1, rc.x2, rc.y2, ColorWhite);
	return sy;
}

void draw::buttonp(int x, int y, int circle_normal, int circle_pressed, const runable& ev, const char* string, int ty) {
	draw::state push;
	draw::setfont(res::FONT3);
	draw::setcolor(ColorButton);
	areas a;
	rect rc;
	if(buttonf(x, y, circle_normal, circle_pressed, ev.ischecked(), false, &a, &rc))
		ev.execute();
	int w1 = textw(string);
	if(a == AreaHilitedPressed) {
		x++;
		y++;
	}
	draw::text(x + (rc.width() - textw(string)) / 2, y + ty, string);
}

void draw::background(int cicle) {
	auto ps = gres(res::INTRFACE);
	auto frame = ps->ganim(cicle, 0);
	draw::image(0, 0, ps, frame, ImageNoOffset, 255);
}

void draw::field(int x, int y, int width, const runable& ev, const char* text, unsigned key) {
	draw::state push;
	draw::setcolor(ColorButton);
	draw::radio(x, y, ev, 299, key);
	draw::text(x + width - draw::textw(text) / 2, y, text);
}

void draw::label(int x, int y, int id, const char* temp, bool checked, bool disabled) {
	draw::state push;
	draw::setfont(res::FONT1);
	int width = draw::textw(temp);
	rect rc;
	rc.x1 = x; rc.y1 = y;
	rc.x2 = rc.x1 + textw(temp);
	rc.y2 = rc.y1 + texth();
	focusing(rc, id);
	if(disabled)
		draw::setcolor(ColorDisable);
	else if(checked)
		draw::setcolor(ColorCheck);
	else
		draw::setcolor(ColorText);
	if(draw::getfocus() == id) {
		//draw::fore = draw::fore.mix(colors::yellow, 64);
		draw::fore = draw::fore.lighten(128);
	}
	draw::text(x, y, temp);
}

void draw::hexagon(short unsigned index, point camera) {
	if(index == 0xFFFF)
		return;
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	short x = index % (map_info::width * 2);
	short y = index / (map_info::width * 2);
	auto pt = m2h(x, y) - camera;
	auto fr = ps->ganim(1, 0);
	auto pf = ps->get(fr);
	image(pt.x - pf.sx / 2, pt.y - pf.sy / 2, ps, fr, ImageNoOffset);
}

static void render_number(int x, int y, int digits, int value, const sprite* ps, int cicle, int symbol_offset, int symbol_width, int symbol_height) {
	char temp[16];
	if(!ps)
		return;
	auto fr = ps->ganim(cicle, 0);
	auto& pf = ps->get(fr);
	sznum(temp, value, digits, 0);
	draw::state push;
	for(int i = 0; i < digits; i++) {
		draw::clipping.x1 = x + i * symbol_width;
		draw::clipping.x2 = x + (i + 1) * symbol_width;
		draw::clipping.y1 = y;
		draw::clipping.y2 = y + symbol_height;
		int sym = (temp[i] == '-') ? 12 : temp[i] - '0';
		draw::image(x + (i - sym) * symbol_width - symbol_offset, y, ps, fr, ImageNoOffset);
	}
}

void draw::number(int x, int y, int digits, int value) {
	if(value >= 0)
		render_number(x, y, digits, value, gres(res::INTRFACE), 170, 0, 14, 24);
	else
		render_number(x, y, digits, value, gres(res::INTRFACE), 170, 12 * 14, 14, 24);
}

void draw::numbersm(int x, int y, int digits, int value, int group) {
	render_number(x, y, digits, value, gres(res::INTRFACE), 82, group * 120, 9, 17);
}

void draw::numberap(int x, int y, int value) {
	const int symbol_width = 10;
	const int symbol_height = 11;
	char temp[16];
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	auto fr = ps->ganim(290, 0);
	auto& pf = ps->get(fr);
	sznum(temp, value);
	draw::state push;
	for(int i = 0; temp[i]; i++) {
		draw::clipping.x1 = x + i * symbol_width;
		draw::clipping.x2 = x + (i + 1) * symbol_width;
		draw::clipping.y1 = y;
		draw::clipping.y2 = y + symbol_height;
		int sym = (temp[i] == '-') ? 12 : temp[i] - '0';
		draw::image(x + (i - sym) * symbol_width, y, ps, fr, ImageNoOffset);
	}
}

void draw::image(int x, int y, res::tokens token, int cicle, int flags, unsigned char alpha) {
	auto ps = gres(token);
	if(!ps)
		return;
	int frame = ps->ganim(cicle, draw::getstamp() / 100);
	image(x, y, ps, frame, flags, alpha);
}

void draw::iteminv(int x, int y, int sx, int sy, item& it, bool resize) {
	if(!it)
		return;
	auto cl = it.get(FrameInventory);
	if(!cl)
		return;
	auto ps = gres(res::INVEN);
	if(!ps)
		return;
	auto fr = ps->ganim(cl, 0);
	int isx = ps->get(fr).sx;
	int isy = ps->get(fr).sy;
	if(!resize || (isx <= sx && isy <= sy))
		draw::image(x + (sx - isx) / 2, y + (sy - isy) / 2, ps, fr, ImageNoOffset);
	else {
		int rsx = sx;
		int rsy = sy;
		if(isx > sx)
			rsy = (isy*sx) / isx;
		else
			rsx = (isx*sy) / isy;
		if(rsy > sy) {
			rsy = sy;
			rsx = (isx*sy) / isy;
		}
		draw::surface real(rsx, rsy, 32);
		if(true) {
			draw::surface zoom(isx, isy, 32);
			draw::state push;
			draw::canvas = &zoom;
			draw::setclip();
			draw::fore.clear();
			draw::fore.a = 255;
			draw::rectf({0, 0, isx, isy});
			draw::image(0, 0, ps, fr, ImageNoOffset);
			draw::blit(real, 0, 0, real.width, real.height, 0, zoom, 0, 0, zoom.width, zoom.height);
		}
		draw::blit(*draw::canvas,
			x + (sx - real.width) / 2,
			y + (sy - real.height) / 2,
			real.width, real.height,
			ImageTransparent, real, 0, 0);
	}
}

void draw::animate(int x, int y, sprite* ps, int cicle, int fps, int frame, int frame_end) {
	auto pc = ps->getcicle(cicle);
	if(!pc || !pc->count)
		return;
	auto fb = pc->start;
	auto fe = pc->start + pc->count - 1;
	if(frame == -2)
		frame = fe;
	else if(frame == -1)
		frame = fb;
	if(frame_end == -2)
		frame_end = fe;
	else if(frame_end == -1)
		frame_end = fb;
	auto d = (frame > frame_end) ? -1 : 1;
	screenshoot push;
	cursorset cursor;
	cursor.set(res::INTRFACE, 295);
	auto tick_rate = 1000 / fps;
	auto next_tick = getstamp() + tick_rate;
	while(ismodal()) {
		push.restore();
		image(x, y, ps, frame, 0);
		domodal();
		if(next_tick < getstamp()) {
			if(frame == frame_end)
				break;
			next_tick += tick_rate;
			frame += d;
		}
	}
	push.restore();
}