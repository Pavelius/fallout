#include "main.h"

static struct settlement {
	const char*	name[2];
	point		position;
	char		size;
	unsigned	flags;
} settlement_data[] = {{{"Arroyo", "јройо"}, {184, 133}, 1},
{{"The Den", "яма"}, {473, 272}, 2},
{{"Klamath", " ламат"}, {373, 122}, 2},
};