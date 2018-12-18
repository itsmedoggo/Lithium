// This file was generated by wepc.
// Edit only if you aren't going to recompile.
#if LITHIUM
#include "lith_common.h"
#include "lith_player.h"

StrEntON

// Extern Objects ------------------------------------------------------------|

#define Placeholder1 "MMMMHMHMMMHMMM"
#define Placeholder2 "YOUSONOFABITCH"
#define A(a) OBJ a "Ammo"
#define O(a) OBJ a
#define P(a) "weapons/" a "/pickup"
#define N(a) .classname = OBJ a, .name = a
#define F(...) .flags = __VA_ARGS__
weaponinfo_t const weaponinfo[weapon_max] = {
   {0, pcl_any, null, "MMMMHMHMMMHMMM"},
   {1,pcl_outcasts,N("ChargeFist"),P("cfist"),AT_None},
   {5,pcl_outcasts,N("MissileLauncher"),P("missile"),AT_AMag,A("Rocket")},
   {6,pcl_outcasts,N("PlasmaDiffuser"),P("plasdiff"),AT_Ammo,A("Plasma")},
   {1,pcl_marine,N("Fist"),Placeholder1,AT_None},
   {2,pcl_marine,N("Pistol"),P("pistol"),AT_NMag},
   {2,pcl_marine,N("Revolver"),P("revolver"),AT_NMag},
   {3,pcl_marine,N("Shotgun"),P("shotgun"),AT_None},
   {3,pcl_marine,N("LazShotgun"),P("lshotgun"),AT_None},
   {3,pcl_marine,N("SuperShotgun"),P("ssg"),AT_Ammo,A("Shell")},
   {4,pcl_marine,N("CombatRifle"),P("rifle"),AT_NMag},
   {4,pcl_marine,N("SniperRifle"),P("sniper"),AT_NMag},
   {5,pcl_marine,N("GrenadeLauncher"),P("rocket"),AT_Ammo,A("Rocket")},
   {6,pcl_marine,N("PlasmaRifle"),P("plasma"),AT_Ammo,A("Plasma")},
   {7,pcl_marine,N("BFG9000"),P("cannon"),AT_Ammo,A("Cannon")},
   {1,pcl_cybermage,N("CFist"),Placeholder2,AT_None},
   {2,pcl_cybermage,N("Mateba"),P("mateba"),AT_NMag},
   {3,pcl_cybermage,N("ShockRifle"),P("erifle"),AT_NMag},
   {3,pcl_cybermage,N("SPAS"),P("cshotgun"),AT_AMag,A("Shell")},
   {4,pcl_cybermage,N("SMG"),P("smg"),AT_NMag},
   {5,pcl_cybermage,N("IonRifle"),P("ionrifle"),AT_AMag,A("Rocket")},
   {6,pcl_cybermage,N("CPlasmaRifle"),P("plasma"),AT_Ammo,A("Plasma")},
   {7,pcl_cybermage,N("StarDestroyer"),P("shipgun"),AT_Ammo,A("Cannon")},
   {0,pcl_cybermage,N("Blade"),Placeholder1,AT_Ammo,O("Mana"),F(wf_magic)},
   {0,pcl_cybermage,N("Delear"),Placeholder1,AT_AMag,O("Mana"),F(wf_magic)},
   {0,pcl_cybermage,N("Feuer"),Placeholder1,AT_Ammo,O("Mana"),F(wf_magic)},
   {0,pcl_cybermage,N("Rend"),Placeholder1,AT_Ammo,O("Mana"),F(wf_magic)},
   {0,pcl_cybermage,N("Hulgyon"),Placeholder1,AT_Ammo,O("Mana"),F(wf_magic)},
   {0,pcl_cybermage,N("StarShot"),Placeholder1,AT_AMag,O("Mana"),F(wf_magic)},
   {0,pcl_cybermage,N("Cercle"),Placeholder1,AT_Ammo,O("Mana"),F(wf_magic)},
   {2,pcl_darklord,N("700Express"),Placeholder2,AT_NMag},
   {3,pcl_darklord,N("4Bore"),Placeholder2,AT_Ammo,A("Shell")},
   {3,pcl_darklord,N("ShrapnelGun"),Placeholder2,AT_Ammo,A("Shell")},
   {4,pcl_darklord,N("Minigun"),Placeholder2,AT_NMag},
   {5,pcl_darklord,N("DualRocket"),Placeholder2,AT_Ammo,A("Rocket")},
   {6,pcl_darklord,N("FatMac"),Placeholder2,AT_Ammo,A("Plasma")},
   {7,pcl_darklord,N("FortuneGun"),Placeholder2,AT_Ammo,A("Cannon")},
   {0,pcl_any,N("WingsOfDeath"),"MMMMHMHMMMHMMM",AT_None},

};

// Extern Functions ----------------------------------------------------------|

int Lith_WeaponFromName(struct player *p, int name)
{
   switch(p->pclass)
   {
   case pcl_marine:
      switch(name) {
      case wepnam_fist: return weapon_fist;
      case wepnam_chainsaw: return weapon_cfist;
      case wepnam_pistol: return weapon_pistol;
      case wepnam_shotgun: return weapon_shotgun;
      case wepnam_supershotgun: return weapon_ssg;
      case wepnam_chaingun: return weapon_rifle;
      case wepnam_rocketlauncher: return weapon_launcher;
      case wepnam_plasmarifle: return weapon_plasma;
      case wepnam_bfg9000: return weapon_bfg;
      }
   case pcl_cybermage:
      switch(name) {
      case wepnam_fist: return weapon_c_fist;
      case wepnam_chainsaw: return weapon_cfist;
      case wepnam_pistol: return weapon_c_mateba;
      case wepnam_shotgun: return weapon_c_rifle;
      case wepnam_supershotgun: return weapon_c_spas;
      case wepnam_chaingun: return weapon_c_smg;
      case wepnam_rocketlauncher: return weapon_c_sniper;
      case wepnam_plasmarifle: return weapon_c_plasma;
      case wepnam_bfg9000: return weapon_c_shipgun;
      }
   case pcl_darklord:
      switch(name) {
      case wepnam_fist: return weapon_fist;
      case wepnam_chainsaw: return weapon_fist;
      case wepnam_pistol: return weapon_d_700e;
      case wepnam_shotgun: return weapon_d_4bore;
      case wepnam_supershotgun: return weapon_d_launcher;
      case wepnam_chaingun: return weapon_d_minigun;
      case wepnam_rocketlauncher: return weapon_d_rocket;
      case wepnam_plasmarifle: return weapon_d_fatmac;
      case wepnam_bfg9000: return weapon_d_railgun;
      }
   }

   return weapon_unknown;
}
#endif

// EOF
