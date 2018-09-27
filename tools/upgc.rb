#!/usr/bin/env ruby
## Copyright � 2018 Graham Sanderson, all rights reserved.
## upgc: Upgrade info text compiler.

arg = ARGV[0]
ifp, ofh, ofc = arg.split(',')
ifp, ofh, ofc = open(ifp, "rt"), open(ofh, "wt"), open(ofc, "wt")

ofc.puts <<-END
// This file was generated by wepc.
// Edit only if you aren't going to recompile.
#include "lith_upgrades_common.h"

// Extern Objects ------------------------------------------------------------|

upgradeinfo_t const upgrinfobase[UPGR_BASE_MAX] = {
END

ofh.puts <<-END
// zsc output: pk7/lzscript/Headers/lith_upgradenames.h

// This file was generated by upgc.
// Edit only if you aren't going to recompile.

enum // Lith_UpgradeName
{
END

cat = nil

for ln in ifp
   ln.chomp!
   next if ln.empty?

   pre = ln[0]
   res = ln[1..-1]
   case pre
   when ':' then cat = "UC_" + res
   when '+' then
      res = res.split

      nam = res.shift
      inf = res.shift
      inf = if inf == "-" then "null"
            else               "\"#{inf}\"" end

      scr = if /[0-9]/ =~ res[0][0] then res.shift
            else                         "0" end

      pcl = res.shift

      prf = if res[0] && /[0-9]/ =~ res[0][0] then res.shift
            else                                   "0" end
      mul = if res[0] && /-?\./  =~ res[0]    then res.shift
            else                                   "0" end
      grp = if res[0] && /[A-Z]/ =~ res[0][0] then "UG_#{res.shift}"
            else                                   "0" end
      req = if res[0] && "("     == res[0][0] then res.shift[1...-1].split('|').collect{|a| "UR_#{a}"}.join("|")
            else                                   "0" end

      ofh.puts "   UPGR_#{nam},"

      ofc.puts <<-END
   {{"#{nam}", #{inf}, #{scr}}, #{pcl}, #{cat}, #{prf}, #{mul}, #{grp}, #{req}, UPGR_#{nam}},
END
   end
end

ofh.puts <<-END
   UPGR_BASE_MAX
};

// EOF
END

ofc.puts <<-END
};

// EOF
END

## EOF
