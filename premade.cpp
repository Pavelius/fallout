#include "main.h"

static pregen_info pregen_data[] = {{1, "beggar", "Нищий", 65, Male, {4, 2, 4, 3, 3, 3, 1}, {}, {Speech, FirstAid, Gambling}},
{1, "chitsa", "Читса", 19, Female, {4, 5, 4, 10, 7, 6, 4}, {OneHanded, SexAppeal}, {Speech, Barter, FirstAid}, {}, "Читса умеет убеждать и договариваться. Ослепительная красота служит девушке в этом подспорьем. Она торгует лучше, чем кто-либо в племени, и не понаслышке знает об опасностях пустыни. Сейчас ее народу нужна помощь, и Читса слышит зов дальней дороги ..."},
{1, "mingun", "Минган", 32, Male, {5, 8, 4, 4, 5, 9, 5}, {SmallFrame, Skilled}, {Sneak, Lockpick, Steal}, {}, "Мягкая поступь и быстрый хватательный рефлекс помогают Мингану удовлетворять свои потребности. Несколько лет назад люди решили, что этому воришке лучше применять свои таланты на каком-нибудь другом племени. С тех пор Минган стал разведчиком. Однако палатки соседних племен уже не представляют для него интереса... Пора отправляться в дальний путь."},
{5, "mutant", "Мутант", 40, Male, {11, 5, 10, 2, 2, 5, 1}, {HeavyHanded}, {BigGuns, Unarmed, MeleeWeapon}, {}},
{1, "narg", "Нарг", 21, Male, {8, 5, 9, 3, 4, 7, 4}, {HeavyHanded, Gifted}, {SmallGuns, MeleeWeapon, Throwing}, {}, "Исключительная сила и телосложение позволили Наргу стать одним из лучших охотников племени. Но обычно Нарг сначала бьет, а потом уже смотрит, что он такое встретил. Постоянно тренируясь Нарг стал большим специалистом по всем видам разрушения. Он очень хочет доказать, что достоин стать вождем племени и ничему не позволит стать у себя на пути."},
{2, "raider", "Рейдер", 30, Male, {6, 5, 5, 4, 4, 6, 4}, {}, {SmallGuns, Gambling, Steal}},
};

const pregen_info* creature::getpregen(const char* id) {
	for(const auto& e : pregen_data) {
		if(strcmp(e.id, id) == 0)
			return &e;
	}
	return 0;
}

void creature::apply(const pregen_info* pg) {
	for(auto i = Strenght; i <= Luck; i = (ability_s)(i + 1))
		stats[i] = pg->stats[i-Strenght];
	for(auto e : pg->perks)
		set(e);
	for(auto e : pg->tagged)
		set(e);
	name = pg->name;
	gender = pg->gender;
	stats[Age] = pg->age;
	for(auto e : pg->gears) {

	}
}

creature::creature(const char* id) {
	clear();
	auto pg = getpregen(id);
	if(pg)
		apply(pg);
}