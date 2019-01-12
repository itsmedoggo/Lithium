// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// u_vitalscan.c: VitalScan upgrade.

#include "u_common.h"
#include "w_world.h"
#include "w_monster.h"

#include <math.h>

#define UData UData_VitalScan(upgr)

// Extern Functions ----------------------------------------------------------|

script
void Upgr_VitalScan_Update(struct player *p, struct upgrade *upgr)
{
   ACS_SetActivator(0, AAPTR_PLAYER_GETTARGET);

   bool six = ACS_StrCmp(ACS_GetActorClass(0), so_DeVileSix, 11) == 0;

   bool validtarget =
      six ||
      ACS_CheckFlag(0, s_COUNTKILL) ||
      ACS_PlayerNumber() != -1;

   if(GetPropI(0, APROP_Health) <= 0)
      UData = (struct upgr_data_VitalScan){};
   else if(validtarget)
   {
      bool legendary = world.legendoom && InvNum(so_LegendaryMonsterToken);
      bool henshin   = world.legendoom && InvNum(so_LegendaryMonsterTransformed);

      bool phantom = InvNum(so_IsPhantom);

      bool freak  = ACS_CheckFlag(0, s_INVULNERABLE) || ACS_CheckFlag(0, s_NODAMAGE);
      bool boss   = ACS_CheckFlag(0, s_BOSS);
      bool shadow = ACS_CheckFlag(0, s_SHADOW);

      i32 chp = GetPropI(0, APROP_Health);
      i32 shp = GetPropI(0, APROP_SpawnHealth);

      i32 id = Lith_UniqueID();
      #if LITHIUM
      dmon_t const *const m = DmonSelf();
      #endif

      i32 ot = UData.target;

      if(UData.target != id)
      {
         memset(&UData, 0, sizeof UData);
         UData.oldhealth = UData.health;
      }

      UData.oldtarget = ot;
      UData.target    = id;

      bool healthset = false;

      if((freak || boss) && !phantom)
      {
         extern str Lith_RandomName(i32 id);

         UData.tagstr = Lith_RandomName(freak ? 0 : id);

         if(p->getCVarI(sc_scanner_bar))
         {
            UData.oldhealth = UData.health = ACS_Random(0, 666666);
            UData.maxhealth = ACS_Random(0, 666666);
            healthset = true;
         }
      }
      else
      {
         char color = p->getCVarI(sc_scanner_color) & 0x7F;

              if(six)     UData.tagstr = st_red_six;
         else if(henshin) UData.tagstr = StrParam("\CgLegendary\C%c %tS", color, 0);
         else if(phantom) UData.tagstr = StrParam("\Cg%tS", 0);
         else             UData.tagstr = StrParam("\C%c%tS", color, 0);
      }

      if(!healthset)
      {
         UData.oldhealth = UData.health;
         UData.health    = chp;
         #if LITHIUM
         UData.maxhealth = m ? m->maxhealth : shp;
         #else
         UData.maxhealth = shp;
         #endif
      }

      #if LITHIUM
      if(m)
      {
         i32 level    = shadow ? m->level - ACS_Random(-5, 5) : m->level;
         UData.tagstr = StrParam("%S lv.%i", UData.tagstr, level);
         UData.rank   = m->rank;
      }
      else
      #endif
         UData.rank = 0;

      i32 splitr = chp % shp;
      i32 split  = chp / shp;
      UData.splitfrac = splitr / (k32)shp;
      UData.split     = minmax(split + 1, 1, 14);

      UData.freak  = six || freak || phantom || boss;
      UData.cangle = ACS_VectorAngle(p->x - GetX(0), p->y - GetY(0)) * tau;

      // Hit indicator
      if(UData.hdtime != 0) UData.hdtime--;
      else                  UData.hdelta = 0;

      if(UData.health < UData.oldhealth) {
         UData.hdelta = UData.oldhealth - UData.health;
         UData.hdtime = 30;
      }
   }
}

stkcall
void Upgr_VitalScan_Render(struct player *p, struct upgrade *upgr)
{
   if(!p->hudenabled || !UData.target) return;

   if(UData.hdtime == 30) SetFade(fid_vscan, 10, 0.1);

   i32 ox = p->getCVarI(sc_scanner_xoffs);
   i32 oy = p->getCVarI(sc_scanner_yoffs);

   if(p->getCVarI(sc_scanner_slide))
   {
      k64 diff = p->yawf - (k64)UData.cangle;
      k32 ds = ACS_Sin(diff / tau) * tau;
      k32 dc = ACS_Cos(diff / tau) * tau;
      UData.oangle = lerplk(UData.oangle, atan2f(ds, dc), 0.1);
      ox += UData.oangle * 64;
   }

   // Rank
   if(UData.rank) for(i32 i = 0; i < UData.rank; i++) {
      i32 y = 216 + (i > 4 ? 8 : 0);
      PrintSprite(StrParam(":UI:Rank%i", i+1), 106+ox + i%5*6,1, y+oy,1);
   }

   // Hit indicator
   if(UData.hdelta && CheckFade(fid_vscan)) {
      PrintTextFmt("-%i", UData.hdelta);
      PrintTextF(s_cbifont, CR_RED, 160+ox,4, 235+oy,2, fid_vscan);
   }

   // Tag and health
   bool afnt = p->getCVarI(sc_scanner_altfont);
   str  font = afnt ? s_chfont : s_cbifont;

   PrintText_str(UData.tagstr, font, CR_WHITE, 160+ox,4, 216+oy,2);

   if(UData.maxhealth) PrintTextFmt("%u/%u", UData.health, UData.maxhealth);
   else                PrintTextFmt("%uhp",  UData.health);
   PrintTextX(UData.freak ? s_alienfont : font, CR_WHITE, 160+ox,4, 225+oy,2);

   // Health bar
   if(p->getCVarI(sc_scanner_bar))
   {
      i32 x = 120 + ox;
      i32 y = (afnt ? 201 : 205) + oy;

      if(UData.split > 1)
         PrintSprite(StrParam(":UI:HealthBar%i", UData.split - 1), x,1, y,1);

      SetClip(x, y, 80 * UData.splitfrac, 2);
      PrintSprite(StrParam(":UI:HealthBar%i", UData.split), x,1, y,1);
      ClearClip();
   }
}

// EOF
