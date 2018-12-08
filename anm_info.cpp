#include "util.h"

using namespace draw;

extern "C" void scale2x(void* void_dst, unsigned dst_slice, const void* void_src, unsigned src_slice, unsigned width, unsigned height);

void anm_info::serialize(res::tokens id, bool write) {
	if(!id)
		return;
	char temp[260];
	szprint(temp, zendof(temp), "animpt/%1.pts", draw::getresname(id));
	if(write) {
		io::file file(temp, StreamWrite);
		if(file)
			file.write(this, sizeof(*this));
	} else {
		io::file file(temp, StreamRead);
		memset(this, 0, sizeof(*this));
		if(file)
			file.read(this, sizeof(*this));
	}
}

void anm_info::validate(int i) {
	static animation_s copied[] = {
		AnimatePickup, AnimateUse, AnimateDodge,
		AnimateDamaged, AnimateDamagedRear,
		AnimateUnarmed1, AnimateUnarmed2, AnimateThrown, AnimateRun,
		AnimateKnockOutBack, AnimateKnockOutForward,
		AnimateKilledChest, AnimateKilledElectro,
		AnimateKilledBurstInHead, AnimateKilledBurstInChest,
		AnimateKilledImmolate, AnimateKilledLaser, AnimateKilledElectroChest,
		AnimateKilledBlowup, AnimateKilledMelt, AnimateKilledFired,
	};
	for(auto id : copied)
		points[id * 6 + i] = points[AnimateUse * 6 + i];
	for(auto w = 0; w < 10; w++)
		validate_weapon(i, w);
}

void anm_info::validate_weapon(int i, int w) {
	static animation_s copied_weapon[] = {
		AnimateWeaponTakeOn, AnimateWeaponHide, AnimateWeaponDodge,
		AnimateWeaponThrust, AnimateWeaponSwing,
		AnimateWeaponAim, AnimateWeaponAttack,
		AnimateWeaponSingle, AnimateWeaponBurst, AnimateWeaponFlame,
		AnimateWeaponThrow,
	};
	if(w<10) {
		for(auto id : copied_weapon) {
			auto b = animation_s(AnimateWeaponStand + w * 13);
			auto a1 = (b + (id - AnimateWeaponStand)) * 6 + i;
			auto a2 = (b + (AnimateWeaponStand - AnimateWeaponStand)) * 6 + i;
			points[a1] = points[a2];
		}
	}
}

static void mmax(int& v, int min, int max) {
	if(v > max)
		v = max;
	if(v < 0)
		v = 0;
}

constexpr animation_s ganm(animation_s w, animation_s a) {
	return animation_s(w + (a - AnimateWeaponStand));
}

void test_animate() {
	char temp[260];
	anm_info ai = {};
	static animation_s actions[] = {AnimateStand, AnimatePickup, AnimateUse, AnimateDodge,
		AnimateDamaged, AnimateDamagedRear,
		AnimateUnarmed1, AnimateUnarmed2, AnimateThrown, AnimateRun,
		AnimateKnockOutBack, AnimateKnockOutForward,
		//
		AnimatePistol, AnimateSMG, AnimateClub, AnimateRifle, AnimateWeaponStand,
		AnimateHammer, AnimateSpear, AnimateHeavyGun, AnimateMachineGun, AnimateRocketLauncher,
		//
		AnimateKilledChest, AnimateKilledElectro,
		AnimateKilledBurstInHead, AnimateKilledBurstInChest,
		AnimateKilledImmolate, AnimateKilledLaser, AnimateKilledElectroChest,
		AnimateKilledBlowup, AnimateKilledMelt, AnimateKilledFired,
		AnimateBloodedBack, AnimateBloodedForward,
		AnimateStandUpForward, AnimateStandUpBack,
		//
		AnimateDeadBackNoBlood, AnimateDeadForwardNoBlood,
		AnimateDeadChest, AnimateDeadElectro,
		AnimateDeadBurstInHead, AnimateDeadBurstInChest,
		AnimateDeadImmolate, AnimateDeadLaser, AnimateDeadElectroChest,
		AnimateDeadBlowup, AnimateDeadMelt, AnimateDeadFired,
		AnimateDeadBack, AnimateDeadForward,
	};
	static res::tokens resources[] = {res::HMLTHR, res::HMCMBT, res::HFLTHR};
	int resource = 0, action = 0, orientation = 2, weapon = 0;
	bool fast_stand = false;
	bool freezy_frame = false;
	bool show_red = true;
	bool lock_stand = true;
	bool normal_mode = false;
	animation_s am = AnimateStand;
	res::tokens last_id = res::NoRes;
	auto rsin = gres(res::INTRFACE);
	draw::surface original(320, 240, 32);
	while(ismodal()) {
		mmax(orientation, 0, 5);
		mmax(action, 0, sizeof(actions) / sizeof(actions[0]) - 1);
		mmax(resource, 0, sizeof(resources) / sizeof(resources[0]) - 1);
		mmax(weapon, 0, 13 - 1);
		if(last_id != resources[resource]) {
			ai.serialize(last_id, true);
			last_id = resources[resource];
			ai.serialize(last_id, false);
		}
		rectf({0, 0, getwidth(), getheight()}, colors::gray);
		short x = 160, y = 160;
		point pt = {x, y};
		auto a = actions[action];
		if(fast_stand)
			a = am;
		if(a >= AnimateWeaponStand)
			a = animation_s(a + weapon);
		auto* pa = ai.points + (a * 6 + orientation);
		szprint(temp, zendof(temp), "{%1i, %2i}", pa->x, pa->y);
		if(fast_stand)
			szprint(zend(temp), zendof(temp), " stand");
		if(freezy_frame)
			szprint(zend(temp), zendof(temp), " freezy");
		if(!lock_stand)
			szprint(zend(temp), zendof(temp), " unlocked");
		if(normal_mode)
			szprint(zend(temp), zendof(temp), " normal");
		if(am!=AnimateStand)
			szprint(zend(temp), zendof(temp), " custom");
		auto ps = gres(resources[resource]);
		auto tick = getstamp() / 200;
		auto c1 = a * 6 + orientation;
		auto fi = ps->ganim(c1, freezy_frame ? 0 : tick);
		auto& fr = ps->get(fi);
		if(true) {
			draw::state push;
			canvas = &original; setclip();
			rectf({0, 0, getwidth(), getheight()}, colors::gray);
			szprint(zend(temp), zendof(temp), " {%1i, %2i}, {%3i, %4i}", fr.sx, fr.sy, fr.ox, fr.oy);
			image(x - 32 / 2, y - 16 / 2, rsin, 1, ImageNoOffset);
			if(!normal_mode)
				pt = pt + *pa;
			image(pt.x, pt.y, ps, fi, 0);
			if(show_red) {
				line(x - 4, y, x + 4, y, colors::red);
				line(x, y - 4, x, y + 4, colors::red);
			}
		}
		scale2x(canvas->ptr(0, 0), canvas->scanline,
			original.ptr(0, 0), original.scanline,
			original.width, original.height);
		text(10, 10, temp);
		domodal();
		switch(hot.key) {
		case KeyEscape: breakmodal(0); break;
		case Alpha + 'Q': resource--; break;
		case Alpha + 'W': resource++; break;
		case Alpha + 'D': orientation--; break;
		case Alpha + 'F': orientation++; break;
		case Alpha + 'A': action--; ai.serialize(last_id, true); break;
		case Alpha + 'S': action++; ai.serialize(last_id, true); break;
		case Alpha + 'Y': weapon++; ai.serialize(last_id, true); break;
		case Alpha + 'T': weapon--; ai.serialize(last_id, true); break;
		case Alpha + 'Z': fast_stand = !fast_stand; break;
		case Alpha + 'X': freezy_frame = !freezy_frame; break;
		case Alpha + 'R': show_red = !show_red; break;
		case Alpha + 'U': lock_stand = !lock_stand; break;
		case Alpha + 'N': normal_mode = !normal_mode; break;
		case Alpha + 'O': am = AnimateStand; break;
		case Alpha + 'L': am = a; break;
		case Alpha + 'V':
			if(!lock_stand) {
				ai.validate(orientation);
				lock_stand = true;
			} else
				dlgmsg("������", "���������� ��������� �������� 'U'");
			break;
		case Alpha + 'B':
			if(!lock_stand) {
				if(a >= AnimateWeaponStand) {
					auto w = (a - AnimateWeaponStand) / 13;
					ai.validate_weapon(orientation, w);
				}
				lock_stand = true;
			} else
				dlgmsg("������", "���������� ��������� �������� 'U'");
			break;
		case KeyLeft:
			if((pa == (ai.points + orientation)) && lock_stand)
				break;
			pa->x--;
			break;
		case KeyRight:
			if((pa == (ai.points + orientation)) && lock_stand)
				break;
			pa->x++;
			break;
		case KeyUp:
			if((pa == (ai.points + orientation)) && lock_stand)
				break;
			pa->y--;
			break;
		case KeyDown:
			if((pa == (ai.points + orientation)) && lock_stand)
				break;
			pa->y++;
			break;
		}
	}
	ai.serialize(last_id, true);
}