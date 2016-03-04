#include "lith_common.h"
#include "lith_player.h"
#include <stdio.h>
#include <math.h>

// ---------------------------------------------------------------------------
// Functions.
//

bool ButtonPressed(player_t *p, int bt)
{
   return p->buttons & bt && !(p->oldbuttons & bt);
}

int Random(int max, int min)
{
   if(max < min)
   {
      int temp = min;
      min = max;
      max = temp;
   }
   
   return min + (rand() % (max - min + 1));
}

fixed RandomFixed(fixed max, fixed min)
{
   return kbits(Random(bitsk(max), bitsk(min)));
}

float RandomFloat(float max, float min)
{
   if(max < min)
   {
      float temp = min;
      min = max;
      max = temp;
   }
   
   return (rand() / (float)RAND_MAX) * (max - min) + min;
}

__str StrParam(__str fmt, ...)
{
   va_list vl;
   
   ACS_BeginPrint();
   
   va_start(vl, fmt);
   __vnprintf_str(fmt, vl);
   va_end(vl);
   
   return ACS_EndStrParam();
}

void HudMessage(__str fmt, ...)
{
   va_list vl;
   
   ACS_BeginPrint();
   
   va_start(vl, fmt);
   __vnprintf_str(fmt, vl);
   va_end(vl);
   
   ACS_MoreHudMessage();
}

void Log(__str fmt, ...)
{
   va_list vl;
   
   ACS_BeginPrint();
   
   va_start(vl, fmt);
   __vnprintf_str(fmt, vl);
   va_end(vl);
   
   ACS_EndLog();
}

accum lerpk(accum a, accum b, accum t)
{
   accum ret = ((1.0 - t) * a) + (t * b);
   
   if(roundk(ret, 10) == b)
   {
      return b;
   }
   
   return ret;
}

float lerpf(float a, float b, float t)
{
   float ret = ((1.0 - t) * a) + (t * b);
   
   if((round(ret << 10) >> 10) == b)
   {
      return b;
   }
   
   return ret;
}

//
// ---------------------------------------------------------------------------

