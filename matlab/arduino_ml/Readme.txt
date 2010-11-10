ARDUINO IO:

This is a MATLAB class that allows performing Analog and Digital Input 
and Output with the Arduino Board from the MATLAB command line.



IMPORTANT:

If you already have an arduino board and have already used its development 
environment you can skip the following 5 items and go directly to the 
"UPLOAD ADIOSRV.PDE" section. 
If you do not yet have the Arduino and want to try the package in 'DEMO'
mode, then go directly to the DEMO section
If you have an arduino MEGA board, then go to the end of the file to learn
about small modifications to the files that will allow you to access all 
the additional pins on the board.



DETAILS ABOUT ARDUINO:

Arduino is a powerful and inexpensive open-source microcontroller board, 
with an associated development environment: http://arduino.cc/
An extensive Knowledge base can be found here: http://www.freeduino.org/



BUYING AN ARDUINO BOARD:

An extensive list of sites where you can buy an Arduino is here:
http://www.arduino.cc/en/Main/Buy

In the US, adafruit industries (http://www.adafruit.com/) 
provides a starter pack that includes pretty much everything that you
need to get started with the board.



GETTING STARTED GUIDES:

The official getting started guide is here : http://arduino.cc/en/Guide/HomePage
and a very good tutorial is here: http://www.ladyada.net/learn/arduino/
For the purpose of using the Arduino IO Package the only things that are needed
out of such tutorials are the the drivers and environment installation.



DOWNLOADING AND INSTALLING DRIVERS:

A step by step driver installation is covered in the first chapter of the
ladyada tutorial: http://www.ladyada.net/learn/arduino/lesson0.html
as well as in the official site: http://arduino.cc/en/Guide/Windows
It basically comes down to the following actions:

1) Download and unzip the drivers from http://www.ftdichip.com/Drivers/VCP.htm
2) Connect the board via USB cable and wait for the "Found new Hardware" wizard
3) Select "Install from a specific location" and browse to the folder 
   that contains the driver you just downloaded. 
4) Repeat step 3 for the new Hardware wizard that comes up next 
   for the USB Serial Port
5) Reboot the Computer 
6) Go to the Device Manager (From the Start Menu, Settings->Control Panel. 
   Double click on System and select the Hardware tab. Then click on the Device 
   Manager button) and Look for an entry under Ports (COM & LPT) that says 
   "USB Serial Port" (COM) that has FTDI as manufacturer.
   This port will be required in the following steps.



DOWNLOADING AND INSTALLING THE INTEGRATED DEVELOPMENT ENVIRONMENT (IDE):

Again this is covered in the two sites previously mentioned, 
and it comes down to the following steps:
 
1) Download the software from http://www.arduino.cc/en/Main/Software
2) Unpack the files in a directory of your choice
3) You might want to create a desktop link to arduino.exe for future use.
4) Double click on the arduino.exe file to open the environment
5) Under the Tools -> Board menu select the entry corresponding to your chip
   (it's written on the chip, newer boards are equipped with the ATMEGA328).
6) Under Tools -> Serial Port select the COM port found after installing
   the drivers.
7) Open the blink sketck under File -> Sketchbook -> Examples -> Digital -> Blink
8) Select Sketch -> Verify/Compile and wait for the "Done Compiling" message
9) Select File -> Upload to I/O Board and wait for the "Done Uploading" message
   At this point lhe translucent led "L" on the board should start blinking.

An intro to the IDE can be found here: http://arduino.cc/en/Guide/Environment



UPLOAD ADIOSRV.PDE TO THE ARDUINO BOARD:

The adiosrv.pde file is the "server" program that will continuously run 
on the microcontroller. It listens for MATLAB commands arriving from the 
serial port, executes the commands, and, if needed, returns a result.
This file is meant to be used for digital and analog read and write on 
every pin. Note that this is not for usage with the Motor shield.
For the motor shield, the file MOTORSRV.PDE needs to be used.
Please have a look at that section in case you are using the motor shield.

This section is needed to upload the adiosrv.pde file into the controller's 
flash memory. As long as no other file is uploaded later, this step is not 
needed anymore and the package can be used as soon as the board is connected.

From the Arduino IDE, go to File > Open, locate the file adiosrv.pde, 
(in the adiosrv folder) and open it. If a dialog appears asking 
for the permission to create a sketck folder and move the file. Press OK. 
This will create an adiosrv folder and move the adiosrv.pde file inside it.
Then, select File -> Upload to I/O Board and wait for the "Done Uploading" 
message.

At this point the adiosrv.pde file is uploaded and you can close the IDE,
which is not needed anymore for the purpose of this package.



USAGE:

Make sure the board is connected to the computer via USB port, and make sure 
you know which serial port the Arduino is connected to. The port is the same
found at the end of the drivers installation step.

From MATLAB, launch the command a=arduino('port') where 'port' is the
COM port to which the Arduino is connected to, e.g. 'COM5' or 'COM8' 
or '/dev/ttyUSB0' on unix (ubuntu) or 'DEMO' (see below for the DEMO mode) 
and make sure the function terminates successfully.

Then use the commands a.pinMode, (to change the mode of each pin
between input and output) a.digitalRead, a.digitalWrite, 
a.analogRead, and a.analogWrite, to perform digital input, 
digital output, analog input, and analog output. 
Consult the help of the files to get more information about their usage. 

Finally, use a.disconnect to disconnect the board from MATLAB when 
the session is over.



DEMO:

Whenever 'DEMO' is specified as argument of the arduino function, i.e. 
arduino('DEMO') then a virtual connection is opened that does 
not involve any hardware. This allows trying the package without actually 
having an arduino board. In this mode, the a.digitalRead and a.digitalWrite
functions return a random value according to their output range. The 
execution time of any function matches the average execution time 
that can be observed when the actual board is connected.



EXAMPLE:

% connect the board
a=arduino('COM5')

% specifies pin mode for pins 4, 13 and 5
a.pinMode(4,'input');
a.pinMode(13,'output');
a.pinMode(5,'output');

% read digital input from pin 4
dv=a.digitalRead(4);

% output the digital value (0 or 1) to pin 13
a.digitalWrite(13,dv);

% read analog input from analog pin 5 (physically != from digital pin 5)
av=a.analogRead(5);

% normalize av from 0:1023 to 0:254
av=(av/1023)*254;

% ouptput value on digital (pwm) pin 5 
% (which is different that the analog pin 5)
a.analogWrite(5,av)

% close session
delete(a)



BLINK CHALLENGE:

The "blink_challenge" is a demo application that switches the leds on and
off with variable frequency and with mode changed by pressing a switch button
Have a look at the m file for more info, and launch blink_challenge to 
execute the demo.

NOTE that running this applicaton only makes sense if the Arduino board is 
connected to an appropriate circuit where digital pins 9 to 13 are configured 
as outputs and connected to leds, digital pin #2 is configured as input and
connected to a button that pulls the voltage down when pressed, and analog
pin #2 is connected to a potentiometer. See the scheme "blinkchallenge.mdl". 



MOTOR SHIELD:

The motor shield is a shield to control dc, stepper and servo motor.
Details are here: http://www.ladyada.net/make/mshield/
If you want to use the motor shield make sure do the following:
1) Download the servotimer library:
   http://www.ladyada.net/media/mshield/ServoTimer1-fixedv13.zip
   then uncompress, and stick the ServoTimer1 directory into the 
   arduino-0018/libraries folder
2) Download the motor library:
   http://www.ladyada.net/media/mshield/AFMotor-08_12_2009.zip
   then uncompress, and stick the AFMotor directory into the 
   arduino-0018/libraries folder. 
It is also a very good idea to have a very good look at the Manual:
http://www.ladyada.net/make/mshield/use.html



UPLOAD MOTORSRV.PDE TO THE ARDUINO BOARD:

The motorsrv.pde file is the "server" program that will continuously run 
on the microcontroller. It listens for MATLAB commands arriving from the 
serial port, executes the commands, and, if needed, returns a results.
This file is meant to be used with the Motor Sield.

From the Arduino IDE, go to File > Sketckbook > Open, locate the file
motorsrv.pde, (in the motorsrv folder) and open it. 
Then, select File -> Upload to I/O Board and wait for the "Done Uploading" 
message.

At this point the motorsrv.pde file is uploaded and you can close the IDE,
which is not needed anymore until you remove the shield.



MOTOR SHIELD EXAMPLE:

% attach servo #1
a.servoAttach(1); 

% return the status of all servos
a.servoStatus; 

% rotates servo #1 of 45 degrees
a.servoWrite(1,45); 

% reads angle from servo #1
val=a.servoRead(1); 

% sets speed of motor 4 as 200/255
a.motorSpeed(4,200)      

% prints the speed of all motors
a.motorSpeed;    
        
% runs motor 1 forward
a.motorRun(4,'forward');    
 
% runs motor 3 backward
a.motorRun(4,'backward');     

% release motor 1
a.motorRun(4,'release');      

% sets speed of stepper 1 as 10 rpm
a.stepperSpeed(1,10)      

% prints the speed of stepper 1
a.stepperSpeed(1);         

% rotates stepper 1 forward of 100 steps in interleave mode 
a.stepperStep(1,'forward','double',100);

% rotates stepper 1 backward of 50 steps in single mode 
a.stepperStep(1,'backward','single',50); 

% releases stepper 1
a.stepperStep(1,'release'); 



ARDUINO MEGA

The Arduino Mega board has several more I/O pins than the Duemilanove. 
Stefan Neuhäuser, [stefan . neuhaeuser at ilek . uni-stuttgart . de]
has suggested modifying the files 'arduino.m' and 'adiosrv.pde' as 
following, to access all the available pins:

1) In the file 'arduino.m', change lines 238,330,394 
   by replacing 19 with 69, as following:
   errstr=arduino.checknum(pin,'pin number',2:69);
   then replace 5 with 15 in line 464, as following:
   errstr=arduino.checknum(pin,'analog input pin number',0:15);
   then finally replace 19 with 69 in lines 122,146 and 295:
   line 122: a.pins=-1*ones(1,69);
   line 146: for i=2:69,
   line 295: for i=2:69,

2) In the file 'adiosrv.pde', change every occurrence of 
   if (val>98 && val <117) { 
   with the following:
   if (val>98 && val <167) {
   (basically replace 117 with 167)
   and also change the line
   if (val>96 && val <103) {
   with the line
   if (val>96 && val <113) {
   (basically replace 103 with 113).

Stefan has a working version of both files.



FILES & HISTORY:

See contents.m for details on files and version history.



Copyright 2009 The MathWorks, Inc.
