#include "main.h"

using namespace draw;

static struct resource {
	const char*			name;
	sprite*				data;
	bool				notfound;
} objects[] = {{"NONE"},
	//
{"BACKGRND"}, {"SKILLDEX"}, {"INVEN"}, {"ITEMS"}, {"MISC"}, {"SCENERY"}, {"WALLS"}, {"TILES"}, {"INTRFACE"},
{"FONT1"}, {"FONT2"}, {"FONT3"}, {"FONT4"},
// humans
{"HANPWR"}, {"HAPOWR"}, {"HAROBE"},
{"HFCMBT"}, {"HFJMPS"}, {"HFLTHR"}, {"HFMAXX"}, {"HFMETL"}, {"HFPRIM"},
{"HMBJMP"}, {"HMBMET"},
{"HMCMBT"}, {"HMJMPS"}, {"HMLTHR"}, {"HMMAXX"}, {"HMMETL"}, {"HMWARR"},
{"MABOS2"}, {"MABOSS"}, {"MAMTNT"}, {"MAMTN2"},
{"MAROBE"}, {"NABLUE"}, {"NABRWN"},
{"NACHLD"},
{"NAGHUL"}, {"NAGLOW"}, {"NAPOWR"}, {"NAROBE"}, {"NAVGUL"}, {"NAWHIT"},
{"NFASIA"},
{"NFBRLP"}, {"NFLYNN"},
{"NFMAXX"}, {"NFMETL"},
{"NFNICE"}, {"NFPEAS"}, {"NFPRIM"}, {"NFTRMP"}, {"NFVALT"}, {"NFVRED"},
{"NMASIA"},
{"NMBONC"}, {"NMBOXX"}, {"NMBPEA"}, {"NMBRLP"}, {"NMBRSR"}, {"NMBSNP"},
{"NMCOPP"}, {"NMDOCC"}, {"NMFATT"}, {"NMGANG"}, {"NMGRCH"}, {"NMLABB"},
{"NMLOSR"}, {"NMLTHR"}, {"NMMAXX"}, {"NMMEXI"}, {"NMMYRN"}, {"NMNICE"}, {"NMOLDD"},
{"NMPEAS"}, {"NMRGNG"}, {"NMVALT"}, {"NMWARR"},
// animals and robots
{"MAANTT"}, {"MABRAN"}, {"MABROM"}, {"MACLAW"}, {"MACLW2"}, {"MADDOG"}, {"MADEGG"},
{"MADETH"}, {"MAFIRE"}, {"MAGCKO"}, {"MAGKO2"}, {"MALIEN"}, {"MAMANT"}, {"MAMRAT"},
{"MAMURT"}, {"MAPLNT"}, {"MAQUEN"}, {"MASCP2"}, {"MASCRP"}, {"MASPHN"}, {"MASRAT"},
{"MATHNG"}, {"MACYBR"}, {"MAFEYE"}, {"MAGUN2"}, {"MAGUNN"}, {"MAHAND"}, {"MAROBO"},
{"MAROBT"},
};
static_assert((sizeof(objects) / sizeof(objects[0])) == res::Count, "Need resource update");
extern unsigned char	color_pallette[256 * 3];
static color			color_values[256];

void draw::setpallette(int daylight) {
	for(int i = 0; i < 256; i++) {
		color_values[i].r = color_pallette[i * 3 + 0] * daylight;
		color_values[i].g = color_pallette[i * 3 + 1] * daylight;
		color_values[i].b = color_pallette[i * 3 + 2] * daylight;
	}
	draw::palt = color_values;
}

void draw::setcolor(color_s value) {
	fore = color_values[value];
}

void draw::initialize() {
	create(-1, -1, 640, 480, 0, 32);
	setcaption("Фоллаут");
	setpallette(4);
	settimer(48);
	syscursor(false);
	setfont(res::FONT1);
	setcolor(ColorText);
	draw::fore_stroke = colors::blue;
}

sprite* draw::gres(res::tokens id) {
	if(objects[id].notfound)
		return 0;
	if(!objects[id].data) {
		if(!id)
			return 0;
		char temp[260];
		objects[id].data = (sprite*)loadb(szurl(temp, "resource", objects[id].name, "pma"));
		if(!objects[id].data)
			objects[id].notfound = true;
	}
	return objects[id].data;
}

const char* draw::getresname(res::tokens id) {
	return objects[id].name;
}

const actinfo* draw::getaction(const sprite* p, int action) {
	// Информация по действиям идет после циклов
	// которые в свою очередь идут в конце.
	if(!p->cicles_offset)
		return 0;
	return (actinfo*)((char*)p + p->cicles_offset + p->cicles * sizeof(sprite::cicle)) + action;
}

void draw::modify(color* pal, int index, int start, int count) {
	color c1 = pal[start];
	for(int i = 0; i < count; i++)
		pal[index + i] = c1.mix(colors::black, (count - i) * 16);
}

void draw::tiles(point screen) {
	auto ps = gres(res::TILES);
	if(!ps)
		return;
	auto a = draw::getstamp() / 100;
	auto pm = s2m(camera);
	int x1 = pm.x - 8; int x2 = x1 + 8 + 11;
	int y1 = pm.y; int y2 = y1 + 18;
	for(auto y = y1; y < y2; y++) {
		if(y < 0 || y >= map::height)
			continue;
		for(int x = x1; x < x2; x++) {
			if(x < 0 || x >= map::width)
				continue;
			auto ti = m2i(x, y);
			auto tv = map::get(ti);
			if(tv > 1) {
				point pt = m2s(x, y);
				point pz = pt + screen - camera;
				draw::image(pz.x, pz.y + tile_height / 2, ps, ps->ganim(tv, a), 0);
			}
		}
	}
}

void draw::setfont(res::tokens e) {
	font = gres(e);
}

void draw::setcamera(point pt) {
	static point ptc = {640 / 2, 480 / 2};
	camera = pt - ptc;
}

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