// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// m_tokbuf.c: Buffered token stream.

#include "m_tokbuf.h"

#include <string.h>

// Extern Functions ----------------------------------------------------------|

stkcall
i32 Lith_TBufProc(struct token *tok)
{
   switch(tok->type) {
   case tok_eof:    return tokproc_done;
   case tok_lnend:
   case tok_cmtblk:
   case tok_cmtlin: return tokproc_skip;
   }
   return tokproc_next;
}

stkcall
i32 Lith_TBufProcL(struct token *tok)
{
   switch(tok->type) {
   case tok_eof:    return tokproc_done;
   case tok_cmtblk:
   case tok_cmtlin: return tokproc_skip;
   }
   return tokproc_next;
}

void Lith_TBufCtor(struct tokbuf *tb)
{
   tb->orig.line = 1;
   tb->toks = Calloc(tb->bend, sizeof *tb->toks);
   if(!tb->tokProcess) tb->tokProcess = Lith_TBufProc;
}

void Lith_TBufDtor(struct tokbuf *tb)
{
   if(tb->toks) for(i32 i = 0; i < tb->bend; i++)
      Vec_Clear(tb->toks[i].text);

   Dalloc(tb->toks);
}

struct token *Lith_TBufGet(struct tokbuf *tb)
{
   if(++tb->tpos < tb->tend) return &tb->toks[tb->tpos];

   // Free beginning of buffer.
   for(i32 i = 0; i < tb->bbeg; i++) {
      Vec_Clear(tb->toks[i].text);
      //memset(&tb->toks[i], 0, sizeof tb->toks[i]);
   }

   // Move end of buffer to beginning.
   if(tb->tend)
   {
      i32 s = tb->tend - tb->bbeg;
      for(i32 i = s, j = 0; i < tb->tend; i++, j++)
         tb->toks[j] = tb->toks[i];
      memset(&tb->toks[s], 0, sizeof tb->toks[s] * (tb->tend - s));
   }

   // Get new tokens.
   for(tb->tpos = tb->tend = tb->bbeg; tb->tend < tb->bend; tb->tend++)
   {
   skip:
      Lith_ParseToken(tb->fp, &tb->toks[tb->tend], &tb->orig);

      switch(tb->tokProcess(&tb->toks[tb->tend])) {
      case tokproc_next: break;
      case tokproc_done: goto done;
      case tokproc_skip: goto skip;
      }
   }

done:
   return &tb->toks[tb->tpos];
}

struct token *Lith_TBufPeek(struct tokbuf *tb)
{
   Lith_TBufGet(tb);
   return Lith_TBufUnGet(tb);
}

stkcall
struct token *Lith_TBufUnGet(struct tokbuf *tb)
{
   return &tb->toks[tb->tpos--];
}

stkcall
struct token *Lith_TBufReGet(struct tokbuf *tb)
{
   return &tb->toks[tb->tpos];
}

bool Lith_TBufDrop(struct tokbuf *tb, i32 t)
{
   if(Lith_TBufGet(tb)->type != t)
      {Lith_TBufUnGet(tb); return false;}
   else
      return true;
}

// EOF
