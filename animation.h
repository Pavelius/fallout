#include "draw.h"

#pragma once

namespace draw {
struct animation {
	point				position;
	const sprite*		ps;
	short unsigned		fi;
	short				zorder;
	unsigned			flags;
	void*				param;
	bool				intersect(const rect& screen) const { return ps->get(fi).getrect(position.x, position.y, 0).intersect(screen); }
	rect				getrect() const { return ps->get(fi).getrect(position.x, position.y, 0); }
	void				painting(point camera) const;
	void				setup(point position, const sprite* ps, short unsigned fi);
	static void			sort(animation* source, unsigned count);
};
}