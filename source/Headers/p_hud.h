// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// p_hud.h: Generalized HUD functions.

#ifndef p_hud_h
#define p_hud_h

#include "w_world.h"

#define Lith_HUD_StaParam(type, ...) \
   __with(static struct type const _a = {__VA_ARGS__}, *const _ap = &_a;)

#define Lith_HUD_WeaponSlots(h, ...) \
   Lith_HUD_StaParam(hud_wsl, __VA_ARGS__) Lith_HUD_WeaponSlots_Impl((h), _ap)

struct hud_wsl
{
   i32 ncol[3];
   str scol;
   i32 x;
   i32 y;
};

void Lith_HUD_WeaponSlots_Impl(struct player *p, struct hud_wsl const *a);
void Lith_HUD_Score(struct player *p, char const *fmt, i96 scr, str font, str cr, i32 x, i32 xa, i32 y, i32 ya);
void Lith_HUD_KeyInd(struct player *p, i32 x, i32 y, bool horz, k32 a);
script void Lith_HUD_Log(struct player *p, i32 cr, i32 x, i32 yy);

#endif
