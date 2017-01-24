#include "lith_upgrades_common.h"


//----------------------------------------------------------------------------
// Extern Functions
//

//
// Activate
//
void Upgr_UNCEUNCE_Activate(player_t *p, upgrade_t *upgr)
{
   ACS_SetMusic("lmusic/Unce.flac");
}

//
// Deactivate
//
void Upgr_UNCEUNCE_Deactivate(player_t *p, upgrade_t *upgr)
{
   ACS_SetMusic("*");
   ACS_FadeTo(0, 0, 0, 0, 0);
}

//
// Update
//
[[__call("ScriptS")]]
void Upgr_UNCEUNCE_Update(player_t *p, upgrade_t *upgr)
{
   fixed reeeed = ACS_Sin(p->ticks / 35.0) + 1.0 / 2.0;
   fixed greeen = ACS_Cos(p->ticks / 24.0) + 1.0 / 2.0;
   fixed bluuue = ACS_Sin(p->ticks / 13.0) + 1.0 / 2.0;
   
   ACS_FadeTo(reeeed * 255, greeen * 255, bluuue * 255, 0.6, TICSECOND);
   p->extrpitch += bluuue * 0.015;
   p->extryaw   += greeen * 0.01;
}

// EOF
