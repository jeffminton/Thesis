function blob(img, threshold)

%when accessing image indeces
%top left (0, 0, :)
%legend: (y, x, :)

west = [-1 0];
north = [0 -1];
ymax = size(img, 1);
xmax = size(img, 2);
regions = {};
regionCount = 0;
regionMap = {};
regionMapCount = {};


%pass one
for x = 1 : xmax
    for y = 1 : ymax
        compWest = [y x] + west;
        compNorth = [y x] + north;
        if min(compWest) < 0
            compWest = [y x];
        end
        if min(compNorth) < 0
            compNorth = [y x];
        end
        
        if (mean(img(y, x, :)) < threshold + mean(img(compWest, :)) || mean(img(y, x, :)) > threshold - mean(img(compWest, :)))
            westMatch = 1;
            [row col] = find(regions == mat2str(compWest));
            if row == 0 || col == 0
                westExists = 0;
            else
                westExists = 1;
            end
        else
            westMatch = 0;
        end
        
        if(mean(img(y, x, :)) < threshold + mean(img(compNorth, :)) || mean(img(y, x, :)) > threshold - mean(img(compNorth, :)))
            northMatch = 1;
        else
            northMatch = 0;
        end
        
        if(westMatch == 0 && northMatch == 0)
            regionCount = regionCount + 1;
            regionMap{regionCount} = mat2str([y; x]);
            regionMapCount{regionCount} = 1;
            regions{regionCount}{regionMapCount} = mat2str([y; x]);
        elseif(westMatch == 1 && westExists == 1)
            
        end
            
    end
end

end