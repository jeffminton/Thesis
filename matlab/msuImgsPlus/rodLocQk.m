%------------------------------------------------------------------------------------------------
% Compute the location of dowel rod with respect to center
% Input image is RGB
function mid = rodLocQk(img)
    grayim = rgb2gray(img);
    bw = im2bw(grayim,25/256);
    h = ones(7,7)/49;
    bwf = imfilter(bw,h);
    for i = 1:5
        bwf = imfilter(bwf,h);
    end
    rvBwf = ~bwf;
    cc = bwconncomp(rvBwf,4);
    itmData = regionprops(cc,'basic');
    [unused, order] = sort([itmData(:).Area],'descend');
    newDat = itmData(order);
    mid = newDat(1).Centroid(1);
end 