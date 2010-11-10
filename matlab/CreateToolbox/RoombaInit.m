function [serPort] = RoombaInit(my_COM);
% initializes serial port for use with Roomba
% COMMport is the number of the comm port 
% ex. RoombaInit(1) sets port = 'COM1'
% note that it sets baudrate to a default of 57600
% can be changed (see SCI).  
% An optional time delay can be added after all commands
% if your code crashes frequently
global td
td = 0.01;
% This code puts the robot in CONTROL(132) mode, which means does NOT stop 
% when cliff sensors or wheel drops are true; can also run while plugged into charger
Contrl = 132;

% Esposito 9/2008 

warning off

%% set up serial comms,   
% output buffer must be big enough to take largest message size
comm = strcat('COM', num2str(my_COM));


a = instrfind('port',comm);
if ~isempty(a)
            disp('That com port is in use.   Closing it.')
            fclose(a)
            delete(a)
end
    
disp('Establishing connection to Roomba...');

% defaults at 57600, can change
serPort = serial(comm,'BaudRate', 57600);
set(serPort,'Terminator','LF')
set(serPort,'InputBufferSize',100)
set(serPort, 'Timeout', 1)
set(serPort, 'ByteOrder','bigEndian');
set(serPort, 'Tag', 'Roomba')

disp('Opening connection to Roomba...');
fopen(serPort);

%% Confirm two way connumication
disp('Setting Roomba to Control Mode...');
% Start! and see if its alive
Start=[128];
fwrite(serPort,Start);
pause(.1)

fwrite(serPort,Contrl);
pause(.1)
% light LEDS
fwrite(serPort,[139 25 0 128]);

% set song
fwrite(serPort, [140 1 1 48 20]);
pause(0.05)
% sing it
fwrite(serPort, [141 1])

disp('I am alive if my two outboard lights came on')

confirmation = (fread(serPort,4))
pause(.1)