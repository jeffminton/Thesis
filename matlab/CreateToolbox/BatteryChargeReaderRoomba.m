function [Charge, Capacity, Percent] = BatteryChargeReaderRoomba(serPort);
% Displays the current percent charge remaining in Create's Battery 

% Barton 6/24/2008

%Initialize preliminary return values
Charge = nan;
Capacity = nan;
Percent = nan;

try
    
%Flush Buffer    
N = serPort.BytesAvailable();
while(N~=0) 
fread(serPort,N);
N = serPort.BytesAvailable();
end

warning off
global td
fwrite(serPort, [142]);  fwrite(serPort,25);
Charge =  fread(serPort, 1, 'uint16');
fwrite(serPort, [142]);  fwrite(serPort,26);
Capacity =  fread(serPort, 1, 'uint16');

Percent=Charge/Capacity*100;


pause(td)
catch
    disp('WARNING:  function did not terminate correctly.  Output may be unreliable.')
end
