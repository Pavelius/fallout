#include "main.h"

using namespace draw;

struct focusable_element {
	int				id;
	rect			rc;
	operator bool() const { return id != 0; }
};
static adat<command_info, 16>	cursor_actions;
static res::tokens			cursor_sprite = res::INTRFACE;
static int					cursor_frame = 267;
static bool					break_modal;
static int					break_result;
static int					current_focus;
static callback_proc		current_execute;
static focusable_element	elements[96];
static bool					keep_hot;
static hotinfo				keep_hot_value;
static unsigned				mouse_left_pressed;
static bool					mouse_left_pressed_action;
static focusable_element*	render_control;
static unsigned				timestamp;
void						apply_pallette_cicle(unsigned char* pal, unsigned stamp);
point						draw::camera;

#ifdef _DEBUG
static void paint_debug() {
	static bool show_mouse;
	draw::state push;
	setfont(res::FONT1);
	setcolor(ColorText);
	char temp[260];
	auto x = 10, y = 10;
	if(hot.key == (Ctrl + Alpha + 'D'))
		show_mouse = !show_mouse;
	if(show_mouse) {
		text(x, y, szprint(temp, zendof(temp), "%1i, %2i", hot.mouse.x, hot.mouse.y));
		y += texth();
	}
}
#endif

static void update_timestamp() {
	timestamp = clock();
	if(draw::palt)
		apply_pallette_cicle((unsigned char*)draw::palt, timestamp);
}

static command_info* paint_action_menu_loop() {
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return 0;
	rect rc;
	const int sxy = 40;
	auto fr = ps->ganim(cursor_frame, 0);
	auto pf = ps->get(fr);
	auto base_mouse_y = hot.mouse.y;
	draw::image(hot.mouse.x, hot.mouse.y, ps, fr, 0);
	rc.x1 = hot.mouse.x + pf.sx + pf.oy - 1;
	rc.y1 = hot.mouse.y + pf.sy + pf.oy - sxy / 2;
	rc.x2 = rc.x1 + sxy;
	rc.y2 = rc.y1 + sxy * cursor_actions.count;
	draw::screenshoot screen;
	cursorset cursor(res::NoRes);
	runstate runpush(false);
	auto current_index = 0;
	while(hot.pressed) {
		screen.restore();
#ifdef _DEBUG
		paint_debug();
#endif
		current_index = (hot.mouse.y - base_mouse_y) / (sxy / 2);
		if(current_index > (int)(cursor_actions.count - 1))
			current_index = cursor_actions.count - 1;
		if(current_index < 0)
			current_index = 0;
		auto x = rc.x1;
		auto y = rc.y1;
		for(unsigned i = 0; i<cursor_actions.count; i++) {
			auto id = cursor_actions[i].action;
			auto an = action_data[id].fid;
			if(an == -1)
				continue;
			if(i != current_index)
				an++;
			draw::image(x, y, ps, ps->ganim(an, 0), ImageNoOffset);
			y += sxy;
		}
		update_timestamp();
		hot.key = draw::rawinput();
	}
	return cursor_actions.data + current_index;
}

static bool paint_action_menu() {
	if(!mouse_left_pressed || !cursor_actions)
		return false;
	unsigned tm = clock();
	if((tm - mouse_left_pressed) < 1000)
		return false;
	mouse_left_pressed = 0;
	auto pa = paint_action_menu_loop();
	if(!pa)
		return false;
	hot.key = pa->action;
	hot.param = pa->param;
	hot.pressed = false;
	if(pa->proc)
		pa->proc();
	return true;
}

cursorset::cursorset(res::tokens r, int f) : r(cursor_sprite), f(cursor_frame) {
	set(r, f);
}

cursorset::~cursorset() {
	set(r, f);
}

void cursorset::set(res::tokens r, int f) {
	cursor_sprite = r;
	cursor_frame = f;
}

void draw::addaction(action_s action, callback_proc proc, int param) {
	cursor_actions.add({action, proc, param});
}

void draw::execute(void(*proc)(), int param) {
	current_execute = proc;
	hot.key = 0;
	hot.param = param;
}

void draw::execute(const hotinfo& value) {
	keep_hot = true;
	keep_hot_value = value;
	hot.key = InputUpdate;
}

static void clear_hot() {
	current_execute = 0;
	render_control = elements;
	cursor_actions.clear();
}

unsigned draw::getstamp() {
	return timestamp;
}

void draw::setfocus(int id) {
	current_focus = id;
}

int draw::getfocus() {
	return current_focus;
}

static void setfocus_proc() {
	setfocus(hot.param);
}

void draw::focusing(const rect& rc, int id) {
	if(!render_control
		|| render_control >= elements + sizeof(elements) / sizeof(elements[0]) - 1)
		render_control = elements;
	render_control[0].id = id;
	render_control[0].rc = rc;
	render_control[1].id = 0;
	render_control++;
	if(hot.key==MouseLeft && area(rc) == AreaHilitedPressed) {
		if(getfocus() != id)
			execute(setfocus_proc, id);
	}
}

static focusable_element* getby(int id) {
	if(!id)
		return 0;
	for(auto& e : elements) {
		if(!e)
			return 0;
		if(e.id == id)
			return &e;
	}
	return 0;
}

static focusable_element* getfirst() {
	for(auto& e : elements) {
		if(!e)
			return 0;
		return &e;
	}
	return 0;
}

static focusable_element* getlast() {
	auto p = elements;
	for(auto& e : elements) {
		if(!e)
			break;
		p = &e;
	}
	return p;
}

int draw::getnext(int id, int key) {
	if(!key)
		return id;
	auto pc = getby(id);
	if(!pc)
		pc = getfirst();
	if(!pc)
		return 0;
	auto pe = pc;
	auto pl = getlast();
	int inc = 1;
	if(key == KeyLeft || key == KeyUp || key == (KeyTab | Shift))
		inc = -1;
	while(true) {
		pc += inc;
		if(pc > pl)
			pc = elements;
		else if(pc < elements)
			pc = pl;
		if(pe == pc)
			return pe->id;
		switch(key) {
		case KeyRight:
			if(pe->rc.y1 >= pc->rc.y1
				&& pe->rc.y1 <= pc->rc.y2
				&& pe->rc.x1 < pc->rc.x1)
				return pc->id;
			break;
		case KeyLeft:
			if(pe->rc.y1 >= pc->rc.y1
				&& pe->rc.y1 <= pc->rc.y2
				&& pe->rc.x1 > pc->rc.x1)
				return pc->id;
			break;
		case KeyDown:
			if(pc->rc.y1 >= pe->rc.y2)
				return pc->id;
			break;
		case KeyUp:
			if(pc->rc.y2 <= pe->rc.y1)
				return pc->id;
			break;
		default:
			return pc->id;
		}
	}
}

int draw::getresult() {
	return break_result;
}

void draw::breakmodal(int result) {
	break_modal = true;
	break_result = result;
}

bool draw::ismodal() {
	clear_hot();
	if(!break_modal)
		return true;
	break_modal = false;
	return false;
}

static void exit_game() {
	exit(0);
}

void draw::domodal() {
	update_timestamp();
	if(current_execute) {
		auto proc = current_execute;
		clear_hot();
		proc();
		clear_hot();
		hot.key = InputUpdate;
		return;
	}
	if(paint_action_menu())
		return;
#ifdef _DEBUG
	paint_debug();
#endif
	if(cursor_sprite == res::INVEN) {
		auto ps = gres(cursor_sprite);
		if(ps) {
			int frame = ps->ganim(cursor_frame, getstamp() / 100);
			auto& fr = ps->get(frame);
			image(hot.mouse.x, hot.mouse.y + fr.sy/2, ps, frame, 0, 0xFF);
		}
	} else
		image(hot.mouse.x, hot.mouse.y, cursor_sprite, cursor_frame);
	if(hot.key == InputUpdate && keep_hot) {
		keep_hot = false;
		hot = keep_hot_value;
	} else {
		hot.key = draw::rawinput();
		if(!hot.key)
			exit_game();
	}
	if(hot.key == MouseLeft && hot.pressed)
		mouse_left_pressed = clock();
	else if(mouse_left_pressed && !hot.pressed)
		mouse_left_pressed = 0;
}

struct layout_info {
	layout_info*		previous;
	callback_proc		proc;
	callback_proc		def_proc;
	static layout_info*	current;
	layout_info(void(*proc)()) : proc(proc), def_proc(proc) {
		previous = current;
		current = this;
	}
	~layout_info() {
		current = previous;
	}
};
layout_info* layout_info::current;

callback_proc draw::getlayout() {
	return layout_info::current ? layout_info::current->def_proc : 0;
}

void draw::setlayout(callback_proc proc) {
	layout_info e(proc);
	while(e.proc)
		e.proc();
}

void draw::setpage(callback_proc proc) {
	if(!layout_info::current)
		return;
	layout_info::current->proc = proc;
	breakmodal(1);
}

void draw::setpagedef(callback_proc proc) {
	if(!layout_info::current)
		return;
	layout_info::current->def_proc = proc;
}

void draw::setpage() {
	if(!layout_info::current)
		return;
	layout_info::current->proc = layout_info::current->def_proc;
	breakmodal(1);
}

bool draw::isnext(callback_proc proc) {
	if(!layout_info::current)
		return false;
	return layout_info::current->proc == proc;
}