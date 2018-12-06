#include "util.h"

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

void anm_info::validate() {
	static animation_s copied[] = {
		AnimatePickup, AnimateUse, AnimateDodge,
		AnimateDamaged, AnimateDamagedRear,
		AnimateUnarmed1, AnimateUnarmed2, AnimateThrown, AnimateRun,
		AnimateKnockOutBack, AnimateKnockOutForward,
		//
		AnimateKilledChest, AnimateKilledElectro,
		AnimateKilledBurstInHead, AnimateKilledBurstInChest,
		AnimateKilledImmolate, AnimateKilledLaser, AnimateKilledElectroChest,
		AnimateKilledBlowup, AnimateKilledMelt, AnimateKilledFired,
	};
	static animation_s copied_weapon[] = {
		AnimateWeaponTakeOn, AnimateWeaponTakeOff, AnimateWeaponDodge,
		AnimateWeaponThrust, AnimateWeaponSwing,
		AnimateWeaponAim, AnimateWeaponAttack,
		AnimateWeaponSingle, AnimateWeaponBurst, AnimateWeaponFlame,
		AnimateWeaponThrow,
	};
	for(auto i = 0; i < 6; i++) {
		for(auto id : copied)
			points[id*6 + i] = points[AnimateUse*6 + i];
		for(auto w = 0; w < 10; w++) {
			for(auto id : copied_weapon) {
				auto b = animation_s(AnimateWeaponStand + w * 13);
				auto a1 = (b + (id - AnimateWeaponStand)) * 6 + i;
				auto a2 = (b + (AnimateWeaponStand - AnimateWeaponStand)) * 6 + i;
				points[a1] = points[a2];
			}
		}
	}
}