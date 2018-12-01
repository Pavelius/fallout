#include "main.h"

const int timeout_base = 1100;

static bool is_move_action(int action)
{
	return action == ActionRun || action == ActionWalk;
}

moveable::moveable() : action(0), tick(0), orientation(0), timeout(timeout_base / 8), timestart(0)
{
}

sprite*	moveable::getsprite() const
{
	return 0;
}

rect moveable::getrect() const
{
	return{pos.x - 100, pos.y - 100, pos.x + 100, pos.y + 64};
}

void moveable::moveto(point goal, int runmode)
{
	this->goal = goal;
	if(runmode == -1)
		runmode = (game::getlongest(pos, goal) > 128) ? 1 : 0;
	setaction(runmode ? ActionRun : ActionWalk);
}

void moveable::stop()
{
	this->goal = pos;
	setaction(ActionStand);
}

void moveable::update()
{
	unsigned tm = draw::gettick();
	if(!timestart)
		timestart = tm;
	while(tm > timestart)
	{
		auto source = getsprite();
		if(!source)
			return;
		if(action == ActionStand)
		{
			auto pa = draw::getaction(source, action);
			auto pc = source->gcicle(getcicle());
			if(tick == pc->count - 1)
			{
				tick = 0;
				timestart += xrand(4, 18) * timeout_base;
			}
			else
			{
				settick(tick + 1);
				timestart += timeout;
			}
		}
		else if(action == ActionWalk || action == ActionRun)
		{
			if(game::getlongest(pos, goal) < 16)
				stop();
			else
			{
				settick(tick + 1);
				timestart += timeout;
				setorientation(game::getorientation(pos, goal));
				auto pa = draw::getaction(source, action);
				auto pc = source->gcicle(getcicle());
				auto pf = source->get(pc->start + tick);
				pos.x += pf.ox;
				pos.y += pf.oy;
			}
		}
		else
		{
			settick(tick + 1);
			timestart += timeout;
		}
	}
}

int moveable::getcicle() const
{
	return action * 6 + orientation;
}

void moveable::settick(int tick)
{
	if(this->tick != tick)
	{
		auto source = getsprite();
		if(!source)
			return;
		auto pc = source->gcicle(getcicle());
		if(!pc->count)
			return;
		this->tick = tick%pc->count;
	}
}

void moveable::setaction(int action)
{
	if(action < 0)
		action = 0;
	else if(action > LastAction)
		action = LastAction;
	if(this->action == action)
		return;
	auto source = getsprite();
	if(!source)
		return;
	auto pa = draw::getaction(source, action);
	if(pa->speed)
		timeout = timeout_base / pa->speed;
	else
		timeout = timeout_base / 10;
	this->action = action;
	timestart = draw::gettick();
	tick = -1;
	settick(0);
}

void moveable::setorientation(int orientation)
{
	if(orientation < 0)
		orientation = 0;
	else if(orientation > 5)
		orientation = 5;
	this->orientation = orientation;
}

void moveable::painting(point camera) const
{
	sprite* source = getsprite();
	if(!source)
		return;
	int cicle = getcicle();
	auto pc = source->gcicle(cicle);
	if(!pc->count)
		return;
	int frame = pc->start + tick;
	point pt = pos - camera;
	if(is_move_action(action))
	{
		auto pa = draw::getaction(source, cicle / 6);
		if(!pa)
			return;
		auto pf = source->get(frame);
		draw::image(
			pt.x - pf.sx / 2 + pa->offset[orientation].x,
			pt.y - pf.sy + pa->offset[orientation].y,
			source, frame, ImageNoOffset);
	}
	else
		draw::image(pt.x, pt.y, source, frame, 0);
}