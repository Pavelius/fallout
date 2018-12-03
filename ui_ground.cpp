#include "main.h"

struct ground : item, drawable {
	short unsigned	index;

	point getpos() const {
		return i2h(index);
	}

	rect getrect() const override {
		if(!owner)
			return{0, 0, 0, 0};
		point pt = getpos();
		return{pt.x - 64, pt.y - 64, pt.x + 64, pt.y + 64};
	}

	point getposition() const override {
		if(!owner)
			return{0, 0};
		return getpos();
	}

	void painting(point camera) const override {
		auto ps = gres(res::ITEMS);
		if(!ps)
			return;
		auto fr = ps->ganim(item::get(object, Image), 0);
		auto pt = getpos() - camera;
		draw::image(pt.x, pt.y, ps, fr, 0);
	}

	bool hittest(point camera, point hitpoint) const override {
		auto ps = gres(res::ITEMS);
		if(!ps)
			return false;
		auto fr = ps->ganim(item::get(object, Image), 0);
		auto pt = getpos() - camera;
		return draw::hittest(pt.x, pt.y, ps, fr, 0, hitpoint);
	}

};
static adat<ground, 2048> objects;