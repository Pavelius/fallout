#include "main.h"

using namespace draw;

static const int		line_height = 4;
static int				item_info;

static int compare(const void* p1, const void* p2) {
	auto i1 = **((item**)p1);
	auto i2 = **((item**)p2);
	auto t1 = i1.getname();
	auto t2 = i2.getname();
	return strcmp(t1, t2);
}

static void preview(int x, int y, const item& armor, gender_s gender, const item& weapon) {
	res::tokens icn = armor.getdress(gender);
	if(icn == res::NoRes) {
		if(gender == Male)
			icn = res::HMWARR;
		else
			icn = res::HFPRIM;
	}
	auto ps = gres(icn);
	if(!ps)
		return;
	auto or = (getstamp() / 300) % 6;
	auto wp = 0;// item::get(bsget(rec, Weapon), Weapon);
	auto cl = actor::byweapon(ActionStand, wp) * 6 + or ;
	auto pa = draw::getaction(ps, cl / 6);
	if(!pa)
		return;
	auto fr = ps->ganim(cl, 0);
	auto pf = ps->get(fr);
	draw::state push;
	draw::setclip({x - 40, y - 100, x + 40, y + 30});
	draw::image(x, y, ps, fr, 0);
}

//static void iteminfo(int x, int y, int width, int player, int id) {
//	damagei damage;
//	char temp[128];
//	int itm = bsget(player, id);
//	int type = item::get(itm, Type);
//	int melee = item::get(itm, MeleeDamage);
//	if(!itm) {
//		type = Weapon;
//		melee = 1;
//		draw::text(x, y, szt("No item", "Нет предмета"));
//	} else
//		draw::text(x, y, item::getname(itm));
//	y += draw::texth();
//	switch(type) {
//	case Weapon:
//		game::getdamage(player, id, damage);
//		szprint(temp, "%1: %2i-%3i", szt("Dam.", "Повр."), damage.min, damage.max);
//		draw::text(x, y, temp);
//		if(!melee) {
//			szprint(temp, "%1: %2i", szt("Ran.", "Длн."), item::get(itm, Range));
//			draw::text(x + width - draw::textw(temp), y, temp);
//			y += draw::texth();
//			szprint(temp, "%1: %2i/%3i", szt("Shoot", "Заряд"), item::get(itm, Ammo), item::get(itm, AmmoCapacity));
//			draw::text(x, y, temp);
//		}
//		break;
//	}
//}

struct dragdrop {
	enum type_s : unsigned char {
		NoTarget,
		Item, Wearable,
	};
	item		element, *source, *target, *hilite;
	type_s		type;
	wearable*	target_player;
	dragdrop() { clear(); }
	~dragdrop() { drop(NoTarget); }
	constexpr operator bool() const { return source != 0; }
	void start() {
		draw::execute(start_proc, (int)this);
	}
	void domodal() {
		if(!source)
			return;
		if((hot.key == MouseLeft || hot.key==MouseRight) && !hot.pressed)
			drop(type);
		type = NoTarget;
		target = 0;
		hilite = 0;
		target_player = 0;
	}
private:
	void clear() {
		memset(this, 0, sizeof(*this));
	}
	void drop(type_s type) {
		auto all_correct = false;
		switch(type) {
		case Item:
			if(target) {
				auto target_element = *target;
				*target = element;
				if(target_element)
					*source = target_element;
				all_correct = true;
			}
			break;
		case Wearable:
			if(target_player) {
				target_player->additem(element);
				all_correct = true;
			}
			break;
		}
		if(!all_correct) {
			if(source)
				*source = element;
		}
		clear();
	}
	static void start_proc() {
		auto p = (dragdrop*)hot.param;
		if(p->hilite) {
			p->source = p->hilite;
			p->element = *p->source;
			p->source->clear();
		}
	}
};

static int itemview(int x, int y, int sx, int sy, item& it, item::proctest test_proc, dragdrop& di) {
	const int dw = 2;
	rect rc = {x, y, x + sx, y + sy};
	auto a = area(rc);
	if(di && (a==AreaHilited || a==AreaHilitedPressed)) {
		if(!test_proc || (di.element.*test_proc)()) {
			di.type = dragdrop::Item;
			di.target = &it;
		}
	}
	if(!it)
		return 0;
	iteminv(rc.x1 + dw, rc.y1 + dw - 1, rc.width() - dw * 2, rc.height() - dw * 2, it, true);
	auto c1 = it.getcount();
	if(c1 > 1) {
		char temp[16];
		draw::state push;
		draw::setcolor(ColorCheck);
		draw::setfont(res::FONT1);
		szprint(temp, zendof(temp), "x%1i", c1);
		draw::text(rc.x1 + 4, rc.y1 + 4, temp);
	}
	if(a == AreaHilited || a == AreaHilitedPressed) {
		di.hilite = &it;
		if(hot.key == MouseLeft && a == AreaHilitedPressed)
			di.start();
	}
	return rc.height();
}

static void backpack(int x, int y, int width, int height, wearable& player, dragdrop& di) {
	static int first;
	rect rc = {x, y, x + width, y + height};
	auto a = area(rc);
	if(di && (a == AreaHilited || a == AreaHilitedPressed)) {
		di.type = dragdrop::Wearable;
		di.target_player = &player;
	}
	draw::state push;
	draw::setfont(res::FONT1);
	draw::setcolor(ColorText);
	draw::setclip({rc.x1, rc.y1, rc.x1 + 120, rc.y2});
	//draw::rectb(rc);
	const int element_height = 50;
	int count_per_page = height / element_height;
	item* source[1025];
	auto result = player.select(source);
	if(!result.count)
		return;
	qsort(result.data, result.count, sizeof(result.data[0]), compare);
	// Mouse input
	if(a== AreaHilited) {
		switch(hot.key) {
		case MouseWheelDown:
			first++;
			break;
		case MouseWheelUp:
			first--;
			break;
		}
	}
	// correction
	if(first + count_per_page >= (int)result.count)
		first = result.count - count_per_page;
	if(first < 0)
		first = 0;
	// Up button
	if(first == 0)
		draw::image(x + 80, y - 4, res::INTRFACE, 53, ImageNoOffset);
	else if(draw::buttonf(x + 80, y - 4, 49, 50, 0))
		first--;
	// Down button
	if(first + count_per_page >= (int)result.count)
		draw::image(x + 80, y + 20, res::INTRFACE, 54, ImageNoOffset);
	else if(draw::buttonf(x + 80, y + 20, 51, 52, 0))
		first++;
	for(auto i = first; i < (int)result.count; i++) {
		if(y >= rc.y2)
			break;
		if(itemview(x, y, width, element_height - 1, *result.data[i], 0, di))
			y += element_height;
	}
}

//static void itemtext(int x, int y, int width, int height, int rec) {
//	char temp[128];
//	draw::state push;
//	draw::setfont(res::FONT1);
//	draw::setcolor(ColorText);
//	rect rc = {x, y, x + width, y + height};
//	const char* name = item::getname(rec);
//	if(name && name[0]) {
//		rc.y1 += draw::text(rc, name) + line_height - 1;
//		draw::line(rc.x1 - 4, rc.y1, rc.x2, rc.y1);
//		rc.y1 += line_height + 4;
//	}
//	rc.y1 += draw::text(rc, item::getdescription(rec));
//	if(item::get(rec, Type) == Weapon) {
//		szprint(temp, szt("Minmal ST: %1i", "Минимальная СЛ: %1i"), item::get(rec, Strenght));
//		rc.y1 += draw::text(rc, temp);
//	}
//	szprint(temp, szt("It weight %1i pounds", "Это весит %1i фунта."), item::get(rec, CarryWeight));
//	rc.y1 += draw::text(rc, temp);
//}

//static void chartext(int x, int y, int width, int height, int rec) {
//	char temp[128];
//	static int values[] = {HP, AC,
//		DamageNormal, DamageLaser, DamageFire, DamagePlasma, DamageExplosive};
//	rect rc = {x, y, x + width, y + height};
//	draw::state push;
//	draw::setfont(res::FONT1);
//	draw::setcolor(ColorText);
//	draw::text(rc.x1, rc.y1, game::getname(temp, rec));
//	rc.y1 += draw::texth() + line_height - 1;
//	draw::line(rc.x1, rc.y1, rc.x2, rc.y1);
//	rc.y1 += line_height;
//	int x1 = rc.x1; int y1 = rc.y1;
//	for(int id = FirstStat; id <= LastStat; id++, y1 += draw::texth()) {
//		draw::text(x1, y1, game::getnameshort(id));
//		sznum(temp, game::get(rec, id));
//		draw::text(x1 + 32 - draw::textw(temp), y1, temp);
//	}
//	x1 = rc.x1 + 44; y1 = rc.y1;
//	for(int id : values) {
//		draw::text(x1, y1, game::getnameshort(id));
//		switch(id) {
//		case HP:
//			szprint(temp, "%1i/%2i", game::get(rec, HP), game::get(rec, MaxHP));
//			break;
//		case DamageNormal:
//		case DamageLaser:
//		case DamageFire:
//		case DamagePlasma:
//		case DamageExplosive:
//			szprint(temp, "%1i/%2i%%", game::get(rec, id), game::get(rec, id - FirstDamageType + FirstDamageResistance));
//			break;
//		default:
//			sznum(temp, game::get(rec, id));
//			break;
//		}
//		draw::text(x1 + 100 - draw::textw(temp), y1, temp);
//		y1 += draw::texth();
//	}
//	rc.y1 = y1 + line_height - 1;
//	draw::line(rc.x1, rc.y1, rc.x2, rc.y1);
//	rc.y1 += line_height - 1;
//	iteminfo(rc.x1, rc.y1, rc.width(), rec, Weapon);
//	rc.y1 += draw::texth() * 3 + line_height - 1;
//	draw::line(rc.x1, rc.y1, rc.x2, rc.y1);
//	rc.y1 += line_height - 1;
//	iteminfo(rc.x1, rc.y1, rc.width(), rec, WeaponSecondonary);
//	rc.y1 += draw::texth() * 3 + line_height - 1;
//	draw::line(rc.x1, rc.y1, rc.x2, rc.y1);
//	rc.y1 += line_height - 1;
//	szprint(temp, "Общий вес %1i/%2i", game::getwearsweight(rec), game::get(rec, CarryWeight));
//	draw::text(rc.x1 + (rc.width() - draw::textw(temp)) / 2, rc.y1, temp);
//}

void creature::inventory() {
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return;
	bool info_mode = false;
	auto fr = ps->ganim(48, 0);
	item_info = 0;
	draw::screenshoot screen;
	cursorset cursor(res::INTRFACE, 286);
	dragdrop di;
	while(ismodal()) {
		screen.restore();
		auto x = (draw::getwidth() - ps->get(fr).sx) / 2;
		auto y = imax(0, (draw::getheight() - ps->get(fr).sy - 100) / 2);
		image(x, y, ps, fr, ImageNoOffset);
		preview(x + 206, y + 116, getarmor(), getgender(), getweapon());
		//if(info_mode && item_info)
		//	itemtext(x + 298, y + 48, 144, 180, item_info);
		//else
		//	chartext(x + 298, y + 48, 144, 180, player);
		itemview(x + 152, y + 184, 92, 60, armor, &item::isarmor, di);
		itemview(x + 152, y + 287, 92, 60, weapon[0], &item::isweapon, di);
		itemview(x + 245, y + 287, 92, 60, weapon[1], &item::isweapon, di);
		backpack(x + 42, y + 40, 70, 300, *this, di);
		//if(info_mode && item_hilite) {
		//	int actions[16]; actions[0] = 0;
		//	zcat(actions, (int)Look);
		//	zcat(actions, (int)Drop);
		//	if(item::get(*item_hilite, Ammo))
		//		zcat(actions, (int)Unload);
		//	draw::setaction(actions);
		//}
		if(buttonf(x + 432, y + 322, 299, 300, 0)
			|| (hot.key == KeyEscape || hot.key == (Alpha + 'I')))
			buttoncancel();
		cursor.set(res::INTRFACE, 286);
		if(di)
			cursor.set(res::INVEN, di.element.get(FrameInventory));
		domodal();
		di.domodal();
		switch(hot.key) {
		case MouseRight:
			if(!hot.pressed)
				break;
			info_mode = !info_mode;
			if(info_mode)
				cursor.set(res::INTRFACE, 250);
			else
				cursor.set(res::INTRFACE, 286);
			break;
			//case Armor:
			//case Weapon:
			//case WeaponSecondonary:
			//case Invertory:
			//	if(!info_mode) {
			//		draw::drag::begin(id, 0);
			//		if(drag_item && *drag_item)
			//			cursor.set(res::INVEN, item::get(*drag_item, Invertory));
			//	}
			//	break;
			//case Look:
			//	if(item_hilite)
			//		item_info = *item_hilite;
			//	break;
			//case Unload:
			//	if(item_hilite) {
			//		int ammo = item::get(*item_hilite, AmmoType);
			//		int count = item::get(*item_hilite, Ammo);
			//		if(ammo == -1)
			//			break; // Bug!
			//		game::additem(player, item::create(ammo, count));
			//		*item_hilite = item::set(*item_hilite, Ammo, 0);
			//	}
			//	break;
			//case Drop:
			//	if(item_hilite) {
			//		game::dropitem(bsget(player, Position), *item_hilite);
			//		*item_hilite = 0;
			//	}
			//	break;
			//case MouseLeft:
			//	if(hot.pressed)
			//		break;
			//	if(info_mode)
			//		break;
			//	break;
		}
	}
}