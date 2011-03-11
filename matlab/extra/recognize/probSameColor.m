%--------------------------------------------------------
% Compute a color signature for object on a black background
function probability = probSameColor(img1,img2)
  sigCbCr1 = cSigCbCr(img1);
  sigCbCr2 = cSigCbCr(img2);
  sigLab1 = cSigLab(img1);
  sigLab2 = cSigLab(img2);
  cbMean1 = sigCbCr1(1,1);
  cbMean2 = sigCbCr2(1,1);
  cbStd1 = sigCbCr1(1,2);
  cbStd2 = sigCbCr2(1,2);
  lbMean1 = sigLab1(2,1);
  lbMean2 = sigLab2(2,1);
  lbStd1= sigLab1(2,2);
  lbStd2 = sigLab2(2,2);
  pmcb = min(cbMean1,cbMean2)/max(cbMean1,cbMean2);
  pmlb = min(lbMean1,lbMean2)/max(lbMean1,lbMean2);
  psdcb = min(cbStd1,cbStd2)/max(cbStd1,cbStd2);
  psdlb = min(lbStd1,lbStd2)/max(lbStd1,lbStd2);
  probability = pmcb*pmlb*psdcb*psdlb;
end
 