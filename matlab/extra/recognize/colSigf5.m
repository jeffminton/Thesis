%--------------------------------------------------------
% Compute a color signature for object on a black background
function signature = colSigf5(img)
  hgt = size(img,1);
  width = size(img,2);
  imgR1d = reshape(medfilt2(img(:,:,1),[5 5]),1,hgt*width);
  imgG1d = reshape(medfilt2(img(:,:,2),[5 5]),1,hgt*width);
  imgB1d = reshape(medfilt2(img(:,:,3),[5 5]),1,hgt*width);
  imgR1dobj = imgR1d(imgR1d > 0);
  imgG1dobj = imgG1d(imgG1d > 0);
  imgB1dobj = imgB1d(imgB1d > 0);
  sigR = [mean(imgR1dobj) std(double(imgR1dobj))];
  sigG = [mean(imgG1dobj) std(double(imgG1dobj))];
  sigB = [mean(imgB1dobj) std(double(imgB1dobj))];
  signature = [sigR; sigG; sigB];
end
 