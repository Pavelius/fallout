#include "main.h"

resist_info damage_data[] = {{"Phisycal", "Урон", "На указанный процент уменьшаются все повреждения от физического источника, например, от удара копья или пули. Меняется в зависимости от одетой брони."},
{"Laser", "Лазер"},
{"Fire", "Огонь"},
{"Plasma", "Плазма"},
{"Electrical", "Электро"},
{"EMP", "Шок"},
{"Explosive", "Взрыв"},
};
assert_enum(damage, Explosive);
getstr_enum(damage);
getsid_enum(damage);