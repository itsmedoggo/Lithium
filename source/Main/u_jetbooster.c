// Copyright © 2016-2017 Graham Sanderson, all rights reserved.
#include "lith_upgrades_common.h"

#define UData UData_JetBooster(upgr)

#define CHARGE_MAX (35 * 7)

// Extern Functions ----------------------------------------------------------|

//
// Activate
//
void Upgr_JetBooster_Activate(player_t *p, upgrade_t *upgr)
{
   UData.charge = CHARGE_MAX;
}

//
// Update
//
[[__call("ScriptS")]]
void Upgr_JetBooster_Update(player_t *p, upgrade_t *upgr)
{
   UData.discharged = UData.charge > 60 && UData.charge < CHARGE_MAX;

   if(UData.charge < CHARGE_MAX)
      UData.charge++;

   if(p->frozen) return;

   fixed grounddist = p->z - p->floorz;

   if(p->buttonPressed(BT_SPEED) && grounddist > 16.0 && UData.charge >= CHARGE_MAX)
   {
      fixed angle = p->yaw - ACS_VectorAngle(p->forwardv, p->sidev);

      ACS_PlaySound(0, "player/rocketboost");
      ACS_GiveInventory("Lith_RocketBooster", 1);
      p->setVel(p->velx + (ACS_Cos(angle) * 16.0), p->vely + (ACS_Sin(angle) * 16.0), 10.0, false, true);

      UData.charge = 0;
      UData_ReflexWetw(p->getUpgr(UPGR_ReflexWetw)).leaped = 0;
   }
}

//
// Render
//
void Upgr_JetBooster_Render(player_t *p, upgrade_t *upgr)
{
   if(!p->hudenabled || UData.charge == CHARGE_MAX) return;

   fixed rocket = UData.charge / (fixed)CHARGE_MAX;
   int max = (hid_jetS - hid_jetE) * rocket;

   DrawSpriteFade("H_B3", hid_jetbg, 320.2, 80.1, 0.0, 0.5);

   HudMessageF("SMALLFNT", "Jet");
   HudMessageParams(HUDMSG_FADEOUT, hid_jettext, CR_RED, 320.2, 160.1, 0.1, 0.5);

   for(int i = 0; i < max; i++)
      DrawSpriteXX(UData.discharged ? "H_C1" : "H_C2",
         HUDMSG_FADEOUT | HUDMSG_ADDBLEND | HUDMSG_ALPHA,
         hid_jetS - i,
         320.2,
         150.1 - (i * 5),
         0.1, 0.5, 0.5);
}

// EOF

