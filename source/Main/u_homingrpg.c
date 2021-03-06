// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// u_homingrpg.c: HomingRPG upgrade.

#include "u_common.h"
#include "w_world.h"

#define UData UData_HomingRPG(upgr)

// Static Functions ----------------------------------------------------------|

script
static i32 CheckTarget(struct player *p)
{
   if(ACS_SetActivator(0, AAPTR_PLAYER_GETTARGET))
      return Lith_UniqueID(0);
   else
      return 0;
}

// Extern Functions ----------------------------------------------------------|

script
void Upgr_HomingRPG_Update(struct player *p, struct upgrade *upgr)
{
   if(p->weapontype == weapon_launcher)
   {
      if(p->buttons & BT_ALTATTACK)
      {
         i32 id;
         if((id = CheckTarget(p)) && id != UData.id)
         {
            ACS_LocalAmbientSound(ss_weapons_rocket_mark, 127);
            ACS_SetPointer(AAPTR_TRACER, 0, AAPTR_PLAYER_GETTARGET);
            UData.id = id;
         }
      }

      if(ACS_SetActivator(0, AAPTR_TRACER) && GetPropI(0, APROP_Health) > 0)
         ACS_SpawnForced(so_TargetMarker, GetX(0), GetY(0), GetZ(0) + (GetPropK(0, APROP_Height) / 2k));
   }
}

// Scripts -------------------------------------------------------------------|

script_str ext("ACS") addr("Lith_HomingMissile")
void Sc_HomingMissile(void)
{
   struct player *p = Lith_GetPlayer(0, AAPTR_TARGET);
   ACS_SetPointer(AAPTR_TRACER, p->tid, AAPTR_TRACER);
}

// EOF
