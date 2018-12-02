#include "main.h"

struct ground : public drawable {
	int				object;
	short unsigned	owner;

	point getpos() const {
		return i2h(owner);
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

int game::getground(int** result, int index) {
	auto pb = result;
	auto pe = pb + 1024; // Maximum 1024 items per creature
	for(auto& e : objects) {
		if(e.owner != index || !e.object)
			continue;
		*pb++ = &e.object;
		if(pb >= pe)
			break;
	}
	*pb = 0;
	return pb - result;
}

void game::dropitem(int index, int rec) {
	auto pe = objects.add();
	pe->owner = index;
	pe->object = rec;
}