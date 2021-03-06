// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// w_font.c: Unicode font handling.

#if LITHIUM
#include "common.h"
#include "w_data.h"
#include "m_file.h"

#define nglyphs 34
#define nblocks 64
#define ngroups 32
#define nplanes 16

// Types ---------------------------------------------------------------------|

// NOTE: DO NOT change the layout of this without also changing `RetOfs`s
struct glyph
{
   i32 key;
   i32 xadv;
   i32 yofs;
   i32 tex;
   i32 w, h;
};

typedef struct glyph glyphdata[nglyphs];
typedef glyphdata   *blockdata[nblocks];
typedef blockdata   *groupdata[ngroups];
typedef groupdata   *planedata[nplanes];
typedef planedata   *font;

// Static Objects ------------------------------------------------------------|

static str const fontnames[] = {
   [font_misaki_gothic] = s"MisakiG",
   [font_misaki_mincho] = s"MisakiM",
   [font_jfdot_gothic]  = s"JFDotG",
};

static font fonts[font_num];

static FILE *fp;

static i32 curfont, setfont;

// Static Functions ----------------------------------------------------------|

stkcall
static struct glyph *AllocFontMetric(font *planes, i32 key)
{
   if(!*planes) *planes = Salloc(planedata);
   groupdata   **groups = &(**planes)[key / (nglyphs * nblocks  * ngroups)];

   if(!*groups) *groups = Salloc(groupdata);
   blockdata   **blocks = &(**groups)[key / (nglyphs * nblocks) % ngroups ];

   if(!*blocks) *blocks = Salloc(blockdata);
   glyphdata   **glyphs = &(**blocks)[key /  nglyphs            % nblocks ];

   if(!*glyphs) *glyphs = Salloc(glyphdata);
   return                 &(**glyphs)[key                       % nglyphs ];
}

stkcall
static void SetFontMetric(i32 key, i32 xadv, i32 yofs)
{
   struct glyph *metr = AllocFontMetric(&fonts[curfont], key);

   i32 tex = DrawCallI(sm_GetTex, StrParam("lgfx/Font/%S/%u.png", fontnames[curfont], key));
   i32   w = DrawCallI(sm_GetTexW, tex);
   i32   h = DrawCallI(sm_GetTexH, tex);

   metr->key  = key;
   metr->xadv = xadv;
   metr->yofs = h + yofs;
   metr->tex  = tex;
   metr->w    = w;
   metr->h    = h;
}

// Scripts -------------------------------------------------------------------|

script ext("ACS") addr(lsc_getfontmetric)
struct glyph *Sc_GetFontMetric(i32 key)
{
   return &(*(*(*(*fonts[setfont])
      [key / (nglyphs * nblocks  * ngroups)])
      [key / (nglyphs * nblocks) % ngroups ])
      [key /  nglyphs            % nblocks ])
      [key                       % nglyphs ];
}

// hand-written assembly for these, saves a few cycles
#define RetOfs(n) \
   __asm \
   ( \
      "AddI(Stk() LocReg(Lit(:metr)) Lit(" #n "_s31.0))" \
      "Retn(Sta(Stk()))" \
   ); \
   return 0 /* dummy return to keep the compiler quiet */

script ext("ACS") addr(lsc_metr_xadv) i32 Sc_Metr_Xadv(struct glyph *metr) {RetOfs(1);}
script ext("ACS") addr(lsc_metr_yofs) i32 Sc_Metr_Yofs(struct glyph *metr) {RetOfs(2);}
script ext("ACS") addr(lsc_metr_tex)  i32 Sc_Metr_Tex (struct glyph *metr) {RetOfs(3);}
script ext("ACS") addr(lsc_metr_w)    i32 Sc_Metr_W   (struct glyph *metr) {RetOfs(4);}
script ext("ACS") addr(lsc_metr_h)    i32 Sc_Metr_H   (struct glyph *metr) {RetOfs(5);}

script_str ext("ACS") addr("Lith_SetFontFace") void Sc_SetFontFace(i32 f) {setfont = f;}

script_str ext("ACS") addr("Lith_SetupFontsBegin")
bool Sc_SetupFontsBegin(i32 fontnum)
{
   if(fontnum >= font_num) return false;

   curfont = fontnum;
   fp = W_Open(StrParam("lfiles/Font_%S.txt", fontnames[fontnum]), c"r");

   if(fp)
      return true;
   else
   {
      Log("Warning: Font file not found for font %u (%S), UI may break",
         fontnum, fontnames[fontnum]);
      return false;
   }
}

script_str ext("ACS") addr("Lith_SetupFontsContinue")
bool Sc_SetupFontsContinue(void)
{
   for(i32 i = 0; i < 300; i++)
   {
      i32 key;
      i32 xadv, yofs;

      if(fscanf(fp, "%i,%i,%i\n", &key, &xadv, &yofs) != 3)
      {
         fclose(fp);
         return false;
      }

      SetFontMetric(key, xadv, yofs);
   }

   return true;
}
#endif

// EOF
