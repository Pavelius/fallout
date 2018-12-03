#include "util.h"

const char* msg_info::get(int id) const {
	for(auto p = this; p->id; p++)
		if(p->id == id)
			return p->name;
	return "";
}