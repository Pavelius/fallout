#include "main.h"

#pragma once

struct msg_info {
	int				id;
	const char*		name;
};
struct lst_info {
	const char*		name;
	int				start;
	int				count;
	res::tokens		token;
};
lst_info			criters_lst[];