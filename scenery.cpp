#include "main.h"

using namespace draw;

void scenery::painting(point camera) const {
	auto ps = gres(res::SCENERY);
	if(!ps)
		return;
	auto fr = ps->ganim(type, draw::getstamp() / 100);
	auto pt = getposition() - camera;
	image(pt.x, pt.y, ps, fr, 0);
}