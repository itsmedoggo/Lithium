// Copyright © 2016-2017 Graham Sanderson, all rights reserved.
#include "lith_bip.h"
#include "lith_pdata.h"

struct page_initializer const bip_pages[] = {
   {.category = BIPC_WEAPONS},
   {pcl_outcasts, "ChargeFist", {"KSKK"}},

   {pcl_marine, "Pistol",          {"Omakeda"}, .isfree = true},
   {pcl_marine, "Revolver",        {"Earth"}},
   {pcl_marine, "Shotgun",         {"Omakeda"}},
   {pcl_marine, "ShotgunUpgr",     {"Shotgun", "AOF", "DurlaPrime"}},
   {pcl_marine, "ShotgunUpg2",     {"Shotgun"}},
   {pcl_marine, "LazShotgun",      {"Earth"}},
   {pcl_marine, "SuperShotgun",    {"ChAri"}},
   {pcl_marine, "CombatRifle",     {"AllPoint"}},
   {pcl_marine, "RifleUpgr",       {"CombatRifle"}},
   {pcl_marine, "RifleUpg2",       {"CombatRifle", "Semaphore"}},
   {pcl_marine, "SniperRifle",     {"Facer"}},
   {pcl_marine, "GrenadeLauncher", {"Sym43"}},
   {pcl_marine, "LauncherUpgr",    {"GrenadeLauncher", "UnrealArms"}},
   {pcl_marine, "LauncherUpg2",    {"GrenadeLauncher", "Sym43"}},
   {pcl_marine, "PlasmaRifle",     {"AllPoint", "MDDO"}},
   {pcl_marine, "PlasmaUpgr",      {"PlasmaRifle"}},
   {pcl_marine, "PlasmaUpg2",      {"PlasmaRifle", "Semaphore"}},
   {pcl_marine, "BFG9000",         {"Cid"}},
   {pcl_marine, "CannonUpgr",      {"BFG9000", "SuperDimension"}},
   {pcl_marine, "CannonUpg2",      {"BFG9000"}},

   {pcl_cybermage, "Mateba",        {"AOF"}, .isfree = true},
   {pcl_cybermage, "MatebaUpgr",    {"Mateba", "AOF", "Algidistari"}},
   {pcl_cybermage, "ShockRifle",    {"ChAri"}},
   {pcl_cybermage, "ShockRifUpgr",  {"ShockRifle"}},
   {pcl_cybermage, "ShockRifUpg2",  {"ShockRifle"}},
   {pcl_cybermage, "SPAS",          {"AOF", "Newvec"}},
   {pcl_cybermage, "SPASUpgr",      {"SPAS"}},
   {pcl_cybermage, "SPASUpg2",      {"SPAS", "Newvec"}},
   {pcl_cybermage, "SMG",           {"Omakeda", "Sym43"}},
   {pcl_cybermage, "SMGUpgr",       {"SMG", "AOF", "Sym43"}},
   {pcl_cybermage, "SMGUpg2",       {"SMG", "AOF"}},
   {pcl_cybermage, "SMGUpg3",       {"SMG", "Sym43"}},
   {pcl_cybermage, "IonRifle",      {"KSKK"}},
   {pcl_cybermage, "IonRifleUpgr",  {"IonRifle", "KSKK"}},
   {pcl_cybermage, "IonRifleUpg2",  {"IonRifle", "KSKK"}},
   {pcl_cybermage, "CPlasmaRifle",  {"AllPoint", "MDDO"}},
   {pcl_cybermage, "CPlasmaUpgr",   {"CPlasmaRifle", "MDDO"}},
   {pcl_cybermage, "StarDestroyer", {"Hell"}},
   {pcl_cybermage, "ShipGunUpgr",   {"StarDestroyer"}},
   {pcl_cybermage, "ShipGunUpg2",   {"StarDestroyer"}},

   {pcl_outcasts, "MissileLauncher"},
   {pcl_outcasts, "PlasmaDiffuser", {"Sym43", "MDDO", "Semaphore"}},

   {pcl_cybermage, "Blade",               .isfree = true},
   {pcl_cybermage, "Delear",   {"Earth"}, .isfree = true},
   {pcl_cybermage, "Feuer"},
   {pcl_cybermage, "Rend"},
   {pcl_cybermage, "Hulgyon",  {"Heaven"}},
   {pcl_cybermage, "StarShot", {"AOF"}},
   {pcl_cybermage, "Cercle",   {"Earth"}},

   {.category = BIPC_ENEMIES},
   {pcl_any, "ZombieMan"},
   {pcl_any, "ShotgunGuy"},
   {pcl_any, "ChaingunGuy"},

   {pcl_any, "Imp"},
   {pcl_any, "Demon"},
   {pcl_any, "Spectre"},

   {pcl_any, "LostSoul"},
   {pcl_any, "Mancubus"},
   {pcl_any, "Arachnotron"},
   {pcl_any, "Cacodemon"},

   {pcl_any, "HellKnight"},
   {pcl_any, "BaronOfHell"},
   {pcl_any, "Revenant"},

   {pcl_any, "PainElemental"},
   {pcl_any, "Archvile"},
   {pcl_any, "SpiderMastermind"},
   {pcl_any, "Cyberdemon"},

   {pcl_any, "Phantom"},
   {pcl_any, "IconOfSin"},

   {.category = BIPC_UPGRADES},
   {pcl_marine, "HeadsUpDisp", {"OFMD"}},
   {pcl_marine, "JetBooster",  {"OFMD"}},
   {pcl_marine, "ReflexWetw",  {"OFMD"}},
   {pcl_marine, "CyberLegs",   {"OFMD"}},
   {pcl_marine, "Yh0",         {"DurlaPrime"}},
   {pcl_marine, "DefenseNuke", {"OFMD"}},
   {pcl_marine, "Adrenaline",  {"KSKK"}},

   {pcl_cybermage, "HeadsUpDispJem", {"AOF"}},
   {pcl_cybermage, "ReflexWetwJem",  {"AOF"}},
   {pcl_cybermage, "Magic"},
   {pcl_cybermage, "SoulCleaver"},

   {pcl_outcasts, "VitalScanner", {"KSKK"}},
   {pcl_outcasts, "AutoReload",   {"KSKK"}},

   {pcl_marine, "WeapnInter", {"OFMD", "AllPoint"}},
   {pcl_marine, "WeapnInte2"},
   {pcl_marine, "ArmorInter"},
   {pcl_marine, "CBIUpgr1",   {"KSKK"}},
   {pcl_marine, "CBIUpgr2",   {"KSKK"}},

   {.category = BIPC_PLACES},
   {pcl_outcasts,  "AetosVi"},
   {pcl_cybermage, "Algidistari"},
   {pcl_outcasts,  "ChAri",          {"AetosVi"}},
   {pcl_marine,    "DurlaPrime",     {"Earth", "AetosVi"}},
   {pcl_outcasts,  "Earth"},
   {pcl_cybermage, "Hell",           {"Earth"}},
   {pcl_outcasts,  "Mars",           {"Earth", "OFMD"}},
   {pcl_outcasts,  "OmicronXevv"},
   {pcl_marine,    "SuperDimension", {"BFG9000", "SIGFPE"}},
   {pcl_cybermage, "Heaven"},

   {.category = BIPC_CORPORATIONS},
   {pcl_outcasts,  "AllPoint"},
   {pcl_marine,    "AOF"},
   {pcl_cybermage, "AOFJem"},
   {pcl_marine,    "Cid",        {"SuperDimension", "Earth"}},
   {pcl_marine,    "Facer"},
   {pcl_marine,    "KSKK",       {"Earth"}},
   {pcl_cybermage, "KSKKJem",    {"Earth"}},
   {pcl_outcasts,  "MDDO",       {"Mars", "OFMD"}},
   {pcl_cybermage, "Newvec",     {"Earth"}},
   {pcl_marine,    "OFMD"},
   {pcl_cybermage, "OFMDJem"},
   {pcl_outcasts,  "Omakeda",    {"Earth"}},
   {pcl_outcasts,  "Semaphore",  {"OmicronXevv"}},
   {pcl_outcasts,  "Sym43",      {"AetosVi"}},
   {pcl_marine,    "UnrealArms", {"AetosVi"}},

   {.category = BIPC_EXTRA},
   {pcl_any, "Extra1"},
   {pcl_any, "Extra2"},
   {pcl_any, "Extra3"},
   {pcl_any, "Extra4"},
   {pcl_any, "Extra5"},

   {},
};

// EOF
