function square(length)
%SQUARE Simulates the Roomba moving in a square.

% Creates a iRobotCreate simulation object with update rate of 5 Hz
iRC = iRobotCreate(1, 5);

% Sets noise to default values
iRC.setnoise(1);

% Puts it in the robot frame
iRC.setworldframe(0);

% Creates a workspace that follows the robot in a 4 x 4 frame
iRC.setworkspace([0 4 0 4]);

% Creates a trail size of infinite length
iRC.trailsize(inf)

for i = 0:3
    iRC.forward(length);
    % Prints out the simulation location
    iRC.loc
    iRC.rotate(pi/2);
end