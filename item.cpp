#include "main.h"

using namespace res;

static_assert(sizeof(item) == sizeof(int), "Not valid items count");
struct ammo_info {
	short			ac;
	char			dr;
	char			dam_mul, dam_div;
};
struct drug_info {
	variant			id;
	short			bonus;
	unsigned		interval[3];
};
struct armor_info {
	short			ac;
	unsigned short	threshold[Explosive + 1];
	unsigned short	resistance[Explosive + 1];
	res::tokens		dress[2];
};
struct item_info {
	const char*		id;
	const char*		name;
	material_s		material;
	unsigned short	frame[4];
	unsigned short	sound[2];
	unsigned short	weight;
	unsigned short	cost;
	attack_info		weapon;
	armor_info		armor;
	ammo_info		ammunition;
	const char*		description;
};
static item_info item_data[] = {{"Нет предмета"},
{"MetalArmor", "Металлическая Броня", Metal, {1, 33, 0}, {48}, 35, 1100, {}, {10, {4, 6, 4, 4, 0, 0, 4}, {30, 75, 10, 20, 0, 500, 25}, {HMMETL, HFMETL}}},
{"SMG10mm", "10мм ПП", Metal, {2, 44, 6}, {48}, 5, 1000, {{FireSingle, FireBurst}, SmallGuns, {5, 12}, 4, 30}},
{"LeatherArmor", "Кожаная броня", Leather, {3, 29, 0}, {48}, 8, 700, {}, {15, {2, 0, 0, 0, 0, 0, 0}, {25, 20, 20, 10, 30, 500, 20}, {HMLTHR, HFLTHR}}},
{"Pistol10mm", "10мм Пистолет", Metal, {4, 34, 5}, {48}, 3, 250, {{FireSingle, NoAction}, SmallGuns, {5, 12}, 3, 12}},
{"Sledgehammer", "Кувалда", Metal, {5, 40, 3}, {48}, 12, 120, {{Swing, Thrust}, SmallGuns, {4, 9}, 6, 0}},
{"Minigun", "Пулемет", Metal, {6, 31, 9}, {48}, 28, 3800, {{FireBurst, NoAction}, BigGuns, {7, 11}, 7, 120}},
{"RocketLauncher", "Базука", Metal, {7, 37, 10}, {48}, 15, 2300, {{FireSingle, NoAction}, BigGuns, {35, 100}, 6, 1}},
{"ExplosiveRocket", "Взрывная ракета", Metal, {28, 38, 0}, {48}, 3, 200, },
{"Club", "Дубинка", Wood, {9, 16, 2}, {48}, 3, 30, {{Swing, NoAction}, SmallGuns, {1, 6}, 3, 0}},
{"Flamer", "Огнемет", Metal, {10, 24, 8}, {48}, 18, 2000, {{FireContinuous, NoAction}, BigGuns, {45, 90}, 6, 5}},
{"HuntingRifle", "Охотничье Ружье", Metal, {23, 36, 7}, {48}, 9, 1000, {{FireSingle, NoAction}, SmallGuns, {8, 20}, 5, 10}},
{"Knife", "Нож", Metal, {96, 28, 1}, {51}, 1, 40, {{Swing, Thrust}, SmallGuns, {1, 6}, 2, 0}},
{"Spear", "Копье", Wood, {13, 42, 4}, {48}, 4, 80, {{Thrust, Throw}, SmallGuns, {3, 10}, 4, 0}},
{"PowerArmor", "Моторизованая Броня", Metal, {15, 35, 0}, {48}, 85, 12500, {}, {25, {12, 18, 12, 10, 12, 15, 20}, {40, 80, 60, 40, 40, 500, 50}, {HAPOWR, HAPOWR}}},
{"PlasmaRifle", "Плазменная Винтовка", Metal, {14, 24, 8}, {48}, 12, 4000, {{FireSingle, NoAction}, SmallGuns, {30, 65}, 6, 10}},
{"LaserPistol", "Лазерный Пистолет", Metal, {21, 34, 5}, {48}, 4, 1400, {{FireSingle, NoAction}, SmallGuns, {10, 22}, 3, 12}},
{"CombatArmor", "Боевая Броня", Metal, {16, 17, 0}, {48}, 20, 6500, {}, {20, {5, 8, 4, 4, 2, 0, 6}, {40, 60, 30, 50, 50, 500, 40}, {HMCMBT, HFCMBT}}},
{"DesertEagle44", "Пустынный Орел .44", Metal, {18, 34, 5}, {48}, 4, 800, {{FireSingle, NoAction}, SmallGuns, {10, 16}, 4, 8}},
{"Rock", "Камень", Stone, {17, 53, 0}, {48}, 1, 0, {{Throw, ThrowPunch}, SmallGuns, {1, 4}, 1, 0}},
{"Crowbar", "Фомка", Metal, {19, 16, 2}, {48}, 5, 65, {{Swing, NoAction}, SmallGuns, {3, 10}, 5, 0}},
{"BrassKnuckles", "Металлические Кастеты", Metal, {140, 47, 0}, {48}, 1, 40, {{ThrowPunch, NoAction}, SmallGuns, {2, 5}, 1, 0}},
{"Pistol14mm", "14мм Пистолет", Metal, {22, 44, 6}, {48}, 4, 1100, {{FireSingle, NoAction}, SmallGuns, {12, 22}, 4, 6}},
{"AssaultRifle", "Десантная Винтовка", Metal, {11, 36, 7}, {48}, 7, 1300, {{FireSingle, FireBurst}, SmallGuns, {8, 16}, 5, 24}},
{"PlasmaPistol", "Плазменный Пистолет", Metal, {24, 34, 5}, {48}, 4, 2750, {{FireSingle, NoAction}, SmallGuns, {15, 35}, 4, 16}},
{"GrenadeFrag", "Граната", Metal, {25, 122, 0}, {48}, 1, 150, {{Throw, NoAction}, SmallGuns, {20, 35}, 3, 0}},
{"GrenadePlasma", "Граната (Плазма)", Metal, {26, 122, 0}, {48}, 1, 300, {{Throw, NoAction}, SmallGuns, {40, 90}, 4, 0}},
{"GrenadePulse", "Граната (Пульс)", Metal, {27, 122, 0}, {48}, 1, 250, {{Throw, NoAction}, SmallGuns, {100, 150}, 4, 0}},
{"GatlingLaser", "Лазер Гатлинга", Metal, {41, 31, 9}, {48}, 24, 7500, {{FireBurst, NoAction}, SmallGuns, {20, 40}, 6, 30}},
{"JHP10mm", "10мм ПСЦТ", Metal, {31, 1, 0}, {48}, 1, 75, },
{"AP10mm", "10мм БП", Metal, {30, 1, 0}, {48}, 1, 100, },
{"MagnumJHP44", ".44 Магнум ПСЦТ", Metal, {34, 1, 0}, {48}, 1, 50, },
{"FlamethrowerFuel", "Топливо для огнемета", Metal, {74, 43, 0}, {48}, 10, 250, },
{"AP14mm", "14мм БП", Metal, {32, 1, 0}, {48}, 1, 150, },
{"FMJ223", ".223 ЦМ", Metal, {35, 1, 0}, {48}, 2, 200, },
{"JHP5mm", "5мм ПСЦТ", Metal, {39, 1, 0}, {48}, 1, 100, },
{"AP5mm", "5мм БП", Metal, {38, 1, 0}, {48}, 1, 120, },
{"RocketAP", "Ракета БП", Metal, {8, 38, 0}, {48}, 3, 400, },
{"SmallEnergyCell", "Малый Энерго-Блок", Metal, {36, 4, 0}, {48}, 3, 400, },
{"MicroFusionCell", "Атомная Батарейка", Metal, {37, 4, 0}, {48}, 5, 1000, },
{"Stimpak", "Стимулятор", Metal, {115, 15, 0}, {48}, 0, 175, },
{"Money", "Деньги", Metal, {40, 3, 0}, {48}, 0, 1, },
{"IceChest", "Ледник", Metal, {50, 27, 0}, {67}, 30, 0, },
{"IceChest", "Ледник", Metal, {50, 26, 0}, {67}, 30, 0, },
{"ThrowingKnife", "Метательный Нож", Metal, {78, 28, 1}, {52}, 1, 100, {{Throw, Swing}, SmallGuns, {3, 6}, 3, 0}},
{"Bag", "Сумка", Leather, {138, 3, 0}, {48}, 3, 25, },
{"FirstAidKit", "Аптечка", Metal, {42, 15, 0}, {48}, 2, 200, },
{"RadAway", "Рад-Эвей", Plastic, {48, 15, 0}, {48}, 0, 500, },
{"Antidote", "Противоядие", Metal, {44, 15, 0}, {48}, 1, 50, },
{"ReservedItem", "Резервный Предмет", Metal, {45, 173, 0}, {48}, 10, 0, },
{"Dynamite", "Динамит", Metal, {46, 23, 0}, {48}, 5, 500, },
{"GeigerCounter", "Счетчик Гейгера", Metal, {47, 121, 0}, {48}, 4, 650, },
{"Mentats", "Ментат", Metal, {72, 15, 0}, {48}, 0, 280, },
{"StealthBoy", "Невидимка", Metal, {49, 14, 0}, {48}, 3, 1800, },
{"WaterChip", "Водяной чип", Metal, {84, 48, 0}, {48}, 2, 1, },
{"DogTags", "Собачьи Метки", Metal, {73, 41, 0}, {48}, 0, 50, },
{"Bug", "Жучок", Metal, {53, 41, 0}, {48}, 2, 50, },
{"Tape", "Пленка", Metal, {75, 14, 0}, {48}, 4, 40, },
{"MotionSensor", "Датчик движения", Metal, {67, 123, 0}, {48}, 7, 800, },
{"Fruit", "Фрукт", Wood, {61, 14, 0}, {48}, 1, 10, },
{"Briefcase", "Дипломат", Leather, {52, 5, 0}, {48}, 5, 35, },
{"BigBookOfScience", "Большая Научная Книга", Wood, {88, 13, 0}, {48}, 5, 400, },
{"LeatherJacket", "Кожаная Куртка", Leather, {77, 30, 0}, {48}, 5, 250, {}, {8, {0, 0, 0, 0, 0, 0, 0}, {20, 20, 10, 10, 30, 500, 20}, {HMMAXX, HFMAXX}}},
{"Tool", "Инструмент", Metal, {54, 41, 0}, {48}, 1, 200, },
{"DeansElectronics", "Учебник", Wood, {55, 13, 0}, {48}, 2, 130, },
{"ElectronicLockpick", "Электронная отмычка", Metal, {57, 118, 0}, {48}, 2, 375, },
{"FuzzyPainting", "Картина", Metal, {58, 119, 0}, {48}, 12, 300, },
{"Flare", "Вспышка", Metal, {59, 46, 0}, {48}, 1, 35, {{Throw, NoAction}, SmallGuns, {1, 1}, 0, 0}},
{"FirstAidBook", "Мед Книжка", Metal, {60, 13, 0}, {48}, 2, 175, },
{"IguanaOnAStick", "Игуана-на-палочке", Leather, {62, 41, 0}, {48}, 1, 20, },
{"Key", "Ключ", Metal, {63, 41, 0}, {48}, 0, 4, },
{"KeyRing", "Кольцо для ключей", Metal, {64, 41, 0}, {48}, 0, 4, },
{"Lockpicks", "Отмычки", Metal, {66, 41, 0}, {48}, 1, 150, },
{"PlasticExplosives", "Пластиковая Взрывчатка", Metal, {70, 125, 0}, {48}, 4, 850, },
{"ScoutHandbook", "Книга скаута", Wood, {71, 13, 0}, {48}, 3, 200, },
{"Buffout", "Баффоут", Metal, {45, 15, 0}, {48}, 0, 200, },
{"Watch", "Часы", Metal, {306, 173, 0}, {48}, 1, 0, },
{"Motor", "Мотор", Metal, {68, 32, 0}, {48}, 14, 210, },
{"BackPack", "Рюкзак", Leather, {69, 2, 0}, {48}, 5, 100, },
{"DoctorSBag", "Докторская сумка", Leather, {56, 117, 0}, {48}, 5, 300, },
{"ScorpionTail", "Xвост Скорпиона", Metal, {90, 39, 0}, {48}, 20, 10, },
{"Bag", "Сумка", Leather, {138, 3, 0}, {48}, 3, 25, },
{"Shotgun", "Винтовка", Metal, {95, 36, 7}, {48}, 5, 800, {{FireSingle, NoAction}, SmallGuns, {12, 22}, 4, 2}},
{"GaShotgunShells12", "12 Винтовочных Патронов", Metal, {91, 1, 0}, {48}, 1, 225, },
{"RedPassKey", "Ключ Красного Допуска", Plastic, {79, 41, 0}, {48}, 1, 10, },
{"BluePassKey", "Ключ Синего Допуска", Plastic, {80, 41, 0}, {48}, 1, 10, },
{"Junk", "Мусор", Metal, {81, 49, 0}, {48}, 12, 40, },
{"GoldLocket", "Золотой Медальон", Metal, {82, 41, 0}, {48}, 1, 400, },
{"Radio", "Радио", Metal, {83, 6, 0}, {48}, 6, 350, },
{"Lighter", "Зажигалка", Metal, {85, 41, 0}, {48}, 0, 100, },
{"GunsAndBullets", "Пушки и Пули", Leather, {86, 13, 0}, {48}, 2, 425, },
{"IguanaOnAStick", "Игуана-на-палочке", Leather, {87, 41, 0}, {48}, 1, 8, },
{"TapeRecorder", "Магнитофон", Metal, {89, 14, 0}, {48}, 5, 80, },
{"Key", "Ключ", Metal, {94, 41, 0}, {48}, 0, 25, },
{"NukaCola", "Нюка-Кола", Metal, {93, 116, 0}, {48}, 0, 3, },
{"RadX", "Рад X", Metal, {43, 15, 0}, {48}, 0, 300, },
{"Psycho", "Психо", Metal, {92, 15, 0}, {48}, 0, 400, },
{"MagnumFMJ44", ".44 магнум ЦМ", Metal, {33, 1, 0}, {48}, 2, 50, },
{"Urn", "Урна", Metal, {97, 127, 0}, {48}, 6, 130, },
{"Robes", "Робы", Leather, {98, 52, 0}, {48}, 10, 90, {}, {5, {0, 0, 0, 0, 0, 0, 0}, {20, 25, 10, 10, 40, 500, 20}, {HAROBE, HAROBE}}},
{"TanglerSHand", "Рука Танглера", Metal, {99, 128, 0}, {48}, 3, 200, },
{"SuperSledge", "Супермолот", Metal, {100, 40, 3}, {48}, 12, 3750, {{Swing, Thrust}, SmallGuns, {18, 36}, 5, 0}},
{"Ripper", "Потрошитель", Metal, {102, 28, 1}, {48}, 2, 900, {{Swing, Thrust}, SmallGuns, {15, 32}, 4, 30}},
{"Flower", "Цветок", Metal, {101, 120, 0}, {48}, 1, 5, },
{"LaserRifle", "Лазерная Винтовка", Metal, {103, 24, 8}, {48}, 12, 5000, {{FireSingle, NoAction}, SmallGuns, {25, 50}, 6, 12}},
{"Necklace", "Ожерелье", Glass, {104, 41, 0}, {48}, 2, 700, },
{"AlienBlaster", "Инопланетный Бластер", Metal, {105, 34, 5}, {48}, 2, 10000, {{FireSingle, NoAction}, SmallGuns, {30, 90}, 2, 30}},
{"Ball9mm", "9ти мм пули", Metal, {106, 1, 0}, {48}, 2, 100, },
{"Mauser9mm", "9мм Маузер", Metal, {107, 34, 5}, {48}, 3, 1500, {{FireSingle, NoAction}, SmallGuns, {5, 10}, 3, 7}},
{"PsychicNullifier", "Психический нейтрализатор", Metal, {108, 14, 0}, {48}, 3, 1000, },
{"Beer", "Пиво", Glass, {117, 116, 0}, {48}, 0, 5, },
{"Booze", "Бузз", Glass, {121, 116, 0}, {48}, 1, 10, },
{"WaterFlask", "Фляга", Metal, {109, 41, 0}, {48}, 2, 25, },
{"Rope", "Веревка", Leather, {110, 126, 0}, {48}, 10, 25, },
{"AccessCard", "Карта Доступа", Metal, {111, 41, 0}, {48}, 1, 25, },
{"COCBadge", "Значок ДX", Metal, {112, 41, 0}, {48}, 1, 400, },
{"COCBadge", "Значок ДX", Metal, {113, 41, 0}, {48}, 1, 12, },
{"SniperRifle", "Снайперская Винтовка", Metal, {114, 36, 7}, {48}, 8, 2200, {{FireSingle, NoAction}, SmallGuns, {14, 34}, 5, 6}},
{"SuperStimpak", "Супер Стимпакет", Metal, {29, 15, 0}, {48}, 1, 350, },
{"MolotovCocktail", "Коктейль Молотова", Metal, {118, 116, 0}, {48}, 1, 50, {{Throw, NoAction}, SmallGuns, {8, 20}, 3, 0}},
{"CattleProd", "Тыкалка для скота", Metal, {129, 16, 2}, {48}, 3, 600, {{Swing, Thrust}, SmallGuns, {12, 20}, 4, 20}},
{"RedRyderBBGun", "Ружье Ред Райдер", Metal, {119, 36, 7}, {48}, 5, 200, {{FireSingle, NoAction}, SmallGuns, {1, 3}, 3, 100}},
{"RedRyderLEBBGun", "Ружье Ред Райдер ББ СС", Metal, {120, 36, 7}, {48}, 5, 3500, {{FireSingle, NoAction}, SmallGuns, {25, 25}, 3, 100}},
{"BBS", "ББы", Metal, {116, 1, 0}, {48}, 2, 20, },
{"BrotherhoodTape", "Лента Братства", Metal, {75, 41, 0}, {48}, 1, 70, },
{"FEVDisk", "ФЕБ диск.", Metal, {75, 41, 0}, {48}, 1, 200, },
{"SecurityDisk", "Диск Безопасности", Metal, {75, 41, 0}, {48}, 1, 50, },
{"AlphaExperimentDisk", "Диск Эксперимента Альфа", Metal, {75, 41, 0}, {48}, 1, 50, },
{"DeltaExperimentDisk", "Диск Эксперимента Дельта", Metal, {75, 41, 0}, {48}, 1, 50, },
{"VreeSExperimentDisk", "Диск эксперимента ВРее", Metal, {75, 41, 0}, {48}, 1, 75, },
{"BrotherhoodHonorCode", "Код Четси Братства", Metal, {0, 173, 0}, {48}, 1, 50, },
{"MutantTransmissions", "Передачи Мутантов", Metal, {75, 41, 0}, {48}, 1, 125, },
{"Flare", "Вспышка", Metal, {123, 46, 0}, {48}, 1, 60, {{Throw, NoAction}, SmallGuns, {1, 1}, 0, 0}},
{"Dynamite", "Динамит", Metal, {122, 23, 0}, {48}, 5, 500, },
{"GeigerCounter", "Счетчик Гейгера", Metal, {124, 121, 0}, {48}, 4, 650, },
{"MotionSensor", "Датчик движения", Metal, {125, 123, 0}, {48}, 7, 800, },
{"PlasticExplosives", "Пластиковая Взрывчатка", Metal, {126, 125, 0}, {48}, 4, 850, },
{"StealthBoy", "Невидимка", Metal, {127, 14, 0}, {48}, 3, 1800, },
{"Tandi", "Танди", Leather, {128, 173, 0}, {48}, 10, 600, },
{"BrotherhoodHistory", "История Братства", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MaxsonSHistory", "История Максона", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MaxsonSJournal", "Журнал Максона", Metal, {75, 41, 0}, {48}, 1, 50, },
{"LightHealing", "Незначительное излечение", Metal, {306, 173, 0}, {48}, 0, 25, },
{"MediumHealing", "Среднее Излечение", Metal, {306, 173, 0}, {48}, 0, 50, },
{"HeavyHealing", "Мощное Исцеление", Metal, {306, 173, 0}, {48}, 0, 100, },
{"SecurityCard", "Карта Безопасности", Metal, {130, 41, 0}, {48}, 0, 40, },
{"FieldSwitch", "Переключатель Полей", Metal, {131, 41, 0}, {48}, 1, 0, },
{"YellowPassKey", "Ключ Желтого Допуска", Plastic, {132, 41, 0}, {48}, 0, 40, },
{"SmallStatuette", "Маленькая статуэтка", Wood, {133, 6, 0}, {48}, 3, 300, },
{"CatSPawMagazine", "Журнал Кошачья Лапка", Leather, {134, 13, 0}, {48}, 1, 5, },
{"BoxOfNoodles", "Коробка Лапши", Wood, {135, 41, 0}, {48}, 1, 35, },
{"SmallDustyBoxOfSomeSort", "Какая-то маленькая пыльная коробка", Metal, {136, 41, 0}, {48}, 2, 20, },
{"TechnicalManual", "Инструкция по экплуатации", Metal, {137, 13, 0}, {48}, 2, 200, },
{"SmallPieceOfMachinery", "Маленький механизм", Metal, {139, 41, 0}, {48}, 1, 150, },
{"VaultRecords", "Записи Xранилища", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MilitaryBaseSecurityCode", "Код Безопасности Военной Базы", Metal, {75, 41, 0}, {48}, 1, 0, },
{"HardenedPowerArmor", "Укрепленная Броня", Metal, {15, 35, 0}, {48}, 100, 15000, {}, {25, {13, 19, 14, 13, 13, 0, 20}, {50, 90, 70, 50, 50, 505, 60}, {HAPOWR, HAPOWR}}},
{"TurboPlasmaRifle", "Турбо Плазменная Винтовка", Metal, {143, 24, 8}, {48}, 14, 10000, {{FireSingle, NoAction}, SmallGuns, {35, 70}, 6, 10}},
{"SpikedKnuckles", "Кастеты с Шипами", Metal, {20, 47, 0}, {48}, 1, 250, {{ThrowPunch, NoAction}, SmallGuns, {4, 10}, 1, 0}},
{"PowerFist", "Мощный Кулак", Metal, {141, 47, 0}, {48}, 7, 2200, {{ThrowPunch, NoAction}, SmallGuns, {12, 24}, 1, 25}},
{"CombatKnife", "Боевой Нож", Metal, {142, 28, 1}, {48}, 2, 165, {{Swing, Thrust}, SmallGuns, {3, 10}, 2, 0}},
{"ChemistryJournals", "Xимические Журналы", Metal, {88, 13, 0}, {48}, 5, 100, },
{"RegulatorTransmission", "Передача Регулятора", Metal, {75, 41, 0}, {48}, 1, 25, },
{"BrotherhoodArmor", "Броня Братства", Metal, {144, 17, 0}, {48}, 25, 4800, {}, {20, {8, 8, 7, 7, 6, 0, 8}, {40, 70, 50, 60, 60, 500, 40}, {HMCMBT, HFCMBT}}},
{"TeslaArmor", "Броня Тесла", Metal, {145, 33, 0}, {48}, 35, 4500, {}, {15, {4, 19, 4, 10, 12, 0, 4}, {20, 90, 10, 80, 80, 500, 20}, {HMMETL, HFMETL}}},
{"Pistol223", ".223 Пистолет", Metal, {146, 44, 6}, {48}, 5, 3500, {{FireSingle, NoAction}, SmallGuns, {20, 30}, 5, 5}},
{"CombatShotgun", "Боевая Винтовка", Metal, {147, 36, 7}, {48}, 10, 2750, {{FireSingle, FireBurst}, SmallGuns, {15, 25}, 5, 12}},
{"AnnaSBones", "Кости Анны", Stone, {148, 136, 0}, {48}, 10, 0, },
{"AnnaSGoldLocket", "Золотой Медальон Анны", Metal, {149, 41, 0}, {48}, 1, 0, },
{"FuelCellController", "Элемент Контроля Топлива.", Metal, {150, 6, 0}, {48}, 5, 0, },
{"FuelCellRegulator", "Элемент Топливного Регулятора", Metal, {151, 14, 0}, {48}, 5, 0, },
{"DayPass", "Дневной Пропуск", Leather, {152, 41, 0}, {48}, 1, 0, },
{"FalseCitizenshipPapers", "Фальшивые Бумаги о Гражданстве", Metal, {153, 41, 0}, {48}, 1, 0, },
{"CorneliusGoldWatch", "Золотые Часы Корнелиуса", Metal, {154, 41, 0}, {48}, 1, 0, },
{"HydroelectricPart", "Гидроэлектрическая Деталь", Metal, {155, 14, 0}, {48}, 10, 0, },
{"Jet", "Джет", Metal, {156, 15, 0}, {48}, 0, 25, },
{"JetAntidote", "Противоядие от Джет", Metal, {157, 15, 0}, {48}, 0, 25, },
{"JonnySBBGun", "ББ-Пушка Джонни", Metal, {158, 36, 7}, {48}, 5, 200, {{FireSingle, NoAction}, SmallGuns, {1, 3}, 3, 100}},
{"RubberBoots", "Резиновые Сапоги", Plastic, {159, 6, 0}, {48}, 2, 0, },
{"SlagMessage", "Послание Слагов", Metal, {160, 41, 0}, {48}, 1, 0, },
{"SmithSCoolItem", "Крутые Вещички Смита", Metal, {161, 41, 0}, {48}, 1, 0, },
{"CombatLeatherJacket", "Боевая Кожанная Куртка", Leather, {162, 30, 0}, {48}, 7, 1000, {}, {18, {2, 0, 2, 0, 0, 0, 0}, {30, 20, 25, 10, 30, 500, 20}, {HMMAXX, HFMAXX}}},
{"VicSRadio", "Радио Вика", Metal, {163, 14, 0}, {48}, 5, 50, },
{"VicSWaterFlask", "Водяная Фляжка Вика", Metal, {164, 41, 0}, {48}, 2, 0, },
{"HKCAWS", "X&К Ворон", Metal, {165, 36, 7}, {48}, 5, 4750, {{FireSingle, FireBurst}, SmallGuns, {15, 25}, 6, 10}},
{"RobotParts", "Части робота", Metal, {166, 6, 0}, {48}, 300, 0, },
{"RoboRocketLauncher", "Ракетница Робота", Metal, {167, 137, 10}, {48}, 1, 0, {{FireSingle, NoAction}, BigGuns, {10, 30}, 1, 6}},
{"BrocFlower", "Цветок Брок", Metal, {168, 139, 0}, {48}, 0, 1, },
{"XanderRoot", "Корень Ксандер", Metal, {169, 140, 0}, {48}, 0, 2, },
{"HealingPowder", "Целебный порошок", Leather, {170, 15, 0}, {48}, 1, 20, },
{"RoboRocketAmmo", "Патроны Ракетного робота", Metal, {171, 138, 0}, {48}, 10, 0, },
{"TrophyOfRecognition", "Признанный трофей", Metal, {97, 127, 0}, {48}, 10, 2000, },
{"GeckoPelt", "Шкура Гекко", Leather, {172, 14, 0}, {48}, 2, 25, },
{"GoldenGeckoPelt", "Золотая шкура Гекко", Leather, {173, 14, 0}, {48}, 5, 125, },
{"Flint", "Кремень", Stone, {174, 41, 0}, {48}, 2, 5, },
{"NeuralInterface", "Нейронный Интерфейс", Metal, {108, 6, 0}, {48}, 3, 800, },
{"SharpenedSpear", "Заостренное Копье", Wood, {176, 42, 4}, {48}, 4, 100, {{Thrust, Throw}, SmallGuns, {4, 12}, 4, 0}},
{"DixonSEye", "Глаз Диксона", Glass, {177, 155, 0}, {48}, 1, 0, },
{"CliftonSEye", "Глаз Клифтона", Glass, {178, 155, 0}, {48}, 1, 0, },
{"TommyGun", "Пушка Томми", Metal, {179, 36, 7}, {48}, 7, 1200, {{FireSingle, FireBurst}, SmallGuns, {3, 20}, 6, 50}},
{"MeatJerky", "Мясо вяленое", Leather, {180, 41, 0}, {48}, 1, 5, },
{"RadscorpionLimbs", "Клешни Радскорпиона", Stone, {181, 142, 0}, {48}, 10, 0, },
{"Firewood", "Дрова", Wood, {182, 141, 0}, {48}, 10, 0, },
{"ScopedHuntingRifle", "Снайперка для охоты", Metal, {183, 36, 7}, {48}, 11, 1500, {{FireSingle, NoAction}, SmallGuns, {8, 20}, 5, 10}},
{"CarFuelCell", "Топливная батарея автомобиля", Metal, {184, 6, 0}, {48}, 10, 0, },
{"Shovel", "Лопата", Metal, {185, 143, 0}, {48}, 10, 15, },
{"RoboMeleeWeapon1", "Оружие робота 1 ", Metal, {186, 144, 0}, {48}, 1, 0, {{ThrowPunch, NoAction}, SmallGuns, {7, 15}, 1, 0}},
{"RoboMeleeWeapon2", "Оружие робота 2", Metal, {186, 144, 0}, {48}, 1, 0, {{ThrowPunch, NoAction}, SmallGuns, {15, 30}, 1, 0}},
{"BoxingGloves", "Боксерские перчатки", Leather, {187, 47, 0}, {48}, 5, 250, {{ThrowPunch, NoAction}, SmallGuns, {1, 1}, 1, 0}},
{"PlatedBoxingGloves", "Боксерские перчатки с кастетом", Leather, {338, 47, 0}, {48}, 10, 300, {{ThrowPunch, NoAction}, SmallGuns, {2, 5}, 1, 0}},
{"Vault13Holodisk", "Голодиск 13 Убежища", Metal, {75, 41, 0}, {48}, 1, 200, },
{"CheezyPoofs", "Сырные Пуфы", Leather, {189, 41, 0}, {48}, 1, 5, },
{"HKP90c", "Xеклер П90с", Metal, {190, 44, 6}, {48}, 8, 2500, {{FireSingle, FireBurst}, SmallGuns, {12, 16}, 4, 24}},
{"MetalPole", "Металлический шест", Wood, {191, 145, 0}, {48}, 10, 5, },
{"TrapperTownKey", "Ключ Траппер-тауна", Metal, {63, 41, 0}, {48}, 0, 0, },
{"PipeRifle", "Линейная винтовка", Metal, {193, 36, 7}, {48}, 10, 200, {{FireSingle, NoAction}, SmallGuns, {5, 12}, 5, 1}},
{"ZipGun", "Зип пушка", Metal, {192, 34, 5}, {48}, 4, 150, {{FireSingle, NoAction}, SmallGuns, {5, 12}, 4, 1}},
{"Clipboard", "Клавиатура", Metal, {194, 41, 0}, {48}, 1, 0, },
{"GeckoHolodisk", "Голодиск Гекко", Metal, {75, 41, 0}, {48}, 1, 50, },
{"ReactorHolodisk", "Голодиск Реактора", Metal, {75, 41, 0}, {48}, 1, 50, },
{"YellowReactorKeycard", "Желтая Ключ-карта Реактора", Metal, {132, 41, 0}, {48}, 1, 25, },
{"RedReactorKeycard", "Красная Ключ-карта Реактора", Metal, {79, 41, 0}, {48}, 1, 25, },
{"PlasmaTransformer", "Плазмопреобразователь", Metal, {196, 41, 0}, {48}, 1, 200, },
{"SuperToolKit", "Инструментальная Супер Аптечка", Metal, {197, 146, 0}, {48}, 10, 1000, },
{"Talisman", "Талисман", Stone, {198, 41, 0}, {48}, 1, 25, },
{"GammaGulpBeer", "Гамма Галп Пиво", Glass, {199, 116, 0}, {48}, 0, 5, },
{"RoentgenRum", "Ром Роентген", Glass, {200, 116, 0}, {48}, 0, 10, },
{"PartRequisitionForm", "Часть Требуемой Формы", Leather, {201, 41, 0}, {48}, 1, 10, },
{"MagnumRevolver44", ".44 Револьвер Магнум", Metal, {202, 34, 5}, {48}, 5, 600, {{FireSingle, NoAction}, SmallGuns, {12, 18}, 5, 6}},
{"CondomBluePackage", "(Синяя упаковка) Презерватив", Plastic, {203, 41, 0}, {48}, 0, 2, },
{"CondomGreenPackage", "(Зеленая упаковка) Презерватив", Plastic, {204, 41, 0}, {48}, 0, 2, },
{"CondomRedPackage", "(Красная упаковка) Презерватив", Plastic, {205, 41, 0}, {48}, 0, 2, },
{"CosmeticsCase", "Косметичка", Plastic, {206, 41, 0}, {48}, 1, 25, },
{"EmptyHypodermic", "Пустой Гиподермик", Glass, {207, 41, 0}, {48}, 1, 2, },
{"Switchblade", "Свичблэйд", Metal, {208, 28, 1}, {48}, 1, 50, {{Swing, Thrust}, SmallGuns, {2, 5}, 1, 0}},
{"SharpenedPole", "Заостренный шест", Wood, {209, 42, 4}, {48}, 3, 5, {{Thrust, Throw}, SmallGuns, {2, 4}, 4, 0}},
{"CyberneticBrain", "Кибернетический Мозг", Metal, {210, 155, 0}, {48}, 8, 2000, },
{"HumanBrain", "Человеческий Мозг", Leather, {211, 155, 0}, {48}, 5, 0, },
{"ChimpanzeeBrain", "Мозг шимпанзе", Leather, {212, 155, 0}, {48}, 4, 0, },
{"AbnormalBrain", "Ненормальный мозг", Leather, {213, 155, 0}, {48}, 5, 0, },
{"Dice", "Кости", Plastic, {214, 41, 0}, {48}, 0, 5, },
{"LoadedDice", "Утяжеленные кости", Plastic, {215, 41, 0}, {48}, 0, 20, },
{"EasterEgg", "Пасхальное яйцо", Leather, {216, 41, 0}, {48}, 0, 1, },
{"Magic8Ball", "Волшебный 8-Болл", Metal, {217, 41, 0}, {48}, 1, 50, },
{"MutagenicSerum", "Мутагенная сыворотка", Plastic, {218, 15, 0}, {48}, 0, 0, },
{"CatSPawIssue5", "Кошачья Лапка выпуск N5", Leather, {219, 13, 0}, {48}, 0, 50, },
{"M3A1GreaseGunSMG", "М3А1 Гризли SMG", Metal, {220, 44, 6}, {48}, 7, 1750, {{FireSingle, FireBurst}, SmallGuns, {10, 20}, 4, 30}},
{"HeartPills", "Сердечные таблетки", Plastic, {221, 15, 0}, {48}, 0, 50, },
{"Poison", "Отрава", Plastic, {222, 15, 0}, {48}, 0, 200, },
{"MooreSBriefcase", "Саквояж Мура", Leather, {52, 5, 0}, {48}, 5, 35, },
{"MooreSBriefcase", "Саквояж Мура", Leather, {52, 5, 0}, {48}, 5, 35, },
{"LynetteHolodisk", "Голодиск Линетт", Metal, {75, 41, 0}, {48}, 1, 50, },
{"WestinHolodisk", "Голодиск Вестина", Metal, {75, 41, 0}, {48}, 1, 50, },
{"NCRSpyHolodisk", "НКР Шпионский голодиск", Metal, {75, 41, 0}, {48}, 1, 50, },
{"DoctorSPapers", "Записки Доктора", Leather, {223, 41, 0}, {48}, 0, 0, },
{"PresidentialPass", "Президентский пропуск", Leather, {224, 41, 0}, {48}, 0, 0, },
{"RangerPin", "Удостоверение Рэйнджера", Metal, {225, 41, 0}, {48}, 1, 10, },
{"RangerSMap", "Карта Рэйнджера", Metal, {226, 41, 0}, {48}, 1, 0, },
{"AdvancedPowerArmor", "Улучшенная Силовая Броня", Metal, {227, 152, 0}, {48}, 90, 20000, {}, {30, {15, 19, 16, 15, 15, 0, 20}, {55, 90, 70, 60, 60, 500, 65}, {HANPWR, HANPWR}}},
{"AdvPowerArmorMKII", "Усил. Силовая Броня МКII", Metal, {227, 152, 0}, {48}, 100, 25000, {}, {35, {18, 19, 16, 18, 15, 0, 20}, {60, 90, 70, 60, 65, 500, 70}, {HANPWR, HANPWR}}},
{"Bozar", "Бозар", Metal, {228, 24, 8}, {48}, 20, 5250, {{FireBurst, NoAction}, BigGuns, {25, 35}, 6, 30}},
{"FNFAL", "ФН ФАЛ", Metal, {229, 36, 7}, {48}, 9, 1500, {{FireSingle, FireBurst}, SmallGuns, {9, 18}, 5, 20}},
{"HKG11", "Xеклер Г11", Metal, {230, 44, 6}, {48}, 8, 6500, {{FireSingle, FireBurst}, SmallGuns, {10, 20}, 5, 50}},
{"XL70E3", "XЛ70Е3", Metal, {231, 36, 7}, {48}, 9, 3000, {{FireSingle, FireBurst}, SmallGuns, {12, 19}, 5, 20}},
{"PancorJackhammer", "Панкор Джекхаммер", Metal, {232, 36, 7}, {48}, 12, 5500, {{FireSingle, FireBurst}, SmallGuns, {18, 29}, 5, 10}},
{"LightSupportWeapon", "Пулемет легкой поддержки", Metal, {233, 24, 8}, {48}, 20, 4750, {{FireBurst, NoAction}, BigGuns, {20, 30}, 6, 30}},
{"ComputerVoiceModule", "Компьютерный Голосовой Модуль", Metal, {234, 41, 0}, {48}, 1, 3000, },
{"Caliber45", ".45 Калибр", Metal, {236, 1, 0}, {48}, 1, 75, },
{"EC2mm", "2мм ЕС", Metal, {235, 1, 0}, {48}, 1, 400, },
{"Caseless47mm", "4.7 мм безотказные", Metal, {237, 1, 0}, {48}, 1, 600, },
{"9mm", "9 мм", Metal, {238, 1, 0}, {48}, 1, 100, },
{"HNNeedlerCartridge", "XН Нидлер Картридж", Metal, {240, 1, 0}, {48}, 1, 250, },
{"HNAPNeedlerCartridge", "XН АП Нидлер Картридж", Metal, {239, 1, 0}, {48}, 1, 300, },
{"762mm", "7.62 мм", Metal, {241, 1, 0}, {48}, 1, 150, },
{"RobotMotivator", "Двигатель робота", Metal, {68, 32, 0}, {48}, 20, 50, },
{"PlantSpike", "Шип растения", Metal, {242, 153, 0}, {48}, 0, 0, {{Throw, NoAction}, SmallGuns, {1, 2}, 2, 0}},
{"GECK", "Г.Э.К.К.", Metal, {243, 154, 0}, {48}, 10, 0, },
{"Claw", "Коготь", Metal, {244, 160, 0}, {48}, 1, 0, {{ThrowPunch, NoAction}, SmallGuns, {3, 4}, 1, 0}},
{"Claw", "Коготь", Metal, {245, 161, 0}, {48}, 1, 0, {{ThrowPunch, NoAction}, SmallGuns, {5, 10}, 1, 0}},
{"Vault15Keycard", "Ключ-карта 15 Убежища", Metal, {79, 41, 0}, {48}, 1, 0, },
{"Vault15ComputerParts", "Компьютерные части с 15 Убежища", Metal, {246, 14, 0}, {48}, 10, 100, },
{"Cookie", "Печенье", Leather, {247, 41, 0}, {48}, 0, 0, },
{"LeatherArmorMarkII", "Кожаная Броня Марк II", Leather, {250, 29, 0}, {48}, 10, 1000, {}, {20, {3, 1, 1, 1, 1, 0, 1}, {25, 20, 25, 10, 40, 500, 25}, {HMLTHR, HFLTHR}}},
{"MetalArmorMarkII", "Металлическая Броня Марк II", Metal, {248, 33, 0}, {48}, 35, 1900, {}, {15, {4, 7, 4, 4, 1, 0, 4}, {35, 80, 15, 25, 10, 500, 30}, {HMMETL, HFMETL}}},
{"CombatArmorMarkII", "Боевая Броня Марк II", Metal, {249, 17, 0}, {48}, 25, 8000, {}, {25, {6, 9, 5, 5, 3, 0, 9}, {40, 65, 35, 50, 55, 500, 45}, {HMCMBT, HFCMBT}}},
{"FlamethrowerFuelMKII", "Огнеметная смесь МКII ", Metal, {260, 43, 0}, {48}, 10, 250, },
{"Shiv", "Шив", Metal, {275, 28, 1}, {51}, 1, 2, {{Swing, Thrust}, SmallGuns, {1, 4}, 1, 0}},
{"Wrench", "Гаечный ключ", Metal, {280, 16, 2}, {48}, 4, 65, {{Swing, NoAction}, SmallGuns, {3, 6}, 3, 0}},
{"SawedOffShotgun", "Обрез", Metal, {278, 36, 7}, {48}, 4, 800, {{FireSingle, NoAction}, SmallGuns, {12, 24}, 4, 2}},
{"LouisvilleSlugger", "Луисвильский Слюггер", Wood, {276, 16, 2}, {48}, 4, 2700, {{Swing, NoAction}, SmallGuns, {12, 30}, 4, 0}},
{"M60", "М60", Metal, {266, 24, 8}, {48}, 23, 3500, {{FireBurst, NoAction}, BigGuns, {18, 26}, 7, 50}},
{"NeedlerPistol", "Пистолет Нидлер", Metal, {270, 34, 5}, {48}, 4, 2200, {{FireSingle, NoAction}, SmallGuns, {12, 24}, 3, 10}},
{"AvengerMinigun", "Минипушка Авенджер", Metal, {253, 31, 9}, {48}, 28, 5500, {{FireBurst, NoAction}, BigGuns, {10, 14}, 7, 120}},
{"SolarScorcher", "Солнечный Скорчер", Metal, {277, 34, 5}, {48}, 5, 2000, {{FireSingle, NoAction}, SmallGuns, {20, 60}, 3, 6}},
{"HKG11E", "Xеклер Г11Е", Metal, {263, 44, 6}, {48}, 8, 8000, {{FireSingle, FireBurst}, SmallGuns, {13, 23}, 6, 50}},
{"M72GaussRifle", "Винтовка Гаусса М72", Metal, {267, 36, 7}, {48}, 9, 8250, {{FireSingle, NoAction}, SmallGuns, {32, 43}, 6, 20}},
{"Phazer", "Фазер", Metal, {271, 34, 5}, {48}, 4, 7500, {{FireSingle, NoAction}, SmallGuns, {20, 30}, 3, 12}},
{"PPK12GaussPistol", "Пистолет Гаусса ППК12 ", Metal, {262, 34, 5}, {48}, 5, 5250, {{FireSingle, NoAction}, SmallGuns, {22, 32}, 4, 12}},
{"VindicatorMinigun", "Минипушка Виндикатора ", Metal, {279, 31, 9}, {48}, 28, 15250, {{FireBurst, NoAction}, BigGuns, {14, 19}, 7, 100}},
{"YK32PulsePistol", "Импульсный пистолет ЮК32", Metal, {273, 34, 5}, {35}, 5, 12500, {{FireSingle, NoAction}, SmallGuns, {32, 46}, 3, 5}},
{"YK42BPulseRifle", "Импульсная винтовка ЮК42Б", Metal, {274, 24, 8}, {48}, 9, 17500, {{FireSingle, NoAction}, SmallGuns, {54, 78}, 6, 10}},
{"MagnumSpeedLoad44", ".44 Магнум (Скоростная перезарядка)", Metal, {252, 34, 5}, {48}, 5, 600, {{FireSingle, NoAction}, SmallGuns, {12, 18}, 5, 6}},
{"SuperCattleProd", "Супер погонщик скота", Metal, {255, 16, 2}, {48}, 3, 600, {{Swing, Thrust}, SmallGuns, {20, 32}, 4, 20}},
{"ImprovedFlamer", "Улучшенный Огнемет", Metal, {264, 24, 8}, {48}, 19, 2000, {{FireContinuous, NoAction}, BigGuns, {45, 90}, 6, 5}},
{"LaserRifleExtCap", "Лазерная винтовка (Улучшенные Батареи)", Metal, {265, 24, 8}, {48}, 12, 5000, {{FireSingle, NoAction}, SmallGuns, {25, 50}, 6, 24}},
{"MagnetoLaserPistol", "Магнито-Лазерный Пистолет", Metal, {268, 34, 5}, {48}, 4, 1400, {{FireSingle, NoAction}, SmallGuns, {10, 22}, 3, 12}},
{"FNFALNightSight", "ФН ФАЛ (Инфракрасный Прицел)", Metal, {261, 36, 7}, {48}, 10, 2000, {{FireSingle, FireBurst}, SmallGuns, {9, 18}, 5, 20}},
{"DesertEagleExpMag", "Пустынный Орел (Увел. Маг.)", Metal, {256, 34, 5}, {48}, 4, 800, {{FireSingle, NoAction}, SmallGuns, {10, 16}, 4, 20}},
{"AssaultRifleExpMag", "Штурмовая винтовка (Увел. Маг.)", Metal, {254, 36, 7}, {48}, 8, 1300, {{FireSingle, FireBurst}, SmallGuns, {8, 16}, 5, 100}},
{"PlasmaPistolExtCap", "Плазменный Пистолет (Увел. Емк.)", Metal, {272, 34, 5}, {48}, 5, 2750, {{FireSingle, NoAction}, SmallGuns, {15, 35}, 4, 32}},
{"MegaPowerFist", "Мега Силовой Кулак", Metal, {269, 47, 0}, {48}, 7, 2200, {{ThrowPunch, NoAction}, SmallGuns, {20, 40}, 1, 25}},
{"FieldMedicFirstAidKit", "Полевая медаптечка ", Metal, {251, 15, 0}, {48}, 2, 400, },
{"ParamedicsBag", "Парамедицинская аптечка", Leather, {257, 117, 0}, {48}, 6, 600, },
{"ExpandedLockpickSet", "Спецнабор Взломщика", Metal, {259, 41, 0}, {48}, 1, 400, },
{"ElectronicLockpickMKII", "Электронный Взломщик МКII", Metal, {258, 118, 0}, {48}, 2, 700, },
{"OilCan", "Канистра Смазки", Metal, {281, 155, 0}, {48}, 3, 150, },
{"StablesIDBadge", "ID значок с конюшни", Plastic, {282, 41, 0}, {48}, 0, 0, },
{"Vault15ShackKey", "Ключ от 15го Убежища", Metal, {318, 41, 0}, {48}, 0, 0, },
{"Spectacles", "Линзы", Metal, {283, 41, 0}, {48}, 0, 50, },
{"EmptyJetCanister", "Пустая Джет канистра", Metal, {284, 15, 0}, {48}, 0, 0, },
{"OxygenTank", "Кислородный баллон", Metal, {285, 14, 0}, {48}, 5, 20, },
{"PoisonTank", "Баллон с ядом", Metal, {286, 6, 0}, {48}, 5, 0, },
{"MineParts", "Запчасти к Шахте ", Metal, {287, 14, 0}, {48}, 10, 0, },
{"MorningstarMineScrip", "Бумага Шахты Утренняя Звезда", Leather, {288, 41, 0}, {48}, 0, 1, },
{"HolyHandGrenade", "Святая ручная граната", Metal, {289, 122, 0}, {48}, 2, 1, {{Throw, NoAction}, SmallGuns, {300, 500}, 2, 0}},
{"ExcavatorChip", "Чип Экскаватора", Metal, {290, 48, 0}, {48}, 1, 50, },
{"GoldNugget", "Золотой Самородок", Metal, {291, 165, 0}, {48}, 10, 50, {{Throw, ThrowPunch}, SmallGuns, {3, 6}, 2, 0}},
{"MonumentChunk", "Кусок Монумента", Metal, {292, 53, 0}, {48}, 5, 0, },
{"UraniumOre", "Урановая руда", Metal, {293, 165, 0}, {48}, 10, 0, {{Throw, ThrowPunch}, SmallGuns, {3, 6}, 2, 0}},
{"FlameBreath", "Огненное дыхание", Metal, {294, 167, 8}, {48}, 1, 0, {{FireContinuous, NoAction}, SmallGuns, {35, 50}, 1, 4}},
{"MedicalSupplies", "Медикаменты", Leather, {295, 15, 0}, {48}, 25, 50, },
{"GoldTooth", "Золотой зуб", Stone, {296, 41, 0}, {48}, 0, 50, },
{"HowitzerShell", "Снаряд Xовитчера", Metal, {297, 38, 0}, {48}, 40, 20, },
{"RamirezBoxClosed", "Коробка Рамиреса, закрытая", Wood, {298, 41, 0}, {48}, 2, 500, },
{"RamirezBoxOpen", "Коробка Рамиреса, открытая", Wood, {299, 41, 0}, {48}, 2, 0, },
{"MirroredShades", "Зеркальные Тени", Plastic, {300, 41, 0}, {48}, 1, 20, },
{"DeckOfCards", "Колода карт", Leather, {301, 41, 0}, {48}, 1, 2, },
{"PackOfMarkedCards", "Упаковка меченых карт", Leather, {302, 41, 0}, {48}, 1, 1, },
{"TempleKey", "Ключ к Xраму", Metal, {303, 41, 0}, {48}, 0, 0, },
{"PocketLint", "Карманная Корпия", Leather, {305, 41, 0}, {48}, 0, 0, },
{"BioMedGel", "Биомедицинский Гель", Glass, {304, 155, 0}, {48}, 1, 0, },
{"BlondieSDogTags", "Собачьи бирки Блондинки", Metal, {73, 41, 0}, {48}, 0, 0, },
{"AngelEyesDogTags", "Собачьи бирки Ангельских Глазок", Metal, {73, 41, 0}, {48}, 0, 0, },
{"TucoSDogTags", "Собачьи бирки Туко", Metal, {73, 41, 0}, {48}, 0, 0, },
{"RaidersMap", "Карта Рейдеров", Metal, {307, 41, 0}, {48}, 1, 0, },
{"SheriffSBadge", "Значок Шерифа", Metal, {308, 41, 0}, {48}, 1, 5, },
{"VertibirdPlans", "Планы Вертиберда", Metal, {309, 41, 0}, {48}, 0, 0, },
{"BishopSHolodisk", "Голодиск Бишопа.", Metal, {75, 41, 0}, {48}, 1, 50, },
{"AccountBook", "Бухгалтерская книга", Metal, {310, 41, 0}, {48}, 2, 0, },
{"Unused", "Неиспользованно", Metal, {306, 173, 0}, {48}, 1, 0, },
{"TornPaper1", "Обрывок Бумаги 1", Leather, {311, 41, 0}, {48}, 0, 0, },
{"TornPaper2", "Обрывок Бумаги 2", Leather, {312, 41, 0}, {48}, 0, 0, },
{"TornPaper3", "Обрывок Бумаги 3", Leather, {313, 41, 0}, {48}, 0, 0, },
{"PasswordPaper", "Бумажка с Паролем", Leather, {314, 41, 0}, {48}, 0, 0, },
{"ExplosiveSwitch", "Взрыватель", Metal, {131, 41, 0}, {48}, 5, 0, },
{"CellDoorKey", "Ключ от Камеры", Metal, {320, 41, 0}, {48}, 0, 0, },
{"HubologistFieldReport", "Xабологистический Полевой Рапорт", Wood, {316, 41, 0}, {48}, 1, 0, },
{"MBHolodisk5", "М.Б. Голодиск 5", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MBHolodisk1", "М.Б. Голодиск 1", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MBHolodisk2", "М.Б. Голодиск 2", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MBHolodisk3", "М.Б. Голодиск 3", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MBHolodisk4", "М.Б. Голодиск 4", Metal, {75, 41, 0}, {48}, 1, 50, },
{"EvacuationHolodisk", "Голодиск Эвакуации", Metal, {75, 41, 0}, {48}, 1, 50, },
{"ExperimentHolodisk", "Голодиск Эксперимента", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MedicalHolodisk", "Голодиск Медицины", Metal, {75, 41, 0}, {48}, 1, 50, },
{"PasswordPaper", "Бумажка с Паролем", Metal, {335, 41, 0}, {48}, 1, 5, },
{"SmittySMeal", "Еда Смитти", Metal, {317, 41, 0}, {48}, 3, 0, },
{"RotGut", "Гнилая Кишка", Metal, {321, 116, 0}, {48}, 0, 5, },
{"BallGag", "Шаровой Кляп", Metal, {322, 41, 0}, {48}, 2, 20, },
{"HubologistHolodisk", "Xабологистический Голодиск", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MutatedToe", "Мутировавший Палец Ноги", Metal, {324, 15, 0}, {48}, 1, 0, },
{"Daisies", "Дэйзис", Cement, {325, 14, 0}, {48}, 5, 0, },
{"Unused", "Неиспользованно", Metal, {306, 173, 0}, {48}, 0, 0, },
{"EnlightenedOneSLetter", "Чье-то Подсвеченное письмо", Metal, {336, 41, 0}, {48}, 0, 0, },
{"BroadcastHolodisk", "Голодиск Радиовещания", Metal, {75, 41, 0}, {48}, 1, 50, },
{"SierraMissionHolodisk", "Голодиск Миссии Сиерра", Metal, {75, 41, 0}, {48}, 1, 50, },
{"NavComParts", "Запчасти от НавКома", Metal, {337, 41, 0}, {48}, 1, 0, },
{"Bonus1Agility", "Бонус +1 Ловкость", Metal, {339, 15, 0}, {48}, 0, 0, },
{"Bonus1Intelligence", "Бонус +1 Интеллект", Metal, {340, 15, 0}, {48}, 0, 0, },
{"Bonus1Strength", "Бонус +1 Сила", Metal, {341, 15, 0}, {48}, 0, 0, },
{"Fallout2Hintbook", "Книга подсказок по Возрождению 2", Leather, {342, 13, 0}, {48}, 0, 0, },
{"PlayerSEar", "Ухо Игрока", Metal, {343, 41, 0}, {48}, 0, 0, },
{"MasticatorSEar", "Ухо Мастикатора", Metal, {344, 41, 0}, {48}, 0, 750, },
{"RefinedUraniumOre", "Обогащенная урановая руда", Metal, {293, 165, 0}, {48}, 15, 0, {{Throw, ThrowPunch}, SmallGuns, {3, 6}, 2, 0}},
{"NoteFromFrancis", "Записка от Франциска", Leather, {334, 41, 0}, {48}, 0, 0, },
{"K9Motivator", "Мотиватор К-9", Metal, {68, 32, 0}, {48}, 20, 50, },
{"SpecialBoxerWeapon", "Специальное оружие боксера", Metal, {345, 172, 2}, {48}, 0, 0, {{Swing, Thrust}, SmallGuns, {1, 1}, 1, 0}},
{"NCRHistoryHolodisk", "Голодиск Истории НКР", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MrNixonDoll", "Кукла Мистера Диксона", Leather, {346, 41, 0}, {48}, 0, 0, },
{"TankerFob", "Обман Танкера", Metal, {347, 41, 0}, {48}, 0, 0, },
{"TeachingsHolodisk", "Голодиск Учений", Metal, {75, 41, 0}, {48}, 1, 50, },
{"KokoweefMineScrip", "Акция Шахт Коковиф", Leather, {348, 41, 0}, {48}, 0, 1, },
{"PresidentialAccessKey", "Президентский Ключ Доступа", Metal, {349, 41, 0}, {48}, 0, 0, },
{"BoxingGloves", "Боксерские перчатки", Leather, {187, 47, 0}, {48}, 5, 250, {{ThrowPunch, NoAction}, SmallGuns, {1, 1}, 1, 0}},
{"PlatedBoxingGloves", "Боксерские перчатки с кастетом", Leather, {338, 47, 0}, {48}, 10, 300, {{ThrowPunch, NoAction}, SmallGuns, {2, 5}, 1, 0}},
{"DualPlasmaCannon", "Сдвоенная Плазменная Пушка", Metal, {350, 174, 5}, {48}, 10, 0, {{FireSingle, NoAction}, SmallGuns, {45, 90}, 1, 200}},
{"PipBoyLingualEnhancer", "Лингвистический Усилитель ПипБоя", Metal, {351, 41, 0}, {48}, 8, 200, },
{"FNFALHPFA", "ФН ФОЛ XПФА", Metal, {229, 36, 7}, {48}, 10, 2250, {{FireSingle, FireBurst}, SmallGuns, {11, 22}, 5, 20}},
{"BlueMemoryModule", "Голубой Модуль Памяти", Metal, {352, 41, 0}, {48}, 0, 0, },
{"GreenMemoryModule", "Зеленый Модуль Памяти", Metal, {353, 41, 0}, {48}, 0, 0, },
{"RedMemoryModule", "Красный Модуль Памяти", Metal, {354, 41, 0}, {48}, 0, 0, },
{"YellowMemoryModule", "Желтый Модуль Памяти", Metal, {355, 41, 0}, {48}, 0, 0, },
{"DecomposingBody", "Расчленненое Тело", Leather, {356, 177, 0}, {48}, 125, 0, },
{"RubberDoll", "Резиновая Кукла", Plastic, {357, 6, 0}, {48}, 20, 50, },
{"DamagedRubberDoll", "Подпорченная Резиновая Кукла", Plastic, {358, 14, 0}, {48}, 20, 0, },
{"PipBoyMedicalEnhancer", "Медицинский Усилитель к ПипБою", Metal, {359, 41, 0}, {48}, 8, 200, },
{"DualMinigun", "Сдвоеный Пулемет", Metal, {361, 184, 9}, {48}, 10, 0, {{FireBurst, NoAction}, BigGuns, {10, 14}, 1, 800}},
{"BottleCaps", "Крышки от бутылок", Metal, {362, 3, 0}, {48}, 0, 0, },
{"HeavyDualMinigun", "Тяжелый Сдвоенный Пулемет", Metal, {361, 184, 9}, {48}, 10, 0, {{FireBurst, NoAction}, BigGuns, {16, 24}, 1, 800}},
{"WakizashiBlade", "Лезвие Вакизаши", Metal, {363, 28, 1}, {48}, 3, 200, {{Swing, Thrust}, SmallGuns, {4, 12}, 2, 0}},
{"SurveyMap", "Топографическая Карта", Leather, {307, 41, 0}, {48}, 0, 0, },
{"BridgekeeperSRobes", "Одеяние Xранителя Моста", Leather, {98, 52, 0}, {48}, 10, 5, {}, {20, {5, 8, 4, 4, 2, 0, 6}, {40, 60, 30, 50, 50, 500, 40}, {HAROBE, HAROBE}}},
{"Hypo", "Гипо", Metal, {364, 15, 0}, {48}, 0, 500, },
{"EndBossKnife", "Нож-убийца Босса", Metal, {365, 190, 1}, {48}, 1, 0, {{Swing, Thrust}, SmallGuns, {15, 30}, 2, 0}},
{"EndBossPlasmaGun", "Плазменная пушка Босса", Metal, {366, 191, 8}, {48}, 1, 0, {{FireSingle, FireBurst}, BigGuns, {30, 70}, 0, 50}},
};
getstr_enum(item);

const char* get_item_id(int i) {
	if(i >= sizeof(item_data) / sizeof(item_data[0]))
		return 0;
	return item_data[i].id;
}

item::item(item_s type, int count) : type(type), count(0) {
	setcount(count);
	auto ammo_cap = getcapacity();
	auto ammo_type = getammo(0);
	if(ammo_cap)
		setammo(ammo_type, ammo_cap);
}

void item::clear() {
	*((int*)this) = 0;
}

unsigned short item::get(item_sprite_s i) const {
	return item_data[type].frame[i];
}

bool item::isweapon() const {
	return item_data[type].weapon.damage.max != 0;
}

bool item::isarmor() const {
	return item_data[type].armor.dress[0] != res::NoRes;
}

int item::getcapacity() const {
	return item_data[type].weapon.capacity;
}

int	item::getweight() const {
	return item_data[type].weight;
}

res::tokens	item::getdress(gender_s gender) const {
	return item_data[type].armor.dress[gender];
}

int	item::getminst() const {
	return item_data[type].weapon.minst;
}

bool item::ismatch(const item& it) const {
	if(type != it.type)
		return false;
	return isweapon() ?
		(ammo_count == it.ammo_count && ammo_index == it.ammo_index) : (count == it.count);
}

void item::join(item& it) {
	if(!ismatch(it))
		return;
	auto src_count = it.getcount();
	auto old_count = getcount();
	setcount(old_count + src_count);
	auto new_count = getcount();
	it.setcount(src_count - (new_count - old_count));
}

int item::getcount() const {
	if(isweapon())
		return weapon_count + 1;
	return count + 1;
}

item_s item::getammo() const {
	if(ammo_count == 0)
		return NoItem;
	return item_data[type].weapon.ammo[ammo_index];
}

bool item::setammo(item_s type, int count) {
	if(type == NoItem || count <= 0)
		return false;
	auto result_ammo_index = getammoindex(type);
	if(result_ammo_index == -1)
		return false;
	if(count > 255)
		count = 255;
	ammo_index = (unsigned char)result_ammo_index;
	ammo_count = (unsigned char)count;
	return true;
}

item_s item::getammo(int index) const {
	return item_data[type].weapon.ammo[index];
}

int	item::getammocount() const {
	if(!isweapon())
		return 0;
	return ammo_count;
}

void item::setammocount(int value) {
	if(!isweapon())
		return;
	if(value < 0)
		value = 0;
	if(value > 255) // TODO: Использовать размер магазина
		value = 255;
	ammo_count = value;
}

void item::setcount(int value) {
	if(value <= 0)
		clear();
	else if(isweapon()) {
		if(value > 32)
			value = 32;
		weapon_count = value - 1;
	} else {
		if(value > 256 * 256)
			value = 256 * 256;
		count = value - 1;
	}
}

const char*	item::getdescription() const {
	return item_data[type].description;
}

const char* item::getname() const {
	return item_data[type].name;
}

int item::getweaponindex() const {
	return item_data[type].frame[FrameWeapon];
}

int	item::getresistance(damage_s id) const {
	return item_data[type].armor.resistance[id];
}

int	item::getthreshold(damage_s id) const {
	return item_data[type].armor.threshold[id];
}

const attack_info& item::getattack() const {
	return item_data[type].weapon;
}

int	item::getammoindex(item_s ammo_type) const {
	const auto im = sizeof(item_data[type].weapon.ammo) / sizeof(item_data[type].weapon.ammo[0]);
	for(unsigned i = 0; i < im; i++) {
		if(item_data[type].weapon.ammo[i] == ammo_type)
			return i;
	}
	return -1;
}