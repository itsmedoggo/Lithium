#!/usr/bin/env ruby
## Copyright © 2018 Alison Sanderson, all rights reserved.
## UpgC: Upgrade info text compiler.

SCR = /[[:digit:]]/
PRF = /[[:digit:]]/
MUL = /-?\./
GRP = /[[:alpha:]]/

def reqs res
   res[1...-1].split(?|).each{|a| a.replace "UR_#{a}"}.join(?|)
end

ifp, ofh = open(ARGV[0], "rt"), open(ARGV[1], "wt")
ofc, off = open(ARGV[2], "wt"), open(ARGV[3], "wt")

ofh.puts <<_end_h_; ofc.puts <<_end_c_; off.puts <<_end_f_
// zsc output: pk7/lzscript/Headers/lith_upgradenames.h

// This file was generated by upgc.
// Edit only if you aren't going to recompile.

enum // UpgradeName
{
_end_h_
// This file was generated by wepc.
// Edit only if you aren't going to recompile.
#include "lith_upgrades_common.h"

StrEntON

// Extern Objects ------------------------------------------------------------|

upgradeinfo_t const upgrinfobase[UPGR_BASE_MAX] = {
_end_c_
// This file was generated by upgc.
// Edit only if you aren't going to recompile.
#include "lith_upgradefunc_start.h"

_end_f_

cat = ""

for ln in ifp
   ln.chomp!
   next if ln.empty?

   res = ln[1..-1]
   case ln[0]
   when ?:; cat.replace "UC_#{res}"
   when ?+
      res = res.split
      nam = res.shift
      inf = res.shift
      inf.replace(inf == ?- ? "null" : %("#{inf}"))
      scr = if res[0][0] =~ SCR; res.shift else ?0 end
      pcl = res.shift
      flg = []

      for c, i in res.shift.chars.each_with_index
            if c == ?-;           next
         elsif c == ?A && i == 0; flg << c
         elsif c == ?D && i == 1; flg << c
         elsif c == ?U && i == 2; flg << c
         elsif c == ?R && i == 3; flg << c
         elsif c == ?E && i == 4; flg << c
         else raise "you done fucked up" end
      end

      prf = if res[0] && res[0][0] =~ PRF;       res.shift   else ?0 end
      mul = if res[0] && res[0]    =~ MUL;       res.shift   else ?0 end
      grp = if res[0] && res[0][0] =~ GRP; "UG_#{res.shift}" else ?0 end
      req = if res[0] && res[0][0] == ?(;  reqs  res.shift   else ?0 end

      ofh.puts "   UPGR_#{nam},"

      ofc.puts <<_end_
   {{"#{nam}", #{inf}, #{scr}}, #{pcl}, #{cat}, #{prf}, #{grp}, #{req}, #{mul}, UPGR_#{nam}},
_end_

      unless flg.empty?
         off.puts <<_end_
Case(#{nam})
   #{flg.map{|a| "#{a}(#{nam})\n"}.join("   ")}
_end_
      end
   end
end

ofh.puts <<_end_h_; ofc.puts <<_end_c_; off.puts <<_end_f_
   UPGR_BASE_MAX
};

// EOF
_end_h_
};

// EOF
_end_c_
#include "lith_upgradefunc_end.h"

// EOF
_end_f_

## EOF
