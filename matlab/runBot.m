function runBot(vid, cbAvg, crAvg)
    global imgNum;
    try
        pause on
        imgNum = 0;

        rob = iRobotCreate(0, 4);
        ard  = ard_create('COM3');
        stopDist = 510;

        getCups(vid, rob, cbAvg, crAvg, 'r');
        
        allignCups(rob, vid, cbAvg, crAvg);
        
        currDist = ard.getIR();
        while(currDist < stopDist)
            [bwf img ccObjs centDat] = findCups(vid, cbAvg, crAvg);
            if(ccObjs == 2)
                allignCups(rob, vid, cbAvg, crAvg);
            end
            move(rob, 'f');
            pause(.9);
            move(rob, 's');
            currDist = ard.getIR()
        end
        
        move(rob, 's');
        rob.beep();
        rob.delete();
    catch ex
        rethrow(ex);
    end
end
    
function allignCups(rob, vid, cbAvg, crAvg)
    alligned = 0;
    side = 'r';
    while(alligned == 0)
        getCups(vid, rob, cbAvg, crAvg, side);
        centerBot(rob, vid, cbAvg, crAvg);
        [bwf img ccObjs centDat] = findCups(vid, cbAvg, crAvg);
        [side] = furtherCup(centDat);

        if(side == 'r')
            rob.rotate(deg2rad(-65));
            rob.forward(.25);
            rob.rotate(deg2rad(90));
%             rob.rotate(deg2rad(45));
        elseif(side == 'l')
            rob.rotate(deg2rad(65));
            rob.forward(.25);
            rob.rotate(deg2rad(-90));
%             rob.rotate(deg2rad(-45));
        else
            alligned = 1;
        end
    end
end
    
function getCups(vid, rob, cbAvg, crAvg, dir)
    [bwf img ccObjs centDat] = findCups(vid, cbAvg, crAvg);
    degMaster = 7;
    left = 0;
    right = 0;
    while ~(ccObjs == 2)
        if(dir == 'r')
            deg = 0 - degMaster;
        else
            deg = degMaster;
        end
        [h w depth] = size(img);
        midImg = w / 2;
        if(dir == 'r')
            if(ccObjs == 1)
                if(midImg > centDat(1).Centroid(1))
                    left = 1;
                end
                if(midImg < centDat(1).Centroid(1) && left == 1)
                    rob.forward(-.25);
                    dir = 'l';
                    left = 0;
                end
            end
        elseif(dir == 'l')
            if(ccObjs == 1)
                if(midImg < centDat(1).Centroid(1))
                    right = 1;
                end
                if(midImg > centDat(1).Centroid(1) && right == 1)
                    rob.forward(-.25);
                    dir = 'r';
                    right = 0;
                end
            end
        end
        rob.rotate(deg2rad(deg));
        pause(.5);
        [bwf img ccObjs centDat] = findCups(vid, cbAvg, crAvg);

    end
end

function move(rob, dir)
    if(dir == 'r')
        rwheel = -.1;
        lwheel = .1;
    elseif(dir == 'l')
        rwheel = .1;
        lwheel = -.1;
    elseif(dir == 'f')
        rwheel = .1;
        lwheel = .1;
    elseif(dir == 'b')
        rwheel = -.1;
        lwheel = -.1;
    elseif(dir == 's')
        rwheel = 0;
        lwheel = 0;
    end
    
    rob.directdrive(lwheel, rwheel);
end


function [side] = furtherCup(centDat)
    thresh = 20;
    
    if centDat(1).Centroid(1) > centDat(2).Centroid(1)
        left = 2;
        right = 1;
%         left = centDat(2).Centroid(1);
%         right = centDat(1).Centroid(1);
    else
        left = 1;
        right = 2;
%         left = centDat(1).Centroid(1);
%         right = centDat(2).Centroid(1);
    end
    
    leftTop = centDat(left).BoundingBox(2);
    leftBottom = leftTop + centDat(left).BoundingBox(4)
    
    rightTop = centDat(right).BoundingBox(2);
    rightBottom = rightTop + centDat(right).BoundingBox(4)
    
    if(rightBottom < leftBottom - thresh)
        side = 'r';
    elseif(leftBottom < rightBottom - thresh)
        side = 'l';
    else
        side = 'x';
    end
end


function centerBot(rob, vid, cbAvg, crAvg)
    
    centered = 0;
    centerThresh = 15;

    while centered == 0
        [bwf img ccObjs centDat] = findCups(vid, cbAvg, crAvg);
        [h w depth] = size(img);
        midImg = w / 2;
        
        if centDat(1).Centroid(1) > centDat(2).Centroid(1)
            left = centDat(2).Centroid(1);
            right = centDat(1).Centroid(1);
        else
            left = centDat(1).Centroid(1);
            right = centDat(2).Centroid(1);
        end

        objCent =(right + left) / 2;

        if(objCent > midImg + centerThresh)
            move(rob, 'r');
            pause(.05);
            move(rob, 's');
        elseif objCent < midImg - centerThresh
            move(rob, 'l');
            pause(.05);
            move(rob, 's');
        else
            centered = 1;
        end
    end
end


function [radians] = deg2rad(degrees)
    radians = degrees * (pi / 180);
end