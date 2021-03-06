// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// p_attrib.h: Player attributes.

#ifndef p_attrib_h
#define p_attrib_h

#define ATTR_BAS_MAX 60
#define ATTR_VIS_MAX 50
#define ATTR_VIS_DIFF (ATTR_BAS_MAX - ATTR_VIS_MAX)

enum
{
   atsys_auto,
   atsys_hybrid,
   atsys_manual,
};

struct player_attributes
{
   u32 exp, expnext;
   u32 level;
   u32 points;
   u32 attrs[at_max];

   char names[at_max][3];
   char lvupstr[130];
};

#endif
