// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// common.h: Common functions and debugging functions.

#ifndef common_h
#define common_h

#pragma GDCC FIXED_LITERAL ON

#include <ACS_ZDoom.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "m_types.h"
#include "m_memory.h"
#include "m_drawing.h"
#include "m_math.h"
#include "m_str.h"
#include "m_stab.h"
#include "m_char.h"

#define ifw(decl, ...) __with(decl;) if(__VA_ARGS__)
#define ifauto(type, name, ...) ifw(type name = (__VA_ARGS__), name)

#define countof(a) (sizeof(a) / sizeof(*(a)))
#define swap(t, a, b) do {t _tmp = a; a = b; b = _tmp;} while(0)

#define IsSmallNumber(x) ((x) > -0.001 && (x) < 0.001)
#define CloseEnough(x, y) (IsSmallNumber(x - y))

#ifndef NDEBUG
#define LogDebug(level, ...) \
   do if(dbglevel & (level)) Log(c"" #level ": " __VA_ARGS__); while(0)
#else
#define LogDebug(...)
#endif

#define TickerT(t, on, off) ((ACS_Timer() % 35) < (t) ? (on) : (off))
#define Ticker(on, off) (TickerT(17, on, off))

#define SCallI ACS_ScriptCall
#define SCallK ACS_ScriptCallFixed
#define SCallS ACS_ScriptCallString

#define GetMembI ACS_GetUserVariable
#define GetMembK ACS_GetUserVariableFixed
#define GetMembS ACS_GetUserVariableString

#define SetMembI ACS_SetUserVariable
#define SetMembK ACS_SetUserVariableFixed

#define GetPropI ACS_GetActorProperty
#define GetPropK ACS_GetActorPropertyFixed
#define GetPropS ACS_GetActorPropertyString

#define SetPropI ACS_SetActorProperty
#define SetPropK ACS_SetActorPropertyFixed
#define SetPropS ACS_SetActorPropertyString

#define GetX ACS_GetActorX
#define GetY ACS_GetActorY
#define GetZ ACS_GetActorZ

#if LITHIUM
#define Lith_IsPaused ServCallI(sm_GetPaused)
#define Lith_PausableTick() do ACS_Delay(1); while(Lith_IsPaused)

#define CVAR  "lith_"
#define DCVAR "__lith_"
#define LANG  "LITH_"
#define OBJ   "Lith_"
#else
#define Lith_IsPaused false
#define Lith_PausableTick()

#define OBJ   "Dt"
#define CVAR  "dtap_"
#define DCVAR "__dtap_"
#define LANG  "DTAP_"
#endif

#define cCVAR  c"" CVAR
#define cDCVAR c"" DCVAR
#define cLANG  c"" LANG
#define cOBJ   c"" OBJ

#define sCVAR  s"" CVAR
#define sDCVAR s"" DCVAR
#define sLANG  s"" LANG
#define sOBJ   s"" OBJ

#define ServCallI(...) SCallI(sm_Serv, __VA_ARGS__)
#define ServCallK(...) SCallK(sm_Serv, __VA_ARGS__)
#define ServCallS(...) SCallS(sm_Serv, __VA_ARGS__)

#define DrawCallI(...) SCallI(sm_Draw, __VA_ARGS__)
#define DrawCallK(...) SCallK(sm_Draw, __VA_ARGS__)
#define DrawCallS(...) SCallS(sm_Draw, __VA_ARGS__)

#define DebugStat(...) \
   (dbglevel & log_devh ? Lith_DebugStat(__VA_ARGS__) : (void)0)

#define DebugNote(...) \
   (dbglevel & log_devh ? Lith_DebugNote(__VA_ARGS__) : (void)0)

#define InvNum  ACS_CheckInventory
#define InvMax(arg) ACS_GetMaxInventory(0, arg)
#define InvTake ACS_TakeInventory
#define InvGive ACS_GiveInventory

#define StrEntON  _Pragma("GDCC STRENT_LITERAL ON")
#define StrEntOFF _Pragma("GDCC STRENT_LITERAL OFF")

// Types ---------------------------------------------------------------------|

enum {
   log_none,
   log_dev   = 1 << 0, // general debug info
   log_devh  = 1 << 1, // prints live stats to the HUD (position, angles, etc)
   log_boss  = 1 << 2, // debug info for the boss system
   log_dmon  = 1 << 3, // debug info for the monster tracker
   log_dmonV = 1 << 4, // verbose debug info for the monster tracker
   log_dlg   = 1 << 5, // debug info for the dialogue/terminal compiler
   log_bip   = 1 << 6, // debug info for the BIP
   log_sys   = 1 << 7, // meta debug info
   log_sysV  = 1 << 7, // tick info
};

enum {
   dbgf_bip   = 1 << 0,
   dbgf_items = 1 << 1,
   dbgf_nomon = 1 << 2,
   dbgf_save  = 1 << 3,
   dbgf_score = 1 << 4,
   dbgf_upgr  = 1 << 5,
};

// Extern Functions ----------------------------------------------------------|

void Log(char const *fmt, ...);

script void Lith_PrintMem(void const *data, size_t size);

stkcall void Lith_FadeFlash(i32 r, i32 g, i32 b, k32 amount, k32 seconds);
script optargs(1) i32 Lith_GetTID(i32 tid, i32 ptr);
script optargs(1) i32 Lith_GetPlayerNumber(i32 tid, i32 ptr);
script optargs(1) bool Lith_ValidPointer(i32 tid, i32 ptr);
script optargs(2) bool Lith_SetPointer(i32 tid, i32 ptr, i32 assign, i32 tid2, i32 ptr2, i32 flags);
stkcall i32 Lith_CheckActorInventory(i32 tid, str item);
stkcall void Lith_GiveActorInventory(i32 tid, str item, i32 amount);
stkcall void Lith_TakeActorInventory(i32 tid, str item, i32 amount);
stkcall void Lith_SetActorInventory (i32 tid, str item, i32 amount);
void Lith_DebugStat(char const *fmt, ...);
void Lith_DebugNote(char const *fmt, ...);

// Extern Objects ------------------------------------------------------------|

extern str dbgstat[],  dbgnote[];
extern i32 dbgstatnum, dbgnotenum;

extern i32 lmvar dbglevel;
extern i32 lmvar dbgflag;

#endif
