#include "main.h"

action_info action_data[] = {{"NoAction", "Нет действия", 252},
{"ThrowPunch", "Хук", 42},
{"KickLeg", "Пнуть", 41},
{"Swing", "Рубить", 44},
{"Thrust", "Тыкнуть", 45},
{"Throw", "Бросить", 117},
{"FireSingle", "Выстрелить", 43},
{"FireBurst", "Очередь", 40},
{"FireContinuous", "Спалить", -1},
{"Drop", "Бросить", 254},
{"Look", "Осмотреть", 258},
{"Talk", "Говорить", 262},
{"Turn", "Развернуть", 260},
{"Unload", "Разрядить", 301},
{"Use", "Использовать", 264},
{"UseItem", "Использовать предмет", 256},
{"UseSkill", "Использовать навык", 303},
};
getsid_enum(action);