#include "main.h"

using namespace draw;

static int compare_animation(const void* p1, const void* p2) {
	auto d1 = (animation*)p1;
	auto d2 = (animation*)p2;
	auto z1 = d1->position.y + d1->zorder;
	auto z2 = d2->position.y + d2->zorder;
	if(z1 < z2)
		return -1;
	if(z1 > z2)
		return 1;
	if(d1->position.x < d1->position.x)
		return -1;
	if(d1->position.x > d1->position.x)
		return 1;
	return 0;
}

void animation::setup(point position, const sprite* ps, short unsigned fi) {
	this->position = position;
	this->ps = ps;
	this->fi = fi;
	this->zorder = 0;
	this->param = 0;
	this->flags = 0;
}

void animation::sort(animation* source, unsigned count) {
	qsort(source, count, sizeof(animation), compare_animation);
}

void animation::painting(point camera) const {
	auto pt = position - camera;
	draw::image(pt.x, pt.y, ps, fi, flags);
}