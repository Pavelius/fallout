#include "main.h"

static struct settlement {
	const char*	name[2];
	point		position;
	char		size;
	unsigned	flags;
} settlement_data[] = {{{"Arroyo", "�����"}, {184, 133}, 1},
{{"The Den", "���"}, {473, 272}, 2},
{{"Klamath", "������"}, {373, 122}, 2},
};