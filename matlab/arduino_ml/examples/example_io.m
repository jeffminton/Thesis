% connect the board
a=arduino('COM5')

% specifies pin mode for pins 4, 13 and 5
a.pinMode(2,'input');
a.pinMode(13,'output');
a.pinMode(9,'output');

% read digital input from pin 4
dv=a.digitalRead(2)

% output the digital value (0 or 1) to pin 13
a.digitalWrite(13,dv)

% read analog input from analog pin 5 (NB: not the _digital_ pin 5)
av=a.analogRead(5)

% normalize av from 0:1023 to 0:255
av=(av/1023)*255

% ouptput value on digital (pwm) pin 5 
% (again, note that analog pin 5 is physically another pin)
a.analogWrite(9,av)

% close session
delete(a)