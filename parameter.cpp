#include "main.h"

static struct parameter_info {
	int					fid;
	const char*			name;
	int					(creature::*get)() const;
	const char*			description;
} parameter_data[] = {{8, "�������", &creature::getlevel},
{7, "����", &creature::getexperience},
{9, "����. ��.", &creature::getnextlevel},
};
assert_enum(parameter, NextLevelExperience);
getstr_enum(parameter);
getfid_enum(parameter);

int creature::get(parameter_s id) const {
	return (this->*parameter_data[id].get)();
}