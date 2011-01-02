%-------------------------------------------------------------------------------

% Find the largest black connected component and return its info

% along with the info of all other connected components

% Input image is RGB

function [newDat bigDat] = infoOfBig(img);

    grayim = rgb2gray(img);

    bw = im2bw(grayim,25/256);

    h = ones(7,7)/49;

    bwf = imfilter(bw,h);

    for i = 1:5

        bwf = imfilter(bwf,h);

    end

    rvBwf = ~bwf;

    cc = bwconncomp(rvBwf,4);
    
    figure, imshow(rvBwf);

    itmData = regionprops(cc,'all');

    [unused, order] = sort([itmData(:).Area],'descend');

    newDat = itmData(order);

    bigDat = newDat(1);

end