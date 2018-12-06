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