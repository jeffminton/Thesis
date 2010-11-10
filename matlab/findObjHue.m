%--------------------------------------------------------
% Try to find an object in ycbcr based on lo, hi values
% Also try to place black pixels around border
function [bw bwf bwb newDat bigDat] = findObjHue(img,loCb,hiCb,loCr,hiCr)
    % parameters to tweak
    
    
    img = img(190:end,:,:);
    
    [h w depth] = size(img);
    imy = rgb2ycbcr(img);
    imCb = imy(:,:,2);
    imCr = imy(:,:,3);
    %figure, imshow(imCb);
    bw = im2bw(img);
    bw = xor(bw,bw);
    %figure, imshow(bw);
    bwb = ~bw;
    for r = 1:h
        for c = 1:w
            if imCb(r,c) > loCb && imCb(r,c) < hiCb && imCr(r, c) > loCr && imCr(r, c) < hiCr
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
    itmData = regionprops(cc,'all');
    [unused, order] = sort([itmData(:).Area],'descend');
    newDat = itmData(order);
    bigDat = newDat(1);
end
      