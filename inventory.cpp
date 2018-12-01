#include "main.h"

static const int	line_height = 4;
static int*			drag_item;
static int*			drag_target;
static int			drag_target_id;
static int			item_info;
static int*			item_hilite;

static int compare(const void* p1, const void* p2) {
	unsigned i1 = **((unsigned**)p1);
	unsigned i2 = **((unsigned**)p2);
	int t1 = item::get(i1, Type);
	int t2 = item::get(i2, Type);
	if(t2 != t1)
		return t1 - t2;
	return strcmp(item::getname(i1), item::getname(i2));
}

static void preview(int x, int y, int rec) {
	res::tokens icn = draw::gethuman(bsget(rec, Armor), bsget(rec, Gender));
	if(icn == res::NoRes)
		return;
	auto ps = gres(icn);
	if(!ps)
		return;
	auto or = (clock() / 300) % 6;
	auto wp = item::get(bsget(rec, Weapon), Weapon);
	auto cl = draw::byweapon(ActionStand, wp) * 6 + or ;
	auto pa = draw::getaction(ps, cl / 6);
	if(!pa)
		return;
	auto fr = ps->ganim(cl, 0);
	auto pf = ps->get(fr);
	draw::state push;
	draw::setclip({x - 40, y - 100, x + 40, y + 30});
	draw::image(x, y, ps, fr, 0);
}

static void iteminfo(int x, int y, int width, int player, int id) {
	damagei damage;
	char temp[128];
	int itm = bsget(player, id);
	int type = item::get(itm, Type);
	int melee = item::get(itm, MeleeDamage);
	if(!itm) {
		type = Weapon;
		melee = 1;
		draw::text(x, y, szt("No item", "Нет предмета"));
	} else
		draw::text(x, y, item::getname(itm));
	y += draw::texth();
	switch(type) {
	case Weapon:
		game::getdamage(player, id, damage);
		szprint(temp, "%1: %2i-%3i", szt("Dam.", "Повр."), damage.min, damage.max);
		draw::text(x, y, temp);
		if(!melee) {
			szprint(temp, "%1: %2i", szt("Ran.", "Длн."), item::get(itm, Range));
			draw::text(x + width - draw::textw(temp), y, temp);
			y += draw::texth();
			szprint(temp, "%1: %2i/%3i", szt("Shoot", "Заряд"), item::get(itm, Ammo), item::get(itm, AmmoCapacity));
			draw::text(x, y, temp);
		}
		break;
	}
}

static int itemview(int x, int y, int sx, int sy, int id, int* it) {
	rect rc = {x, y, x + sx, y + sy};
	if(!it)
		return 0;
	if(drag_item && hot.mouse.in(rc)) {
		drag_target = it;
		drag_target_id = id;
	}
	if(draw::drag::active(id, 0) && drag_item == it)
		return 0;
	if(*it == 0)
		return 0;
	auto a = draw::area(rc);
	draw::iteminv(rc.x1 + 3, rc.y1 + 3, rc.width() - 6, rc.height() - 6, *it, true);
	unsigned c1 = item::get(*it, Count);
	if(c1 > 1) {
		char temp[16];
		draw::state push;
		draw::setcolor(ColorCheck);
		draw::setfont(res::FONT1);
		szprint(temp, "x%1i", c1);
		draw::text(rc.x1 + 4, rc.y1 + 4, temp);
	}
	if(a == AreaHilited || a == AreaHilitedPressed) {
		if(hot.key == MouseLeft && hot.pressed) {
			drag_item = it;
			draw::execute(id);
		}
		item_hilite = it;
	}
	return rc.height();
}

static void backpack(int x, int y, int width, int height, int player) {
	static int first;
	rect rc = {x, y, x + width, y + height};
	draw::state push;
	draw::setfont(res::FONT1);
	draw::setcolor(ColorText);
	draw::setclip({rc.x1, rc.y1, rc.x1 + 120, rc.y2});
	//draw::rectb(rc);
	const int element_height = 50;
	int count_per_page = height / element_height;
	int* source[1025];
	int count = game::getwears(source, player);
	if(!count)
		return;
	qsort(source, count, sizeof(source[0]), compare);
	// Mouse input
	if(draw::area(rc) == AreaHilited) {
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
	if(first + count_per_page >= count)
		first = count - count_per_page;
	if(first < 0)
		first = 0;
	// Up button
	if(first == 0)
		draw::image(x + 80, y - 4, res::INTRFACE, 53, ImageNoOffset);
	else if(draw::buttonf(x + 80, y - 4, 49, 50, 0))
		first--;
	// Down button
	if(first + count_per_page >= count)
		draw::image(x + 80, y + 20, res::INTRFACE, 54, ImageNoOffset);
	else if(draw::buttonf(x + 80, y + 20, 51, 52, 0))
		first++;
	for(int i = first; i < count; i++) {
		if(y >= rc.y2)
			break;
		if(itemview(x, y, width, element_height - 1, Invertory, source[i]))
			y += element_height;
	}
}

static void itemtext(int x, int y, int width, int height, int rec) {
	char temp[128];
	draw::state push;
	draw::setfont(res::FONT1);
	draw::setcolor(ColorText);
	rect rc = {x, y, x + width, y + height};
	const char* name = item::getname(rec);
	if(name && name[0]) {
		rc.y1 += draw::text(rc, name) + line_height - 1;
		draw::line(rc.x1 - 4, rc.y1, rc.x2, rc.y1);
		rc.y1 += line_height + 4;
	}
	rc.y1 += draw::text(rc, item::getdescription(rec));
	if(item::get(rec, Type) == Weapon) {
		szprint(temp, szt("Minmal ST: %1i", "Минимальная СЛ: %1i"), item::get(rec, Strenght));
		rc.y1 += draw::text(rc, temp);
	}
	szprint(temp, szt("It weight %1i pounds", "Это весит %1i фунта."), item::get(rec, CarryWeight));
	rc.y1 += draw::text(rc, temp);
}

static void chartext(int x, int y, int width, int height, int rec) {
	char temp[128];
	static int values[] = {HP, AC,
		DamageNormal, DamageLaser, DamageFire, DamagePlasma, DamageExplosive};
	rect rc = {x, y, x + width, y + height};
	draw::state push;
	draw::setfont(res::FONT1);
	draw::setcolor(ColorText);
	draw::text(rc.x1, rc.y1, game::getname(temp, rec));
	rc.y1 += draw::texth() + line_height - 1;
	draw::line(rc.x1, rc.y1, rc.x2, rc.y1);
	rc.y1 += line_height;
	int x1 = rc.x1; int y1 = rc.y1;
	for(int id = FirstStat; id <= LastStat; id++, y1 += draw::texth()) {
		draw::text(x1, y1, game::getnameshort(id));
		sznum(temp, game::get(rec, id));
		draw::text(x1 + 32 - draw::textw(temp), y1, temp);
	}
	x1 = rc.x1 + 44; y1 = rc.y1;
	for(int id : values) {
		draw::text(x1, y1, game::getnameshort(id));
		switch(id) {
		case HP:
			szprint(temp, "%1i/%2i", game::get(rec, HP), game::get(rec, MaxHP));
			break;
		case DamageNormal:
		case DamageLaser:
		case DamageFire:
		case DamagePlasma:
		case DamageExplosive:
			szprint(temp, "%1i/%2i%%", game::get(rec, id), game::get(rec, id - FirstDamageType + FirstDamageResistance));
			break;
		default:
			sznum(temp, game::get(rec, id));
			break;
		}
		draw::text(x1 + 100 - draw::textw(temp), y1, temp);
		y1 += draw::texth();
	}
	rc.y1 = y1 + line_height - 1;
	draw::line(rc.x1, rc.y1, rc.x2, rc.y1);
	rc.y1 += line_height - 1;
	iteminfo(rc.x1, rc.y1, rc.width(), rec, Weapon);
	rc.y1 += draw::texth() * 3 + line_height - 1;
	draw::line(rc.x1, rc.y1, rc.x2, rc.y1);
	rc.y1 += line_height - 1;
	iteminfo(rc.x1, rc.y1, rc.width(), rec, WeaponSecondonary);
	rc.y1 += draw::texth() * 3 + line_height - 1;
	draw::line(rc.x1, rc.y1, rc.x2, rc.y1);
	rc.y1 += line_height - 1;
	szprint(temp, "Общий вес %1i/%2i", game::getwearsweight(rec), game::get(rec, CarryWeight));
	draw::text(rc.x1 + (rc.width() - draw::textw(temp)) / 2, rc.y1, temp);
}

int show::inventory(int player) {
	auto ps = gres(res::INTRFACE);
	if(!ps)
		return 0;
	bool info_mode = false;
	auto fr = ps->ganim(48, 0);
	int x = (draw::getwidth() - ps->get(fr).sx) / 2;
	int y = imax(0, (draw::getheight() - ps->get(fr).sy - 100) / 2);
	item_info = 0;
	draw::screenshoot screen;
	cursorset cursor(res::INTRFACE, 286);
	while(true) {
		item_hilite = 0;
		screen.restore();
		draw::image(x, y, ps, fr, ImageNoOffset);
		preview(x + 206, y + 116, player);
		if(info_mode && item_info)
			itemtext(x + 298, y + 48, 144, 180, item_info);
		else
			chartext(x + 298, y + 48, 144, 180, player);
		itemview(x + 152, y + 184, 92, 60, Armor, (int*)bsptr(player, Armor));
		itemview(x + 152, y + 287, 92, 60, Weapon, (int*)bsptr(player, Weapon));
		itemview(x + 245, y + 287, 92, 60, WeaponSecondonary, (int*)bsptr(player, WeaponSecondonary));
		backpack(x + 42, y + 40, 70, 300, player);
		if(info_mode && item_hilite) {
			int actions[16]; actions[0] = 0;
			zcat(actions, (int)Look);
			zcat(actions, (int)Drop);
			if(item::get(*item_hilite, Ammo))
				zcat(actions, (int)Unload);
			draw::setaction(actions);
		}
		if(draw::buttonf(x + 432, y + 322, 299, 300, 0))
			draw::execute(Cancel);
		int id = draw::input();
		switch(id) {
		case 0:
			return 0;
		case Cancel:
		case Alpha + 'I':
		case KeyEscape:
			return Cancel;
		case MouseRight:
			if(!hot.pressed)
				break;
			info_mode = !info_mode;
			if(info_mode)
				cursor.set(res::INTRFACE, 250);
			else
				cursor.set(res::INTRFACE, 286);
			break;
		case Armor:
		case Weapon:
		case WeaponSecondonary:
		case Invertory:
			if(!info_mode) {
				draw::drag::begin(id, 0);
				if(drag_item && *drag_item)
					cursor.set(res::INVEN, item::get(*drag_item, Invertory));
			}
			break;
		case Look:
			if(item_hilite)
				item_info = *item_hilite;
			break;
		case Unload:
			if(item_hilite) {
				int ammo = item::get(*item_hilite, AmmoType);
				int count = item::get(*item_hilite, Ammo);
				if(ammo == -1)
					break; // Bug!
				game::additem(player, item::create(ammo, count));
				*item_hilite = item::set(*item_hilite, Ammo, 0);
			}
			break;
		case Drop:
			if(item_hilite) {
				game::dropitem(bsget(player, Position), *item_hilite);
				*item_hilite = 0;
			}
			break;
		case MouseLeft:
			if(hot.pressed)
				break;
			if(info_mode)
				break;
			if(drag_item) {
				if(drag_target_id) {
					int drag_target_type = drag_target_id;
					if(drag_target_type == WeaponSecondonary)
						drag_target_type = Weapon;
					int drag_item_type = draw::drag::id;
					if(drag_item_type == WeaponSecondonary)
						drag_item_type = Weapon;
					if(drag_target_type == Invertory) {
						if(drag_item_type != Invertory) {
							game::additem(player, *drag_item);
							*drag_item = 0;
						}
					} else if(drag_item_type == Invertory) {
						if(drag_target_type == item::get(*drag_item, Type)) {
							int rec = *drag_target;
							*drag_target = *drag_item;
							*drag_item = 0;
							game::additem(player, rec);
						}
					} else if(drag_item_type == drag_target_type && drag_target)
						iswap(*drag_target, *drag_item);
				}
				drag_item = 0;
				drag_target = 0;
				drag_target_id = 0;
				cursor.set(res::INTRFACE, 286);
			}
			break;
		}
	}
}