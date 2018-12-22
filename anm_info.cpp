#include "util.h"

using namespace draw;

extern "C" void scale2x(void* void_dst, unsigned dst_slice, const void* void_src, unsigned src_slice, unsigned width, unsigned height);

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
	static res::tokens resources[] = {res::HFLTHR, res::HANPWR, res::HMLTHR, res::HMCMBT, };
	int resource = 0, action = 0, orientation = 2, weapon = 0;
	bool fast_stand = false;
	bool freezy_frame = false;
	bool show_red = true;
	bool lock_stand = true;
	animation_s am = AnimateStand;
	res::tokens last_id = res::NoRes;
	auto rsin = gres(res::INTRFACE);
	draw::surface original(320, 240, 32);
	while(ismodal()) {
		mmax(orientation, 0, 5);
		mmax(action, 0, sizeof(actions) / sizeof(actions[0]) - 1);
		mmax(resource, 0, sizeof(resources) / sizeof(resources[0]) - 1);
		mmax(weapon, 0, 13 - 1);
		if(last_id != resources[resource])
			last_id = resources[resource];
		rectf({0, 0, getwidth(), getheight()}, colors::gray);
		short x = 160, y = 160;
		point pt = {x, y};
		auto a = actions[action];
		if(fast_stand)
			a = am;
		if(a >= AnimateWeaponStand)
			a = animation_s(a + weapon);
		auto* pa = ai.points + (a * 6 + orientation);
		auto ps = gres(resources[resource]);
		auto psa = draw::getaction(ps, a);
		szprint(temp, zendof(temp), "{%1i, %2i}", pa->x, pa->y);
		if(fast_stand)
			szprint(zend(temp), zendof(temp), " stand");
		if(freezy_frame)
			szprint(zend(temp), zendof(temp), " freezy");
		if(!lock_stand)
			szprint(zend(temp), zendof(temp), " unlocked");
		if(am!=AnimateStand)
			szprint(zend(temp), zendof(temp), " custom");
		auto tick = getstamp() / 200;
		auto c1 = a * 6 + orientation;
		auto fi = ps->ganim(c1, freezy_frame ? 0 : tick);
		auto& fr = ps->get(fi);
		if(true) {
			draw::state push;
			canvas = &original; setclip();
			rectf({0, 0, getwidth(), getheight()}, colors::gray);
			szprint(zend(temp), zendof(temp), " {%1i, %2i}, {%3i, %4i}", fr.sx, fr.sy, fr.ox, fr.oy);
			if(psa) {
				szprint(zend(temp), zendof(temp), " action {%1i, %2i}/{%3i, %4i}",
					psa->offset[orientation].x, psa->offset[orientation].y,
					psa->lastof[orientation].x, psa->lastof[orientation].y);
			}
			image(x - 32 / 2, y - 16 / 2, rsin, 1, ImageNoOffset);
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
		case Alpha + 'A': action--; break;
		case Alpha + 'S': action++; break;
		case Alpha + 'Y': weapon++; break;
		case Alpha + 'T': weapon--; break;
		case Alpha + 'Z': fast_stand = !fast_stand; break;
		case Alpha + 'X': freezy_frame = !freezy_frame; break;
		case Alpha + 'R': show_red = !show_red; break;
		case Alpha + 'U': lock_stand = !lock_stand; break;
		case Alpha + 'O': am = AnimateStand; break;
		case Alpha + 'L': am = a; break;
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
}