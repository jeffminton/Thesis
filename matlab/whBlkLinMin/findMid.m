%------------------------------------------------------------------------------------------------
% Compute the middle element of a segment of black at last row
function mid = findMid(bwImg);
    v = [];
    [r c] = size(bwImg);
    for i = 1:c
        if bwImg(r,i) ==0, v = [v i]; end
    end
    mid = mean(v);
end