classdef ard_create < handle
    properties
        arduinoConn = 0
    end
    
    methods
        function ardConn = ard_create(port)
            ardConn.arduinoConn = arduino(port);
        end
        function dist = getIR(ardConn)
            distTot = 0;
            
            for i = 1:20
                distTot = distTot + ardConn.arduinoConn.analogRead(3);
            end
            
            dist = distTot / i;
        end
    end
end

        
