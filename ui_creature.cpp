#include "main.h"

using namespace draw;

static int info_mode;
static int current_skill;

static int gtv(int value) {
	return (value <= 1) ? 0 : ((value <= 2) ? 1 : 2);
}

static const char* szscore(int value) {
	static const char* score_text[3] = {"����", "����", "�����"};
	return score_text[gtv(value)];
}

static void set_info_mode() {
	info_mode = hot.param;
}

void creature::decrease(variant e, int& points) {
	int k;
	switch(e.type) {
	case Abilities:
		if(stats[e.ability] > 1) {
			stats[e.ability]--;
			points++;
		}
		break;
	case Skills:
		k = 1;
		if(skills[e.skill] >= 1) {
			skills[e.skill]--;
			points += k;
		}
		break;
	}
}

void creature::increase(variant e, int& points) {
	int k;
	if(!points)
		return;
	switch(e.type) {
	case Abilities:
		if(stats[e.ability] < 10) {
			stats[e.ability]++;
			points--;
		}
		break;
	case Skills:
		k = 1;
		if(points < k)
			return;
		if(skills[e.skill] < 150) {
			skills[e.skill]++;
			points -= k;
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
		if(count > 0)
			draw::execute(increase_proc, 1);
		if(count < 0)
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
		draw::button(x + 25, y + 45, 90, cmd(buttoncancel), "������");
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

static int render_stat(int x, int y, int width, variant id) {
	draw::label(x, y, id, getstr(id), false, false);
	return texth();
}

static int render_stat(int x, int y, int width, variant id, const char* value) {
	char temp[260]; szprint(temp, zendof(temp), "%1:", getstr(id));
	draw::label(x, y, id, temp, false, false);
	draw::text(x + width - draw::textw(value), y, value);
	return texth();
}

static int render_stat(int x, int y, int width, variant id, int value, const char* format) {
	char temp[260]; szprint(temp, zendof(temp), format, value);
	return render_stat(x, y, width, id, temp);
}

int creature::render_stats(int x, int y, int width, aref<variant> elements, bool show_maximum_only) const {
	auto y0 = y;
	char temp[260];
	for(auto id : elements) {
		get(temp, zendof(temp), id, show_maximum_only);
		switch(id.type) {
		case Abilities:
			if(id.ability == RadiationLevel || id.ability == PoisonLevel)
				draw::label(x, y, id, ability_data[id.ability].name_short, false, get(id.ability) == 0);
			else {
				draw::label(x, y, id, id.getnameshort(), false, false);
				draw::text(x + width - draw::textw(temp), y, temp);
			}
			break;
		case Damages:
			draw::label(x, y, id, id.getnameshort(), false, false);
			draw::text(x + width - draw::textw(temp), y, temp);
			break;
		case Wounds:
			draw::label(x, y, id, getstr(id), false, !is(id.wound));
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
			dlgerr("", "������������ ��� %1i %2 �������������.", current.ability_points, szscore(current.ability_points));
			return;
		}
		if(current.tag_skill_points) {
			dlgerr("", "������������ ��� %1i %2 �����.", current.tag_skill_points, szscore(current.tag_skill_points));
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

bool creature::choose_stats(int traits_points, int tag_skill_points, int ability_points, bool charsheet_mode, int skill_points) {
	char temp[260];
	draw::state push;
	draw::setcolor(ColorText);
	draw::setfont(res::FONT1);
	auto show_ability = (ability_points > 0);
	auto show_tag = (traits_points > 0);
	auto show_maximum_only = !charsheet_mode;
	setfocus(variant(Strenght));
	if(!current_skill)
		current_skill = SmallGuns;
	while(ismodal()) {
		if(charsheet_mode) {
			draw::background(177);
			variant id = getfocus();
			if(id.type == Skills)
				current_skill = id.skill;
		} else
			draw::background(169);
		// ���������� �������
		cmd_creature ev;
		ev.player = this;
		// ������ ������
		buttonp(13, 0, 185, 184, cmd(buttonok), name ? name : "�����");
		buttonp(157, 0, 188, 189, cmd(buttonok), szprint(temp, zendof(temp), "%1i ���", get(Age)));
		buttonp(236, 0, 188, 189, cmd(gender_proc, (int)this), gender_data[getgender()].name_short);
		// text labels
		if(true) {
			draw::state push;
			draw::setfont(res::FONT3);
			draw::setcolor(ColorButton);
			if(!charsheet_mode) {
				draw::text(50, 326, "�����������");
				draw::text(18, 286, "���� ���.");
			} else {
				static int info_mode_frame[] = {180, 178, 179};
				auto x = 10, y = 326, w = 100;
				image(x, y, res::INTRFACE, maptbl(info_mode_frame, info_mode), ImageNoOffset);
				static const char* names[] = {"������", "�����", "�����"};
				for(auto i = 0; i < 3; i++) {
					auto x1 = x + w * i + 10;
					rect rc = {x1 + 4, y, x1 + w - 4, y + 28};
					//rectb(rc, colors::red);
					if((areb(rc) && hot.key == MouseLeft && hot.pressed)
						|| (hot.key == (Alpha + '1' + i)))
						execute(set_info_mode, i);
					text(x1 + (w - textw(names[i])) / 2, y + ((i==info_mode) ? 4 : 6), names[i]);
				}
			}
			draw::text(383, 5, "������");
			if(charsheet_mode)
				draw::text(390, 233, "���� ������");
			else
				draw::text(428, 233, "�����");
		}
		// stats
		for(auto i = Strenght; i <= Luck; i = (ability_s)(i + 1)) {
			rect rc = {102, 37 + 33 * i, 102 + 42, 46 + 33 * (i - 1)};
			int value = get(i);
			ev.element = i;
			ev.points = &ability_points;
			if(show_ability) {
				if(draw::buttonf(149, 38 + 33 * (i - Strenght), 193, 194, false, false))
					ev.modify(1);
				if(draw::buttonf(149, 49 + 33 * (i - Strenght), 191, 192, false, false))
					ev.modify(-1);
			}
			const char* p = maptbl(ability_values, value);
			//rectb({102, 45 + 33 * (i - Strenght), 102 + 54, 45 + 33 * (i - Strenght) + 12}, colors::red);
			if(charsheet_mode)
				draw::label(102 + (54 - textw(p)) / 2, 45 + 33 * (i - Strenght), ev.element, p, false, false);
			else
				draw::label(102, 45 + 33 * (i - Strenght), ev.element, p, false, false);
			draw::number(59, 37 + 33 * (i - Strenght), 2, value);
		}
		if(charsheet_mode) {
			auto x = 30, y = 282, w =120;
			y += render_stat(x, y, 120, Level, get(Level), "%1i");
			y += render_stat(x, y, 120, Experience, get(Experience), "%1i");
			y += render_stat(x, y, 120, NextLevelExperience, get(NextLevelExperience), "%1i");
			y = 366;
			switch(info_mode) {
			case 0:
				for(auto i = FirstPerk; i <= LastTraits; i = (perk_s)(i + 1)) {
					if(y >= clipping.y2)
						break;
					if(is(i))
						y += render_stat(x, y, 120, i);
				}
				break;
			}
		} else {
			if(show_ability)
				draw::number(126, 282, 2, ability_points);
		}
		// secondanary stats
		static variant secondanary_stat_health[] = {HP, PoisonLevel, RadiationLevel, WoundEye, WoundRightHand, WoundLeftHand, WoundRightLeg, WoundLeftLeg};
		render_stats(194, 46, 120, secondanary_stat_health, show_maximum_only);
		static variant secondanary_stat_other[] = {AC, APMax, CarryWeight, DamageMelee, PhisycalResistance, PoisonResistance, RadiationResistance, Sequence, HealingRate, CriticalHit};
		render_stats(194, 182, 120, secondanary_stat_other, show_maximum_only);
		// traits
		if(!charsheet_mode) {
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
		}
		// skills
		auto w1 = 220;
		for(auto i = FirstSkill; i <= LastSkill; i = skill_s(i + 1)) {
			ev.element = i;
			ev.checked = is(i);
			ev.points = &tag_skill_points;
			draw::label(377, 27 + 11 * (i - FirstSkill), ev.element, getstr(ev.element.skill), ev.checked, false);
			auto s1 = szprint(temp, zendof(temp), "%1i%%", get(i));
			draw::text(377 + w1 - draw::textw(s1), 27 + 11 * (i - FirstSkill), s1);
			if(!charsheet_mode)
				draw::radio(347, 27 + 11 * (i - FirstSkill), ev, 215);
		}
		if(charsheet_mode) {
			auto x = 634;
			auto y = 32 + 11 * (current_skill - FirstSkill);
			draw::image(x, y, res::INTRFACE, 190);
			ev.element = (skill_s)current_skill;
			ev.points = &skill_points;
			if(draw::buttonf(x - 20, y - 12, 193, 194, false, false) || (hot.key == Alpha + '+'))
				ev.modify(1);
			if(draw::buttonf(x - 20, y, 191, 192, false, false) || (hot.key == Alpha + '-'))
				ev.modify(-1);
		}
		if(charsheet_mode)
			draw::number(524, 228, 2, skill_points);
		else
			draw::number(524, 228, 2, tag_skill_points);
		// description
		variant focus = draw::getfocus();
		if(focus) {
			draw::state push;
			setfont(res::FONT2);
			setcolor(ColorInfo);
			line(350, 300, 620, 300);
			line(350, 301, 620, 301);
			image(483, 308, res::SKILLDEX, getfid(focus), ImageNoOffset);
			auto p = getstr(focus);
			auto w1 = draw::textw(p);
			draw::text(350, 275, p);
			setfont(res::FONT1);
			if(focus.type == Skills) {
				szprint(temp, zendof(temp), "%1: ", "����");
				skill_data[focus.skill].base.get(zend(temp), zendof(temp));
				text(350 + w1 + 8, 275 + 14, temp);
			}
			p = focus.getdescription();
			if(p && p[0])
				textf(350, 308, 135, p);
		}
		// ������ �������
		button(345, 454, 90, cmd(open_options), "�����");
		button(452, 454, 80, cmdk(this, traits_points, tag_skill_points, ability_points), "�����", KeyEnter);
		button(552, 454, 80, cmd(buttoncancel), "�����", KeyEscape);
		// ������� �������
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

static void biography(int x1, int y1, const pregen_info* pg) {
	char temp[260];
	char temz[260];
	szprint(temp, zendof(temp), "��������� %1", grammar::of(temz, zendof(temz), pg->name));
	draw::text(x1, y1, temp);
	y1 += 30;
	draw::textf(x1, y1, 160, pg->text);
}

static void pregen_left() {
	auto p = (int*)hot.param;
	if(*p > 0)
		*p = *p - 1;
}

static void pregen_right() {
	auto p = (int*)hot.param;
	if(*p < 2)
		*p = *p + 1;
}

static void take_character() {
	buttonok();
}

static void create_new_character() {
	auto player = (creature*)hot.param;
	player->clear();
	if(player->choose_stats(2, 3, 5))
		buttonok();
}

static void change_character() {
	auto player = (creature*)hot.param;
	if(player->choose_stats(0, 0, 0))
		buttonok();
}

void creature::newgame() {
	char temp[64];
	static const char* pregen_names[] = {"narg", "mingun", "chitsa"};
	creature	player;
	int			index = 1;
	while(ismodal()) {
		int width = draw::getwidth();
		int height = draw::getheight();
		auto pg = getpregen(pregen_names[index]);
		player.clear();
		player.apply(pg);
		background(174);
		radio(81, 322, cmd(buttonok), 8);
		radio(436, 319, cmd(change_character, (int)&player), 8);
		radio(81, 424, cmd(create_new_character, (int)&player), 8);
		radio(461, 424, cmd(buttoncancel), 8, KeyEscape);
		if(draw::buttonf(292, 320, 122, 123, 0) || (hot.key == KeyLeft))
			draw::execute(pregen_left, (int)&index);
		if(draw::buttonf(318, 320, 124, 125, 0) || (hot.key == KeyRight))
			draw::execute(pregen_right, (int)&index);
		// premade info
		draw::image(width / 2, 40 + height / 2, res::INTRFACE, 201 + index);
		draw::text((width - draw::textw(pg->name)) / 2, 40, pg->name);
		biography(width / 2 + 120, 40, pg);
		// stats
		auto x1 = 295, y1 = 70, w1 = 70, w2 = 20;
		for(auto i = Strenght; i <= Luck; i = (ability_s)(i + 1)) {
			auto p = ability_data[i].name;
			auto value = player.get(i);
			draw::text(x1 + w1 - 8 - draw::textw(p), y1, p);
			draw::text(x1 + w1, y1, sznum(temp, value, 2));
			draw::text(x1 + w1 + w2, y1, maptbl(ability_values, value));
			y1 += draw::texth();
		}
		y1 += draw::texth() / 2;
		x1 += 20;
		static variant fast_info[] = {HPMax, AC, AP, DamageMelee};
		for(auto id : fast_info) {
			const char* p = id.getnameshort();
			draw::text(x1 + w1 + w2 - 4 - draw::textw(p), y1, p);
			player.get(temp, zendof(temp), id, true);
			draw::text(x1 + w1 + w2, y1, temp);
			y1 += draw::texth();
		}
		// tags
		for(auto e : pg->tagged) {
			const char* p = skill_data[e].name;
			draw::text(x1 + w1 + w2 - 4 - draw::textw(p), y1, p);
			draw::text(x1 + w1 + w2, y1, szprint(temp, zendof(temp), "%1i%%", player.get(e)));
			y1 += draw::texth();
		}
		// traits
		for(auto e : pg->perks) {
			const char* p = perk_data[e].name;
			draw::text(x1 + w1 + w2 - 4 - draw::textw(p), y1, p);
			y1 += draw::texth();
		}
		domodal();
	}
}