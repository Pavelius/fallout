#include "main.h"

using namespace draw;

animable::animable() : point{0, 0} {
}

rect animable::getrect() const {
	if(!(*this))
		return{0, 0, 0, 0};
	auto fi = ps->ganim(frame, getstamp() / 100);
	return ps->get(fi).getrect(x, y, 0);
}

void animable::painting(point camera) const {
	if(!(*this))
		return;
	point pt = static_cast<const point&>(*this) - camera;
	draw::image(pt.x, pt.y, ps, frame, 0);
}

bool animable::hittest(point camera) const {
	if(!(*this))
		return false;
	return false;
	//return hittest(pt, source, frame, 0, mouse);
}