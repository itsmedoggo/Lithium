#include "lith_common.h"
#include "lith_player.h"
#include "lith_pickups.h"

#include <math.h>

#define ValidateWeapon(parm) (parm < weapon_max && parm >= weapon_min)


//----------------------------------------------------------------------------
// Extern Objects
//

weaponinfo_t weaponinfo[weapon_max] = {
// {S, "Type-----------", "Pickup Sound-----------", AT_Type},
   {0, null,              "MMMMHMHMMMHMMM"},
   {1, "Fist",            "MMMMHMHMMMHMMM"},
   {2, "Pistol",          "weapons/pistol/pickup",   AT_Mag},
   {2, "Revolver",        "weapons/revolver/pickup", AT_Mag},
   {3, "Shotgun",         "weapons/shotgun/pickup",  AT_Mag},
   {4, "CombatRifle",     "weapons/rifle/pickup",    AT_Mag},
   {5, "GrenadeLauncher", "weapons/rocket/pickup",   AT_Ammo},
   {6, "PlasmaRifle",     "weapons/plasma/pickup",   AT_Ammo},
   {7, "BFG9000",         "weapons/cannon/pickup",   AT_Ammo},
};


//----------------------------------------------------------------------------
// Extern Functions
//

//
// Lith_SetupWeaponsTables
//
void Lith_SetupWeaponsTables(void)
{
   for(int i = 0; i < weapon_max; i++)
   {
      weaponinfo[i].type = i;
      weaponinfo[i].class = StrParam("Lith_%S", weaponinfo[i].name);
   }
}

//
// Lith_GetWeaponType
//
// Update information on what kind of weapons we have.
//
void Lith_GetWeaponType(player_t *p)
{
   weaponinfo_t *weapon = null;
   
   for(int i = 0; i < SLOT_MAX; i++)
      p->hasslot[i] = false;
   
   for(int i = weapon_min; i < weapon_max; i++)
   {
      weaponinfo_t *info = &weaponinfo[i];
      
      p->hasslot[info->slot] |= p->hasweapon[i] = ACS_CheckInventory(info->class);
      
      if(!weapon && ACS_StrICmp(p->weaponclass, info->class) == 0)
         weapon = info;
   }
   
   p->curweapon = weapon ? weapon : &weaponinfo[weapon_unknown];
}


//----------------------------------------------------------------------------
// Scripts
//

//
// Lith_WeaponPickup
//
[[__call("ScriptI"), __address(14242), __extern("ACS")]]
void Lith_WeaponPickup(int parm, int tid)
{
   player_t *p = Lith_LocalPlayer;
   
   if(!ValidateWeapon(parm) || p->hasweapon[parm])
      return;
   
   weaponinfo_t *info = &weaponinfo[parm];
   
   p->weaponsheld++;
   
   if(!ACS_GetCVar("sv_weaponstay"))
      ACS_Thing_Remove(tid);
   
   if(!p->upgrades[UPGR_7777777].active)
      ACS_LocalAmbientSound(info->pickupsound, 127);
   else
      ACS_LocalAmbientSound("marathon/pickup", 127);
   
   if(info->name) Lith_UnlockBIPPage(&p->bip, info->name);
   
   switch(parm)
   {
   case weapon_launcher: ACS_GiveInventory("Lith_RocketAmmo", 10);   break;
   case weapon_plasma:   ACS_GiveInventory("Lith_PlasmaAmmo", 1500); break;
   case weapon_bfg:      ACS_GiveInventory("Lith_CannonAmmo", 4);    break;
   }
   
   Lith_PickupMessage(p, info);
}

//
// Lith_PickupScore
//
[[__call("ScriptS"), __extern("ACS")]]
int Lith_PickupScore(int parm, int spritetid)
{
   ACS_SetActivatorToTarget(0);
   player_t *p = Lith_LocalPlayer;
   
   if(ACS_GetCVar("sv_weaponstay") || !ValidateWeapon(parm) || !p->hasweapon[parm])
      return true;
   
   Lith_Log(p, "> You sold the weapon for Score.");
   Lith_GiveScore(p, 11100 * parm);
   
   ACS_Thing_Remove(spritetid);
   
   return false;
}

//
// Lith_CircleSpread
//
[[__call("ScriptS"), __extern("ACS")]]
int Lith_CircleSpread(fixed mdx, fixed mdy, bool getpitch)
{
   static fixed A;
   static fixed P;
   
   if(!getpitch)
   {
      fixed dx = RandomFixed(mdx, 0.0);
      fixed dy = RandomFixed(mdy, 0.0);
      fixed a  = RandomFixed(1.0, -1.0);
      
      A = sink(a) * dx;
      P = cosk(a) * dy;
      
      return bitsk(A);
   }
   else
      return bitsk(P);
}

//
// Lith_PunctuatorFire
//
[[__call("ScriptS"), __extern("ACS")]]
void Lith_PunctuatorFire(void)
{
   player_t *p = Lith_LocalPlayer;
   int ptid = ACS_UniqueTID();
   
   ACS_LineAttack(0, p->yaw, p->pitch, 128, "Lith_PunctuatorPuff", "None", 2048.0, FHF_NORANDOMPUFFZ, ptid);
   
   if(ACS_ThingCount(T_NONE, ptid))
   {
      fixed x = ACS_GetActorX(ptid);
      fixed y = ACS_GetActorY(ptid);
      fixed z = ACS_GetActorZ(ptid);
      
      float yaw = atan2f(y - p->y, x - p->x);
      
      float cx = cos(yaw);
      float cy = sin(yaw);
      
      for(int i = 0; i < 10; i++)
      {
         float sx = x + (cx * (32 * i));
         float sy = y + (cy * (32 * i));
         
         int etid = ACS_UniqueTID();
         
         ACS_SpawnForced("Lith_PunctuatorExplosion", sx, sy, z, etid);
         
         ACS_SetActivator(etid);
         ACS_SetPointer(AAPTR_TARGET, p->tid);
         ACS_SetActivator(p->tid);
      }
   }
}

// EOF
