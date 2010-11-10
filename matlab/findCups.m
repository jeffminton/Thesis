%--------------------------------------------------------
% Try to find an object in ycbcr based on lo, hi values
% Also try to place black pixels around border
function [bwf img ccObjs centDat] = findCups(vid, cbAvg, crAvg)
    pause(.1);
    img = getsnapshot(vid);

%     if strcmpi(color, 'red')
%         loCb = 115;
%         hiCb = 130;
%         loCr = 150;
%         hiCr = 165;
%     elseif strcmpi(color, 'blue')
%         loCb = 165;
%         hiCb = 190;
%         loCr = 90;
%         hiCr = 105;
%     end
    offset = 10;
    img = img(190:end,:,:);
    
    [h w depth] = size(img);
    imy = rgb2ycbcr(img);

    %figure, imshow(imy);
    bw = im2bw(img);
    bw = xor(bw,bw);
    %figure, imshow(bw);
    bwb = ~bw;
    for r = 1:h
        for c = 1:w
            if (imy(r,c, 2) > cbAvg - offset) && (imy(r,c, 2) < cbAvg + offset) && (imy(r, c, 3) > crAvg - offset) && (imy(r, c, 3) < crAvg + offset)
                bw(r,c) = true;
            else
                bw(r,c) = false;
            end
        end
    end
    filt = ones(7,7)/49;
    bwf = imfilter(bw,filt);
    for i = 1:5
        bwf = imfilter(bwf,filt);
    end
    cc = bwconncomp(bwf,4);

    ccObjs = cc.NumObjects

    centDat = regionprops(cc, 'all');
    
    saveImg(img, bwf);
    
end
      