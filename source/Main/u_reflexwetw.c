// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// u_reflexwetw.c: ReflexWetw upgrade.

#include "u_common.h"

#define UData UData_ReflexWetw(upgr)

#define CHARGE_MAX (35 * 0.8)

// Static Functions ----------------------------------------------------------|

script
static void DodgeView(struct player *p)
{
   k32 vh = p->viewheight;

   for(i32 i = 0; i < 20; i++)
   {
      k32 mul = 1.0 - (ACS_Sin(i / 40.0) * 0.6);
      SetPropK(0, APROP_ViewHeight, vh * mul);
      ACS_Delay(1);
   }

   SetPropK(0, APROP_ViewHeight, vh);
}

// Extern Functions ----------------------------------------------------------|

stkcall
void Upgr_ReflexWetw_Activate(struct player *p, struct upgrade *upgr)
{
   #if LITHIUM
   p->speedmul += 0.3;
   #endif
   UData.charge = CHARGE_MAX;
   UData.leaped = 0;
}

stkcall
void Upgr_ReflexWetw_Deactivate(struct player *p, struct upgrade *upgr)
{
   #if LITHIUM
   p->speedmul -= 0.3;
   #endif
}

script
void Upgr_ReflexWetw_Update(struct player *p, struct upgrade *upgr)
{
   if(UData.charge < CHARGE_MAX)
      UData.charge++;

   if(p->frozen) return;

   if(UData.charge >= CHARGE_MAX)
   {
      if(p->onground) UData.leaped = 0;

      if(p->buttons & BT_SPEED && (p->onground ||
         !p->getUpgrActive(UPGR_JetBooster) ||
         UData_JetBooster(p->getUpgr(UPGR_JetBooster)).discharged))
      {
         k32 angle = p->yaw - ACS_VectorAngle(p->forwardv, p->sidev);

         ACS_LocalAmbientSound(ss_player_slide, 127);
         p->setVel(p->velx + (ACS_Cos(angle) * 32.0), p->vely + (ACS_Sin(angle) * 32.0), 0);

         DodgeView(p);

         UData.charge = 0;
      }
   }

   if(p->waterlevel == 0 && p->buttonPressed(BT_JUMP) && !InvNum(so_PowerFlight) &&
      !InvNum(so_RocketBooster) && !UData.leaped &&
      (!p->onground || UData.charge < CHARGE_MAX))
   {
      k32 angle = p->yaw - ACS_VectorAngle(p->forwardv, p->sidev);

      ACS_LocalAmbientSound(ss_player_doublejump, 127);
      p->setVel(p->velx + (ACS_Cos(angle) * 4.0), p->vely + (ACS_Sin(angle) * 4.0), 12.0);

      UData.leaped = 1;
   }
}

// Scripts -------------------------------------------------------------------|

script_str ext("ACS") addr("Lith_DodgeView")
void Sc_DodgeView(void)
{
   withplayer(LocalPlayer) DodgeView(p);
}

// EOF
