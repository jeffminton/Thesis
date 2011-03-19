classdef unionfind
    
    properties
        regions;
        regionCount;
        regionMap;
    end
    
    methods
        function uf = unionfind()
            uf.regions = {};
            uf.regionCount = 0;
        end
        
        function make(uf, val)
            uf.regions{uf.regionCount}{1} = val;
            (uf.regionMap(uf.regionCount)).id = val;
            (uf.regionMap(uf.regionCount)).count = 1;
        end
    end
end