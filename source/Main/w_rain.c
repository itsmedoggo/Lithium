// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// w_rain.c: Weather code.

#if LITHIUM
#include "common.h"
#include "p_player.h"
#include "w_world.h"

static bool lmvar rain_chk;
static k32  lmvar rain_px;
static k32  lmvar rain_py;
static i32  lmvar rain_dist;

script
void Lith_DoRain(void)
{
   struct player *p = &players[0];
   p->setActivator();

   ACS_PlaySound(p->weathertid, ss_amb_wind, CHAN_BODY,  0.001, true, ATTN_NONE);
   ACS_PlaySound(p->weathertid, ss_amb_rain, CHAN_VOICE, 0.001, true, ATTN_NONE);

   k32 skydist, curskydist = 1;
   for(;;)
   {
      if((rain_chk = !ACS_CheckActorCeilingTexture(0, s_F_SKY1)))
      {
         rain_dist = 1024;
         rain_px = p->x;
         rain_py = p->y;
      }
      else
         InvTake(so_SMGHeat, 1);

      if((InHell || InSecret) && !world.islithmap)
         ServCallI(sm_SpawnRain, so_BloodRainDrop);
      else
         ServCallI(sm_SpawnRain, so_RainDrop);

      ACS_Delay(1);

      if(rain_chk)
      {
         skydist = rain_dist / 1024.0;
         skydist = minmax(skydist, 0, 1);
      }
      else
         skydist = 0;

      curskydist = lerpk(curskydist, skydist, 0.035);
      ACS_SoundVolume(p->weathertid, CHAN_BODY,  1 - curskydist);
      ACS_SoundVolume(p->weathertid, CHAN_VOICE, 1 - curskydist);
   }
}

// Scripts -------------------------------------------------------------------|

script ext("ACS") addr(lsc_raindropspawn)
void Sc_RainDropSpawn(void)
{
   if(rain_chk)
   {
      i32 dist = mag2i(GetX(0) - rain_px, GetY(0) - rain_py);
      if(dist < rain_dist) rain_dist = dist;
   }
}
#endif

// EOF
