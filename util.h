#include "main.h"

#pragma once

enum flags_ex_item {
	HiddenItem = 0x08000000,
	UseCanBe = 0x00000800, // can be used
	UseOnSmth = 0x00001000, // Можно использовать на что-то
	PickUp = 0x00008000,
	BigGun = 0x00000100,
	TwoHnd = 0x00000200, // Оружие двуручное
};
struct msg_info {
	int				id;
	const char*		name;
	const char*		get(int id) const;
};
struct lst_info {
	const char*		name;
	int				start;
	int				count;
	res::tokens		token;
};
lst_info			criters_lst[];
msg_info			lst_pro_crit[];
msg_info			lst_pro_item[];
msg_info			lst_pro_item_en[];
msg_info			lst_pro_misc[];
msg_info			lst_pro_scen[];
msg_info			lst_pro_tile[];
msg_info			lst_pro_wall[];