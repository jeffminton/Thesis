function [cbAvg crAvg mask, imy] = calibrate(vid)
    img = getsnapshot(vid);
    
    imy = rgb2ycbcr(img);
    mask = roipoly(img);
    
    [row col] = find(mask>0.9);
    
    cbAvg = mean2(imy(row, col, 2));
    crAvg = mean2(imy(row, col, 3));
end