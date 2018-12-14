#include "main.h"

void map_render::add(int x, int y, const sprite* ps, short unsigned frame) {
	animable m;
	m.x = x;
	m.y = y;
	m.frame = frame;
	m.ps = ps;
	auto pm = elements.add();
	*pm = m;
	adat::add(pm);
}