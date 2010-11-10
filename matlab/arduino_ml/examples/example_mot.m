
% connect the board
a=arduino('COM5')

% prints the status of both servos
a.servoStatus(1); 
  
% attach servo #1
a.servoAttach(1);

% prints the status of both servos
a.servoStatus;

% move servo #1 to 5 degrees position
a.servoWrite(1,5);

% move servo #1 to 175 degrees position
a.servoWrite(1,175);

% move servo #1 to 50 degrees position
a.servoWrite(1,50);

% reads angle from servo #1
val=a.servoRead(1);

% attach servo #1
a.servoDetach(1);

% gets speed of motor 4
a.motorSpeed(4)      

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

% sets speed of stepper 2 as 50 rpm
a.stepperSpeed(1,50)      

% prints the speed of stepper 2
a.stepperSpeed(1);         

% rotates stepper 1 forward of 100 steps in interleave mode 
a.stepperStep(1,'forward','double',100);

% rotates stepper 2 backward of 50 steps in single mode 
a.stepperStep(1,'backward','single',50); 

% releases stepper 2
a.stepperStep(1,'release'); 

% close session
delete(a)
