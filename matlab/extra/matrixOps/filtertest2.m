%--------------------------------------------------------
% Testing simple filter
function logval = filtertest2(vec,lo1);
   if vec(1) >= lo1
       logval = true
   else
       logval = false
   end
end
      