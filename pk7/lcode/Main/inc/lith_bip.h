#if defined(LITH_X)

  LITH_X(0, WEP, WEAPONS,      "Weapons")
// TODO
//LITH_X(1, ENE, ENEMIES,      "Enemies")
// Add when things like the Division Sigil are in
//LITH_X(2, ITE, ITEMS,        "Items")
  LITH_X(1, UPG, UPGRADES,     "Upgrades")
  LITH_X(2, PLA, PLACES,       "Places")
  LITH_X(3, COR, CORPORATIONS, "Companies and Corporations")

#undef LITH_X

#elif !defined(LITH_BIP_H)
#define LITH_BIP_H

#include "lith_list.h"


//----------------------------------------------------------------------------
// BIP - Biotic Information Panel
//

#define MAX_BIP_UNLOCKS 5

enum
{
   BIPC_MIN,
#define LITH_X(n, id, name, capt) BIPC_##name = n,
#include "lith_bip.h"
   BIPC_MAX,
   BIPC_MAIN
};

typedef __str bip_unlocks_t[MAX_BIP_UNLOCKS];

typedef struct bippage_s
{
   __str  name;
   int    category;
   bool   unlocked;
   list_t link;
   bip_unlocks_t unlocks;
} bippage_t;

typedef struct bip_s
{
   // Stats
   int categoryavail[BIPC_MAX];
   int categorymax[BIPC_MAX];
   int pageavail;
   int pagemax;
   
   // State
   bippage_t *curpage;
   int curpagenum;
   int curcategory;
   int scroll;
   
   // Info
   list_t infogr[BIPC_MAX];
} bip_t;

[[__call("ScriptS")]] void Lith_PlayerInitBIP(struct player_s *p);
bippage_t *Lith_FindBIPPage(bip_t *bip, __str name);
bippage_t *Lith_UnlockBIPPage(bip_t *bip, __str name);
void Lith_UnlockAllBIPPages(bip_t *bip);
[[__call("ScriptS")]] void Lith_DeallocateBIP(bip_t *bip);
void Lith_PlayerLoseBIPPages(bip_t *bip);

#endif
