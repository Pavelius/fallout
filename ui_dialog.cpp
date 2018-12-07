#include "main.h"

using namespace draw;

void draw::dlgmsg(const char* title, const char* text) {
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	draw::state push;
	setfont(res::FONT1);
	int cl = 217;
	int dx = 20;
	int width = draw::getwidth();
	int height = draw::getheight();
	int id = ps->ganim(cl, 0);
	int sx = ps->get(id).sx;
	int sy = ps->get(id).sy;
	int w1 = sx - dx * 2;
	int x1 = (width - sx) / 2;
	int y1 = (height - sy) / 2;
	screenshoot screen(true);
	while(ismodal()) {
		screen.restore();
		// message window and button
		image(x1, y1, ps, id, ImageNoOffset);
		image(x1 + 90, y1, ps, ps->ganim(209, 0), ImageNoOffset);
		setcolor(ColorButton);
		draw::text((width - draw::textw(title)) / 2 - 8, (height - sy) / 2 + 8, title);
		setcolor(ColorState);
		rect rc = {x1 + 32, y1 + 32, x1 + sx - 24, y1 + sy - 48};
		//rectb(rc, colors::red);
		draw::text(rc, text, AlignCenterCenter);
		button(x1 + 100, y1 + 100, 90, cmd(buttonok), "Далее");
		domodal();
		switch(hot.key) {
		case KeyEscape:
		case KeyEnter:
			hot.key = InputUpdate;
			hot.param = 0;
			return;
		}
	}
}

void draw::dlgerr(const char* title, const char* format, ...) {
	char temp[4096]; szprintvs(temp, zendof(temp), format, xva_start(format));
	dlgmsg("Ошибка", temp);
}

bool dlgask(const char* title, const char* text) {
	return false;
}