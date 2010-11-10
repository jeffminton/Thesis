function travelDist(serPort, roombaSpeed, distance);
%Moves the Create the distance entered in meters. Positive distances move the
%Create foward, negative distances move the Create backwards.
%roombaSpeed should be between 0.025 and 0.5 m/s
%Esposito & Koehler 7/31/09

try
    
%Flush Buffer    
N = serPort.BytesAvailable();
while(N~=0) 
fread(serPort,N);
N = serPort.BytesAvailable();
end

warning off
global td
if (roombaSpeed < 0) %Speed given by user shouldn't be negative
    disp('WARNING: Speed inputted is negative. Should be positive. Taking the absolute value');
    roombaSpeed = abs(roombaSpeed);
end

if (abs(roombaSpeed) < .025) %Speed too low
    disp('WARNING: Speed inputted is too low. Setting speed to minimum, .025 m/s');
    roombaSpeed = .025;
end

if (distance < 0) %Definition of SetFwdVelRAdius Roomba, speed has to be negative to go backwards. Takes care of this case. User shouldn't worry about negative speeds
    roombaSpeed = -1 * roombaSpeed;
end

SetFwdVelRadiusRoomba(serPort, roombaSpeed, inf);
distanceMM = distance * 1000;
fwrite(serPort, [156]);  fwrite(serPort,distanceMM, 'int16'); 
pause(td)
SetFwdVelRadiusRoomba(serPort, 0, 0);
pause(td)
fwrite(serPort, [154]);
while( serPort.BytesAvailable() ==0)
    %disp('waiting to finish')
end
disp('Done travelDist.')
pause(td)
catch
   disp('WARNING:  function did not terminate correctly.  Output may be unreliable.') 
end