// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// p_m_dbg.c: Debug module.

#include "common.h"
#include "p_player.h"

void P_Dbg_InitPre(struct player *p)
{
   LogDebug(log_sys, "%s", __func__);
}

void P_Dbg_InitPst(struct player *p)
{
   LogDebug(log_sys, "%s", __func__);
}

void P_Dbg_Init(struct player *p)
{
   LogDebug(log_sys, "%s", __func__);
}

void P_Dbg_ReInit(struct player *p)
{
   LogDebug(log_sys, "%s", __func__);
}

void P_Dbg_Quit(struct player *p)
{
   LogDebug(log_sys, "%s", __func__);
}

void P_Dbg_Dealloc(struct player *p)
{
   LogDebug(log_sys, "%s", __func__);
}

void P_Dbg_TickPre(struct player *p)
{
   LogDebug(log_sysV, "%s", __func__);
}

void P_Dbg_Tick(struct player *p)
{
   LogDebug(log_sysV, "%s", __func__);
}

void P_Dbg_TickPst(struct player *p)
{
   LogDebug(log_sysV, "%s", __func__);
}

// EOF
