#include "main.h"

using namespace draw;

animable::animable() : icn(res::NoRes), action(0), tick(0), timeout(1000 / 8), timestart(0) {
	pos.x = 0;
	pos.y = 0;
}

animable::animable(point pos, res::tokens icn, int action) : pos(pos), icn(icn), action(action), tick(0), timeout(1000 / 8), timestart(0) {}

rect animable::getrect() const {
	auto ps = gres(icn);
	if(!ps)
		return{0, 0, 0, 0};
	return ps->get(ps->ganim(action, tick)).getrect(pos.x, pos.y, 0);
}

void animable::update() {
	unsigned tm = draw::gettick();
	if(!timestart)
		timestart = tm;
	while(tm > timestart) {
		auto source = gres(icn);
		if(!source)
			return;
		auto pc = source->gcicle(action);
		if(++tick >= pc->count)
			tick = 0;
		timestart += timeout;
	}
}

void animable::painting(point camera) const {
	sprite* source = gres(icn);
	if(!source)
		return;
	auto pc = source->gcicle(action);
	if(!pc->count)
		return;
	int frame = pc->start + tick;
	point pt = pos - camera;
	draw::image(pt.x, pt.y, source, frame, 0);
}

bool animable::hittest(point camera) const {
	sprite* source = gres(icn);
	if(!source)
		return false;
	auto pc = source->gcicle(action);
	if(!pc->count)
		return false;
	int frame = pc->start + tick;
	point pt = pos - camera;
	return false;
	//return hittest(pt, source, frame, 0, mouse);
}