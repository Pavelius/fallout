#include "main.h"

void draw::radio(int x, int y, const runable& ev, int cicle, unsigned key) {
	auto ps = gres(res::INTRFACE);
	int frame = ps->ganim(cicle, 0);
	rect rc = {x, y,
		x + ps->get(frame).sx - 1,
		y + ps->get(frame).sy - 1};
	areas a = area(rc);
	if((hot.key == MouseLeft && !hot.pressed
		&& (a == AreaHilited || a==AreaHilitedPressed))
		|| (key && hot.key==key))
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
		|| (key && hot.key==key))
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

void draw::field(int x, int y, int width, const runable& ev, const char* text) {
	draw::state push;
	draw::setcolor(ColorButton);
	draw::radio(x, y, ev, 299);
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

void draw::hexagon(int index, point camera) {
	if(index == -1)
		return;
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	point pt = i2h(index) - camera;
	auto fr = ps->ganim(1, 0);
	auto pf = ps->get(fr);
	image(pt.x - pf.sx / 2, pt.y - pf.sy / 2, ps, fr, ImageNoOffset);
}

void draw::number(int x, int y, int digits, int value) {
	draw::state push;
	char temp[16];
	int fid = 170;
	int n = 0;
	if(value < 0) {
		n = 12;
		value = -value;
	}
	sznum(temp, value, digits, 0);
	for(int i = 0; i < digits; i++) {
		draw::clipping.x1 = x + i * 14;
		draw::clipping.x2 = x + (i + 1) * 14;
		draw::clipping.y1 = y;
		draw::clipping.y2 = y + 24;
		draw::image(x + i * 14 - (temp[i] - '0' + n) * 14, y, res::INTRFACE, 170, ImageNoOffset);
	}
}

void draw::numbersm(int x, int y, int digits, int value, int group) {
	const int symbol_width = 9;
	const int symbol_height = 17;
	const int group_width = 120;
	char temp[16];
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	auto fr = ps->ganim(82, 0);
	auto pf = ps->get(fr);
	int n = 0;
	sznum(temp, value, digits, 0);
	draw::state push;
	for(int i = 0; i < digits; i++) {
		draw::clipping.x1 = x + i * symbol_width;
		draw::clipping.x2 = x + (i + 1) * symbol_width;
		draw::clipping.y1 = y;
		draw::clipping.y2 = y + symbol_height;
		int sym = (temp[i] == '-') ? 12 : temp[i] - '0';
		draw::image(x + i * symbol_width - sym * symbol_width - group * group_width, y, ps, fr, ImageNoOffset);
	}
	//draw::rectb({x, y, x + digits*symbol_width, y + symbol_height}, colors::green);
}

void draw::image(int x, int y, res::tokens token, int cicle, int flags, unsigned char alpha) {
	auto ps = gres(token);
	if(!ps)
		return;
	int frame = ps->ganim(cicle, draw::gettick() / 100);
	image(x, y, ps, frame, flags, alpha);
}