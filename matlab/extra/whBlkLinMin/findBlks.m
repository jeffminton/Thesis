%------------------------------------------------------------------------------------------------
% Code to invoke Matlab with camera running
function v = findBlks(bwImg);
    v = [];
    [r c] = size(bwImg);
    for i = 1:c
        if bwImg(r,i) == 0, v = [v i]; end
    end
%    mid = mean(v);
end