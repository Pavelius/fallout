#include "main.h"

wound_info wound_data[] = {{13, "Ранен в глаз"},
{14, "Рана прав. руки"},
{15, "Рана левая руки"},
{16, "Рана прав. ноги"},
{17, "Рана левая ноги"},
};
assert_enum(wound, WoundLeftLeg);
getstr_enum(wound);