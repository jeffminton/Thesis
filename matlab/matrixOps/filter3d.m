%--------------------------------------------------------
% Based on low/high values for vector of 3 values, return true/false
function logval = filter3d(vec,lo1,hi1,lo2,hi2,lo3,hi3);
   if (vec(1) >= lo1 & vec(1) <= hi1 & vec(2) >= lo2 & vec(2) <= hi2 & vec(3) >= lo3 & vec(3) <= hi3)
       logval = true
   else
       logval = false
   end
end      