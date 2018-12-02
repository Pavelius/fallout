#include "main.h"

// TODO: Удача может влиять на обыск объектов сцены.
// Например, есть объект сцены бутылки. Время от времени в бутылках можно найти колу. Шанс зависит от удачи.
// В машинах можно попытаться найти какую-то запчасть к машине.
// Каждый объект сцены должен иметь встроенный таймер, который показывает когда наступило то самое время.
// Может быть от недели до месяца, когда объект сцены можно снова обыскать.
// TODO: У объекта сцены может быть статус поврежден. Время от времени его будут чинить в городе местные люди.

struct itemwear {
	int			owner;
	int			object;
};
static aref<itemwear> wears;

void game::additem(int player, int rec) {
	int item_type = item::gettype(rec);
	int c2 = item::get(rec, Count);
	for(auto& e : wears) {
		if(e.owner != player || item::gettype(e.object) != item_type)
			continue;
		if(item::add(e.object, rec))
			return;
	}
	for(auto& e : wears) {
		if(e.owner)
			continue;
		e.owner = player;
		e.object = rec;
		return;
	}
	wears.reserve();
	auto& e = wears.add();
	e.owner = player;
	e.object = rec;
}

int game::getwears(int** result, int player) {
	auto pb = result;
	auto pe = pb + 1024; // Maximum 1024 items per creature
	for(auto& e : wears) {
		if(e.owner != player || !e.object)
			continue;
		*pb++ = &e.object;
		if(pb >= pe)
			break;
	}
	*pb = 0;
	return pb - result;
}

int game::getwearsweight(int player) {
	int* wear[1024 + 2];
	int result = 0;
	result += item::get(bsget(player, Weapon), CarryWeight);
	result += item::get(bsget(player, WeaponSecondonary), CarryWeight);
	result += item::get(bsget(player, Armor), CarryWeight);
	int count = getwears(wear, player);
	for(int i = 0; i < count; i++)
		result += item::get(*wear[i], CarryWeight);
	return result;
}

void game::passtime(int count) {
	datetime += count;
}