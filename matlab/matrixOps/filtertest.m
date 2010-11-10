%--------------------------------------------------------
% Testing simple filter
function logval = filtertest(vec);
   if sum(vec) < 100
       logval = true
   else
       logval = false
   end
end
      