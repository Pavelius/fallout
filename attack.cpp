#include "main.h"

bool attack_info::is(action_s id) const {
	return actions[0] == id
		|| actions[1] == id;
}

int attack_info::getap(action_s id) const {
	if(actions[0] == id)
		return ap[0];
	if(actions[1] == id)
		return ap[1];
	return 0;
}

int attack_info::getrange(action_s id) const {
	if(actions[0] == id)
		return range[0];
	if(actions[1] == id)
		return range[1];
	return 0;
}