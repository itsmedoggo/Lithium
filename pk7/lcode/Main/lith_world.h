#ifndef LITH_WORLD_H
#define LITH_WORLD_H

#include <stdbool.h>

#include "lith_wdata.h"

enum
{
   skill_tourist,
   skill_easy,
   skill_normal,
   skill_hard,
   skill_extrahard,
   skill_nightmare
};

enum
{
   cupg_hasupgr1,
   cupg_hasupgr2,
   cupg_armorinter,
   cupg_weapninter,
   cupg_weapninte2,
   cupg_rdistinter
};

enum
{
   cupg_c_slot5spell,
   cupg_c_slot6spell,
   cupg_c_armorinter,
   cupg_c_slot3spell,
   cupg_c_weapninte2,
   cupg_c_rdistinter,
};

enum
{
   CANONTIME_FULL,
   CANONTIME_SHORT,
   CANONTIME_DATE
};

typedef struct payoutinfo_s
{
   int killnum, killmax;
   int itemnum, itemmax;
   
   fixed64_t killpct;
   fixed64_t itempct;
   
   int killscr;
   int itemscr;
   
   int total;
   int tax;
} payoutinfo_t;

__str Lith_CanonTime(int type);

enum game_s
{
   Game_Doom2,
   Game_Episodic,
};

typedef struct worldinfo_s
{
   property mapsecrets {get: ACS_GetLevelInfo(LEVELINFO_FOUND_SECRETS)}
   property mapkills   {get: ACS_GetLevelInfo(LEVELINFO_KILLED_MONSTERS)}
   property mapitems   {get: ACS_GetLevelInfo(LEVELINFO_FOUND_ITEMS)}
   property mapnum     {get: ACS_GetLevelInfo(LEVELINFO_LEVELNUM)}
   property mapkillmax {get: ACS_GetLevelInfo(LEVELINFO_TOTAL_MONSTERS)}
   property mapitemmax {get: ACS_GetLevelInfo(LEVELINFO_TOTAL_ITEMS)}
   property cluster    {get: ACS_GetLevelInfo(LEVELINFO_CLUSTERNUM)}
   property canontime      {get: Lith_CanonTime(CANONTIME_FULL)}
   property canontimeshort {get: Lith_CanonTime(CANONTIME_SHORT)}
   property canondate      {get: Lith_CanonTime(CANONTIME_DATE)}
   property difficulty     {get: ACS_GetCVar("lith_sv_difficulty")}
   
   bool singleplayer;
   int mapscleared;
   int prevcluster;
   int mapseed;
   bool unloaded;
   bool islithmap;
   bool enemycompat;
   
   int secretsfound;
   double scoremul;
   
   long ticks;
   
   bool scoregolf;
   
   int game;
   
   // Bosses
   bool boss[3][3];
   bool bossspawned;
   
   property boss1p1scr {get: ACS_GetCVar("lith_sv_boss1p1_scr")}
   property boss1p2scr {get: ACS_GetCVar("lith_sv_boss1p2_scr")}
   
   property boss2p1scr {get: ACS_GetCVar("lith_sv_boss2p1_scr")}
   property boss2p2scr {get: ACS_GetCVar("lith_sv_boss2p2_scr")}
   property boss2p3scr {get: ACS_GetCVar("lith_sv_boss2p3_scr")}
   
   property boss3p1scr {get: ACS_GetCVar("lith_sv_boss3p1_scr")}
   property boss3p2scr {get: ACS_GetCVar("lith_sv_boss3p2_scr")}
   property boss3p3scr {get: ACS_GetCVar("lith_sv_boss3p3_scr")}
   
   // CBI global information
   int  cbiperf;
   bool cbiupgr[6];
   
   // Mod compat
   bool legendoom;
   bool drlamonsters;
   
   // Extras
   bool grafZoneEntered;
   bool pauseinmenus;
   
   // DECORATE data
   int a_x, a_y;
   struct polar a_angles[8];
   int a_cur;
   int decvars[8];
   
   // Debugging
   int dbgLevel;
   bool dbgItems  : 1;
   bool dbgBIP    : 1;
   bool dbgScore  : 1;
   bool dbgUpgr   : 1;
   bool dbgSave   : 1;
} worldinfo_t;

__addrdef extern __mod_arr lmvar;
__addrdef extern __hub_arr lwvar;

extern bool lmvar mapinit;

extern worldinfo_t world;
extern payoutinfo_t payout;

[[__optional_args(1)]] int Lith_UniqueID(int tid);

#endif

