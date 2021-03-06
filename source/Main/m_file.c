// Distributed under the CC0 public domain license.
// By Alison Sanderson. Attribution is encouraged, though not required.
// See licenses/cc0.txt for more information.

// m_file.c: I/O stream handling.

#define _GNU_SOURCE // Needed for fopencookie. See: man 7 feature_test_macros

#include "common.h"
#include "m_file.h"
#include "w_world.h"

#include "m_base64.h"

#define SAVE_BLOCK_SIZE 230

// Type Definitions ----------------------------------------------------------|

struct memfile
{
   byte  *mem;
   size_t len;
   size_t pos;
};

struct netfile
{
   anonymous struct memfile memfile;
   str pcvar;
   i32 pnum;
};

// Static Functions ----------------------------------------------------------|

// fclose for netfiles.
// Output to the CVar with a Base64 representation of the output buffer.
static i32 NetClose(void *nfdata)
{
   struct netfile *nf = nfdata;

   // If debugging, print out information about the buffer being written.
   if(dbgflag & dbgf_save)
   {
      printf("NetClose: Writing netfile \"%S\" (%zub)\n", nf->pcvar, nf->pos);
      printf("Data follows\n");
      Lith_PrintMem(nf->mem, nf->pos);
   }

   // Base64 encode the buffer.
   size_t outsize;
   byte *coded = base64_encode((void *)nf->mem, nf->pos, &outsize);

   if(coded)
   {
      i32 cvarnum = 0;

      for(byte const *itr = coded; outsize; cvarnum++)
      {
         size_t itrsize;

         if(outsize <= SAVE_BLOCK_SIZE)
            itrsize = outsize;
         else
            itrsize = SAVE_BLOCK_SIZE;

         ACS_SetUserCVarString(nf->pnum, StrParam("%S_%i", nf->pcvar, cvarnum), l_strndup(itr, itrsize));

         itr     += itrsize;
         outsize -= itrsize;
      }

      ACS_SetUserCVarString(nf->pnum, StrParam("%S_%i", nf->pcvar, cvarnum), s_NIL);

      Dalloc(coded);
   }

   Dalloc(nf->mem);
   Dalloc(nf);

   return 0;
}

static ssize_t MemRead(void *memdata, char *buf, size_t size)
{
   struct memfile *mem   = memdata;
   size_t     avail = mem->len - mem->pos;

   if(size > avail)
      size = avail;

   memmove(buf, mem->mem + mem->pos, size);
   mem->pos += size;
   return size;
}

static ssize_t MemWrite(void *memdata, char const *buf, size_t size)
{
   struct memfile *mem = memdata;
   size_t avail = mem->len - mem->pos;

   if(size >= avail)
   {
      size_t len = mem->len + mem->len / 2 + size + 1;
      void  *newmem = Ralloc(mem->mem, len);

      if(!mem)
         return 0;

      mem->len = len;
      mem->mem = newmem;
   }

   memmove(mem->mem + mem->pos, buf, size);
   mem->mem[mem->pos += size] = '\0';
   return size;
}

static i32 MemSeek(void *memdata, off_t *offset, i32 whence)
{
   struct memfile *mem = memdata;
   size_t     pos;

   switch(whence) {
   case SEEK_SET: pos = *offset;            break;
   case SEEK_CUR: pos = *offset + mem->pos; break;
   case SEEK_END: pos = *offset + mem->len; break;
   default: return EOF;
   }

   if(pos > mem->len) return EOF;

   *offset = mem->pos = pos;

   return 0;
}

static i32 MemClose(void *memdata)
{
   struct memfile *mem = memdata;
   Dalloc(mem->mem);
   Dalloc(mem);

   return 0;
}

// Extern Functions ----------------------------------------------------------|

FILE *W_Open(str fname, char const *rw)
{
   str f;
   ifw(i32 lmp = W_Find(fname), lmp == -1)
      return nil;
   else
      f = W_Read(lmp);
   return __fmemopen_str(f, ACS_StrLen(f), rw);
}

// fopen() equivalent for netfiles.
FILE *Lith_NFOpen(i32 pnum, str pcvar, char rw)
{
   FILE *fp = nil;

   if(rw == 'w')
   {
      struct netfile *nf = Salloc(struct netfile);

      nf->pcvar = pcvar;
      nf->pnum  = pnum;

      fp = fopencookie(nf, "w", (cookie_io_functions_t){
         .write = MemWrite,
         .close = NetClose
      });
   }
   else if(rw == 'r')
   {
      // Get inputs from all possible CVars.
      char  *input   = nil;
      size_t inputsz = 0;

      for(i32 cvarnum;; cvarnum++)
      {
         str  cvar  = ACS_GetUserCVarString(pnum, StrParam("%S_%i", pcvar, cvarnum));
         size_t inlen = ACS_StrLen(cvar);

         if(inlen)
         {
            input = Ralloc(input, inputsz + inlen + 1);
            lstrcpy_str(input + inputsz, cvar);

            inputsz += inlen;
         }
         else
            break;
      }

      if(input)
      {
         // Decode the base64 input.
         size_t size;
         byte *data = base64_decode((void *)input, inputsz, &size);

         Dalloc(input);

         // If debugging, print out information about the buffer being read.
         if(dbgflag & dbgf_save)
         {
            printf("Lith_NFOpen: Opening memfile \"%S\" (%zub)\n", pcvar, size);
            printf("Data follows\n");
            Lith_PrintMem(data, size);
         }

         if(data)
         {
            struct memfile *mem = Salloc(struct memfile);

            mem->mem = data;
            mem->len = size;

            fp = fopencookie(mem, "r", (cookie_io_functions_t){
               .read  = MemRead,
               .seek  = MemSeek,
               .close = MemClose,
            });
         }
      }
   }

   return fp;
}

// Unpacks integers into a file stream.
size_t Lith_FWrite32(void const *restrict ptr, size_t count, size_t bytes, FILE *restrict fp)
{
   size_t res = 0;

   for(byte const *itr = ptr; count--; res += bytes)
   {
      u32 c = *itr++;
      for(i32 i = 0; i < bytes; i++)
         if(fputc((c & (0xFF << (i * 8))) >> (i * 8), fp) == EOF)
            return res;
   }

   return res;
}

size_t Lith_FWrite(void const *restrict ptr, size_t count, FILE *restrict fp)
{
   size_t res = 0;

   for(char const *itr = ptr; count--; res++)
      if(fputc(*itr++, fp) == EOF) return res;

   return res;
}

size_t Lith_FWrite_str(void const __str_ars *restrict ptr, size_t count, FILE *restrict fp)
{
   size_t res = 0;

   for(char const __str_ars *itr = ptr; count--; res++)
      if(fputc(*itr++, fp) == EOF) return res;

   return res;
}

// Reads packed integers from a file stream.
size_t Lith_FRead32(void *restrict buf, size_t count, size_t bytes, FILE *restrict fp)
{
   size_t res = 0;

   for(char *itr = buf; count--;)
   {
      i32 c = 0, t;

      for(i32 i = 0; i < bytes; i++, res++)
      {
         if((t = fgetc(fp)) == EOF)
         {
            *itr = c;
            return res;
         }

         c |= (t & 0xFF) << (i * 8);
      }

      *itr++ = c;
   }

   return res;
}

// EOF
