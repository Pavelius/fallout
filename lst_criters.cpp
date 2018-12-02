#include "util.h"

lst_info criters_lst[] = {{"", 0, 0, res::NoRes},
{"HAPOWR", 21, 1, res::HAPOWR},
{"HAROBE", 11, 1, res::HAROBE},
{"HFCMBT", 11, 1, res::HFCMBT},
{"HFJMPS", 11, 1, res::HFJMPS},
{"HFLTHR", 11, 1, res::HFLTHR},
{"HFMAXX", 11, 1, res::HFMAXX},
{"HFMETL", 11, 1, res::HFMETL},
{"HMBJMP", 11, 0, res::HMBJMP},
{"HMBMET", 11, 0, res::HMBMET},
{"HMCMBT", 11, 1, res::HMCMBT},
{"HMJMPS", 11, 1, res::HMJMPS},
{"HMLTHR", 11, 1, res::HMLTHR},
{"HMMAXX", 11, 1, res::HMMAXX},
{"HMMETL", 11, 1, res::HMMETL},
{"MABROM", 15, 0, res::MABROM},
{"MADDOG", 16, 1, res::MADDOG},
{"MAHAND", 17, 0, res::MAHAND},
{"", 18, 0, res::NoRes},
{"MAMRAT", 19, 0, res::MAMRAT},
{"MAMTN2", 21, 0, res::MAMTN2},
{"MAMTNT", 21, 0, res::MAMTNT},
{"MASCRP", 22, 0, res::MASCRP},
{"MASPHN", 23, 0, res::MASPHN},
{"MASRAT", 24, 0, res::MASRAT},
{"MATHNG", 25, 0, res::MATHNG},
{"NABLUE", 11, 1, res::NABLUE},
{"NACHLD", 27, 1, res::NACHLD},
{"NAGHUL", 11, 0, res::NAGHUL},
{"NAGLOW", 11, 0, res::NAGLOW},
{"NAPOWR", 21, 1, res::NAPOWR},
{"NAROBE", 11, 0, res::NAROBE},
{"", 11, 0, res::NoRes},
{"NFBRLP", 11, 0, res::NFBRLP},
{"NFMAXX", 11, 1, res::NFMAXX},
{"NFMETL", 11, 1, res::NFMETL},
{"NFPEAS", 11, 1, res::NFPEAS},
{"NFTRMP", 11, 0, res::NFTRMP},
{"NFVRED", 11, 0, res::NFVRED},
{"NMBPEA", 11, 1, res::NMBPEA},
{"NMBRLP", 11, 0, res::NMBRLP},
{"NMBSNP", 11, 0, res::NMBSNP},
{"NMGRCH", 27, 0, res::NMGRCH},
{"NMLOSR", 11, 0, res::NMLOSR},
{"NMLTHR", 11, 1, res::NMLTHR},
{"NMMAXX", 11, 1, res::NMMAXX},
{"", 46, 0, res::NoRes},
{"", 11, 0, res::NoRes},
{"NMPEAS", 11, 0, res::NMPEAS},
{"", 49, 0, res::NoRes},
{"", 50, 0, res::NoRes},
{"MACLAW", 51, 0, res::MACLAW},
{"MAMANT", 52, 0, res::MAMANT},
{"MAROBO", 53, 1, res::MAROBO},
{"MAFEYE", 54, 0, res::MAFEYE},
{"MAMURT", 55, 0, res::MAMURT},
{"NABRWN", 11, 1, res::NABRWN},
{"NMDOCC", 11, 0, res::NMDOCC},
{"MADEGG", 58, 0, res::MADEGG},
{"MASCP2", 59, 0, res::MASCP2},
{"MACLW2", 60, 0, res::MACLW2},
{"HFPRIM", 11, 1, res::HFPRIM},
{"HMWARR", 11, 1, res::HMWARR},
{"NFPRIM", 11, 1, res::NFPRIM},
{"NMWARR", 11, 1, res::NMWARR},
{"MAPLNT", 65, 0, res::MAPLNT},
{"MAROBT", 66, 0, res::MAROBT},
{"MAGKO2", 67, 1, res::MAGKO2},
{"MAGCKO", 68, 1, res::MAGCKO},
{"NMVALT", 11, 1, res::NMVALT},
{"MACYBR", 16, 1, res::MACYBR},
{"HANPWR", 21, 1, res::HANPWR},
{"NMNICE", 11, 0, res::NMNICE},
{"NFNICE", 11, 1, res::NFNICE},
{"NFVALT", 11, 0, res::NFVALT},
{"MACYBR", 16, 1, res::MACYBR},
{"MABRAN", 19, 0, res::MABRAN},
{"NMBONC", 11, 0, res::NMBONC},
{"NMBRSR", 21, 0, res::NMBRSR},
{"NAVGUL", 11, 1, res::NAVGUL},
{"MALIEN", 80, 0, res::MALIEN},
{"MAFIRE", 68, 1, res::MAFIRE},
{"NMASIA", 11, 1, res::NMASIA},
{"NFLYNN", 11, 1, res::NFLYNN},
{"NAWHIT", 11, 1, res::NAWHIT},
{"MABOSS", 85, 0, res::MABOSS},
{"MAQUEN", 86, 0, res::MAQUEN},
{"NMCOPP", 11, 0, res::NMCOPP},
{"NMMYRN", 11, 1, res::NMMYRN},
{"NMLABB", 11, 1, res::NMLABB},
{"MAGUN2", 90, 0, res::MAGUN2},
{"NMFATT", 11, 1, res::NMFATT},
{"NMRGNG", 11, 1, res::NMRGNG},
{"NMGANG", 11, 1, res::NMGANG},
{"NFASIA", 11, 0, res::NFASIA},
{"NMMEXI", 11, 1, res::NMMEXI},
{"NMBOXX", 11, 0, res::NMBOXX},
{"MAANTT", 97, 0, res::MAANTT},
{"NMOLDD", 11, 0, res::NMOLDD},
{"MAROBE", 99, 0, res::MAROBE},
{"MADETH", 100, 0, res::MADETH},
{"MAGUNN", 101, 0, res::MAGUNN},
{"MABOS2", 102, 0, res::MABOS2},
};