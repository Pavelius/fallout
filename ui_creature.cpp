#include "main.h"

using namespace draw;

static const char* score_text[3] = {"очко", "очка", "очков"};

static char* szpercent(char* result, const char* result_maximum, int value) {
	return szprint(result, result_maximum, "%1i%%", value);
}

static int gtv(int value) {
	if(value <= 1)
		return 0;
	else if(value <= 2)
		return 1;
	else
		return 2;
}

static const char* szscore(int value) {
	return score_text[gtv(value)];
}

void creature::decrease(variant e, int& points) {
	switch(e.type) {
	case Abilities:
		if(stats[e.ability] > 1) {
			stats[e.ability]--;
			points++;
		}
		break;
	}
}

void creature::increase(variant e, int& points) {
	if(!points)
		return;
	switch(e.type) {
	case Abilities:
		if(stats[e.ability] < 10) {
			stats[e.ability]++;
			points--;
		}
		break;
	}
}

void creature::mark(variant e, int& points) {
	switch(e.type) {
	case Perks:
		if(is(e.perk)) {
			remove(e.perk);
			points++;
		} else if(points) {
			set(e.perk);
			points--;
		}
		break;
	case Skills:
		if(is(e.skill)) {
			remove(e.skill);
			points++;
		} else if(points) {
			set(e.skill);
			points--;
		}
		break;
	}
}

struct cmd_creature : runable {

	creature*	player;
	variant		element;
	bool		checked;
	int*		points;
	static cmd_creature	current;

	constexpr cmd_creature() : player(0), element(), checked(false), points(0) {}

	int	getid() const override {
		return element;
	}

	bool ischecked() const override {
		return false;
	}

	static void decrease_proc() {
		if(!current.player || !current.element || !current.points)
			return;
		setfocus(current.element);
		current.player->decrease(current.element, *current.points);
	}

	static void increase_proc() {
		if(!current.player || !current.element || !current.points)
			return;
		setfocus(current.element);
		current.player->increase(current.element, *current.points);
	}

	static void execute_proc() {
		if(!current.player || !current.element)
			return;
		setfocus(current.element);
		current.player->mark(current.element, *current.points);
	}

	void execute() const {
		current = *this;
		draw::execute(execute_proc);
	}

	void modify(int count) {
		current = *this;
		if(count>0)
			draw::execute(increase_proc, 1);
		if(count<0)
			draw::execute(decrease_proc, -1);
	}

};
cmd_creature cmd_creature::current;

bool creature::choose_gender(int x, int y) {
	const int cmd_base = 10;
	draw::screenshoot screen;
	while(ismodal()) {
		screen.restore();
		draw::image(x, y, res::INTRFACE, 208, ImageNoOffset);
		draw::image(x + 15, y + 42, res::INTRFACE, 209, ImageNoOffset);
		if(draw::buttonf(x + 24, y + 2, 212, 213, (gender == Male) ? Checked : 0, false))
			draw::breakmodal(Male + cmd_base);
		if(draw::buttonf(x + 74, y + 2, 210, 211, (gender == Female) ? Checked : 0, false))
			draw::breakmodal(Female + cmd_base);
		draw::button(x + 25, y + 45, 90, cmd(buttoncancel), "Отмена");
		domodal();
		switch(hot.key) {
		case KeyEscape:
			return false;
		case Alpha + 'M':
			breakmodal(Male + cmd_base);
			break;
		case Alpha + 'F':
			breakmodal(Female + cmd_base);
			break;
		}
	}
	switch(getresult()) {
	case Male + cmd_base:
		gender = Male;
		return true;
	case Female + cmd_base:
		gender = Female;
		return true;
	}
	return false;
}

static void gender_proc() {
	((creature*)hot.param)->choose_gender(188, 1);
}

int creature::render_stats(int x, int y, int width, aref<variant> elements, bool show_maximum_only) const {
	auto y0 = y;
	char temp[260];
	for(auto id : elements) {
		const char* pr;
		switch(id.type) {
		case Parameters:
			draw::label(x, y, id, parameter_data[id.parameter].name_short, false, false);
			pr = "%1i";
			if(parameter_data[id.parameter].percent)
				pr = "%1i%%";
			if(parameter_data[id.parameter].maximum && !show_maximum_only)
				pr = "%1i/%2i";
			if(show_maximum_only)
				szprint(temp, zendof(temp), pr, parameter_data[id.parameter].getmax ? getmax(id.parameter) : get(id.parameter));
			else
				szprint(temp, zendof(temp), pr, get(id.parameter), getmax(id.parameter));
			draw::text(x + width - draw::textw(temp), y, temp);
			break;
		case Damages:
			draw::label(x, y, id, damage_data[id.damage].name_short, false, false);
			szprint(temp, zendof(temp), "%1i%%", getresistance(id.damage));
			draw::text(x + width - draw::textw(temp), y, temp);
			break;
		case Wounds:
			draw::label(x, y, id, getstr(id), false, !is(id.wound));
			break;
		case Illness:
			draw::label(x, y, id, getstr(id), false, get(id.illness) == 0);
			break;
		}
		y += 13;
	}
	return y - y0;
}

static void open_options() {}

struct cmdk : runable {
	cmdk() {}
	cmdk(creature* player, int traits_points, int tag_skill_points, int ability_points) : player(player),
		traits_points(traits_points), tag_skill_points(tag_skill_points), ability_points(ability_points) {}
	int	getid() const { return (int)player; }
	static void execute_proc() {
		if(current.ability_points > 0) {
			dlgerr("", "Распределите еще %1i %2 характеристик.", current.ability_points, szscore(current.ability_points));
			return;
		}
		if(current.tag_skill_points) {
			dlgerr("", "Распределите еще %1i %2 меток.", current.tag_skill_points, szscore(current.tag_skill_points));
			return;
		}
		breakmodal(1);
	}
	void execute() const {
		current = *this;
		draw::execute(execute_proc);
	}
private:
	creature * player;
	int			traits_points, tag_skill_points, ability_points;
	static cmdk	current;
};
cmdk cmdk::current;

bool creature::choose_stats(int traits_points, int tag_skill_points, int ability_points) {
	char temp[260];
	draw::state push;
	draw::setcolor(ColorText);
	draw::setfont(res::FONT1);
	auto show_ability = (ability_points > 0);
	auto show_tag = (traits_points > 0);
	auto show_maximum_only = true;
	setfocus(variant(Strenght));
	while(ismodal()) {
		draw::background(169);
		// Заполнение команды
		cmd_creature ev;
		ev.player = this;
		// Кнопки игрока
		buttonp(13, 0, 185, 184, cmd(buttonok), name ? name : "Некто");
		buttonp(157, 0, 188, 189, cmd(buttonok), szprint(temp, zendof(temp), "%1i лет", age));
		buttonp(236, 0, 188, 189, cmd(gender_proc, (int)this), gender_data[getgender()].name_short);
		// text labels
		if(true) {
			draw::state push;
			draw::setfont(res::FONT3);
			draw::setcolor(ColorButton);
			draw::text(50, 326, "Особенности");
			draw::text(18, 286, "Очки хар.");
			draw::text(383, 5, "Навыки");
			draw::text(428, 233, "Метки");
		}
		// stats
		for(auto i = Strenght; i <= Luck; i = (ability_s)(i + 1)) {
			rect rc = {102, 37 + 33 * i, 102 + 42, 46 + 33 * (i - 1)};
			int value = get(i);
			ev.element = i;
			ev.points = &ability_points;
			if(draw::buttonf(149, 38 + 33 * i, 193, 194, false, false))
				ev.modify(1);
			if(draw::buttonf(149, 49 + 33 * i, 191, 192, false, false))
				ev.modify(-1);
			const char* p = maptbl(ability_values, value);
			draw::label(102, 45 + 33 * i, ev.element, p, false, false);
			draw::number(59, 37 + 33 * i, 2, value);
		}
		if(show_ability)
			draw::number(126, 282, 2, ability_points);
		// secondanary stats
		static variant secondanary_stat_health[] = {HitPoints, Poison, Radiation, WoundEye, WoundRightHand, WoundLeftHand, WoundRightLeg, WoundLeftLeg};
		render_stats(194, 46, 120, secondanary_stat_health, show_maximum_only);
		static variant secondanary_stat_other[] = {ArmorClass, ActionPoints, CarryWeight, MeleeDamage, Phisycal, PoisonResistance, RadiationResistance, Sequence, HealingRate, CriticalHit};
		render_stats(194, 182, 120, secondanary_stat_other, show_maximum_only);
		// traits
		for(int i = 0; i < 16; i++) {
			ev.element = (perk_s)(FirstTraits + i);
			ev.checked = is(ev.element.perk);
			ev.points = &traits_points;
			if(i > 7) {
				radio(299, 352 + 13 * (i - 8), ev, 215);
				label(189, 353 + 13 * (i - 8), ev.element, getstr(ev.element.perk), ev.checked, false);
			} else {
				radio(23, 352 + 13 * i, ev, 215);
				label(48, 353 + 13 * i, ev.element, getstr(ev.element.perk), ev.checked, false);
			}
		}
		// skills
		auto w1 = 240;
		for(auto i = FirstSkill; i <= LastSkill; i = skill_s(i + 1)) {
			ev.element = i;
			ev.checked = is(i);
			ev.points = &tag_skill_points;
			draw::radio(347, 27 + 11 * (i - FirstSkill), ev, 215);
			draw::label(377, 27 + 11 * (i - FirstSkill), ev.element, getstr(ev.element.skill), ev.checked, false);
			auto s1 = szpercent(temp, zendof(temp), get(i));
			draw::text(377 + w1 - draw::textw(s1), 27 + 11 * (i - FirstSkill), s1);
		}
		if(show_tag)
			draw::number(522, 228, 2, tag_skill_points);
		// description
		variant focus = draw::getfocus();
		if(focus) {
			draw::state push;
			setfont(res::FONT2);
			setcolor(ColorInfo);
			line(350, 300, 620, 300);
			line(350, 301, 620, 301);
			image(483, 308, res::SKILLDEX, focus.getimage(), ImageNoOffset);
			auto p = getstr(focus);
			auto w1 = draw::textw(p);
			draw::text(350, 275, p);
			setfont(res::FONT1);
			if(focus.type == Skills) {
				szprint(temp, zendof(temp), "%1: ", "База");
				skill_data[focus.skill].base.get(zend(temp), zendof(temp));
				text(350 + w1 + 8, 275 + 14, temp);
			}
			p = focus.getdescription();
			if(p && p[0])
				textf(350, 308, 135, p);
		}
		// Кнопки диалога
		button(345, 454, 90, cmd(open_options), "Опции");
		button(452, 454, 80, cmdk(this, traits_points, tag_skill_points, ability_points), "Далее", KeyEnter);
		button(552, 454, 80, cmd(buttoncancel), "Отказ", KeyEscape);
		// Ожидаем событий
		domodal();
		switch(hot.key) {
		case KeyTab:
		case KeyUp:
		case KeyDown:
		case KeyRight:
		case KeyLeft:
			setfocus(getnext(getfocus(), hot.key));
			break;
		}
	}
	return getresult() != 0;
}

static void biography(int x1, int y1, const char* id) {
	char temp[260];
	char temz[260];
	auto pg = creature::getpregen(id);
	if(!pg)
		return;
	szprint(temp, zendof(temp), "Биография %1", grammar::of(temz, zendof(temz), pg->name));
	draw::text(x1, y1, temp);
	y1 += 30;
	draw::textf(x1, y1, 160, temp);
}

//int show::newgame() {
//	char temp[64];
//	int premade = PremadeCombat;
//	unsigned char data[LastToken + 1];
//	game::generate::creature(data, premade);
//	while(true) {
//		int width = draw::getwidth();
//		int height = draw::getheight();
//		draw::background(174);
//		draw::radio(81, 322, Continue, 0, 8);
//		draw::radio(436, 319, Change, 0, 8);
//		draw::radio(81, 424, Create, 0, 8);
//		draw::radio(461, 424, Cancel, 0, 8);
//		if(draw::buttonf(292, 320, 122, 123, 0))
//			draw::execute(KeyLeft);
//		if(draw::buttonf(318, 320, 124, 125, 0))
//			draw::execute(KeyRight);
//		// premade info
//		draw::image(width / 2, 40 + height / 2, res::INTRFACE, 201 + (premade - FirstPremade - 1));
//		int nameid = game::findname(bsgets(premade, Name));
//		const char* p1 = game::getnamepart(nameid);
//		draw::text((width - draw::textw(p1)) / 2, 50, p1);
//		biography(width / 2 + 120, 50, premade);
//		int x1 = 290;
//		int y1 = 80;
//		int w1 = 70;
//		int w2 = 20;
//		for(int i = FirstStat; i <= LastStat; i++) {
//			const char* p = game::getnameshort(i);
//			int value = game::get(i, data);
//			draw::text(x1 + w1 - 8 - draw::textw(p), y1, p);
//			draw::text(x1 + w1, y1, sznum(temp, value, 2));
//			draw::text(x1 + w1 + w2, y1, bsgets(FirstStatStr + value - 1, Name));
//			y1 += draw::texth();
//		}
//		y1 += draw::texth();
//		static int info1[] = {MaxHP, AC, MaxAP, MeleeDamage};
//		for(auto i : info1) {
//			const char* p = game::getnameshort(i);
//			draw::text(x1 + w1 + w2 - 4 - draw::textw(p), y1, p);
//			switch(i) {
//			case MaxHP:
//				szprint(temp, "%1i/%2i", game::get(MaxHP, data), game::get(MaxHP, data));
//				draw::text(x1 + w1 + w2, y1, temp);
//				break;
//			default:
//				draw::text(x1 + w1 + w2, y1, sznum(temp, game::get(i, data)));
//				break;
//			}
//			y1 += draw::texth();
//		}
//		y1 += draw::texth();
//		// tags
//		for(int n = Tag1; n <= Tag4; n++) {
//			int i = bsget(premade, n);
//			if(!i)
//				continue;
//			const char* p = game::getnameshort(i);
//			draw::text(x1 + w1 + w2 - 4 - draw::textw(p), y1, p);
//			draw::text(x1 + w1 + w2, y1, szpercent(temp, game::get(i, data)));
//			y1 += draw::texth();
//		}
//		// traits
//		for(int n = Trait1; n <= Trait2; n++) {
//			int i = bsget(premade, n);
//			if(!i)
//				continue;
//			const char* p = game::getnameshort(i);
//			draw::text(x1 + w1 + w2 - 4 - draw::textw(p), y1, p);
//			y1 += draw::texth();
//		}
//		int id = draw::input();
//		switch(id) {
//		case 0:
//			return 0;
//		case Cancel:
//		case KeyEscape:
//			return Cancel;
//		case KeyRight:
//			if(premade < PremadeDiplomat) {
//				premade++;
//				game::generate::creature(data, premade);
//			}
//			break;
//		case KeyLeft:
//			if(premade > PremadeCombat) {
//				premade--;
//				game::generate::creature(data, premade);
//			}
//			break;
//		case Change:
//			id = character(data);
//			if(!id)
//				return 0;
//			return Continue;
//		case Create:
//			game::generate::creature(data, FirstPremade);
//			id = character(data);
//			if(!id)
//				return 0;
//			return Continue;
//		case Continue:
//			return Continue;
//		}
//	}
//}