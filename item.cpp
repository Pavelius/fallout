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
static item_info item_data[] = {{"��� ��������"},
{"MetalArmor", "������������� �����", Metal, {1, 33, 0}, {48}, 35, 1100, {}, {10, {4, 6, 4, 4, 0, 0, 4}, {30, 75, 10, 20, 0, 500, 25}, {HMMETL, HFMETL}}},
{"SMG10mm", "10�� ��", Metal, {2, 44, 6}, {48}, 5, 1000, {{FireSingle, FireBurst}, SmallGuns, {5, 12}, 4, 30}},
{"LeatherArmor", "������� �����", Leather, {3, 29, 0}, {48}, 8, 700, {}, {15, {2, 0, 0, 0, 0, 0, 0}, {25, 20, 20, 10, 30, 500, 20}, {HMLTHR, HFLTHR}}},
{"Pistol10mm", "10�� ��������", Metal, {4, 34, 5}, {48}, 3, 250, {{FireSingle, NoAction}, SmallGuns, {5, 12}, 3, 12}},
{"Sledgehammer", "�������", Metal, {5, 40, 3}, {48}, 12, 120, {{Swing, Thrust}, SmallGuns, {4, 9}, 6, 0}},
{"Minigun", "�������", Metal, {6, 31, 9}, {48}, 28, 3800, {{FireBurst, NoAction}, BigGuns, {7, 11}, 7, 120}},
{"RocketLauncher", "������", Metal, {7, 37, 10}, {48}, 15, 2300, {{FireSingle, NoAction}, BigGuns, {35, 100}, 6, 1}},
{"ExplosiveRocket", "�������� ������", Metal, {28, 38, 0}, {48}, 3, 200, },
{"Club", "�������", Wood, {9, 16, 2}, {48}, 3, 30, {{Swing, NoAction}, SmallGuns, {1, 6}, 3, 0}},
{"Flamer", "�������", Metal, {10, 24, 8}, {48}, 18, 2000, {{FireContinuous, NoAction}, BigGuns, {45, 90}, 6, 5}},
{"HuntingRifle", "��������� �����", Metal, {23, 36, 7}, {48}, 9, 1000, {{FireSingle, NoAction}, SmallGuns, {8, 20}, 5, 10}},
{"Knife", "���", Metal, {96, 28, 1}, {51}, 1, 40, {{Swing, Thrust}, SmallGuns, {1, 6}, 2, 0}},
{"Spear", "�����", Wood, {13, 42, 4}, {48}, 4, 80, {{Thrust, Throw}, SmallGuns, {3, 10}, 4, 0}},
{"PowerArmor", "������������� �����", Metal, {15, 35, 0}, {48}, 85, 12500, {}, {25, {12, 18, 12, 10, 12, 15, 20}, {40, 80, 60, 40, 40, 500, 50}, {HAPOWR, HAPOWR}}},
{"PlasmaRifle", "���������� ��������", Metal, {14, 24, 8}, {48}, 12, 4000, {{FireSingle, NoAction}, SmallGuns, {30, 65}, 6, 10}},
{"LaserPistol", "�������� ��������", Metal, {21, 34, 5}, {48}, 4, 1400, {{FireSingle, NoAction}, SmallGuns, {10, 22}, 3, 12}},
{"CombatArmor", "������ �����", Metal, {16, 17, 0}, {48}, 20, 6500, {}, {20, {5, 8, 4, 4, 2, 0, 6}, {40, 60, 30, 50, 50, 500, 40}, {HMCMBT, HFCMBT}}},
{"DesertEagle44", "��������� ���� .44", Metal, {18, 34, 5}, {48}, 4, 800, {{FireSingle, NoAction}, SmallGuns, {10, 16}, 4, 8}},
{"Rock", "������", Stone, {17, 53, 0}, {48}, 1, 0, {{Throw, ThrowPunch}, SmallGuns, {1, 4}, 1, 0}},
{"Crowbar", "�����", Metal, {19, 16, 2}, {48}, 5, 65, {{Swing, NoAction}, SmallGuns, {3, 10}, 5, 0}},
{"BrassKnuckles", "������������� �������", Metal, {140, 47, 0}, {48}, 1, 40, {{ThrowPunch, NoAction}, SmallGuns, {2, 5}, 1, 0}},
{"Pistol14mm", "14�� ��������", Metal, {22, 44, 6}, {48}, 4, 1100, {{FireSingle, NoAction}, SmallGuns, {12, 22}, 4, 6}},
{"AssaultRifle", "��������� ��������", Metal, {11, 36, 7}, {48}, 7, 1300, {{FireSingle, FireBurst}, SmallGuns, {8, 16}, 5, 24}},
{"PlasmaPistol", "���������� ��������", Metal, {24, 34, 5}, {48}, 4, 2750, {{FireSingle, NoAction}, SmallGuns, {15, 35}, 4, 16}},
{"GrenadeFrag", "�������", Metal, {25, 122, 0}, {48}, 1, 150, {{Throw, NoAction}, SmallGuns, {20, 35}, 3, 0}},
{"GrenadePlasma", "������� (������)", Metal, {26, 122, 0}, {48}, 1, 300, {{Throw, NoAction}, SmallGuns, {40, 90}, 4, 0}},
{"GrenadePulse", "������� (�����)", Metal, {27, 122, 0}, {48}, 1, 250, {{Throw, NoAction}, SmallGuns, {100, 150}, 4, 0}},
{"GatlingLaser", "����� ��������", Metal, {41, 31, 9}, {48}, 24, 7500, {{FireBurst, NoAction}, SmallGuns, {20, 40}, 6, 30}},
{"JHP10mm", "10�� ����", Metal, {31, 1, 0}, {48}, 1, 75, },
{"AP10mm", "10�� ��", Metal, {30, 1, 0}, {48}, 1, 100, },
{"MagnumJHP44", ".44 ������ ����", Metal, {34, 1, 0}, {48}, 1, 50, },
{"FlamethrowerFuel", "������� ��� ��������", Metal, {74, 43, 0}, {48}, 10, 250, },
{"AP14mm", "14�� ��", Metal, {32, 1, 0}, {48}, 1, 150, },
{"FMJ223", ".223 ��", Metal, {35, 1, 0}, {48}, 2, 200, },
{"JHP5mm", "5�� ����", Metal, {39, 1, 0}, {48}, 1, 100, },
{"AP5mm", "5�� ��", Metal, {38, 1, 0}, {48}, 1, 120, },
{"RocketAP", "������ ��", Metal, {8, 38, 0}, {48}, 3, 400, },
{"SmallEnergyCell", "����� ������-����", Metal, {36, 4, 0}, {48}, 3, 400, },
{"MicroFusionCell", "������� ���������", Metal, {37, 4, 0}, {48}, 5, 1000, },
{"Stimpak", "����������", Metal, {115, 15, 0}, {48}, 0, 175, },
{"Money", "������", Metal, {40, 3, 0}, {48}, 0, 1, },
{"IceChest", "������", Metal, {50, 27, 0}, {67}, 30, 0, },
{"IceChest", "������", Metal, {50, 26, 0}, {67}, 30, 0, },
{"ThrowingKnife", "����������� ���", Metal, {78, 28, 1}, {52}, 1, 100, {{Throw, Swing}, SmallGuns, {3, 6}, 3, 0}},
{"Bag", "�����", Leather, {138, 3, 0}, {48}, 3, 25, },
{"FirstAidKit", "�������", Metal, {42, 15, 0}, {48}, 2, 200, },
{"RadAway", "���-����", Plastic, {48, 15, 0}, {48}, 0, 500, },
{"Antidote", "�����������", Metal, {44, 15, 0}, {48}, 1, 50, },
{"ReservedItem", "��������� �������", Metal, {45, 173, 0}, {48}, 10, 0, },
{"Dynamite", "�������", Metal, {46, 23, 0}, {48}, 5, 500, },
{"GeigerCounter", "������� �������", Metal, {47, 121, 0}, {48}, 4, 650, },
{"Mentats", "������", Metal, {72, 15, 0}, {48}, 0, 280, },
{"StealthBoy", "���������", Metal, {49, 14, 0}, {48}, 3, 1800, },
{"WaterChip", "������� ���", Metal, {84, 48, 0}, {48}, 2, 1, },
{"DogTags", "������� �����", Metal, {73, 41, 0}, {48}, 0, 50, },
{"Bug", "�����", Metal, {53, 41, 0}, {48}, 2, 50, },
{"Tape", "������", Metal, {75, 14, 0}, {48}, 4, 40, },
{"MotionSensor", "������ ��������", Metal, {67, 123, 0}, {48}, 7, 800, },
{"Fruit", "�����", Wood, {61, 14, 0}, {48}, 1, 10, },
{"Briefcase", "��������", Leather, {52, 5, 0}, {48}, 5, 35, },
{"BigBookOfScience", "������� ������� �����", Wood, {88, 13, 0}, {48}, 5, 400, },
{"LeatherJacket", "������� ������", Leather, {77, 30, 0}, {48}, 5, 250, {}, {8, {0, 0, 0, 0, 0, 0, 0}, {20, 20, 10, 10, 30, 500, 20}, {HMMAXX, HFMAXX}}},
{"Tool", "����������", Metal, {54, 41, 0}, {48}, 1, 200, },
{"DeansElectronics", "�������", Wood, {55, 13, 0}, {48}, 2, 130, },
{"ElectronicLockpick", "����������� �������", Metal, {57, 118, 0}, {48}, 2, 375, },
{"FuzzyPainting", "�������", Metal, {58, 119, 0}, {48}, 12, 300, },
{"Flare", "�������", Metal, {59, 46, 0}, {48}, 1, 35, {{Throw, NoAction}, SmallGuns, {1, 1}, 0, 0}},
{"FirstAidBook", "��� ������", Metal, {60, 13, 0}, {48}, 2, 175, },
{"IguanaOnAStick", "������-��-�������", Leather, {62, 41, 0}, {48}, 1, 20, },
{"Key", "����", Metal, {63, 41, 0}, {48}, 0, 4, },
{"KeyRing", "������ ��� ������", Metal, {64, 41, 0}, {48}, 0, 4, },
{"Lockpicks", "�������", Metal, {66, 41, 0}, {48}, 1, 150, },
{"PlasticExplosives", "����������� ����������", Metal, {70, 125, 0}, {48}, 4, 850, },
{"ScoutHandbook", "����� ������", Wood, {71, 13, 0}, {48}, 3, 200, },
{"Buffout", "�������", Metal, {45, 15, 0}, {48}, 0, 200, },
{"Watch", "����", Metal, {306, 173, 0}, {48}, 1, 0, },
{"Motor", "�����", Metal, {68, 32, 0}, {48}, 14, 210, },
{"BackPack", "������", Leather, {69, 2, 0}, {48}, 5, 100, },
{"DoctorSBag", "���������� �����", Leather, {56, 117, 0}, {48}, 5, 300, },
{"ScorpionTail", "X���� ���������", Metal, {90, 39, 0}, {48}, 20, 10, },
{"Bag", "�����", Leather, {138, 3, 0}, {48}, 3, 25, },
{"Shotgun", "��������", Metal, {95, 36, 7}, {48}, 5, 800, {{FireSingle, NoAction}, SmallGuns, {12, 22}, 4, 2}},
{"GaShotgunShells12", "12 ����������� ��������", Metal, {91, 1, 0}, {48}, 1, 225, },
{"RedPassKey", "���� �������� �������", Plastic, {79, 41, 0}, {48}, 1, 10, },
{"BluePassKey", "���� ������ �������", Plastic, {80, 41, 0}, {48}, 1, 10, },
{"Junk", "�����", Metal, {81, 49, 0}, {48}, 12, 40, },
{"GoldLocket", "������� ��������", Metal, {82, 41, 0}, {48}, 1, 400, },
{"Radio", "�����", Metal, {83, 6, 0}, {48}, 6, 350, },
{"Lighter", "���������", Metal, {85, 41, 0}, {48}, 0, 100, },
{"GunsAndBullets", "����� � ����", Leather, {86, 13, 0}, {48}, 2, 425, },
{"IguanaOnAStick", "������-��-�������", Leather, {87, 41, 0}, {48}, 1, 8, },
{"TapeRecorder", "����������", Metal, {89, 14, 0}, {48}, 5, 80, },
{"Key", "����", Metal, {94, 41, 0}, {48}, 0, 25, },
{"NukaCola", "����-����", Metal, {93, 116, 0}, {48}, 0, 3, },
{"RadX", "��� X", Metal, {43, 15, 0}, {48}, 0, 300, },
{"Psycho", "�����", Metal, {92, 15, 0}, {48}, 0, 400, },
{"MagnumFMJ44", ".44 ������ ��", Metal, {33, 1, 0}, {48}, 2, 50, },
{"Urn", "����", Metal, {97, 127, 0}, {48}, 6, 130, },
{"Robes", "����", Leather, {98, 52, 0}, {48}, 10, 90, {}, {5, {0, 0, 0, 0, 0, 0, 0}, {20, 25, 10, 10, 40, 500, 20}, {HAROBE, HAROBE}}},
{"TanglerSHand", "���� ��������", Metal, {99, 128, 0}, {48}, 3, 200, },
{"SuperSledge", "����������", Metal, {100, 40, 3}, {48}, 12, 3750, {{Swing, Thrust}, SmallGuns, {18, 36}, 5, 0}},
{"Ripper", "�����������", Metal, {102, 28, 1}, {48}, 2, 900, {{Swing, Thrust}, SmallGuns, {15, 32}, 4, 30}},
{"Flower", "������", Metal, {101, 120, 0}, {48}, 1, 5, },
{"LaserRifle", "�������� ��������", Metal, {103, 24, 8}, {48}, 12, 5000, {{FireSingle, NoAction}, SmallGuns, {25, 50}, 6, 12}},
{"Necklace", "��������", Glass, {104, 41, 0}, {48}, 2, 700, },
{"AlienBlaster", "������������ �������", Metal, {105, 34, 5}, {48}, 2, 10000, {{FireSingle, NoAction}, SmallGuns, {30, 90}, 2, 30}},
{"Ball9mm", "9�� �� ����", Metal, {106, 1, 0}, {48}, 2, 100, },
{"Mauser9mm", "9�� ������", Metal, {107, 34, 5}, {48}, 3, 1500, {{FireSingle, NoAction}, SmallGuns, {5, 10}, 3, 7}},
{"PsychicNullifier", "����������� �������������", Metal, {108, 14, 0}, {48}, 3, 1000, },
{"Beer", "����", Glass, {117, 116, 0}, {48}, 0, 5, },
{"Booze", "����", Glass, {121, 116, 0}, {48}, 1, 10, },
{"WaterFlask", "�����", Metal, {109, 41, 0}, {48}, 2, 25, },
{"Rope", "�������", Leather, {110, 126, 0}, {48}, 10, 25, },
{"AccessCard", "����� �������", Metal, {111, 41, 0}, {48}, 1, 25, },
{"COCBadge", "������ �X", Metal, {112, 41, 0}, {48}, 1, 400, },
{"COCBadge", "������ �X", Metal, {113, 41, 0}, {48}, 1, 12, },
{"SniperRifle", "����������� ��������", Metal, {114, 36, 7}, {48}, 8, 2200, {{FireSingle, NoAction}, SmallGuns, {14, 34}, 5, 6}},
{"SuperStimpak", "����� ���������", Metal, {29, 15, 0}, {48}, 1, 350, },
{"MolotovCocktail", "�������� ��������", Metal, {118, 116, 0}, {48}, 1, 50, {{Throw, NoAction}, SmallGuns, {8, 20}, 3, 0}},
{"CattleProd", "������� ��� �����", Metal, {129, 16, 2}, {48}, 3, 600, {{Swing, Thrust}, SmallGuns, {12, 20}, 4, 20}},
{"RedRyderBBGun", "����� ��� ������", Metal, {119, 36, 7}, {48}, 5, 200, {{FireSingle, NoAction}, SmallGuns, {1, 3}, 3, 100}},
{"RedRyderLEBBGun", "����� ��� ������ �� ��", Metal, {120, 36, 7}, {48}, 5, 3500, {{FireSingle, NoAction}, SmallGuns, {25, 25}, 3, 100}},
{"BBS", "���", Metal, {116, 1, 0}, {48}, 2, 20, },
{"BrotherhoodTape", "����� ��������", Metal, {75, 41, 0}, {48}, 1, 70, },
{"FEVDisk", "��� ����.", Metal, {75, 41, 0}, {48}, 1, 200, },
{"SecurityDisk", "���� ������������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"AlphaExperimentDisk", "���� ������������ �����", Metal, {75, 41, 0}, {48}, 1, 50, },
{"DeltaExperimentDisk", "���� ������������ ������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"VreeSExperimentDisk", "���� ������������ ����", Metal, {75, 41, 0}, {48}, 1, 75, },
{"BrotherhoodHonorCode", "��� ����� ��������", Metal, {0, 173, 0}, {48}, 1, 50, },
{"MutantTransmissions", "�������� ��������", Metal, {75, 41, 0}, {48}, 1, 125, },
{"Flare", "�������", Metal, {123, 46, 0}, {48}, 1, 60, {{Throw, NoAction}, SmallGuns, {1, 1}, 0, 0}},
{"Dynamite", "�������", Metal, {122, 23, 0}, {48}, 5, 500, },
{"GeigerCounter", "������� �������", Metal, {124, 121, 0}, {48}, 4, 650, },
{"MotionSensor", "������ ��������", Metal, {125, 123, 0}, {48}, 7, 800, },
{"PlasticExplosives", "����������� ����������", Metal, {126, 125, 0}, {48}, 4, 850, },
{"StealthBoy", "���������", Metal, {127, 14, 0}, {48}, 3, 1800, },
{"Tandi", "�����", Leather, {128, 173, 0}, {48}, 10, 600, },
{"BrotherhoodHistory", "������� ��������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MaxsonSHistory", "������� �������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MaxsonSJournal", "������ �������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"LightHealing", "�������������� ���������", Metal, {306, 173, 0}, {48}, 0, 25, },
{"MediumHealing", "������� ���������", Metal, {306, 173, 0}, {48}, 0, 50, },
{"HeavyHealing", "������ ���������", Metal, {306, 173, 0}, {48}, 0, 100, },
{"SecurityCard", "����� ������������", Metal, {130, 41, 0}, {48}, 0, 40, },
{"FieldSwitch", "������������� �����", Metal, {131, 41, 0}, {48}, 1, 0, },
{"YellowPassKey", "���� ������� �������", Plastic, {132, 41, 0}, {48}, 0, 40, },
{"SmallStatuette", "��������� ���������", Wood, {133, 6, 0}, {48}, 3, 300, },
{"CatSPawMagazine", "������ ������� �����", Leather, {134, 13, 0}, {48}, 1, 5, },
{"BoxOfNoodles", "������� �����", Wood, {135, 41, 0}, {48}, 1, 35, },
{"SmallDustyBoxOfSomeSort", "�����-�� ��������� ������� �������", Metal, {136, 41, 0}, {48}, 2, 20, },
{"TechnicalManual", "���������� �� �����������", Metal, {137, 13, 0}, {48}, 2, 200, },
{"SmallPieceOfMachinery", "��������� ��������", Metal, {139, 41, 0}, {48}, 1, 150, },
{"VaultRecords", "������ X��������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MilitaryBaseSecurityCode", "��� ������������ ������� ����", Metal, {75, 41, 0}, {48}, 1, 0, },
{"HardenedPowerArmor", "����������� �����", Metal, {15, 35, 0}, {48}, 100, 15000, {}, {25, {13, 19, 14, 13, 13, 0, 20}, {50, 90, 70, 50, 50, 505, 60}, {HAPOWR, HAPOWR}}},
{"TurboPlasmaRifle", "����� ���������� ��������", Metal, {143, 24, 8}, {48}, 14, 10000, {{FireSingle, NoAction}, SmallGuns, {35, 70}, 6, 10}},
{"SpikedKnuckles", "������� � ������", Metal, {20, 47, 0}, {48}, 1, 250, {{ThrowPunch, NoAction}, SmallGuns, {4, 10}, 1, 0}},
{"PowerFist", "������ �����", Metal, {141, 47, 0}, {48}, 7, 2200, {{ThrowPunch, NoAction}, SmallGuns, {12, 24}, 1, 25}},
{"CombatKnife", "������ ���", Metal, {142, 28, 1}, {48}, 2, 165, {{Swing, Thrust}, SmallGuns, {3, 10}, 2, 0}},
{"ChemistryJournals", "X��������� �������", Metal, {88, 13, 0}, {48}, 5, 100, },
{"RegulatorTransmission", "�������� ����������", Metal, {75, 41, 0}, {48}, 1, 25, },
{"BrotherhoodArmor", "����� ��������", Metal, {144, 17, 0}, {48}, 25, 4800, {}, {20, {8, 8, 7, 7, 6, 0, 8}, {40, 70, 50, 60, 60, 500, 40}, {HMCMBT, HFCMBT}}},
{"TeslaArmor", "����� �����", Metal, {145, 33, 0}, {48}, 35, 4500, {}, {15, {4, 19, 4, 10, 12, 0, 4}, {20, 90, 10, 80, 80, 500, 20}, {HMMETL, HFMETL}}},
{"Pistol223", ".223 ��������", Metal, {146, 44, 6}, {48}, 5, 3500, {{FireSingle, NoAction}, SmallGuns, {20, 30}, 5, 5}},
{"CombatShotgun", "������ ��������", Metal, {147, 36, 7}, {48}, 10, 2750, {{FireSingle, FireBurst}, SmallGuns, {15, 25}, 5, 12}},
{"AnnaSBones", "����� ����", Stone, {148, 136, 0}, {48}, 10, 0, },
{"AnnaSGoldLocket", "������� �������� ����", Metal, {149, 41, 0}, {48}, 1, 0, },
{"FuelCellController", "������� �������� �������.", Metal, {150, 6, 0}, {48}, 5, 0, },
{"FuelCellRegulator", "������� ���������� ����������", Metal, {151, 14, 0}, {48}, 5, 0, },
{"DayPass", "������� �������", Leather, {152, 41, 0}, {48}, 1, 0, },
{"FalseCitizenshipPapers", "��������� ������ � �����������", Metal, {153, 41, 0}, {48}, 1, 0, },
{"CorneliusGoldWatch", "������� ���� ����������", Metal, {154, 41, 0}, {48}, 1, 0, },
{"HydroelectricPart", "������������������ ������", Metal, {155, 14, 0}, {48}, 10, 0, },
{"Jet", "����", Metal, {156, 15, 0}, {48}, 0, 25, },
{"JetAntidote", "����������� �� ����", Metal, {157, 15, 0}, {48}, 0, 25, },
{"JonnySBBGun", "��-����� ������", Metal, {158, 36, 7}, {48}, 5, 200, {{FireSingle, NoAction}, SmallGuns, {1, 3}, 3, 100}},
{"RubberBoots", "��������� ������", Plastic, {159, 6, 0}, {48}, 2, 0, },
{"SlagMessage", "�������� ������", Metal, {160, 41, 0}, {48}, 1, 0, },
{"SmithSCoolItem", "������ ������� �����", Metal, {161, 41, 0}, {48}, 1, 0, },
{"CombatLeatherJacket", "������ �������� ������", Leather, {162, 30, 0}, {48}, 7, 1000, {}, {18, {2, 0, 2, 0, 0, 0, 0}, {30, 20, 25, 10, 30, 500, 20}, {HMMAXX, HFMAXX}}},
{"VicSRadio", "����� ����", Metal, {163, 14, 0}, {48}, 5, 50, },
{"VicSWaterFlask", "������� ������ ����", Metal, {164, 41, 0}, {48}, 2, 0, },
{"HKCAWS", "X&� �����", Metal, {165, 36, 7}, {48}, 5, 4750, {{FireSingle, FireBurst}, SmallGuns, {15, 25}, 6, 10}},
{"RobotParts", "����� ������", Metal, {166, 6, 0}, {48}, 300, 0, },
{"RoboRocketLauncher", "��������� ������", Metal, {167, 137, 10}, {48}, 1, 0, {{FireSingle, NoAction}, BigGuns, {10, 30}, 1, 6}},
{"BrocFlower", "������ ����", Metal, {168, 139, 0}, {48}, 0, 1, },
{"XanderRoot", "������ �������", Metal, {169, 140, 0}, {48}, 0, 2, },
{"HealingPowder", "�������� �������", Leather, {170, 15, 0}, {48}, 1, 20, },
{"RoboRocketAmmo", "������� ��������� ������", Metal, {171, 138, 0}, {48}, 10, 0, },
{"TrophyOfRecognition", "���������� ������", Metal, {97, 127, 0}, {48}, 10, 2000, },
{"GeckoPelt", "����� �����", Leather, {172, 14, 0}, {48}, 2, 25, },
{"GoldenGeckoPelt", "������� ����� �����", Leather, {173, 14, 0}, {48}, 5, 125, },
{"Flint", "�������", Stone, {174, 41, 0}, {48}, 2, 5, },
{"NeuralInterface", "��������� ���������", Metal, {108, 6, 0}, {48}, 3, 800, },
{"SharpenedSpear", "����������� �����", Wood, {176, 42, 4}, {48}, 4, 100, {{Thrust, Throw}, SmallGuns, {4, 12}, 4, 0}},
{"DixonSEye", "���� �������", Glass, {177, 155, 0}, {48}, 1, 0, },
{"CliftonSEye", "���� ��������", Glass, {178, 155, 0}, {48}, 1, 0, },
{"TommyGun", "����� �����", Metal, {179, 36, 7}, {48}, 7, 1200, {{FireSingle, FireBurst}, SmallGuns, {3, 20}, 6, 50}},
{"MeatJerky", "���� �������", Leather, {180, 41, 0}, {48}, 1, 5, },
{"RadscorpionLimbs", "������ ������������", Stone, {181, 142, 0}, {48}, 10, 0, },
{"Firewood", "�����", Wood, {182, 141, 0}, {48}, 10, 0, },
{"ScopedHuntingRifle", "��������� ��� �����", Metal, {183, 36, 7}, {48}, 11, 1500, {{FireSingle, NoAction}, SmallGuns, {8, 20}, 5, 10}},
{"CarFuelCell", "��������� ������� ����������", Metal, {184, 6, 0}, {48}, 10, 0, },
{"Shovel", "������", Metal, {185, 143, 0}, {48}, 10, 15, },
{"RoboMeleeWeapon1", "������ ������ 1 ", Metal, {186, 144, 0}, {48}, 1, 0, {{ThrowPunch, NoAction}, SmallGuns, {7, 15}, 1, 0}},
{"RoboMeleeWeapon2", "������ ������ 2", Metal, {186, 144, 0}, {48}, 1, 0, {{ThrowPunch, NoAction}, SmallGuns, {15, 30}, 1, 0}},
{"BoxingGloves", "���������� ��������", Leather, {187, 47, 0}, {48}, 5, 250, {{ThrowPunch, NoAction}, SmallGuns, {1, 1}, 1, 0}},
{"PlatedBoxingGloves", "���������� �������� � ��������", Leather, {338, 47, 0}, {48}, 10, 300, {{ThrowPunch, NoAction}, SmallGuns, {2, 5}, 1, 0}},
{"Vault13Holodisk", "�������� 13 �������", Metal, {75, 41, 0}, {48}, 1, 200, },
{"CheezyPoofs", "������ ����", Leather, {189, 41, 0}, {48}, 1, 5, },
{"HKP90c", "X����� �90�", Metal, {190, 44, 6}, {48}, 8, 2500, {{FireSingle, FireBurst}, SmallGuns, {12, 16}, 4, 24}},
{"MetalPole", "������������� ����", Wood, {191, 145, 0}, {48}, 10, 5, },
{"TrapperTownKey", "���� �������-�����", Metal, {63, 41, 0}, {48}, 0, 0, },
{"PipeRifle", "�������� ��������", Metal, {193, 36, 7}, {48}, 10, 200, {{FireSingle, NoAction}, SmallGuns, {5, 12}, 5, 1}},
{"ZipGun", "��� �����", Metal, {192, 34, 5}, {48}, 4, 150, {{FireSingle, NoAction}, SmallGuns, {5, 12}, 4, 1}},
{"Clipboard", "����������", Metal, {194, 41, 0}, {48}, 1, 0, },
{"GeckoHolodisk", "�������� �����", Metal, {75, 41, 0}, {48}, 1, 50, },
{"ReactorHolodisk", "�������� ��������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"YellowReactorKeycard", "������ ����-����� ��������", Metal, {132, 41, 0}, {48}, 1, 25, },
{"RedReactorKeycard", "������� ����-����� ��������", Metal, {79, 41, 0}, {48}, 1, 25, },
{"PlasmaTransformer", "���������������������", Metal, {196, 41, 0}, {48}, 1, 200, },
{"SuperToolKit", "���������������� ����� �������", Metal, {197, 146, 0}, {48}, 10, 1000, },
{"Talisman", "��������", Stone, {198, 41, 0}, {48}, 1, 25, },
{"GammaGulpBeer", "����� ���� ����", Glass, {199, 116, 0}, {48}, 0, 5, },
{"RoentgenRum", "��� ��������", Glass, {200, 116, 0}, {48}, 0, 10, },
{"PartRequisitionForm", "����� ��������� �����", Leather, {201, 41, 0}, {48}, 1, 10, },
{"MagnumRevolver44", ".44 ��������� ������", Metal, {202, 34, 5}, {48}, 5, 600, {{FireSingle, NoAction}, SmallGuns, {12, 18}, 5, 6}},
{"CondomBluePackage", "(����� ��������) �����������", Plastic, {203, 41, 0}, {48}, 0, 2, },
{"CondomGreenPackage", "(������� ��������) �����������", Plastic, {204, 41, 0}, {48}, 0, 2, },
{"CondomRedPackage", "(������� ��������) �����������", Plastic, {205, 41, 0}, {48}, 0, 2, },
{"CosmeticsCase", "����������", Plastic, {206, 41, 0}, {48}, 1, 25, },
{"EmptyHypodermic", "������ ����������", Glass, {207, 41, 0}, {48}, 1, 2, },
{"Switchblade", "���������", Metal, {208, 28, 1}, {48}, 1, 50, {{Swing, Thrust}, SmallGuns, {2, 5}, 1, 0}},
{"SharpenedPole", "����������� ����", Wood, {209, 42, 4}, {48}, 3, 5, {{Thrust, Throw}, SmallGuns, {2, 4}, 4, 0}},
{"CyberneticBrain", "��������������� ����", Metal, {210, 155, 0}, {48}, 8, 2000, },
{"HumanBrain", "������������ ����", Leather, {211, 155, 0}, {48}, 5, 0, },
{"ChimpanzeeBrain", "���� ��������", Leather, {212, 155, 0}, {48}, 4, 0, },
{"AbnormalBrain", "������������ ����", Leather, {213, 155, 0}, {48}, 5, 0, },
{"Dice", "�����", Plastic, {214, 41, 0}, {48}, 0, 5, },
{"LoadedDice", "����������� �����", Plastic, {215, 41, 0}, {48}, 0, 20, },
{"EasterEgg", "���������� ����", Leather, {216, 41, 0}, {48}, 0, 1, },
{"Magic8Ball", "��������� 8-����", Metal, {217, 41, 0}, {48}, 1, 50, },
{"MutagenicSerum", "���������� ���������", Plastic, {218, 15, 0}, {48}, 0, 0, },
{"CatSPawIssue5", "������� ����� ������ N5", Leather, {219, 13, 0}, {48}, 0, 50, },
{"M3A1GreaseGunSMG", "�3�1 ������ SMG", Metal, {220, 44, 6}, {48}, 7, 1750, {{FireSingle, FireBurst}, SmallGuns, {10, 20}, 4, 30}},
{"HeartPills", "��������� ��������", Plastic, {221, 15, 0}, {48}, 0, 50, },
{"Poison", "������", Plastic, {222, 15, 0}, {48}, 0, 200, },
{"MooreSBriefcase", "������� ����", Leather, {52, 5, 0}, {48}, 5, 35, },
{"MooreSBriefcase", "������� ����", Leather, {52, 5, 0}, {48}, 5, 35, },
{"LynetteHolodisk", "�������� ������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"WestinHolodisk", "�������� �������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"NCRSpyHolodisk", "��� ��������� ��������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"DoctorSPapers", "������� �������", Leather, {223, 41, 0}, {48}, 0, 0, },
{"PresidentialPass", "������������� �������", Leather, {224, 41, 0}, {48}, 0, 0, },
{"RangerPin", "������������� ���������", Metal, {225, 41, 0}, {48}, 1, 10, },
{"RangerSMap", "����� ���������", Metal, {226, 41, 0}, {48}, 1, 0, },
{"AdvancedPowerArmor", "���������� ������� �����", Metal, {227, 152, 0}, {48}, 90, 20000, {}, {30, {15, 19, 16, 15, 15, 0, 20}, {55, 90, 70, 60, 60, 500, 65}, {HANPWR, HANPWR}}},
{"AdvPowerArmorMKII", "����. ������� ����� ��II", Metal, {227, 152, 0}, {48}, 100, 25000, {}, {35, {18, 19, 16, 18, 15, 0, 20}, {60, 90, 70, 60, 65, 500, 70}, {HANPWR, HANPWR}}},
{"Bozar", "�����", Metal, {228, 24, 8}, {48}, 20, 5250, {{FireBurst, NoAction}, BigGuns, {25, 35}, 6, 30}},
{"FNFAL", "�� ���", Metal, {229, 36, 7}, {48}, 9, 1500, {{FireSingle, FireBurst}, SmallGuns, {9, 18}, 5, 20}},
{"HKG11", "X����� �11", Metal, {230, 44, 6}, {48}, 8, 6500, {{FireSingle, FireBurst}, SmallGuns, {10, 20}, 5, 50}},
{"XL70E3", "X�70�3", Metal, {231, 36, 7}, {48}, 9, 3000, {{FireSingle, FireBurst}, SmallGuns, {12, 19}, 5, 20}},
{"PancorJackhammer", "������ ����������", Metal, {232, 36, 7}, {48}, 12, 5500, {{FireSingle, FireBurst}, SmallGuns, {18, 29}, 5, 10}},
{"LightSupportWeapon", "������� ������ ���������", Metal, {233, 24, 8}, {48}, 20, 4750, {{FireBurst, NoAction}, BigGuns, {20, 30}, 6, 30}},
{"ComputerVoiceModule", "������������ ��������� ������", Metal, {234, 41, 0}, {48}, 1, 3000, },
{"Caliber45", ".45 ������", Metal, {236, 1, 0}, {48}, 1, 75, },
{"EC2mm", "2�� ��", Metal, {235, 1, 0}, {48}, 1, 400, },
{"Caseless47mm", "4.7 �� �����������", Metal, {237, 1, 0}, {48}, 1, 600, },
{"9mm", "9 ��", Metal, {238, 1, 0}, {48}, 1, 100, },
{"HNNeedlerCartridge", "X� ������ ��������", Metal, {240, 1, 0}, {48}, 1, 250, },
{"HNAPNeedlerCartridge", "X� �� ������ ��������", Metal, {239, 1, 0}, {48}, 1, 300, },
{"762mm", "7.62 ��", Metal, {241, 1, 0}, {48}, 1, 150, },
{"RobotMotivator", "��������� ������", Metal, {68, 32, 0}, {48}, 20, 50, },
{"PlantSpike", "��� ��������", Metal, {242, 153, 0}, {48}, 0, 0, {{Throw, NoAction}, SmallGuns, {1, 2}, 2, 0}},
{"GECK", "�.�.�.�.", Metal, {243, 154, 0}, {48}, 10, 0, },
{"Claw", "������", Metal, {244, 160, 0}, {48}, 1, 0, {{ThrowPunch, NoAction}, SmallGuns, {3, 4}, 1, 0}},
{"Claw", "������", Metal, {245, 161, 0}, {48}, 1, 0, {{ThrowPunch, NoAction}, SmallGuns, {5, 10}, 1, 0}},
{"Vault15Keycard", "����-����� 15 �������", Metal, {79, 41, 0}, {48}, 1, 0, },
{"Vault15ComputerParts", "������������ ����� � 15 �������", Metal, {246, 14, 0}, {48}, 10, 100, },
{"Cookie", "�������", Leather, {247, 41, 0}, {48}, 0, 0, },
{"LeatherArmorMarkII", "������� ����� ���� II", Leather, {250, 29, 0}, {48}, 10, 1000, {}, {20, {3, 1, 1, 1, 1, 0, 1}, {25, 20, 25, 10, 40, 500, 25}, {HMLTHR, HFLTHR}}},
{"MetalArmorMarkII", "������������� ����� ���� II", Metal, {248, 33, 0}, {48}, 35, 1900, {}, {15, {4, 7, 4, 4, 1, 0, 4}, {35, 80, 15, 25, 10, 500, 30}, {HMMETL, HFMETL}}},
{"CombatArmorMarkII", "������ ����� ���� II", Metal, {249, 17, 0}, {48}, 25, 8000, {}, {25, {6, 9, 5, 5, 3, 0, 9}, {40, 65, 35, 50, 55, 500, 45}, {HMCMBT, HFCMBT}}},
{"FlamethrowerFuelMKII", "���������� ����� ��II ", Metal, {260, 43, 0}, {48}, 10, 250, },
{"Shiv", "���", Metal, {275, 28, 1}, {51}, 1, 2, {{Swing, Thrust}, SmallGuns, {1, 4}, 1, 0}},
{"Wrench", "������� ����", Metal, {280, 16, 2}, {48}, 4, 65, {{Swing, NoAction}, SmallGuns, {3, 6}, 3, 0}},
{"SawedOffShotgun", "�����", Metal, {278, 36, 7}, {48}, 4, 800, {{FireSingle, NoAction}, SmallGuns, {12, 24}, 4, 2}},
{"LouisvilleSlugger", "������������ �������", Wood, {276, 16, 2}, {48}, 4, 2700, {{Swing, NoAction}, SmallGuns, {12, 30}, 4, 0}},
{"M60", "�60", Metal, {266, 24, 8}, {48}, 23, 3500, {{FireBurst, NoAction}, BigGuns, {18, 26}, 7, 50}},
{"NeedlerPistol", "�������� ������", Metal, {270, 34, 5}, {48}, 4, 2200, {{FireSingle, NoAction}, SmallGuns, {12, 24}, 3, 10}},
{"AvengerMinigun", "��������� ��������", Metal, {253, 31, 9}, {48}, 28, 5500, {{FireBurst, NoAction}, BigGuns, {10, 14}, 7, 120}},
{"SolarScorcher", "��������� �������", Metal, {277, 34, 5}, {48}, 5, 2000, {{FireSingle, NoAction}, SmallGuns, {20, 60}, 3, 6}},
{"HKG11E", "X����� �11�", Metal, {263, 44, 6}, {48}, 8, 8000, {{FireSingle, FireBurst}, SmallGuns, {13, 23}, 6, 50}},
{"M72GaussRifle", "�������� ������ �72", Metal, {267, 36, 7}, {48}, 9, 8250, {{FireSingle, NoAction}, SmallGuns, {32, 43}, 6, 20}},
{"Phazer", "�����", Metal, {271, 34, 5}, {48}, 4, 7500, {{FireSingle, NoAction}, SmallGuns, {20, 30}, 3, 12}},
{"PPK12GaussPistol", "�������� ������ ���12 ", Metal, {262, 34, 5}, {48}, 5, 5250, {{FireSingle, NoAction}, SmallGuns, {22, 32}, 4, 12}},
{"VindicatorMinigun", "��������� ����������� ", Metal, {279, 31, 9}, {48}, 28, 15250, {{FireBurst, NoAction}, BigGuns, {14, 19}, 7, 100}},
{"YK32PulsePistol", "���������� �������� ��32", Metal, {273, 34, 5}, {35}, 5, 12500, {{FireSingle, NoAction}, SmallGuns, {32, 46}, 3, 5}},
{"YK42BPulseRifle", "���������� �������� ��42�", Metal, {274, 24, 8}, {48}, 9, 17500, {{FireSingle, NoAction}, SmallGuns, {54, 78}, 6, 10}},
{"MagnumSpeedLoad44", ".44 ������ (���������� �����������)", Metal, {252, 34, 5}, {48}, 5, 600, {{FireSingle, NoAction}, SmallGuns, {12, 18}, 5, 6}},
{"SuperCattleProd", "����� �������� �����", Metal, {255, 16, 2}, {48}, 3, 600, {{Swing, Thrust}, SmallGuns, {20, 32}, 4, 20}},
{"ImprovedFlamer", "���������� �������", Metal, {264, 24, 8}, {48}, 19, 2000, {{FireContinuous, NoAction}, BigGuns, {45, 90}, 6, 5}},
{"LaserRifleExtCap", "�������� �������� (���������� �������)", Metal, {265, 24, 8}, {48}, 12, 5000, {{FireSingle, NoAction}, SmallGuns, {25, 50}, 6, 24}},
{"MagnetoLaserPistol", "�������-�������� ��������", Metal, {268, 34, 5}, {48}, 4, 1400, {{FireSingle, NoAction}, SmallGuns, {10, 22}, 3, 12}},
{"FNFALNightSight", "�� ��� (������������ ������)", Metal, {261, 36, 7}, {48}, 10, 2000, {{FireSingle, FireBurst}, SmallGuns, {9, 18}, 5, 20}},
{"DesertEagleExpMag", "��������� ���� (����. ���.)", Metal, {256, 34, 5}, {48}, 4, 800, {{FireSingle, NoAction}, SmallGuns, {10, 16}, 4, 20}},
{"AssaultRifleExpMag", "��������� �������� (����. ���.)", Metal, {254, 36, 7}, {48}, 8, 1300, {{FireSingle, FireBurst}, SmallGuns, {8, 16}, 5, 100}},
{"PlasmaPistolExtCap", "���������� �������� (����. ���.)", Metal, {272, 34, 5}, {48}, 5, 2750, {{FireSingle, NoAction}, SmallGuns, {15, 35}, 4, 32}},
{"MegaPowerFist", "���� ������� �����", Metal, {269, 47, 0}, {48}, 7, 2200, {{ThrowPunch, NoAction}, SmallGuns, {20, 40}, 1, 25}},
{"FieldMedicFirstAidKit", "������� ���������� ", Metal, {251, 15, 0}, {48}, 2, 400, },
{"ParamedicsBag", "��������������� �������", Leather, {257, 117, 0}, {48}, 6, 600, },
{"ExpandedLockpickSet", "��������� ���������", Metal, {259, 41, 0}, {48}, 1, 400, },
{"ElectronicLockpickMKII", "����������� �������� ��II", Metal, {258, 118, 0}, {48}, 2, 700, },
{"OilCan", "�������� ������", Metal, {281, 155, 0}, {48}, 3, 150, },
{"StablesIDBadge", "ID ������ � �������", Plastic, {282, 41, 0}, {48}, 0, 0, },
{"Vault15ShackKey", "���� �� 15�� �������", Metal, {318, 41, 0}, {48}, 0, 0, },
{"Spectacles", "�����", Metal, {283, 41, 0}, {48}, 0, 50, },
{"EmptyJetCanister", "������ ���� ��������", Metal, {284, 15, 0}, {48}, 0, 0, },
{"OxygenTank", "����������� ������", Metal, {285, 14, 0}, {48}, 5, 20, },
{"PoisonTank", "������ � ����", Metal, {286, 6, 0}, {48}, 5, 0, },
{"MineParts", "�������� � ����� ", Metal, {287, 14, 0}, {48}, 10, 0, },
{"MorningstarMineScrip", "������ ����� �������� ������", Leather, {288, 41, 0}, {48}, 0, 1, },
{"HolyHandGrenade", "������ ������ �������", Metal, {289, 122, 0}, {48}, 2, 1, {{Throw, NoAction}, SmallGuns, {300, 500}, 2, 0}},
{"ExcavatorChip", "��� �����������", Metal, {290, 48, 0}, {48}, 1, 50, },
{"GoldNugget", "������� ���������", Metal, {291, 165, 0}, {48}, 10, 50, {{Throw, ThrowPunch}, SmallGuns, {3, 6}, 2, 0}},
{"MonumentChunk", "����� ���������", Metal, {292, 53, 0}, {48}, 5, 0, },
{"UraniumOre", "�������� ����", Metal, {293, 165, 0}, {48}, 10, 0, {{Throw, ThrowPunch}, SmallGuns, {3, 6}, 2, 0}},
{"FlameBreath", "�������� �������", Metal, {294, 167, 8}, {48}, 1, 0, {{FireContinuous, NoAction}, SmallGuns, {35, 50}, 1, 4}},
{"MedicalSupplies", "�����������", Leather, {295, 15, 0}, {48}, 25, 50, },
{"GoldTooth", "������� ���", Stone, {296, 41, 0}, {48}, 0, 50, },
{"HowitzerShell", "������ X��������", Metal, {297, 38, 0}, {48}, 40, 20, },
{"RamirezBoxClosed", "������� ��������, ��������", Wood, {298, 41, 0}, {48}, 2, 500, },
{"RamirezBoxOpen", "������� ��������, ��������", Wood, {299, 41, 0}, {48}, 2, 0, },
{"MirroredShades", "���������� ����", Plastic, {300, 41, 0}, {48}, 1, 20, },
{"DeckOfCards", "������ ����", Leather, {301, 41, 0}, {48}, 1, 2, },
{"PackOfMarkedCards", "�������� ������� ����", Leather, {302, 41, 0}, {48}, 1, 1, },
{"TempleKey", "���� � X����", Metal, {303, 41, 0}, {48}, 0, 0, },
{"PocketLint", "��������� ������", Leather, {305, 41, 0}, {48}, 0, 0, },
{"BioMedGel", "�������������� ����", Glass, {304, 155, 0}, {48}, 1, 0, },
{"BlondieSDogTags", "������� ����� ���������", Metal, {73, 41, 0}, {48}, 0, 0, },
{"AngelEyesDogTags", "������� ����� ���������� ������", Metal, {73, 41, 0}, {48}, 0, 0, },
{"TucoSDogTags", "������� ����� ����", Metal, {73, 41, 0}, {48}, 0, 0, },
{"RaidersMap", "����� ��������", Metal, {307, 41, 0}, {48}, 1, 0, },
{"SheriffSBadge", "������ ������", Metal, {308, 41, 0}, {48}, 1, 5, },
{"VertibirdPlans", "����� ����������", Metal, {309, 41, 0}, {48}, 0, 0, },
{"BishopSHolodisk", "�������� ������.", Metal, {75, 41, 0}, {48}, 1, 50, },
{"AccountBook", "������������� �����", Metal, {310, 41, 0}, {48}, 2, 0, },
{"Unused", "���������������", Metal, {306, 173, 0}, {48}, 1, 0, },
{"TornPaper1", "������� ������ 1", Leather, {311, 41, 0}, {48}, 0, 0, },
{"TornPaper2", "������� ������ 2", Leather, {312, 41, 0}, {48}, 0, 0, },
{"TornPaper3", "������� ������ 3", Leather, {313, 41, 0}, {48}, 0, 0, },
{"PasswordPaper", "������� � �������", Leather, {314, 41, 0}, {48}, 0, 0, },
{"ExplosiveSwitch", "����������", Metal, {131, 41, 0}, {48}, 5, 0, },
{"CellDoorKey", "���� �� ������", Metal, {320, 41, 0}, {48}, 0, 0, },
{"HubologistFieldReport", "X���������������� ������� ������", Wood, {316, 41, 0}, {48}, 1, 0, },
{"MBHolodisk5", "�.�. �������� 5", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MBHolodisk1", "�.�. �������� 1", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MBHolodisk2", "�.�. �������� 2", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MBHolodisk3", "�.�. �������� 3", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MBHolodisk4", "�.�. �������� 4", Metal, {75, 41, 0}, {48}, 1, 50, },
{"EvacuationHolodisk", "�������� ���������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"ExperimentHolodisk", "�������� ������������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MedicalHolodisk", "�������� ��������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"PasswordPaper", "������� � �������", Metal, {335, 41, 0}, {48}, 1, 5, },
{"SmittySMeal", "��� ������", Metal, {317, 41, 0}, {48}, 3, 0, },
{"RotGut", "������ �����", Metal, {321, 116, 0}, {48}, 0, 5, },
{"BallGag", "������� ����", Metal, {322, 41, 0}, {48}, 2, 20, },
{"HubologistHolodisk", "X���������������� ��������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MutatedToe", "������������ ����� ����", Metal, {324, 15, 0}, {48}, 1, 0, },
{"Daisies", "������", Cement, {325, 14, 0}, {48}, 5, 0, },
{"Unused", "���������������", Metal, {306, 173, 0}, {48}, 0, 0, },
{"EnlightenedOneSLetter", "���-�� ������������ ������", Metal, {336, 41, 0}, {48}, 0, 0, },
{"BroadcastHolodisk", "�������� ������������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"SierraMissionHolodisk", "�������� ������ ������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"NavComParts", "�������� �� �������", Metal, {337, 41, 0}, {48}, 1, 0, },
{"Bonus1Agility", "����� +1 ��������", Metal, {339, 15, 0}, {48}, 0, 0, },
{"Bonus1Intelligence", "����� +1 ���������", Metal, {340, 15, 0}, {48}, 0, 0, },
{"Bonus1Strength", "����� +1 ����", Metal, {341, 15, 0}, {48}, 0, 0, },
{"Fallout2Hintbook", "����� ��������� �� ����������� 2", Leather, {342, 13, 0}, {48}, 0, 0, },
{"PlayerSEar", "��� ������", Metal, {343, 41, 0}, {48}, 0, 0, },
{"MasticatorSEar", "��� �����������", Metal, {344, 41, 0}, {48}, 0, 750, },
{"RefinedUraniumOre", "����������� �������� ����", Metal, {293, 165, 0}, {48}, 15, 0, {{Throw, ThrowPunch}, SmallGuns, {3, 6}, 2, 0}},
{"NoteFromFrancis", "������� �� ���������", Leather, {334, 41, 0}, {48}, 0, 0, },
{"K9Motivator", "��������� �-9", Metal, {68, 32, 0}, {48}, 20, 50, },
{"SpecialBoxerWeapon", "����������� ������ �������", Metal, {345, 172, 2}, {48}, 0, 0, {{Swing, Thrust}, SmallGuns, {1, 1}, 1, 0}},
{"NCRHistoryHolodisk", "�������� ������� ���", Metal, {75, 41, 0}, {48}, 1, 50, },
{"MrNixonDoll", "����� ������� �������", Leather, {346, 41, 0}, {48}, 0, 0, },
{"TankerFob", "����� �������", Metal, {347, 41, 0}, {48}, 0, 0, },
{"TeachingsHolodisk", "�������� ������", Metal, {75, 41, 0}, {48}, 1, 50, },
{"KokoweefMineScrip", "����� ���� �������", Leather, {348, 41, 0}, {48}, 0, 1, },
{"PresidentialAccessKey", "������������� ���� �������", Metal, {349, 41, 0}, {48}, 0, 0, },
{"BoxingGloves", "���������� ��������", Leather, {187, 47, 0}, {48}, 5, 250, {{ThrowPunch, NoAction}, SmallGuns, {1, 1}, 1, 0}},
{"PlatedBoxingGloves", "���������� �������� � ��������", Leather, {338, 47, 0}, {48}, 10, 300, {{ThrowPunch, NoAction}, SmallGuns, {2, 5}, 1, 0}},
{"DualPlasmaCannon", "��������� ���������� �����", Metal, {350, 174, 5}, {48}, 10, 0, {{FireSingle, NoAction}, SmallGuns, {45, 90}, 1, 200}},
{"PipBoyLingualEnhancer", "��������������� ��������� ������", Metal, {351, 41, 0}, {48}, 8, 200, },
{"FNFALHPFA", "�� ��� X���", Metal, {229, 36, 7}, {48}, 10, 2250, {{FireSingle, FireBurst}, SmallGuns, {11, 22}, 5, 20}},
{"BlueMemoryModule", "������� ������ ������", Metal, {352, 41, 0}, {48}, 0, 0, },
{"GreenMemoryModule", "������� ������ ������", Metal, {353, 41, 0}, {48}, 0, 0, },
{"RedMemoryModule", "������� ������ ������", Metal, {354, 41, 0}, {48}, 0, 0, },
{"YellowMemoryModule", "������ ������ ������", Metal, {355, 41, 0}, {48}, 0, 0, },
{"DecomposingBody", "������������ ����", Leather, {356, 177, 0}, {48}, 125, 0, },
{"RubberDoll", "��������� �����", Plastic, {357, 6, 0}, {48}, 20, 50, },
{"DamagedRubberDoll", "������������ ��������� �����", Plastic, {358, 14, 0}, {48}, 20, 0, },
{"PipBoyMedicalEnhancer", "����������� ��������� � ������", Metal, {359, 41, 0}, {48}, 8, 200, },
{"DualMinigun", "�������� �������", Metal, {361, 184, 9}, {48}, 10, 0, {{FireBurst, NoAction}, BigGuns, {10, 14}, 1, 800}},
{"BottleCaps", "������ �� �������", Metal, {362, 3, 0}, {48}, 0, 0, },
{"HeavyDualMinigun", "������� ��������� �������", Metal, {361, 184, 9}, {48}, 10, 0, {{FireBurst, NoAction}, BigGuns, {16, 24}, 1, 800}},
{"WakizashiBlade", "������ ��������", Metal, {363, 28, 1}, {48}, 3, 200, {{Swing, Thrust}, SmallGuns, {4, 12}, 2, 0}},
{"SurveyMap", "��������������� �����", Leather, {307, 41, 0}, {48}, 0, 0, },
{"BridgekeeperSRobes", "������� X�������� �����", Leather, {98, 52, 0}, {48}, 10, 5, {}, {20, {5, 8, 4, 4, 2, 0, 6}, {40, 60, 30, 50, 50, 500, 40}, {HAROBE, HAROBE}}},
{"Hypo", "����", Metal, {364, 15, 0}, {48}, 0, 500, },
{"EndBossKnife", "���-������ �����", Metal, {365, 190, 1}, {48}, 1, 0, {{Swing, Thrust}, SmallGuns, {15, 30}, 2, 0}},
{"EndBossPlasmaGun", "���������� ����� �����", Metal, {366, 191, 8}, {48}, 1, 0, {{FireSingle, FireBurst}, BigGuns, {30, 70}, 0, 50}},
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
	if(value > 255) // TODO: ������������ ������ ��������
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