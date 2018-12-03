#include "main.h"

template<> const char* getstr<variant>(variant value) {
	switch(value.type) {
	case Abilities: return getstr(value.ability);
	case Damages: return getstr(value.damage);
	case Illness: return getstr(value.illness);
	case Parameters: return getstr(value.parameter);
	case Perks: return getstr(value.perk);
	case Skills: return getstr(value.skill);
	case Wounds: return getstr(value.wound);
	default: return "Без имени";
	}
}

const char* variant::getnameshort() const {
	switch(type) {
	case Abilities: return ability_data[skill].name_short;
	case Damages: return damage_data[damage].name_short;
	case Parameters: return parameter_data[parameter].name_short;
	case Skills: return skill_data[skill].name;
	default: return "Нет сокращения";
	}
}

const char* variant::getnameshortest() const {
	switch(type) {
	case Abilities: return ability_data[skill].name_short;
	case Damages: return damage_data[damage].just_name;
	case Parameters: return parameter_data[parameter].name_shortest;
	case Skills: return skill_data[skill].name;
	default: return "Нет сокращения";
	}
}

const char* variant::getdescription() const {
	switch(type) {
	case Abilities: return ability_data[skill].description;
	case Damages: return damage_data[damage].description;
	case Illness: return illness_data[illness].description;
	case Parameters: return parameter_data[parameter].description;
	case Perks: return perk_data[perk].description;
	case Skills: return skill_data[skill].description;
	default: return "Без описания";
	}
}

int variant::getimage() const {
	switch(type) {
	case Abilities: return ability_data[skill].fid;
	case Damages: return damage_data[damage].fid;
	case Illness: return illness_data[illness].fid;
	case Parameters: return parameter_data[parameter].fid;
	case Perks: return perk_data[perk].fid;
	case Skills: return skill_data[skill].fid;
	case Wounds: return wound_data[wound].fid;
	default: return 0;
	}
}