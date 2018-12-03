#include "main.h"

using namespace draw;

static int modify_weapon_base[] = {
	49, 50, -1, -1, -1, 53,
	-1, -1,
	54, 55, 61, -1, -1, -1,
	-1, -1,
	52, 52,
	56, 58, 59, 57,
};

int actor::byweapon(animation_s action, int weapon_index) {
	if(!weapon_index || modify_weapon_base[action] == -1)
		return action;
	return modify_weapon_base[action] + (weapon_index-1) * 13;
}

static res::tokens bygender(gender_s gender, res::tokens tm, res::tokens tf) {
	return (gender == Female) ? tf : tm;
}

int actor::getcicle() const {
	int wt = getweapon().getweaponindex();
	return byweapon(action, wt) * 6 + orientation;
}

const sprite* actor::getsprite() const {
	//return gres(gethuman(getarmor().get(), getgender()));
	return 0;
}

rect actor::getrect() const {
	auto pos = getposition();
	return{pos.x - 100, pos.y - 100, pos.x + 100, pos.y + 64};
}

void actor::moveto(point position, int run) {}

void actor::painting(point camera) const {
	auto source = getsprite();
	if(!source)
		return;
	auto cicle = getcicle();
	auto pc = source->gcicle(cicle);
	if(!pc->count)
		return;
	point pt = getposition() - camera;
	if(action == ActionRun || action == ActionWalk) {
		auto pa = draw::getaction(source, cicle / 6);
		if(!pa)
			return;
		auto pf = source->get(frame);
		draw::image(
			pt.x - pf.sx / 2 + pa->offset[orientation].x,
			pt.y - pf.sy + pa->offset[orientation].y,
			source, frame, ImageNoOffset);
	} else
		draw::image(pt.x, pt.y, source, frame, 0);
}

char actor::getorientation(point from, point to) {
	//static const char orientations[25] = {
	//	5, 5, 0, 0, 0,
	//	5, 5, 0, 0, 0,
	//	4, 4, 2, 1, 1,
	//	3, 3, 2, 2, 2,
	//	3, 3, 3, 2, 2
	//};
	static const char orientations[25] = {
		5, 5, 0, 0, 0,
		5, 5, 0, 0, 0,
		4, 4, 2, 1, 1,
		3, 3, 2, 2, 2,
		3, 3, 3, 2, 2
	};
	int dx = to.x - from.x;
	int dy = to.y - from.y;
	int div = imax(iabs(dx), iabs(dy));
	if(!div)
		return 2; //default
	if(div > 3)
		div /= 2;
	int ax = dx / div;
	int ay = dy / div;
	return orientations[(ay + 2) * 5 + ax + 2];
}

int actor::getlongest(const point from, const point to) {
	return imax(iabs(from.x - to.x), iabs(from.y - to.y));
}

int actor::getdistance(const point p1, const point p2) {
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return isqrt(dx*dx + dy * dy);
}

//short unsigned actor::getnext(short unsigned index, int orientation) {
//	// 0 - Right Up
//	// 1 - Right
//	// 2 - Right Down
//	// 3 - Left Down
//	// 4 - Left
//	// 5 - Left 
//	int nx = i2x(index);
//	switch(orientation) {
//	case 0:
//		if(nx & 1)
//			return index - 256 + 1;
//		return index + 1;
//	case 1:
//		if(nx & 1)
//			return index + 1;
//		return index + 256 + 1;
//	case 2:
//		return index + 1 * 256;
//	case 3:
//		if(nx & 1)
//			return index - 1;
//		return index + 256 - 1;
//	case 4:
//		if(nx & 1)
//			return index - 256 - 1;
//		return index - 1;
//	case 5:
//		return index - 1 * 256;
//	}
//	return index;
//}

//bool hittest(point camera, point mouse) const override {
//	sprite* source = getsprite();
//	if(!source)
//		return false;
//	int cicle = getcicle();
//	auto pc = source->gcicle(cicle);
//	if(!pc->count)
//		return false;
//	int frame = pc->start + tick;
//	point pt = getzpos() - camera;
//	if(action == ActionRun || action == ActionWalk) {
//		auto pa = draw::getaction(source, cicle / 6);
//		if(!pa)
//			return false;
//		auto pf = source->get(frame);
//		return draw::hittest(
//			pt.x - pf.sx / 2 + pa->offset[orientation].x,
//			pt.y - pf.sy + pa->offset[orientation].y,
//			source, frame, ImageNoOffset, mouse);
//	} else
//		return draw::hittest(pt.x, pt.y, source, frame, 0, mouse);
//}

//void moveto(short unsigned index, int runmode) {
//	if(!index || this->index == index)
//		return;
//	if(runmode == -1)
//		runmode = 0;
//	setorientation(
//		game::getorientation(
//			m2h(i2x(this->index), i2y(this->index)),
//			m2h(i2x(index), i2y(index))));
//	//setaction(runmode ? ActionRun : ActionWalk);
//	this->index = index;
//}
//
//void update() {
//	unsigned tm = draw::gettick();
//	if(!timestart)
//		timestart = tm;
//	while(tm > timestart) {
//		auto source = getsprite();
//		if(!source)
//			return;
//		if(action == ActionStand) {
//			auto pa = draw::getaction(source, action);
//			auto pc = source->gcicle(getcicle());
//			if(tick == pc->count - 1) {
//				tick = 0;
//				timestart += xrand(4, 18) * 1000;
//			} else {
//				settick(tick + 1);
//				timestart += timeout;
//			}
//		}
//		//else if(action == ActionWalk || action == ActionRun)
//		//{
//		//	int lp = game::getdistance(start, goal);
//		//	int lr = game::getdistance(start, pos);
//		//	if(lr >= lp)
//		//		nextstep();
//		//	if(start == goal)
//		//		stop();
//		//	else
//		//	{
//		//		settick(tick + 1);
//		//		timestart += timeout;
//		//		auto pa = draw::getaction(source, action);
//		//		auto pc = source->gcicle(getcicle());
//		//		auto pf = source->get(pc->start + tick);
//		//		pos.x += pf.ox;
//		//		pos.y += pf.oy;
//		//	}
//		//}
//		else if(isanimationback(action)) {
//			if(tick == 0)
//				setaction(ActionStand);
//			else {
//				tick--;
//				timestart += timeout;
//			}
//		} else {
//			auto pa = draw::getaction(source, action);
//			auto pc = source->gcicle(getcicle());
//			if(tick == pc->count - 1)
//				setaction(ActionStand);
//			else {
//				tick++;
//				timestart += timeout;
//			}
//		}
//	}
//}
//
//void settick(int tick) {
//	if(this->tick != tick) {
//		auto source = getsprite();
//		if(!source)
//			return;
//		auto pc = source->gcicle(getcicle());
//		if(!pc->count)
//			return;
//		this->tick = tick % pc->count;
//	}
//}
//
//static bool isanimationback(int action) {
//	switch(action) {
//	case ActionPrepareWeapon:
//		return true;
//	default:
//		return false;
//	}
//}
//
//void setaction(int action) {
//	if(action < 0)
//		action = 0;
//	if(this->action == action)
//		return;
//	if((action == ActionHideWeapon
//		|| action == ActionPrepareWeapon)
//		&& wear[Weapon - FirstWear] == 0)
//		return;
//	auto ps = getsprite();
//	if(!ps)
//		return;
//	auto pa = draw::getaction(ps, action);
//	if(pa->speed)
//		timeout = 1000 / pa->speed;
//	else
//		timeout = animation_speed;
//	this->action = action;
//	timestart = draw::gettick();
//	if(isanimationback(action)) {
//		auto pc = ps->gcicle(getcicle());
//		tick = pc->count - 1;
//	} else
//		tick = 0;
//}