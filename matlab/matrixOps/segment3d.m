%--------------------------------------------------------
% Based on low/high values segment color image into BW
function bwimg = segment3d(img,lo1,hi1,lo2,hi2,lo3,hi3);
   greenRlo = im2bw(img(:,:,1),lo1/256);
   greenRhi = im2bw(img(:,:,1),hi1/256);
   greenGlo = im2bw(img(:,:,2),lo2/256);
   greenGhi = im2bw(img(:,:,2),hi2/256);
   greenBlo = im2bw(img(:,:,3),lo3/256);
   greenBhi = im2bw(img(:,:,3),hi3/256);
   notGreenR = greenRlo & ~greenRhi;
   notGreenG = greenGlo & ~greenGhi;
   notGreenB = greenBlo & ~greenGhi;
   bwimg = notGreenR & notGreenG & notGreenB;
end      