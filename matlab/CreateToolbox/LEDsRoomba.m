function [] = SetLEDsRoomba(serPort, LED,Color, Intensity)
% Manipulates LEDS
% LED = 0 for Play or 1 for Advance 
% Color determines which color(Red/Green) that the Power LED will
% illuminate as, from 0-100%
% 0 is pure green, 100 is pure red.
% Intensity determines how bright the Power LED appears from 1-100%


% Barton 6/23/2008
warning off
global td
%fwrite(serPort, [139, 2,0, 255])
if(LED==0)
    LED=bin2dec('00000010');
else if(LED==1)
    LED=bin2dec('00001000');
    end
end
   
L= LED;
aColor= (Color/100)*255;
aIntensity = (Intensity/100)*255;

fwrite(serPort, [139]);  fwrite(serPort,L); fwrite(serPort,aColor);fwrite(serPort,aIntensity);
disp('LEDs Changing')
pause(td)

