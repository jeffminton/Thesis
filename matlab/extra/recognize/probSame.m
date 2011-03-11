%--------------------------------------------------------
% Compute a color signature for object on a black background
function signature = probSame(img)
  hgt = size(img,1);
  width = size(img,2);
  imgCbCr = rgb2ycbcr(img);
  imgB1d = reshape(imgCbCr(:,:,2),1,hgt*width);
  imgR1d = reshape(imgCbCr(:,:,3),1,hgt*width);
  imgB1dobj = imgB1d(imgB1d ~= 128);
  imgR1dobj = imgR1d(imgR1d ~= 128);
  sigB = [mean(imgB1dobj) std(double(imgB1dobj))];
  sigR = [mean(imgR1dobj) std(double(imgR1dobj))];
  signature = [sigB; sigR];
end
 