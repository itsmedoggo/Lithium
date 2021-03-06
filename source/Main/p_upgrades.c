// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// p_upgrades.c: Upgrade handling.

#include "u_common.h"
#include "w_world.h"

// Static Objects ------------------------------------------------------------|

static struct upgradeinfo g_upgrinfoex[UPGR_EXTRA_NUM];

static struct upgradeinfo *g_upgrinfo;
static i32 g_upgrmax = UPGR_BASE_MAX;

// Static Functions ----------------------------------------------------------|

static bool Lith_UpgrCanBuy(struct player *p, struct shopdef const *, void *upgr)
{
   return !((struct upgrade *)upgr)->owned;
}

static void Lith_UpgrShopBuy(struct player *p, struct shopdef const *, void *upgr)
{
   ((struct upgrade *)upgr)->setOwned(p);
}

static bool Lith_UpgrGive(struct player *, struct shopdef const *, void *upgr_, i32 tid)
{
   struct upgrade const *upgr = upgr_;

   SetMembI(tid, sm_UpgradeId, upgr->info->key);

   switch(upgr->info->category) {
   case UC_Body: SetMembI(tid, sm_UpgradeBody, true); break;
   case UC_Weap: SetMembI(tid, sm_UpgradeWeap, true); break;
   default:      SetMembI(tid, sm_UpgradeExtr, true); break;
   }

   return true;
}

static i32 Compg_upgrinfo(void const *lhs, void const *rhs)
{
   struct upgradeinfo const *u1 = lhs, *u2 = rhs;
   i32 c1 = u1->category - u2->category;
   if(c1 != 0) return c1;
   else        return u1->key - u2->key;
}

stkcall
static void SetDataPtr(struct player *p, struct upgrade *upgr)
{
   upgr->dataptr = &p->upgrdata;
}

// Extern Functions ----------------------------------------------------------|

struct upgradeinfo *Lith_UpgradeRegister(struct upgradeinfo const *upgr)
{
   struct upgradeinfo *ui = &g_upgrinfoex[g_upgrmax++ - UPGR_BASE_MAX];
   *ui = *upgr;
   return ui;
}

void Lith_MInit_Upgrade(void)
{
   for(i32 i = 0; i < g_upgrmax; i++)
   {
      struct upgradeinfo *ui = &g_upgrinfo[i];

      // Set up static function pointers
      ui->Init = SetDataPtr; // this is set again by UpgrReinit

      ui->ShopBuy    = Lith_UpgrShopBuy;
      ui->ShopCanBuy = Lith_UpgrCanBuy;
      ui->ShopGive   = Lith_UpgrGive;

      // Set up individual upgrades' function pointers
      switch(ui->key)
      {
      #define Ret(n) continue;
      #define Fn_F(n, cb) ui->cb = Upgr_##n##_##cb;
      #define Fn_S(n, cb) Fn_F(n, cb)
      #include "u_func.h"
         continue;
      }
   }
}

void Lith_GInit_Upgrade(void)
{
   g_upgrinfo = Calloc(g_upgrmax, sizeof *g_upgrinfo);
   memmove(g_upgrinfo, upgrinfobase, sizeof upgrinfobase);

   for(i32 i = 0; i < countof(g_upgrinfoex); i++)
      if(g_upgrinfoex[i].name)
         g_upgrinfo[UPGR_BASE_MAX + i] = g_upgrinfoex[i];

   qsort(g_upgrinfo, g_upgrmax, sizeof *g_upgrinfo, Compg_upgrinfo);

   for(i32 i = 0; i < g_upgrmax; i++)
      g_upgrinfo[i].id = i;
}

void Lith_UpgrSetOwned(struct player *p, struct upgrade *upgr)
{
   if(upgr->owned) return;

   upgr->owned = true;
   p->upgradesowned++;

   if(upgr->info->category == UC_Body && upgr->info->cost == 0)
      upgr->toggle(p);
}

script
void Lith_PlayerInitUpgrades(struct player *p)
{
   #define CheckPClass() (g_upgrinfo[i].pclass & p->pclass)
   for(i32 i = 0; i < g_upgrmax; i++)
      if(CheckPClass())
         p->upgrmax++;

   upgrademap_t_ctor(&p->upgrademap, p->upgrmax, 1);
   memset(p->upgrades, 0, sizeof p->upgrades[0] * countof(p->upgrades));

   for(i32 i = 0, j = 0; i < g_upgrmax; i++)
      if(CheckPClass())
   {
      struct upgrade *upgr = &p->upgrades[j];

      g_upgrinfo[i].Init(p, upgr);
      upgr->info = &g_upgrinfo[i];

      p->upgrademap.insert(upgr);

      if(upgr->info->cost == 0 || dbgflag & dbgf_upgr)
         Lith_UpgrBuy(p, upgr, true, true);

      j++;
   }

   p->upgrinit = true;
   #undef CheckPClass
}

void Lith_PlayerDeallocUpgrades(struct player *p)
{
   upgrademap_t_dtor(&p->upgrademap);
   p->upgrmax = 0;

   for(i32 i = 0; i < countof(p->upgrades); i++)
      memset(&p->upgrades[i], 0, sizeof p->upgrades[i]);

   p->upgrinit = false;
}

void Lith_PlayerDeinitUpgrades(struct player *p)
{
   ForUpgrade(upgr)
      if(upgr->active)
         upgr->wasactive = true,  upgr->toggle(p);
}

void Lith_PlayerReinitUpgrades(struct player *p)
{
   ForUpgrade(upgr)
      if(upgr->wasactive)
         upgr->wasactive = false, upgr->toggle(p);
}

script
void Lith_PlayerUpdateUpgrades(struct player *p)
{
   if(Lith_IsPaused)
      return;

   ForUpgrade(upgr)
      if(upgr->active && upgr->info->Update)
         upgr->info->Update(p, upgr);
}

script
void Lith_PlayerRenderUpgrades(struct player *p)
{
   ForUpgrade(upgr) if(upgr->active && upgr->info->Render)
   {
      ACS_SetHudSize(320, 240);
      ACS_SetHudClipRect(0, 0, 0, 0);
      SetSize(320, 240);
      ClearClip();
      upgr->info->Render(p, upgr);
   }
}

void Lith_PlayerEnterUpgrades(struct player *p)
{
   ForUpgrade(upgr)
      if(upgr->active && upgr->info->Enter)
         upgr->info->Enter(p, upgr);
}

bool Lith_UpgrCanActivate(struct player *p, struct upgrade *upgr)
{
   if(!upgr->active)
   {
      if((p->pclass == pcl_marine &&
         CheckRequires_AI  ||
         CheckRequires_WMD ||
         CheckRequires_WRD ||
         CheckRequires_RDI ||
         CheckRequires_RA) ||
         p->cbi.pruse + upgr->info->perf > world.cbiperf)
      {
         return false;
      }
   }

   return upgr->owned;
}

bool Lith_UpgrToggle(struct player *p, struct upgrade *upgr)
{
   if(!upgr->canUse(p)) return false;

   upgr->active = !upgr->active;

   if(upgr->active) p->cbi.pruse += upgr->info->perf;
   else             p->cbi.pruse -= upgr->info->perf;

   if(upgr->active && upgr->info->group)
      ForUpgrade(other)
         if(other != upgr && other->active && other->info->group == upgr->info->group)
            other->toggle(p);

   if(upgr->active)
   {
      if(upgr->info->Activate)
         upgr->info->Activate(p, upgr);

      p->scoremul += upgr->info->scoreadd;
   }
   else
   {
      if(upgr->info->Deactivate)
         upgr->info->Deactivate(p, upgr);

      p->scoremul -= upgr->info->scoreadd;
   }

   return true;
}

// EOF
