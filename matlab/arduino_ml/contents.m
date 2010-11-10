% Arduino IO Class
% Version 2.5 (R2010a), G. Campa,  12-Aug-2010
% Copyright 2009 The MathWorks, Inc.
% 
% This is a MATLAB class that allows performing Analog/Digital Input 
% and Output with the Arduino Board from the MATLAB command line.
% Please read the file readme.txt for more info
% 

% FILES:
% 
% Readme.txt              : file you should read before doing anything
% contents.m              : this file (needed for matlab help)
% 
% arduino.m               : defines the arduino class
% 
% adiosrv                 : folder containing adiosrv.pde sketch
% motorsrv                : folder containing motorsrv.pde sketch
% 
% examples                : folder containing a few examples, specifically:
% blink_challenge.m       : blink challenge code
% blinkchallenge.mdl      : blink challenge schematics
% example_io.m            : IO example
% example_mot.m           : motor example
% 
% license                 : folder containing license files
% 

% 
% HISTORY:
% 
% Ver  1.0 - Jul 2009  - Initial Version
% Ver  1.1 - Aug 2009  - Readme.txt file added, additional checks on variables
% Ver  1.2 - Aug 2009  - Readme.txt and arduino_connect extensively rewritten
% Ver  1.4 - Aug 2009  - Additional checks and DEMO mode capability added
% Ver  1.5 - Aug 2009  - Change pin mode added and using fwrite instead of fprint
% Ver  1.7 - Sep 2009  - Many improvements in adiosrv.pde and functions help
% Ver  1.9 - Sep 2009  - Motor shield support and error check improvements
% Ver  1.A - Sep 2009  - Improvements to help and motor shield handling
% Ver  1.B - Sep 2009  - Improved connection, fixed demo mode bug
% Ver  2.0 - Oct 2009  - Rewritten using m-class objects
% Ver  2.1 - Feb 2010  - Display function added
% Ver  2.2 - Apr 2010  - Display function improved
% Ver  2.3 - Jun 2010  - Very minor corrections in the examples and in readme.txt
% Ver  2.4 - Jul 2010  - License files added, and delete function refined
% Ver  2.5 - Aug 2010  - Instructions for the Arduino Mega board included
% 