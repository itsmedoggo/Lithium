// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

#if LITHIUM
#include "common.h"
#include "p_weapons.h"
#include "p_player.h"
#include "p_hudid.h"
#include "w_world.h"

script ext("ACS")
void Lith_DOGS()
{
   withplayer(LocalPlayer)
   {
      i32 tid = ACS_UniqueTID();

      ACS_SetMusic(sp_lmusic_DOGS);

      for(i32 i = 0; i < (35 * 30) / 10; i++)
      {
         k32 ang = ACS_RandomFixed(0, 1);
         k32 dst = ACS_RandomFixed(0, 64);
         ACS_SpawnForced(so_Steggles, p->x + ACS_Cos(ang) * dst, p->y + ACS_Sin(ang) * dst, p->z + 8, tid);
         InvGive(so_Alerter, 1);
         ACS_Delay(10);
      }

      ACS_Delay(35);

      ACS_SetMusic(sp_lsounds_Silence);

      SetPropI(tid, APROP_MasterTID, p->tid);
      ACS_SetActorState(tid, sm_PureSteggleEnergy);
   }
}

script ext("ACS")
void Lith_SteggleEnergy()
{
   withplayer(Lith_GetPlayer(0, AAPTR_FRIENDPLAYER))
   {
      ACS_SetPointer(AAPTR_TARGET, 0, AAPTR_FRIENDPLAYER);

      for(;;)
      {
         k32 x = GetX(0);
         k32 y = GetY(0);
         k32 z = GetZ(0);

         k32 nx = lerpk(x, p->x, 0.01);
         k32 ny = lerpk(y, p->y, 0.01);
         k32 nz = lerpk(z, p->z, 0.01);

         ACS_Warp(0, nx, ny, nz, 0, WARPF_ABSOLUTEPOSITION|WARPF_NOCHECKPOSITION|WARPF_INTERPOLATE);
         ACS_SetActorAngle(0, ACS_VectorAngle(p->x - x, p->y - y));

         ACS_Delay(1);
      }
   }
}

script ext("ACS")
void Lith_BarrierBullets()
{
   withplayer(Lith_GetPlayer(0, AAPTR_TARGET))
   {
      k32 ang   = ACS_VectorAngle(GetX(0) - p->x, GetY(0) - p->y);
      k32 xang  = ACS_VectorAngle(p->x - GetX(0), p->y - GetY(0));
      k32 zdiff = p->z - GetZ(0);
      k32 s     = ACS_Sin(ang) * 48.0;
      k32 c     = ACS_Cos(ang) * 48.0;
      k32 z     = (p->z + p->viewheight / 2) - (zdiff / 2);

      ACS_SpawnForced(so_BarrierFX, p->x + c, p->y + s, z);
      ACS_LineAttack(p->tid, ang + ACS_RandomFixed(-0.1, 0.1), PITCH_BASE + ACS_RandomFixed(0.45, 0.55), 10);
   }
}

script ext("ACS")
bool Lith_BarrierCheck()
{
   ACS_SetActivatorToTarget(0);
   return ACS_CheckFlag(0, s_COUNTKILL);
}

script ext("ACS")
void Lith_GetSigil()
{
   withplayer(LocalPlayer)
   {
      p->closeGUI();

      p->sgacquired = true;

      ACS_Thing_Remove(InvNum(so_DivisionSigilSpriteTID));
      InvTake(so_DivisionSigilSpriteTID, 0x7FFFFFFF);

      if(world.dbgLevel)
         return;

      world.freeze(true);

      ACS_FadeTo(0, 0, 0, 0.4, TS * 3);
      ACS_SetHudSize(320, 200);

      ACS_Delay(3);

      ACS_SetFont(s_dbigfont);
      ACS_BeginPrint();
      ACS_PrintString(st_div_get);
      ACS_MoreHudMessage();
      HudMessageParams(HUDMSG_TYPEON, hid_sigil_title, CR_ORANGE, 160.4, 100.2, 1.5, TS * 5, 0.3);

      ACS_SetFont(s_dbigfont);
      ACS_BeginPrint();
      ACS_PrintString(st_div_warning);
      ACS_MoreHudMessage();
      HudMessageParams(HUDMSG_TYPEON, hid_sigil_subtitle, CR_RED, 160.4, 100.1, 1.0, TS * 2, 0.3);

      ACS_Delay(35 * 7);

      ACS_FadeTo(0, 0, 0, 0.0, TS * 5);

      world.freeze(false);
   }
}
#endif

// EOF
