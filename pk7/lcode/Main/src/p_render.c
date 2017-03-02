#include "lith_common.h"
#include "lith_player.h"
#include "lith_hudid.h"

#include <math.h>


//----------------------------------------------------------------------------
// Static Functions
//

static void HUD_StringStack(player_t *p);
static void HUD_Waves(player_t *p);


//----------------------------------------------------------------------------
// Extern Functions
//

//
// Lith_PlayerDamageBob
//
// Update view bobbing when you get damaged.
//
[[__call("ScriptS")]]
void Lith_PlayerDamageBob(player_t *p)
{
   if(!ACS_CheckInventory("PowerStrength") && p->health < p->old.health)
   {
      float angle = RandomFloat(tau, -tau);
      float distance;
      
      distance = (p->old.health - p->health) / (float)p->maxhealth;
      distance *= 0.2f;
      distance *= (200 - p->armor) / 200.0f;
      
      p->bobyaw   = sinf(angle) * distance;
      p->bobpitch = cosf(angle) * distance;
   }
   
   p->bobpitch = lerpf(p->bobpitch, 0.0f, 0.1f);
   p->bobyaw   = lerpf(p->bobyaw,   0.0f, 0.1f);
}

//
// Lith_PlayerView
//
// Update additive view.
//
[[__call("ScriptS")]]
void Lith_PlayerView(player_t *p)
{
   float addp = 0, addy = 0;
   
   if(ACS_GetUserCVar(p->number, "lith_player_damagebob"))
   {
      float bobmul = ACS_GetUserCVarFixed(p->number, "lith_player_damagebobmul");
      addp += p->bobpitch * bobmul;
      addy += p->bobyaw   * bobmul;
   }
   
   p->extrpitch = lerpf(p->extrpitch, 0.0f, 0.1f);
   p->extryaw   = lerpf(p->extryaw,   0.0f, 0.1f);
   
   p->addpitch = addp + p->extrpitch;
   p->addyaw   = addy + p->extryaw;
}

//
// Lith_PlayerStyle
//
void Lith_PlayerStyle(player_t *p)
{
   if(p->scopetoken)
   {
      ACS_SetActorProperty(0, APROP_RenderStyle, STYLE_Subtract);
      ACS_SetActorPropertyFixed(0, APROP_Alpha, ACS_GetUserCVarFixed(p->number, "lith_weapons_scopealpha"));
   }
   else
   {
      ACS_SetActorProperty(0, APROP_RenderStyle, STYLE_Translucent);
      ACS_SetActorPropertyFixed(0, APROP_Alpha, ACS_GetUserCVarFixed(p->number, "lith_weapons_alpha"));
   }
}

//
// Lith_PlayerHUD
//
[[__call("ScriptS")]]
void Lith_PlayerHUD(player_t *p)
{
   ACS_SetHudSize(320, 200);
   
   if(p->old.scopetoken && !p->scopetoken)
   {
      Lith_ListFree(&p->hudstrlist, free);
      
      for(int i = hid_scope_clearS; i <= hid_scope_clearE; i++)
      {
         HudMessage("");
         HudMessagePlain(i, 0.0, 0.0, 0.0);
      }
   }
   
   if(p->scopetoken)
   {
      HUD_Waves(p);
      HUD_StringStack(p);
   }
}


//----------------------------------------------------------------------------
// Static Functions
//

//
// HUD_StringStack
//
static void HUD_StringStack(player_t *p)
{
   typedef struct hudstr_s
   {
      __str str;
      list_t link;
   } hudstr_t;
   
   if((ACS_Timer() % 3) == 0)
   {
      hudstr_t *hudstr = calloc(1, sizeof(hudstr_t));
      Lith_LinkDefault(&hudstr->link, hudstr);
      hudstr->str = StrParam("%.8X", ACS_Random(0, 0x7FFFFFFF));
      
      Lith_ListLink(&p->hudstrlist, &hudstr->link);
      
      if(Lith_ListSize(&p->hudstrlist) == HUDSTRS_MAX)
         free(Lith_ListUnlink(p->hudstrlist.next));
   }
   
   ACS_SetHudSize(320, 200);
   ACS_SetFont("CONFONT");
   
   size_t i = 0;
   for(list_t *rover = p->hudstrlist.prev; rover != &p->hudstrlist; rover = rover->prev, i++)
   {
      hudstr_t *hudstr = rover->object;
      HudMessage("%S", hudstr->str);
      HudMessageParams(HUDMSG_ALPHA | HUDMSG_ADDBLEND, hid_scope_stringstackS - i, CR_RED, 300.2, 20.1 + (i * 9), 0.0, 0.5);
   }
}

//
// HUD_Waves
//
static void HUD_Waves(player_t *p)
{
   fixed health = (fixed)p->health / (fixed)p->maxhealth;
   int frame = minmax(health * 4, 1, 5);
   int timer = ACS_Timer();
   int pos;
   
   ACS_SetHudSize(320, 200);
   
   // Sine (health)
   pos = (10 + timer) % 160;
   DrawSpriteFade(StrParam("H_D1%i", frame),
      hid_scope_sineS - pos,
      300.1 + roundk(sink(pos / 32.0) * 7.0, 0),
      25.1 + pos,
      1.5, 0.3);
   
   // Square
   {
      fixed a = cosk(pos / 32.0);
      
      pos = (7 + timer) % 160;
      DrawSpriteFade(roundk(a, 2) != 0.0 ? "H_D16" : "H_D46",
         hid_scope_squareS - pos,
         300.1 + (a >= 0) * 7.0,
         25.1 + pos,
         1.9, 0.1);
   }
   
   // Triangle
   pos = (5 + timer) % 160;
   DrawSpriteFade("H_D14", hid_scope_triS - pos, 300.1 + abs((pos % 16) - 8), 25.1 + pos, 1.2, 0.2);
}

// EOF
