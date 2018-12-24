#include "main.h"

using namespace draw;

static animation_s animation_sequence[][2] = {{AnimateKilledBlowup, AnimateDeadBlowup},
{AnimateKilledBurstInChest, AnimateDeadBurstInChest},
{AnimateKilledBurstInHead, AnimateDeadBurstInHead},
{AnimateKilledChest, AnimateDeadChest},
{AnimateKilledElectro, AnimateDeadElectro},
{AnimateKilledImmolate, AnimateDeadImmolate},
{AnimateKilledLaser, AnimateDeadLaser},
{AnimateKilledElectroChest, AnimateDeadElectroChest},
{AnimateKilledMelt, AnimateDeadMelt},
{AnimateKilledFired, AnimateDeadFired},
};
static animation_s modify_weapon_base[] = {
	AnimateWeaponStand, AnimateWeaponWalk, AnimatePickup, AnimateUse, AnimateWeaponDodge,
	AnimateDamaged, AnimateDamagedRear,
	AnimateWeaponThrust, AnimateWeaponSwing, AnimateWeaponThrow, AnimateRun,
};

animation_s actor::getbase(animation_s id) {
	if(id >= AnimateWeaponStand) {
		const auto k = (AnimateWeaponThrow - AnimateWeaponStand + 1);
		auto wi = (id - AnimateWeaponStand) / k;
		return animation_s(AnimateWeaponStand + wi * k);
	}
	return AnimateStand;
}

animation_s actor::getsubaction(animation_s id) {
	if(id >= AnimateWeaponStand) {
		const auto k = (AnimateWeaponThrow - AnimateWeaponStand + 1);
		auto wi = (id - AnimateWeaponStand) / k;
		return animation_s(AnimateWeaponStand + (id - AnimateWeaponStand) % k);
	}
	return AnimateStand;
}

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
	auto pc = ps->getcicle(getcicle());
	if(!pc->count)
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

void actor::moveto(point position, int run) {}

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

animation_s actor::getnextanim(animation_s id) {
	for(auto& e : animation_sequence) {
		if(e[0] == id)
			return e[1];
	}
	return AnimateStand;
}

animation_s actor::getprevanim(animation_s id) {
	for(auto& e : animation_sequence) {
		if(e[1] == id)
			return e[0];
	}
	return AnimateStand;
}

void actor::setaction(animation_s value, bool backward) {
	if(value > AnimateWeaponStand) {
		static animation_s actions[13] = {
			AnimateStand, AnimateWalk, AnimateWeaponTakeOn, AnimateWeaponHide, AnimateDodge,
			AnimateWeaponThrust, AnimateWeaponSwing,
			AnimateWeaponAim, AnimateWeaponAimEnd,
			AnimateWeaponSingle, AnimateWeaponBurst, AnimateWeaponFlame,
			AnimateWeaponThrow,
		};
		value = actions[(value - AnimateWeaponStand) % 13];
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
	if(ismoving())
		moveshift();
}

unsigned actor::getfps() const {
	return 8;
	auto ps = getsprite();
	if(!ps)
		return 8;
	auto pi = draw::getaction(ps, getcicle() / 6);
	if(!pi)
		return 8;
	return pi->speed;
}

void actor::moveshift() {
	auto ps = getsprite();
	if(!ps)
		return;
	auto pc = ps->getcicle(getcicle());
	auto& pf = ps->get(pc->start + frame);
	x += pf.ox;
	y += pf.oy;
}

bool actor::ismoving() const {
	return action == AnimateWalk
		|| action == AnimateRun
		|| action == AnimateWeaponWalk;
}

void actor::update() {
	if(!next_stamp)
		return;
	if(gametick() < next_stamp)
		return;
	next_stamp += 1000 / getfps();
	auto ps = getsprite();
	if(!ps)
		return;
	auto need_shift = ismoving();
	if(frame == frame_maximum) {
		if(action == AnimateStand) {
			frame = 0;
			next_stamp += xrand(2 * 1000, 8 * 1000);
		} else if (need_shift) {
			frame = 0;
			moveshift();
		} else
			setaction(AnimateStand);
		return;
	}
	if(frame < frame_maximum)
		frame++;
	else
		frame--;
	if(need_shift)
		moveshift();
}

void actor::getanimation(animation& result) {
	memset(&result, 0, sizeof(result));
	result.ps = getsprite();
	if(!result.ps)
		return;
	auto cl = getcicle();
	auto pc = result.ps->getcicle(cl);
	if(!pc->count)
		return;
	result.fi = pc->start + frame;
	if(action == AnimateRun || action == AnimateWalk) {
		auto pa = draw::getaction(result.ps, cl / 6);
		if(!pa)
			return;
		auto& pf = result.ps->get(result.fi);
		result.position.x = x - pf.sx / 2 + pa->offset[orientation].x;
		result.position.y = y - pf.sy + pa->offset[orientation].y;
		result.flags = ImageNoOffset;
	} else
		result.position = *this;
}