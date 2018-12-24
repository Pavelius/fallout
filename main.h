#include "archive.h"
#include "collection.h"
#include "crt.h"
#include "datetime.h"
#include "draw.h"
#include "grammar.h"
#include "screenshoot.h"
#include "stringcreator.h"

#pragma once

enum ability_s : unsigned char {
	NoStat,
	Strenght, Perception, Endurance, Charisma, Intellegence, Agility, Luck,
	HPMax, APMax, AC, DamageUnarmed, DamageMelee, CarryWeight, Sequence, HealingRate,
	CriticalHit, CriticalHitTable,
	PhisycalThreshold, LaserThreshold, FireThreshold, PlasmaThreshold, ElectricalThreshold, EMPThreshold, ExplosiveThreshold,
	PhisycalResistance, LaserResistance, FireResistance, PlasmaResistance, ElectricalResistance, EMPResistance, ExplosiveResistance,
	RadiationResistance, PoisonResistance,
	Age, Gender, HP,
	PoisonLevel, RadiationLevel,
	// Не вошедшие в основной движек
	AP,
	Random,
};
enum parameter_s : unsigned char {
	Level, Experience, NextLevelExperience,
};
enum perk_s : unsigned char {
	Awareness, BonusHtHAttacks, BonusHtHDamage, BonusMove, BonusRangedDamage, BonusROF,
	EarlierSequence, FasterHealing, MoreCriticals, NightVision, Presence, RadResistance,
	Toughness, StrongBack, SharpShooter, SilenRunnng, Survivalist, MasterTrader, Educated,
	Healer, FortuneFinder, BetterCriticals, Empathy, Slayer, Sniper, SilentDeath,
	ActionBoy, MentalBlock, Lifegiver, Dodger, SnakeEater, MrFixit, Medic, MasterTheif,
	Speaker, HeaveHo, FriendlyFoe, PickPocket, Ghost, CultOfPersonality, Scrounger,
	Explorer, FlowerChild, Pathfinder, AnimalFriend, Scout, MysteriousStranger,
	Ranger, QuickPockets, SmoothTalker, SwiftLearner, Tag, Mutate,
	AddictedNukeCola, AddictedBuffout, AddictedMentat, AddictedPsicho, AddictedRadaway,
	WeaponLongRange, WeaponAccuracy, WeaponPenetrating, WeaponKnockback,
	ArmorPowerPerk, ArmorCombatPerk,
	WeaponScopeRange, WeaponFastReload, WeaponNightSight, WeaponFlameboy,
	ArmorAdvance, ArmorAdvanceII,
	AddictedJet, AddictedGambling,
	ArmorCharisma,
	GeckoSkinning,
	DermalImpactArmor, DermalImpactAssaultEnchantments,
	PhoenixArmorImplants, PhoenixAssaultEnchantments, VaultCityInoculation,
	AdrenalineRush, CautiousNature, Comprehension, DemolitionExpert, Gambler,
	GainStrenght, GainPerception, GainEndurance, GainCharisma, GainIntellegence, GainAgility, GainLuck,
	Harmless, HereAndNow, HtHEvade, KamaSutraMaster, KarmaBeacon, LightStep, LivingAnatomy,
	MagneticPersonality, Negotiator, PackRat, Pyromaniac, QuickRecovery, Salesman,
	Stonewall, Thief, WeaponHandling, VaultCityTraining,
	AlcoholRaisedHP, AlcoholRaisedHP2, AlcoholLoweredHP, AlcoholLoweredHP2,
	AutodocRaisedHP, AutodocRaisedHP2, AutodocLoweredHP, AutodocLoweredHP2,
	ExpertExcrementExpeditor,
	WeaponEnchencedKnockout, MegaUnlucky,
	FirstPerk = Awareness, LastPerk = MegaUnlucky,
	//
	FastMetabolism, Bruiser, SmallFrame, OneHanded, Finesse, Kamikaze, HeavyHanded, FastShoot,
	BloodyMess, Jinxed, GoodNatured, ChemReliant, ChemResistant, SexAppeal, Skilled, Gifted,
	FirstTraits = FastMetabolism, LastTraits = Gifted,
};
enum skill_s : unsigned char {
	SmallGuns, BigGuns, EnergyWeapon,
	Unarmed, MeleeWeapon, Throwing,
	FirstAid, Doctor,
	Sneak, Lockpick, Steal, Traps,
	Science, Repair,
	Speech, Barter, Gambling,
	Outdoorsman,
	FirstSkill = SmallGuns, LastSkill = Outdoorsman,
};
enum gender_s : unsigned char {
	Male, Female,
};
enum action_s : unsigned char {
	NoAction,
	ThrowPunch, KickLeg, Swing, Thrust, Throw,
	FireSingle, FireBurst, FireContinuous,
	Drop, Look, Talk, Turn, Unload,
	Use, UseItem, UseSkill
};
enum wound_s : unsigned char {
	WoundEye,
	WoundRightHand, WoundLeftHand,
	WoundRightLeg, WoundLeftLeg,
};
enum damage_s : unsigned char {
	Phisycal, Laser, Fire, Plasma, Electrical, EMP, Explosive,
};
enum item_type_s : unsigned char {
	Armor, Container, Drug, Weapon, Ammo, Misc, KeyIt
};
enum caliber_s : unsigned char {
	NoAmmo,
	CaliberRocket, CaliberGas, CaliberEnergyC, CaliberEnergyD,
	Caliber233, Caliber5mm, Caliber40, Caliber10mm, Caliber44, Caliber14mm, CaliberFract,
	Caliber9mm, CaliberBB, Caliber45, Caliber2mm, Caliber47mm, CaliberXH, Caliber762
};
enum item_s : unsigned short {
	NoItem,
	MetalArmor, SMG10mm, LeatherArmor, Pistol10mm, Sledgehammer,
	Minigun, RocketLauncher, ExplosiveRocket, Club, Flamer,
	HuntingRifle, Knife, Spear, PowerArmor, PlasmaRifle,
	LaserPistol, CombatArmor, DesertEagle44, Rock, Crowbar,
	BrassKnuckles, Pistol14mm, AssaultRifle, PlasmaPistol, GrenadeFrag,
	GrenadePlasma, GrenadePulse, GatlingLaser, JHP10mm, AP10mm,
	MagnumJHP44, FlamethrowerFuel, AP14mm, FMJ223, JHP5mm,
	AP5mm, RocketAP, SmallEnergyCell, MicroFusionCell, Stimpak,
	Money, IceChest, IceChest1, ThrowingKnife, Bag,
	FirstAidKit, RadAway, Antidote, ReservedItem, Dynamite,
	GeigerCounter, Mentats, StealthBoy, WaterChip, DogTags,
	Bug, Tape, MotionSensor, Fruit, Briefcase,
	BigBookOfScience, LeatherJacket, Tool, DeansElectronics, ElectronicLockpick,
	FuzzyPainting, Flare, FirstAidBook, IguanaOnAStick, Key,
	KeyRing, Lockpicks, PlasticExplosives, ScoutHandbook, Buffout,
	Watch, Motor, BackPack, DoctorSBag, ScorpionTail,
	Bag1, Shotgun, GaShotgunShells12, RedPassKey, BluePassKey,
	Junk, GoldLocket, Radio, Lighter, GunsAndBullets,
	IguanaOnAStick1, TapeRecorder, Key1, NukaCola, RadX,
	Psycho, MagnumFMJ44, Urn, Robes, TanglerSHand,
	SuperSledge, Ripper, Flower, LaserRifle, Necklace,
	AlienBlaster, Ball9mm, Mauser9mm, PsychicNullifier, Beer,
	Booze, WaterFlask, Rope, AccessCard, COCBadge,
	COCBadge1, SniperRifle, SuperStimpak, MolotovCocktail, CattleProd,
	RedRyderBBGun, RedRyderLEBBGun, BBS, BrotherhoodTape, FEVDisk,
	SecurityDisk, AlphaExperimentDisk, DeltaExperimentDisk, VreeSExperimentDisk, BrotherhoodHonorCode,
	MutantTransmissions, Flare1, Dynamite1, GeigerCounter1, MotionSensor1,
	PlasticExplosives1, StealthBoy1, Tandi, BrotherhoodHistory, MaxsonSHistory,
	MaxsonSJournal, LightHealing, MediumHealing, HeavyHealing, SecurityCard,
	FieldSwitch, YellowPassKey, SmallStatuette, CatSPawMagazine, BoxOfNoodles,
	SmallDustyBoxOfSomeSort, TechnicalManual, SmallPieceOfMachinery, VaultRecords, MilitaryBaseSecurityCode,
	HardenedPowerArmor, TurboPlasmaRifle, SpikedKnuckles, PowerFist, CombatKnife,
	ChemistryJournals, RegulatorTransmission, BrotherhoodArmor, TeslaArmor, Pistol223,
	CombatShotgun, AnnaSBones, AnnaSGoldLocket, FuelCellController, FuelCellRegulator,
	DayPass, FalseCitizenshipPapers, CorneliusGoldWatch, HydroelectricPart, Jet,
	JetAntidote, JonnySBBGun, RubberBoots, SlagMessage, SmithSCoolItem,
	CombatLeatherJacket, VicSRadio, VicSWaterFlask, HKCAWS, RobotParts,
	RoboRocketLauncher, BrocFlower, XanderRoot, HealingPowder, RoboRocketAmmo,
	TrophyOfRecognition, GeckoPelt, GoldenGeckoPelt, Flint, NeuralInterface,
	SharpenedSpear, DixonSEye, CliftonSEye, TommyGun, MeatJerky,
	RadscorpionLimbs, Firewood, ScopedHuntingRifle, CarFuelCell, Shovel,
	RoboMeleeWeapon1, RoboMeleeWeapon2, BoxingGloves, PlatedBoxingGloves, Vault13Holodisk,
	CheezyPoofs, HKP90c, MetalPole, TrapperTownKey, PipeRifle,
	ZipGun, Clipboard, GeckoHolodisk, ReactorHolodisk, YellowReactorKeycard,
	RedReactorKeycard, PlasmaTransformer, SuperToolKit, Talisman, GammaGulpBeer,
	RoentgenRum, PartRequisitionForm, MagnumRevolver44, CondomBluePackage, CondomGreenPackage,
	CondomRedPackage, CosmeticsCase, EmptyHypodermic, Switchblade, SharpenedPole,
	CyberneticBrain, HumanBrain, ChimpanzeeBrain, AbnormalBrain, Dice,
	LoadedDice, EasterEgg, Magic8Ball, MutagenicSerum, CatSPawIssue5,
	M3A1GreaseGunSMG, HeartPills, PoisonItem, MooreSBriefcase, MooreSBriefcase1,
	LynetteHolodisk, WestinHolodisk, NCRSpyHolodisk, DoctorSPapers, PresidentialPass,
	RangerPin, RangerSMap, AdvancedPowerArmor, AdvPowerArmorMKII, Bozar,
	FNFAL, HKG11, XL70E3, PancorJackhammer, LightSupportWeapon,
	ComputerVoiceModule, Ammo45, EC2mm, Caseless47mm, Ammo9mm,
	HNNeedlerCartridge, HNAPNeedlerCartridge, Ammo762mm, RobotMotivator, PlantSpike,
	GECK, Claw, Claw1, Vault15Keycard, Vault15ComputerParts,
	Cookie, LeatherArmorMarkII, MetalArmorMarkII, CombatArmorMarkII, FlamethrowerFuelMKII,
	Shiv, Wrench, SawedOffShotgun, LouisvilleSlugger, M60,
	NeedlerPistol, AvengerMinigun, SolarScorcher, HKG11E, M72GaussRifle,
	Phazer, PPK12GaussPistol, VindicatorMinigun, YK32PulsePistol, YK42BPulseRifle,
	MagnumSpeedLoad44, SuperCattleProd, ImprovedFlamer, LaserRifleExtCap, MagnetoLaserPistol,
	FNFALNightSight, DesertEagleExpMag, AssaultRifleExpMag, PlasmaPistolExtCap, MegaPowerFist,
	FieldMedicFirstAidKit, ParamedicsBag, ExpandedLockpickSet, ElectronicLockpickMKII, OilCan,
	StablesIDBadge, Vault15ShackKey, Spectacles, EmptyJetCanister, OxygenTank,
	PoisonTank, MineParts, MorningstarMineScrip, HolyHandGrenade, ExcavatorChip,
	GoldNugget, MonumentChunk, UraniumOre, FlameBreath, MedicalSupplies,
	GoldTooth, HowitzerShell, RamirezBoxClosed, RamirezBoxOpen, MirroredShades,
	DeckOfCards, PackOfMarkedCards, TempleKey, PocketLint, BioMedGel,
	BlondieSDogTags, AngelEyesDogTags, TucoSDogTags, RaidersMap, SheriffSBadge,
	VertibirdPlans, BishopSHolodisk, AccountBook, Unused, TornPaper1,
	TornPaper2, TornPaper3, PasswordPaper, ExplosiveSwitch, CellDoorKey,
	HubologistFieldReport, MBHolodisk5, MBHolodisk1, MBHolodisk2, MBHolodisk3,
	MBHolodisk4, EvacuationHolodisk, ExperimentHolodisk, MedicalHolodisk, PasswordPaper1,
	SmittySMeal, RotGut, BallGag, HubologistHolodisk, MutatedToe,
	Daisies, Unused1, EnlightenedOneSLetter, BroadcastHolodisk, SierraMissionHolodisk,
	NavComParts, Bonus1Agility, Bonus1Intelligence, Bonus1Strength, Fallout2Hintbook,
	PlayerSEar, MasticatorSEar, RefinedUraniumOre, NoteFromFrancis, K9Motivator,
	SpecialBoxerWeapon, NCRHistoryHolodisk, MrNixonDoll, TankerFob, TeachingsHolodisk,
	KokoweefMineScrip, PresidentialAccessKey, BoxingGloves1, PlatedBoxingGloves1, DualPlasmaCannon,
	PipBoyLingualEnhancer, FNFALHPFA, BlueMemoryModule, GreenMemoryModule, RedMemoryModule,
	YellowMemoryModule, DecomposingBody, RubberDoll, DamagedRubberDoll, PipBoyMedicalEnhancer,
	DualMinigun, BottleCaps, HeavyDualMinigun, WakizashiBlade, SurveyMap,
	BridgekeeperSRobes, Hypo, EndBossKnife, EndBossPlasmaGun,
};
enum material_s : unsigned char {
	Glass, Metal, Plastic, Wood, Dirt, Stone, Cement, Leather
};
enum direction_s : unsigned char {
	LeftUp, Up, RightUp, Right, RightDown, Down, LeftDown, Left,
	Center,
};
enum settlement_s : unsigned char {
	SettlementArojo, SettlementDen, SettlementKlamath,
};
enum light_flag : short unsigned {
	NorthSouth = 0,
	EastWest = 0x0800,
	NorthCorner = 0x1000,
	SouthCorner = 0x2000,
	EastCorner = 0x4000,
	WestCorner = 0x8000
};
enum action_flag {
	KneelDownWhenUsing = 0x0001,
	CanBeUsed = 0x0008,
	UseOnSomething = 0x0010,
	CanLookAt = 0x0020,
	CanTalkWith = 0x0040,
	CanPickUp = 0x0080,
};
enum flag_all {
	Flat = 0x00000008,
	NoBlock = 0x00000010, // doesn't block the tile
	MultiHex = 0x00000800,
	NoHighlight = 0x00001000, // doesn't highlight the border; used for containers
	TransRed = 0x00004000,
	TransNone = 0x00008000, // opaque
	TransWall = 0x00010000,
	TransGlass = 0x00020000,
	TransSteam = 0x00040000,
	TransEnergy = 0x00080000,
	LightThru = 0x20000000,
	ShootThru = 0x80000000 // Можно стрелять через него
};
enum critical_effect_s : unsigned {
	KnockOut = 0x10000000,
	KnockDown = 0x02000000,
	CrippledLeftLeg = 0x04000000,
	CrippledRightLeg = 0x08000000,
	CrippledLeftArm = 0x10000000,
	CrippledRightArm = 0x20000000,
	Blinded = 0x40000000,
	InstantDeath = 0x80000000,
	FireDeath = 0x00040000,
	BypassArmor = 0x00080000,
	DroppedWeapon = 0x00400000,
	LoseNextTurn = 0x00800000,
	RandomCriticalEffect = 0x00002000
};
enum animation_s : unsigned char {
	AnimateStand, AnimateWalk, AnimatePickup, AnimateUse, AnimateDodge,
	AnimateDamaged, AnimateDamagedRear,
	AnimateUnarmed1, AnimateUnarmed2, AnimateThrown, AnimateRun,
	AnimateKnockOutBack, AnimateKnockOutForward,
	// Kill animation
	AnimateKilledChest, AnimateKilledElectro,
	AnimateKilledBurstInHead, AnimateKilledBurstInChest,
	AnimateKilledImmolate, AnimateKilledLaser, AnimateKilledElectroChest,
	AnimateKilledBlowup, AnimateKilledMelt, AnimateKilledFired,
	AnimateBloodedBack, AnimateBloodedForward,
	AnimateStandUpForward, AnimateStandUpBack,
	// Dead body (1 frame animation)
	AnimateDeadBackNoBlood, AnimateDeadForwardNoBlood,
	AnimateDeadChest, AnimateDeadElectro,
	AnimateDeadBurstInHead, AnimateDeadBurstInChest,
	AnimateDeadImmolate, AnimateDeadLaser, AnimateDeadElectroChest,
	AnimateDeadBlowup, AnimateDeadMelt, AnimateDeadFired,
	AnimateDeadBack, AnimateDeadForward,
	// Weapon Block
	AnimateWeaponStand, AnimateWeaponWalk, AnimateWeaponTakeOn, AnimateWeaponHide, AnimateWeaponDodge,
	AnimateWeaponThrust, AnimateWeaponSwing,
	AnimateWeaponAim, AnimateWeaponAimEnd,
	AnimateWeaponSingle, AnimateWeaponBurst, AnimateWeaponFlame,
	AnimateWeaponThrow,
	// Weapon Animate
	AnimateClub,
	AnimateHammer = AnimateClub + 13,
	AnimateSpear = AnimateHammer + 13,
	AnimatePistol = AnimateSpear + 13,
	AnimateSMG = AnimatePistol + 13,
	AnimateRifle = AnimateSMG + 13,
	AnimateHeavyGun = AnimateRifle + 13,
	AnimateMachineGun = AnimateHeavyGun + 13,
	AnimateRocketLauncher = AnimateMachineGun + 13,
	LastAnimation = AnimateRocketLauncher + 13
};
enum map_object_s : short unsigned {
	NoObject,
	FirstWall = 1, LastWall = 1633,
	FirstScenery = 2000, LastScenery = FirstScenery + 1861,
};
enum color_s : unsigned char {
	ColorDisable = 0x60, ColorText = 0xD7, ColorCheck = 0x03, ColorInfo = 0xE4, ColorButton = 0x3C,
	ColorState = 0x90
};
enum item_sprite_s : unsigned char {
	FrameInventory, FrameGround, FrameWeapon,
};
namespace res {
enum tokens {
	NoRes,
	BACKGRND, SKILLDEX, INVEN, ITEMS, MISC, SCENERY, WALLS, TILES, INTRFACE,
	FONT1, FONT2, FONT3, FONT4,
	// спрайты людей
	HANPWR, HAPOWR, HAROBE,
	HFCMBT, HFJMPS, HFLTHR, HFMAXX, HFMETL, HFPRIM,
	HMBJMP, HMBMET,
	HMCMBT, HMJMPS, HMLTHR, HMMAXX, HMMETL, HMWARR,
	MABOS2, MABOSS, MAMTNT, MAMTN2,
	MAROBE, NABLUE, NABRWN,
	NACHLD,
	NAGHUL, NAGLOW, NAPOWR, NAROBE, NAVGUL, NAWHIT,
	NFASIA,
	NFBRLP, NFLYNN,
	NFMAXX, NFMETL,
	NFNICE, NFPEAS, NFPRIM, NFTRMP, NFVALT, NFVRED,
	NMASIA,
	NMBONC, NMBOXX, NMBPEA, NMBRLP, NMBRSR, NMBSNP,
	NMCOPP, NMDOCC, NMFATT, NMGANG, NMGRCH, NMLABB,
	NMLOSR, NMLTHR, NMMAXX, NMMEXI, NMMYRN, NMNICE, NMOLDD,
	NMPEAS, NMRGNG, NMVALT, NMWARR,
	// спрайты животных и роботов
	MAANTT, MABRAN, MABROM, MACLAW, MACLW2, MADDOG, MADEGG,
	MADETH, MAFIRE, MAGCKO, MAGKO2, MALIEN, MAMANT, MAMRAT,
	MAMURT, MAPLNT, MAQUEN, MASCP2, MASCRP, MASPHN, MASRAT,
	MATHNG, MACYBR, MAFEYE, MAGUN2, MAGUNN, MAHAND, MAROBO,
	MAROBT,
	//
	Count,
};
}
const unsigned short	Blocked = 0xFFFF;
enum variant_s : unsigned char {
	NoVariant,
	Abilities, Damages, Parameters, Perks, Skills, Wounds,
};
struct variant {
	variant_s			type;
	union {
		ability_s		ability;
		damage_s		damage;
		perk_s			perk;
		skill_s			skill;
		wound_s			wound;
		parameter_s		parameter;
	};
	constexpr variant() : type(NoVariant), perk(FirstPerk) {}
	constexpr variant(unsigned short v) : type((variant_s)(v >> 8)), perk(perk_s(v & 0xFF)) {}
	constexpr variant(ability_s v) : type(Abilities), ability(v) {}
	constexpr variant(damage_s v) : type(Damages), damage(v) {}
	constexpr variant(parameter_s v) : type(Parameters), parameter(v) {}
	constexpr variant(perk_s v) : type(Perks), perk(v) {}
	constexpr variant(skill_s v) : type(Skills), skill(v) {}
	constexpr variant(wound_s v) : type(Wounds), wound(v) {}
	constexpr operator unsigned short() const { return (type << 8) | perk; }
	constexpr explicit operator bool() const { return type != NoVariant; }
	const char*			getnameshort() const;
	const char*			getnameabr() const;
	const char*			getdescription() const;
};
struct damage_info {
	unsigned char		min, max;
	damage_s			type;
};
struct attack_info {
	action_s			actions[2];
	damage_info			damage;
	unsigned char		minst;
	unsigned char		capacity;
	caliber_s			cliber;
	unsigned short		range[2];
	unsigned char		ap[2];
	short				critical_fail;
	int					perk;
	unsigned char		burst;
	int					getap(action_s id) const;
	int					getrange(action_s id) const;
	bool				is(action_s id) const;
};
struct ammo_info {
	caliber_s			caliber;
	unsigned char		count;
	char				ac;
	char				dam_resist, dam_mul, dam_div;
};
struct result_info {
	short				chance, effect;
};
struct roll_info {
	result_info			hit, miss;
};
struct hit_info {
	attack_info			attack;
	ammo_info			ammo;
	roll_info			critical;
	skill_s				skill;
};
struct caliber_info {
	const char*			id;
	const char*			name;
	item_s				items[4];
};
struct drug_info {
	ability_s			id[3]; // Три атрибута
	short				value[3][3]; // Каждый из них имеет 3 значения
	unsigned			durations[2]; // Сразу и в двух других временных рамках
	char				addiction_chance;
	int					addiction_perk;
	unsigned			addiction_duration;
};
struct formula {
	struct term {
		ability_s		ability;
		char			multiplier;
		char			divider;
		int				get(int value) const { return divider ? value * multiplier / divider : value * multiplier; }
		const char*		get(char* result, const char* result_maximum) const;
	};
	char				modifier;
	term				a1, a2;
	const char*			get(char* result, const char* result_maximum) const;
};
struct action_info {
	const char*			id;
	const char*			name;
	const int			fid;
};
struct material_info {
	const char*			id;
	const char*			name;
};
struct skill_info {
	const char*			id;
	short				fid;
	const char*			name;
	formula				base;
	const char*			description;
};
struct ability_info {
	const char*			id;
	short				fid;
	const char*			name;
	const char*			name_short;
	const char*			name_abr;
	bool				percent;
	const char*			description;
};
struct perk_info {
	int					fid;
	const char*			name;
	const char*			description;
};
struct resist_info {
	const char*			id;
	const char*			name;
	const char*			description;
};
struct wound_info {
	short				fid;
	const char*			name;
};
struct gender_info {
	const char*			id;
	const char*			name;
	const char*			name_short;
};
struct tile_info {
	int					fid;
	const char*			name;
	material_s			material;
};
struct wall_info {
	int					id;
	int					fid;
	const char*			name;
	material_s			material;
	light_flag			light;
	unsigned			action;
};
struct land_info {
	const char*			name;
	unsigned char		percent;
	short unsigned		central[4];
	short unsigned		tiles[8];
	short unsigned		corners[4];
	short unsigned		random() const;
};
struct item {
	typedef bool		(item::*proctest)() const;
	constexpr item() : type(), count(0) {}
	constexpr item(item_s type) : type(type), count(0) {}
	item(item_s type, int count);
	constexpr operator bool() const { return type != NoItem; }
	void				clear();
	item_s				get() const { return type; }
	unsigned short		get(item_sprite_s i) const;
	item_s				getammo() const;
	item_s				getammo(int index) const;
	int					getammocount() const;
	int					getammoindex(item_s ammo_type) const;
	const ammo_info&	getammoinfo() const;
	const attack_info&	getattack() const;
	int					getcapacity() const;
	int					getcount() const;
	const char*			getdescription() const;
	res::tokens			getdress(gender_s gender) const;
	item_type_s			getgroup() const;
	int					getminst() const;
	const char*			getname() const;
	int					getresistance(damage_s id) const;
	skill_s				getskill(action_s id) const;
	int					getthreshold(damage_s id) const;
	int					getweaponindex() const;
	animation_s			getweaponanimation() const;
	int					getweight() const;
	bool				isarmor() const { return getgroup() == Armor; }
	bool				isbig() const;
	bool				ismatch(const item& it) const;
	bool				ismelee() const;
	bool				istwohanded() const;
	bool				isweapon() const { return getgroup() == Weapon; }
	void				join(item& it);
	bool				setammo(item_s type, int count);
	void				setammocount(int value);
	void				setcount(int value);
private:
	item_s				type;
	union {
		struct {
			unsigned char	weapon_count : 6;
			unsigned char	ammo_index : 2;
			unsigned char	ammo_count;
		};
		short unsigned	count;
	};
};
struct cursorset {
	cursorset(res::tokens r = res::INTRFACE, int cicle = 267);
	~cursorset();
	void				set(res::tokens r = res::INTRFACE, int cicle = 267);
private:
	res::tokens			r;
	int					f;
	int					action;
};
struct drawable {
	virtual unsigned	getfps() const { return 8; }
	virtual point		getposition() const = 0;
	virtual rect		getrect() const = 0;
	virtual int			getzorder() const { return 0; } // Priority for z-order sortering (lesser was be most visible). If there is two drawable in same position.
	virtual bool		hittest(point position) const { return false; }
	virtual bool		isvisible() const { return true; }
	virtual bool		isvisibleactive() const { return false; } // Drawable visible only when active.
	virtual void		painting(point position) const = 0; // How to paint drawable.
	virtual void		update() {}
};
struct animable : drawable, point {
	const sprite*		ps;
	short unsigned		frame;
	constexpr animable() : point(), ps(), frame() {}
	constexpr animable(const point pt, const sprite* ps, short unsigned frame) : point(pt), ps(ps), frame(frame) {}
	explicit operator bool() const { return ps != 0; }
	rect				getrect() const override;
	point				getposition() const override { return *this; }
	bool				hittest(point position) const override;
	void				painting(point screen) const override;
};
struct pregen_info {
	unsigned char		level;
	const char*			id;
	const char*			name;
	short				age;
	gender_s			gender;
	unsigned char		stats[Luck + 1];
	adat<perk_s, 8>		perks;
	adat<skill_s, 4>	tagged;
	item				gears[8];
	const char*			text;
};
struct wearable {
	struct element : item {
		wearable*		source;
	};
	void				additem(item& it);
	item*				find(item_s type);
	aref<item*>			select(aref<item*> source) const;
};
struct actor : drawable, point, wearable {
	constexpr actor() : point{0, 0}, action(AnimateDeadBack), orientation(0), frame(0), frame_maximum(0), next_stamp(0) {}
	static int			byweapon(animation_s action, int weapon);
	animation_s			getaction() const { return action; }
	static animation_s	getbase(animation_s id);
	static animation_s	getsubaction(animation_s id);
	int					getcicle() const;
	static int			getdistance(const point p1, const point p2);
	unsigned			getfps() const;
	unsigned short		getlastframe() const;
	static int			getlongest(const point p1, const point p2);
	static animation_s	getnextanim(animation_s id);
	unsigned char		getorientation() const { return orientation; }
	static char			getorientation(point from, point to);
	point				getposition() const override { return *this; }
	static animation_s	getprevanim(animation_s id);
	rect				getrect() const override;
	virtual const sprite* getsprite() const = 0;
	virtual item&		getweapon() const = 0;
	bool				hittest(point position) const override { return false; }
	bool				ismoving() const;
	void				moveto(point position, int run);
	void				painting(point screen) const override;
	static void			preview(int x, int y, gender_s gender, const item& armor, const item& weapon, int orientation = -1, animation_s action = AnimateStand, unsigned tick = 0, const rect& clip = {-40, -100, 40, 30});
	void				setaction(animation_s value, bool backward = false);
	void				setorientation(unsigned char value) { orientation = value; }
	void				setposition(point value) { x = value.x; y = value.y; }
	void				update() override;
	void				wait();
private:
	animation_s			action;
	unsigned char		orientation;
	short unsigned		frame, frame_maximum;
	unsigned			next_stamp;
	void				moveshift();
};
struct weaponable {
	item				weapons[2];
	char				weapon_action[2];
	void				change_weapon();
	char				getweaponactionindex() const { return weapon_action[0]; }
	void				setweaponactionindex(int value) { weapon_action[0] = value; }
};
struct creature : actor, weaponable {
	creature() { clear(); }
	creature(const char* id) { create(id); }
	void				add(const item& it);
	void				act(const char* format, ...) {}
	static void			adventure();
	bool				equip(const item& it);
	void				apply(const pregen_info* pg);
	void				clear();
	void				create(const char* id);
	bool				choose_gender(int x, int y);
	bool				choose_stats(int trait_points, int tag_skill_points, int ability_points, bool explore_mode = false, int skill_points = 0);
	void				damage(const damage_info& di);
	void				decrease(variant, int& points);
	void				get(hit_info& ai, const item weapon, action_s id) const;
	int					get(ability_s id) const;
	int					get(parameter_s id) const;
	int					get(skill_s id) const;
	char*				get(char* result, const char* result_maximum, variant id, bool show_maximum_only) const;
	item&				getarmor() const { return const_cast<creature*>(this)->armor; }
	int					getbase(skill_s id) const;
	int					getcriticalmiss() const;
	int					getequipweight() const;
	int					getexperience() const { return experience; }
	static const datetime& getdate();
	gender_s			getgender() const { return gender; }
	short unsigned		getindex() const;
	int					getlevel() const { return level; }
	static ability_s	getmaximum(ability_s id);
	const char*			getname() const { return name; }
	int					getnextlevel() const;
	int					getpartylimit() const;
	int					getperkrate() const;
	static creature*	getplayer();
	const sprite*		getsprite() const override;
	static const pregen_info* getpregen(const char* id);
	int					getskillrate() const;
	item&				getweapon() const override { return const_cast<creature*>(this)->weapons[0]; }
	item&				getweaponfirst() { return weapons[0]; }
	item&				getweaponsecond() { return weapons[1]; }
	void				increase(variant, int& points);
	void				inventory();
	bool				is(perk_s id) const { return (perks[id / 32] & (1 << (id % 32))) != 0; }
	bool				is(skill_s id) const { return (skills_tag & (1 << id)) != 0; }
	bool				is(wound_s id) const { return (wounds & (1 << id)) != 0; }
	bool				isalive() const { return get(HP) >= 0; }
	bool				isplayer() const;
	static void			newgame();
	void				mark(variant e, int& points);
	static void			passtime(unsigned minutes);
	void				random_name() { name = "Вонг"; }
	bool				reload(item& target, bool run, bool interactive);
	void				set(ability_s id, int value) { stats[id] = value; }
	void				set(perk_s id) { perks[id / 32] |= (1 << (id % 32)); }
	void				set(skill_s id) { skills_tag |= (1 << id); }
	void				remove(perk_s id) { perks[id / 32] &= ~(1 << (id % 32)); }
	void				remove(skill_s id) { skills_tag &= ~(1 << id); }
	static void			worldmap();
private:
	const char*			name;
	unsigned char		level;
	gender_s			gender;
	char				ap;
	short				stats[Random];
	short				stats_boost[Random];
	unsigned char		skills[LastSkill + 1];
	unsigned			perks[1 + LastTraits / 32];
	unsigned			skills_tag;
	item				armor, money;
	unsigned char		wounds;
	unsigned			experience;
	int					render_stats(int x, int y, int width, aref<variant> elements, bool show_maximum_only) const;
};
struct dwvariant {
	enum type_s { None, Creature, Scenery, Wall, Tile};
	type_s				type;
	short unsigned		index;
	struct creature*	creature;
	constexpr dwvariant() : type(None), index(Blocked), creature(0) {}
	constexpr dwvariant(short unsigned i) : type(Scenery), index(i), creature(0) {}
	constexpr dwvariant(short unsigned i, type_s t) : type(t), index(i), creature(0) {}
	constexpr dwvariant(short unsigned i, struct creature* v) : type(Creature), index(i), creature(v) {}
};
struct settlement {
	const char*			name;
	point				position;
	char				size;
	unsigned			flags;
};
struct ground_info : item {
	point				position;
};
struct map_info {
	struct node {
		short unsigned	index;
		node*			next;
	};
	static const unsigned height = 100;
	static const unsigned width = 100;
	node*				addnode();
	void				blockimpassable(short unsigned free_state = Blocked - 1);
	void				clear();
	void				createwave(short unsigned start);
	void				createwave(short unsigned start, short unsigned max_cost);
	unsigned short		getcost(unsigned short index);
	int					getfree(unsigned short index, int radius, int size);
	short unsigned		geth(int x, int y) const;
	short unsigned		getm(int x, int y) const;
	int					getnodecount() const;
	static short unsigned getland(short unsigned tile);
	unsigned short		getlandtile(short unsigned index, short unsigned value) const;
	short unsigned		getobject(short unsigned index) const;
	short unsigned		getroof(short unsigned index) const;
	short unsigned		gettile(short unsigned index) const;
	bool				isblocked(short unsigned index) const;
	node*				remove(node* p);
	node*				removeall(node* p);
	node*				removeback(node* p);
	node*				route(short unsigned start, short unsigned(*proc)(short unsigned index), short unsigned maximum_range, short unsigned minimal_reach);
	void				serialize(bool write_mode);
	void				setcost(short unsigned index, short unsigned value);
	void				setgroup(short unsigned index, short unsigned group);
	void				setgroup(short unsigned index, short unsigned start, short unsigned width, short unsigned height, int count = 0);
	void				setnone(short unsigned index);
	void				setland(short unsigned index, short unsigned value);
	void				setlandx(short unsigned index, short unsigned value);
	void				setroof(short unsigned index, short unsigned value);
	void				setscene(short unsigned index, short unsigned value);
	void				settile(short unsigned index, short unsigned value);
	void				setwall(short unsigned index, short unsigned value);
	static short unsigned stepfrom(short unsigned index);
	static short unsigned stepto(short unsigned index);
	static short unsigned to(short unsigned index, direction_s d);
	static short unsigned tot(short unsigned index, direction_s d);
private:
	friend archive;
	unsigned short		floor[width*height];
	unsigned short		roof[width*height];
	unsigned short		objects[width*height * 4];
	unsigned char		flags[width*height * 4];
	void				updateland();
};
namespace draw {
struct actinfo {
	unsigned short		speed;
	unsigned short		action;
	int					count;
	point				offset[6];
	point				lastof[6];
};
struct command_info {
	action_s			action;
	callback_proc		proc;
	int					param;
};
struct runstate {
	runstate(bool new_value);
	~runstate();
private:
	bool				value;
};
struct runable {
	virtual void		execute() const = 0;
	virtual int			getid() const = 0;
	virtual bool		ischecked() const { return false; }
};
struct cmd : runable {
	constexpr cmd(void(*proc)(), int param = 0) : proc(proc), param(param) {}
	void				execute() const override { draw::execute(proc, param); }
	int					getid() const override { return (int)proc; }
private:
	void(*proc)();
	int					param;
};
void					addaction(action_s action, callback_proc proc, int param = 0);
void					animate(int x, int y, sprite* ps, int cicle, int fps = 6, int frame = -1, int frame_end = -2);
void					background(int frame);
bool					buttonf(int x, int y, int cicles_normal, int cicle_pressed, bool checked = false, bool need_pressed = false, areas* ra = 0, rect* rrc = 0);
void					buttonp(int x, int y, int circle_normal, int circle_pressed, const runable& ev, const char* string, int ty = 6);
int						button(int x, int y, int width, const runable& ev, const char* string, unsigned key = 0);
extern point			camera; // Current view on map
void					dlgerr(const char* title, const char* format, ...);
void					dlgmsg(const char* title, const char* text);
void					field(int x, int y, int width, const runable& ev, const char* string, unsigned key = 0);
void					focusing(const rect& rc, int id);
unsigned				gametick();
const actinfo*			getaction(const sprite* p, int action);
int						getfocus();
callback_proc			getlayout();
int						getnext(int id, int key);
unsigned				getstamp();
unsigned				gettick(unsigned start);
sprite*					gres(res::tokens id);
const char*				getresname(res::tokens id);
void					hexagon(short unsigned index, point screen);
void					image(int x, int y, res::tokens token, int id, int flags = 0, unsigned char alpha = 0xFF);
void					initialize();
bool					isnext(callback_proc proc);
bool					ispause();
void					iteminv(int x, int y, int sx, int sy, item& it, bool resize = false);
void					label(int x, int y, int id, const char* temp, bool checked, bool disabled);
void					modify(color* pal, int index, int start, int count);
void					number(int x, int y, int digits, int value);
void					numberap(int x, int y, int value);
void					numbersm(int x, int y, int digits, int value, int type = 0);
void					radio(int x, int y, const runable& ev, int frame, unsigned key = 0);
void					setcamera(point pt);
void					setcolor(color_s value);
void					setlayout(callback_proc proc);
void					setfont(res::tokens e);
void					setpage();
void					setpage(callback_proc proc);
void					setpagedef(callback_proc proc);
void					setpallette(int daylight);
void					setpause(bool value);
void					tiles(point screen);
}
const int				tile_width = 80; // Width of isometric tile
const int				tile_height = 36; // Height of isometric tile
point					h2m(point pt);
point					s2m(point s); // Convert screen coordinates to tile index
point					m2s(int x, int y);
point					m2h(int x, int y);
extern ability_info		ability_data[];
extern const char*		ability_values[11];
extern action_info		action_data[];
extern caliber_info		caliber_data[];
extern creature			player;
extern adat<creature, 128> creature_data;
extern resist_info		damage_data[];
extern gender_info		gender_data[];
extern land_info		land_data[];
extern map_info			map;
extern material_info	material_data[];
extern perk_info		perk_data[];
extern skill_info		skill_data[];
extern tile_info		tile_data[];
extern wall_info		wall_data[];
wound_info				wound_data[];