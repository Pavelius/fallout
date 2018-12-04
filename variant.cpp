#include "main.h"

template<> const char* getstr<variant>(variant value) {
	switch(value.type) {
	case Abilities: return getstr(value.ability);
	case Damages: return getstr(value.damage);
	case Perks: return getstr(value.perk);
	case Skills: return getstr(value.skill);
	case Wounds: return getstr(value.wound);
	default: return "��� �����";
	}
}

const char* variant::getnameshort() const {
	switch(type) {
	case Abilities: return ability_data[skill].name_short;
	case Damages: return damage_data[damage].name;
	case Skills: return skill_data[skill].name;
	default: return "��� ����������";
	}
}

const char* variant::getnameabr() const {
	switch(type) {
	case Abilities: return ability_data[skill].name_abr;
	case Damages: return damage_data[damage].name;
	case Skills: return skill_data[skill].name;
	default: return "��� ����������";
	}
}

const char* variant::getdescription() const {
	switch(type) {
	case Abilities: return ability_data[skill].description;
	case Damages: return damage_data[damage].description;
	case Perks: return perk_data[perk].description;
	case Skills: return skill_data[skill].description;
	default: return "��� ��������";
	}
}

int variant::getimage() const {
	switch(type) {
	case Abilities: return ability_data[skill].fid;
	case Perks: return perk_data[perk].fid;
	case Skills: return skill_data[skill].fid;
	case Wounds: return wound_data[wound].fid;
	default: return 0;
	}
}