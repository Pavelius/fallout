#include "main.h"

illness_info illness_data[] = {{11, "Отравлен", "Текущий уровень Яда в крови."},
{12, "Облучен", "Текущий уровень Радиации в организме."},
};
assert_enum(illness, LastIllness);
getstr_enum(illness);