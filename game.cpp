#include "main.h"

// TODO: ����� ����� ������ �� ����� �������� �����.
// ��������, ���� ������ ����� �������. ����� �� ������� � �������� ����� ����� ����. ���� ������� �� �����.
// � ������� ����� ���������� ����� �����-�� �������� � ������.
// ������ ������ ����� ������ ����� ���������� ������, ������� ���������� ����� ��������� �� ����� �����.
// ����� ���� �� ������ �� ������, ����� ������ ����� ����� ����� ��������.
// TODO: � ������� ����� ����� ���� ������ ���������. ����� �� ������� ��� ����� ������ � ������ ������� ����.

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