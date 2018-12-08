#include "main.h"

using namespace draw;

static animation_s modify_weapon_base[] = {
	AnimateWeaponStand, AnimateWeaponWalk, AnimatePickup, AnimateUse, AnimateWeaponDodge,
	AnimateDamaged, AnimateDamagedRear,
	AnimateWeaponThrust, AnimateWeaponSwing, AnimateWeaponThrow, AnimateRun,
};

int actor::byweapon(animation_s action, int weapon_index) {
	if(!weapon_index)
		return action;
	auto a1 = action;
	if(action <= AnimateRun)
		a1 = modify_weapon_base[action];
	if(a1 >= AnimateWeaponStand)
		return a1 + (weapon_index - 1) * 13;
	return a1;
}

int actor::getcicle() const {
	return byweapon(action, getweapon().getweaponindex()) * 6 + orientation;
}

unsigned short actor::getlastframe() const {
	auto ps = getsprite();
	if(!ps)
		return 0;
	auto pc = ps->gcicle(getcicle());
	if(!pc || !pc->count)
		return 0;
	return pc->count - 1;
}

void actor::preview(int x, int y, gender_s gender, const item& armor, const item& weapon, int orientation, animation_s action, unsigned tick, const rect& clip) {
	res::tokens icn = armor.getdress(gender);
	if(icn == res::NoRes) {
		if(gender == Male)
			icn = res::HMWARR;
		else
			icn = res::HFPRIM;
	}
	auto ps = gres(icn);
	if(!ps)
		return;
	if(orientation == -1)
		orientation = (getstamp() / 300) % 6;
	auto wp = weapon.get(FrameWeapon);
	auto c0 = actor::byweapon(action, wp);
	auto cl = c0 * 6 + orientation;
	auto pa = draw::getaction(ps, c0);
	if(!pa)
		return;
	auto fr = ps->ganim(cl, tick);
	draw::state push;
	draw::setclip({x + clip.x1, y + clip.y1, x + clip.x2, y + clip.y2});
	draw::image(x, y, ps, fr, 0);
}

rect actor::getrect() const {
	auto pos = getposition();
	return{pos.x - 100, pos.y - 100, pos.x + 100, pos.y + 64};
}

void actor::moveto(point position, int run) {}

void actor::painting(point camera) const {
	auto ps = getsprite();
	if(!ps)
		return;
	auto pc = ps->gcicle(getcicle());
	if(!pc->count)
		return;
	point pt = getposition() - camera;
	if(action == AnimateRun || action == AnimateWalk) {
		//auto pa = getaction(source, cicle / 6);
		//if(!pa)
		//	return;
		//auto pf = source->get(frame);
		//draw::image(
		//	pt.x - pf.sx / 2 + pa->offset[orientation].x,
		//	pt.y - pf.sy + pa->offset[orientation].y,
		//	source, frame, ImageNoOffset);
	} else {
		auto fr = pc->start + frame;
		image(pt.x, pt.y, ps, fr, 0);
	}
}

char actor::getorientation(point from, point to) {
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

void actor::setaction(animation_s value, bool backward) {
	if(action > AnimateWeaponStand) {
		static animation_s actions[13] = {
			AnimateStand, AnimateWalk, AnimateWeaponTakeOn, AnimateWeaponHide, AnimateDodge,
			AnimateWeaponThrust, AnimateWeaponSwing,
			AnimateWeaponAim, AnimateWeaponAttack,
			AnimateWeaponSingle, AnimateWeaponBurst, AnimateWeaponFlame,
			AnimateWeaponThrow,
		};
		action = actions[(action - AnimateWeaponStand) % 13];
	}
	action = value;
	frame = 0;
	frame_maximum = getlastframe();
	if(backward)
		iswap(frame, frame_maximum);
	if(action == AnimateStand)
		next_stamp = gametick() + xrand(2 * 1000, 12 * 1000);
	else
		next_stamp = gametick() + 1000 / getfps();
}

void actor::update() {
	unsigned current_tick = gametick();
	if(!next_stamp)
		next_stamp = current_tick + 1000 / getfps();
	while(current_tick > next_stamp) {
		next_stamp += 1000 / getfps();
		auto ps = getsprite();
		if(!ps)
			return;
		if(frame == frame_maximum) {
			if(action == AnimateStand) {
				frame = 0;
				next_stamp += xrand(2, 8) * 500;
			} else
				setaction(AnimateStand);
		} else {
			if(frame < frame_maximum)
				frame++;
			else
				frame--;
		}
	}
}

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