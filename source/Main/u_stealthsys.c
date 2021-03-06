// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// u_stealthsys.c: StealthSys upgrade.

#include "u_common.h"

#define UData UData_StealthSys(upgr)

// Extern Functions ----------------------------------------------------------|

stkcall
void Upgr_StealthSys_Deactivate(struct player *p, struct upgrade *upgr)
{
   p->alpha = 1;
   InvTake(so_StealthSystem, 1);
}

script
void Upgr_StealthSys_Update(struct player *p, struct upgrade *upgr)
{
   k32 vel = absk(p->getVel()) / 10.0;
   p->alpha = UData.mulvel = lerpk(UData.mulvel, vel, 0.02);

   i32 time = UData.mulvel * 20;
   if(!time || ACS_Timer() % time == 0) InvGive(so_StealthSystem, 1);
   else                                 InvTake(so_StealthSystem, 1);
}

// EOF
