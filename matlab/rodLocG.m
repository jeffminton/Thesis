%-----------------------------------------------------------------
% Compute the location of dowel rod with respect to center
% Input image is already grayscale
function [mid loc] = rodLocG(grayim);
    figure, imshow(grayim);
    bw = im2bw(grayim,25/256);
    figure, imshow(bw);
    h = ones(7,7)/49;
    bwf = imfilter(bw,h);
    figure, imshow(bwf);
    for i = 1:5
        figure, imshow(bwf);
        bwf = imfilter(bwf,h);
    end
    [hgt width] = size(bwf);
    midV = ceil(hgt/2);
    midHrz = ceil(width/2);
    v = [];
    for i = 1:width
        if bwf(midV,i) == 0, v = [v i]; end
    end
    mid = mean(v);
    loc = (mid - midHrz)/width;
end