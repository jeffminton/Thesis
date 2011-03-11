%--------------------------------------------------------
% Compute a color signature for object on a black background
function signature = cSigLab(img)
  hgt = size(img,1);
  width = size(img,2);
  cform = makecform('srgb2lab');
  imgLAB = applycform(img, cform);
  imgA1d = reshape(imgLAB(:,:,2),1,hgt*width);
  imgB1d = reshape(imgLAB(:,:,3),1,hgt*width);
  imgA1dobj = imgA1d(imgA1d ~= 128);
  imgB1dobj = imgB1d(imgB1d ~= 128);
  sigA = [mean(imgA1dobj) std(double(imgA1dobj))];
  sigB = [mean(imgB1dobj) std(double(imgB1dobj))];
  signature = [sigA; sigB];
end
 