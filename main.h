#include "collection.h"
#include "crt.h"
#include "draw.h"
#include "grammar.h"
#include "screenshoot.h"
#include "stringcreator.h"

#pragma once

enum ability_s : unsigned char {
	Strenght, Perception, Endurance, Charisma, Intellegence, Agility, Luck
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
	Punch, Swing, Thrust, Throw,
	FireSingle, FireBurst, FireAuto, FireFlame,
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
	PoisonResistance, RadiationResistance,
	FirstDamageType = Phisycal, LastDamageType = RadiationResistance,
};
enum illness_s : unsigned char {
	Poison, Radiation,
	LastIllness = Radiation,
};
enum parameter_s : unsigned char {
	ArmorClass, ActionPoints, CarryWeight, CriticalHit, HealingRate, HitPoints, MeleeDamage, Sequence,
};
enum item_s : unsigned short {
	NoItem,
	ArmorMetal, Pistol10mmAuto, ArmorLeather, Pistol10mm,
	WarHammer, MachineGun, RoketLauncher, AmmoRocket, Batton, FlameThrower,
	ArmorCombat, ArmorPower, ArmorReinforced,
	Ammo10mm,
	FirstItem = ArmorMetal, LastItem = Ammo10mm,
};
enum wear_s : unsigned char {
	Misc, Armor, Weapon,
};
enum material_s : unsigned char {
	Cement, Glass, Leather, Metal, Plastic, Stone, Wood,
	FirstMaterial = Cement, LastMaterial = Wood,
};
enum direction_s : unsigned char {
	RightUp, Up, LeftUp, Right, Left, RightDown, Down, LeftDown,
};
enum settlement_s : unsigned char {
	SettlementArojo, SettlementDen, SettlementKlamath,
	FirstSettlement = SettlementArojo, LastSettlement = SettlementKlamath,
};
enum animation_s : unsigned char {
	ActionStand, ActionWalk, ActionClimb, ActionPickup, ActionUse, ActionDodge,
	ActionDamaged, ActionDamagedRear,
	ActionUnarmed1, ActionUnarmed2, ActionThrown, ActionRun, ActionHandsUp, ActionCloackOff,
	ActionKnockOutBack, ActionKnockOutForward,
	LastAction = ActionKnockOutForward,
	ActionHideWeapon, ActionPrepareWeapon,
	ActionPrepareShoot, ActionShoot, ActionShootAuto, ActionEndShoot,
};
enum color_s : unsigned char {
	ColorDisable = 0x60, ColorText = 0xD7, ColorCheck = 0x03, ColorInfo = 0xE4, ColorButton = 0x3C,
	ColorState = 0x90
};
namespace res {
enum tokens {
	NoRes,
	BACKGRND, SKILLDEX, INVEN, ITEMS, MISC, SCENERY, WALLS, TILES, INTRFACE,
	FONT1, FONT2, FONT3, FONT4,
	// ������� �����
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
	// ������� �������� � �������
	MAANTT, MABRAN, MABROM, MACLAW, MACLW2, MADDOG, MADEGG,
	MADETH, MAFIRE, MAGCKO, MAGKO2, MALIEN, MAMANT, MAMRAT,
	MAMURT, MAPLNT, MAQUEN, MASCP2, MASCRP, MASPHN, MASRAT,
	MATHNG, MACYBR, MAFEYE, MAGUN2, MAGUNN, MAHAND, MAROBO,
	MAROBT,
	//
	Count,
};
}
enum variant_s : unsigned char {
	NoVariant,
	Abilities, Damages, Illness, Parameters, Perks, Skills, Wounds,
};
struct variant {
	variant_s			type;
	union {
		ability_s		ability;
		damage_s		damage;
		illness_s		illness;
		parameter_s		parameter;
		perk_s			perk;
		skill_s			skill;
		wound_s			wound;
	};
	constexpr variant() : type(NoVariant), perk(FirstPerk) {}
	constexpr variant(unsigned short v) : type((variant_s)(v >> 8)), perk(perk_s(v & 0xFF)) {}
	constexpr variant(ability_s v) : type(Abilities), ability(v) {}
	constexpr variant(damage_s v) : type(Damages), damage(v) {}
	constexpr variant(illness_s v) : type(Illness), illness(v) {}
	constexpr variant(parameter_s v) : type(Parameters), parameter(v) {}
	constexpr variant(perk_s v) : type(Perks), perk(v) {}
	constexpr variant(skill_s v) : type(Skills), skill(v) {}
	constexpr variant(wound_s v) : type(Wounds), wound(v) {}
	constexpr operator unsigned short() const { return (type << 8) | perk; }
	constexpr explicit operator bool() const { return type != NoVariant; }
	const char*			getdescription() const;
	int					getimage() const;
};
struct damage_info {
	unsigned char		min, max;
	damage_s			type;
};
struct attack_info {
	adat<action_s, 4>	actions;
	skill_s				skill;
	damage_info			damage;
	item_s				ammo;
};
struct armor_info {
	unsigned char		ac;
	unsigned char		threshold[Explosive + 1];
	unsigned char		resistance[Explosive + 1];
};
struct illness_info {
	int					fid;
	const char*			name;
	const char*			description;
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
struct skill_info {
	short				fid;
	const char*			name;
	formula				base;
	const char*			description;
};
struct ability_info {
	short				fid;
	const char*			name;
	const char*			name_short;
	const char*			description;
};
struct perk_info {
	int					fid;
	const char*			name;
	const char*			description;
};
struct resist_info {
	int					fid;
	const char*			name;
	const char*			name_short;
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
struct item {
	constexpr item() : type(), count(0), ammo(0) {}
	constexpr item(item_s type) : type(type), count(0), ammo(0) {}
	constexpr item(item_s type, unsigned char count) : type(type), count(count - 1), ammo(0) {}
	item_s				get() const { return type; }
	const char*			getdescription() const;
	const char*			getname() const;
	int					getresistance(damage_s id) const;
	int					getweaponindex() const;
private:
	item_s				type;
	unsigned char		count;
	unsigned char		ammo;
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
	virtual int			getcursor() const { return 0; } // Get cursor index when over this drawable
	virtual unsigned	getfps() const { return 20; }
	virtual point		getposition() const = 0;
	virtual rect		getrect() const = 0;
	virtual int			getzorder() const { return 0; } // Priority for z-order sortering (lesser was be most visible). If there is two drawable in same position.
	virtual bool		hittest(point position) const { return false; }
	virtual bool		isvisible() const { return true; }
	virtual bool		isvisibleactive() const { return false; } // Drawable visible only when active.
	virtual void		painting(point position) const = 0; // How to paint drawable.
	virtual void		update() {}
};
struct animable : drawable {
	short unsigned		index;
	short unsigned		action, tick, orientation, timeout;
	res::tokens			icn;
	point				pos;
	animable();
	animable(point pos, res::tokens icn, int action);
	rect				getrect() const override;
	point				getposition() const override { return pos; }
	bool				hittest(point position) const override;
	void				painting(point screen) const override;
	virtual void		update() override;
private:
	unsigned			timestart;
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
struct actor : drawable, point {
	constexpr actor() : point{0, 0}, action(ActionStand), orientation(0), frame(0), timestart(0) {}
	virtual item		getarmor() const = 0;
	int					getcicle() const;
	virtual gender_s	getgender() const { return Male; }
	rect				getrect() const override;
	point				getposition() const override { return *this; }
	const sprite*		getsprite() const;
	virtual item		getweapon() const = 0;
	bool				hittest(point position) const override { return false; }
	void				moveto(point position, int run);
	void				painting(point screen) const override;
	void				setaction(animation_s value) { action = value; }
	void				setorientation(unsigned char value) { orientation = value; }
	void				setposition(point value) { x = value.x; x = value.y; }
	virtual void		update() override {}
private:
	animation_s			action;
	unsigned char		orientation;
	short unsigned		frame;
	unsigned			timestart;
};
struct creature : actor {
	creature() { clear(); }
	void				apply(const pregen_info* pg);
	void				clear();
	bool				choose_gender(int x, int y);
	bool				choose_stats(int trait_points, int tag_skill_points, int ability_points);
	void				decrease(variant, int& points);
	int					get(ability_s id) const;
	int					get(illness_s id) const { return illness[id]; }
	int					get(parameter_s id) const;
	int					get(skill_s id) const;
	int					getabilitypoints() const;
	int					getac() const;
	int					getap() const { return ap; }
	int					getapmax() const;
	item				getarmor() const { return wears[0]; }
	int					getbase(skill_s id) const;
	int					getcarryweight() const;
	int					getcritical() const;
	int					gethealrate() const;
	int					gethp() const { return hp; }
	int					gethpmax() const;
	gender_s			getgender() const override { return gender; }
	int					getmax(parameter_s id) const;
	int					getmeleedamage() const;
	int					getpartylimit() const;
	int					getperkrate() const;
	static const pregen_info* getpregen(const char* id);
	int					getresistance(damage_s id) const;
	int					getsequence() const;
	int					getskillrate() const;
	item				getweapon() const override { return wears[1]; }
	void				increase(variant, int& points);
	bool				is(perk_s id) const { return (perks[id / 32] & (1 << (id % 32))) != 0; }
	bool				is(skill_s id) const { return (skills_tag & (1 << id)) != 0; }
	bool				is(wound_s id) const { return (wounds & (1 << id)) != 0; }
	void				mark(variant e, int& points);
	void				set(perk_s id) { perks[id / 32] |= (1 << (id % 32)); }
	void				set(skill_s id) { skills_tag |= (1 << id); }
	void				remove(perk_s id) { perks[id / 32] &= ~(1 << (id % 32)); }
	void				remove(skill_s id) { skills_tag &= ~(1 << id); }
private:
	const char*			name;
	unsigned char		level;
	gender_s			gender;
	unsigned short		age;
	unsigned short		hp;
	char				ap;
	unsigned char		stats[Luck + 1];
	unsigned short		illness[LastIllness + 1];
	unsigned char		skills[LastSkill + 1];
	unsigned			perks[1 + LastTraits / 32];
	unsigned			skills_tag;
	item				wears[3];
	unsigned char		wounds;
	int					render_stats(int x, int y, int width, aref<variant> elements, bool show_maximum_only) const;
};
struct parameter_info {
	int					fid;
	const char*			name;
	const char*			name_short;
	bool				maximum;
	bool				percent;
	int					(creature::*get)() const;
	int					(creature::*getmax)() const;
	const char*			description;
};
namespace draw {
struct actinfo {
	unsigned short		speed;
	unsigned short		action;
	int					count;
	point				offset[6];
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
void					background(int frame);
bool					buttonf(int x, int y, int cicles_normal, int cicle_pressed, bool checked = false, bool need_pressed = false, areas* ra = 0, rect* rrc = 0);
void					buttonp(int x, int y, int circle_normal, int circle_pressed, const runable& ev, const char* string, int ty = 6);
int						button(int x, int y, int width, const runable& ev, const char* string, unsigned key = 0);
extern point			camera; // Current view on map
void					dlgerr(const char* title, const char* format, ...);
void					dlgmsg(const char* title, const char* text);
void					field(int x, int y, int width, const runable& ev, const char* string);
void					focusing(const rect& rc, int id);
const actinfo*			getaction(const sprite* p, int action);
int						getfocus();
int						getnext(int id, int key);
unsigned				gettick();
unsigned				gettick(unsigned start);
rect					getrect(int id);
sprite*					gres(res::tokens id);
const char*				getresname(res::tokens id);
void					hexagon(int index, point screen);
inline void				image(int x, int y, res::tokens token, int id, int flags = 0, unsigned char alpha = 0xFF);
void					initialize();
bool					ispause();
void					iteminv(int x, int y, int sx, int sy, int rec, bool resize = false);
void					label(int x, int y, int id, const char* temp, bool checked, bool disabled);
void					modify(color* pal, int index, int start, int count);
void					number(int x, int y, int digits, int value);
void					numbersm(int x, int y, int digits, int value, int type = 0);
void					radio(int x, int y, const runable& ev, int frame);
void					setcamera(point pt);
void					setcolor(color_s value);
void					setfont(res::tokens e);
void					setpallette(int daylight);
void					setpause(bool value);
void					tiles(point screen);
}
const int				world_map_tile_width = 50; // Width of world map tile
const int				world_map_tile_height = 50; // Height of world map tile
const int				world_map_width = 28; // Width of world map int tiles
const int				world_map_height = 30; // Height of world map int tiles
const int				tile_width = 80; // Width of isometric tile
const int				tile_height = 36; // Height of isometric tile
point					h2m(point pt);
point					s2m(point s); // Convert screen coordinates to tile index
inline short unsigned	m2i(short unsigned x, short unsigned y) { return (y << 8) + x; }
inline short unsigned	m2i(point pt) { return m2i(pt.x, pt.y); }
point					m2s(int x, int y);
point					m2h(int x, int y);
inline int				i2x(int index) { return index & 0xFF; }
inline int				i2y(int index) { return index >> 8; }
inline point			i2s(int index) { return m2s(i2x(index), i2y(index)); }
inline point			i2h(int index) { return m2h(i2x(index), i2y(index)); }
namespace map {
const int				mvc = 24;
const int				scanline = 256;
const int				gridw = 32;
const int				gridh = 16;
void					create();
extern int				height;
short unsigned			get(int index);
void					set(int index, short unsigned fid);
void					set(rect rc, int landscape);
int						moveto(int index, int d);
extern int				width;
}
namespace game {
namespace generate {
void					creature(unsigned char* data, int premade);
int						scenery(int type, int index);
int						player(unsigned char* data, int index, int orientation);
int						player(int type, int index, int orientation = 2);
}
void					additem(int player, int itm);
extern unsigned			datetime;
void					dropitem(int index, int rec);
int						find(int id, unsigned char* data, int i1, int i2);
int						get(int id, unsigned char* data);
int						get(int player, int id);
int						getdistance(point p1, point p2);
char*					getformula(char* result, int id);
int						getground(int** result, int index);
int						getlongest(point start, point to);
int						getorientation(point start, point to);
char*					getname(char* result, unsigned char* data);
char*					getname(char* result, int rec);
const char*				getnamepart(int id);
const char*				getnameshort(int id);
short unsigned			getnextindex(short unsigned index, int orientation);
int						getwears(int** result, int player);
int						getwearsweight(int player);
bool					ispause();
void					setaction(int player, int action);
void					setpause(bool value);
int						getwm(int x, int y);
void					setwm(int x, int y, int value);
void					moveto(int player, short unsigned goal, int run = -1);
void					passtime(int count);
int						summ(unsigned char* data, int i1, int i2);
int						summnz(unsigned char* data, int i1, int i2);
}
extern ability_info		ability_data[];
extern const char*		ability_values[11];
extern gender_info		gender_data[];
extern resist_info		damage_data[];
extern illness_info		illness_data[];
extern parameter_info	parameter_data[];
extern perk_info		perk_data[];
extern skill_info		skill_data[];
wound_info				wound_data[];