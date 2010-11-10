classdef iRobotCreate < handle
    %IROBOTCREATE A Matlab interface to the iRobotCreate.
    %   This is a wrapper to the iRobotCreate.
    %  
    %   Usage:
    %       This creates an interface for Matlab to communicate with the
    %       robot and give commands to the robot. There are two modes in
    %       this: simulation and robot. In simulation mode, it will create a
    %       graphical representation of the robot and respond to the same
    %       commands as a real robot, emulating the behaviour of a robot. 
    %       The constructor will, by default, draw the roomba on a figure.
    %       In the robot mode, it will connect with the roomba and
    %       give it commands. The constructor in this mode will connect
    %       with the port defined.
    %
    %       When the constructor is called, it will create handles to the
    %       appropriate functions. Thus, you can run commands in simulation
    %       mode and use similar, if not the same, code in the real mode.
    %
    %       For more information on any method, type the following at the prompt:
    %       help iRobotCreate.methodname
    %
    %   Example constructing object:
    %       Simulation mode:
    %           iRC = iRobotCreate(1, 5;
    %           This will create an object iRC in simulation mode with an
    %           update rate of 5 Hz.
    %
    %       Robot mode:
    %           iRC = iRobotCreate(0, 5);
    %           This will connect Matlab to the roomba through the object
    %           iRC.
    %
    %   Methods:
    %       iRobotCreate
    %       connect
    %       comm (robot)
    %       drawroomba (simulation)
    %       moveroomba (simulation)
    %       resumecontrol (robot)
    %       beep (robot)
    %       setnoise (simulation)
    %       setupdaterate (simulation)
    %       setvel
    %       directdrive
    %       forward
    %       rotate
    %       isbumped (robot)
    %       worldframe (simulation)
    %       workspace (simulation)
    %       trailsize (simulation)
    %       distancesensor
    %       anglesensor
    %       delete
    %
    %   Note: Certain methods are only available in certain modes.
    
    %The handles to the various functions
    properties
        connect_h;
        comm_h;
        drawroomba_h;
        moveroomba_h;
        resumecontrol_h;
        beep_h;
        setnoise_h;
        setvel_h;
        trailsize_h;
        directdrive_h;
        forward_h;
        rotate_h;
        isbumped_h;
        setworkspace_h;
        setworldframe_h;
        setupdaterate_h;
        distancesensor_h;
        anglesensor_h
        delete_h;
    end
    
    %Counters and such...
	properties (Access = public, Hidden)
        angle = 0;
        anglecount = 0;
        autoconnect = 1;
        com;
        comnum;
        dimensions = [-2 12 -2 12];
        distance = 0;
        h_arrow;
        h_body;
        h_trail;
        loc;            %Holds the location of the robot in simulation
        noise = 1;
        sigma_omega = 0;
        sigma_v = 0;
        steps = 0;
        time;
        totalangle = 0;
        totaldist = 0;
        trail;
        traillength = 100;
        update = 5;
        worldfr = 1;
    end
	
    %Constants to be used...
	properties (Constant, Access = private, Hidden)
		v = 0.4;
		vturn = 0.1;
		d = 0.30;
        wheelbase = 0.26;
		op = struct( ...
			'Start', 128, ...
			'Baud', 129, ...
			'Control', 130, ...
			'Safe', 131, ...
			'Full', 132, ...
			'Spot', 134, ...
			'Cover', 135, ...
			'Demo', 136, ...
			'Drive', 137, ...
			'LSD', 138, ...
			'LED', 139, ...
			'Song', 140, ...
			'PlaySong', 141, ...
			'Sensors', 142, ...
			'CoverDock', 143, ...
			'PWMLSD', 144, ...
			'DriveDirect', 145, ...
			'DigitalOutput', 147, ...
			'Stream', 148, ...
			'QueryList', 149, ...
			'PauseResumeStream', 150, ...
			'SendIR', 151, ...
			'Script', 152, ...
			'PlayScript', 153, ...
			'ShowScript', 154, ...
			'WaitTime', 155, ...
			'WaitDistance', 156, ...
			'WaitAngle', 157, ...
			'WaitEvent', 158);
    end
    
    %The method handles
    methods
        function iRC = iRobotCreate(sim, varargin)
            %IROBOTCREATE   The iRobot Create constructor
            %   Takes in multiple arguments, depending on whether or not
            %   simulation mode is active or not. The first argument is
            %   to see whether the object is to be in simulation mode or
            %   not. If it is a 1, the object will be set to simulation
            %   mode. If it is a 0, the object will be set to real mode.
            %
            %   If the object is set to simulation mode, the second
            %   argument defines the update rate in Hertz. If this is not
            %   set, the simulation will default to 5 Hz. By default, this
            %   will draw the roomba, thus it is unnecessary to use the
            %   drawroomba command. The figure is initially drawn in the
            %   world frame, but this can be changed with the worldframe
            %   function.
            %
            %   If the object is set to real mode, the second argument
            %   defines the port of the computer. This must be defined. By
            %   default the constructor, this automatically connects with 
            %   the port, so it is unnecessary to use the connect command.
            
            % Checks if it is in simulation
            if sim
                
                % Assigns the handles to point to the set of functions
                % specific to simulation mode
                
                iRC.connect_h = @iRC.connect_sim;
                iRC.drawroomba_h = @iRC.drawroomba_sim;
                iRC.moveroomba_h = @iRC.moveroomba_sim;
                iRC.setnoise_h = @iRC.setnoise_sim;
                iRC.trailsize_h = @iRC.trailsize_sim;
                iRC.setworkspace_h = @iRC.setworkspace_sim;
                iRC.setworldframe_h = @iRC.setworldframe_sim;
                iRC.setupdaterate_h = @iRC.setupdaterate_sim;
                iRC.setvel_h = @iRC.setvel_sim;
                iRC.directdrive_h = @iRC.directdrive_sim;
                iRC.forward_h = @iRC.forward_sim;
                iRC.rotate_h = @iRC.rotate_sim;
                iRC.distancesensor_h = @iRC.distancesensor_sim;
                iRC.anglesensor_h = @iRC.anglesensor_sim;
                iRC.delete_h = @iRC.delete_sim;
               
                % Draws the roomba figure
                iRC.drawroomba_h();
                
                % Reads in the update rate.
                if (nargin == 2)
                    
                    % The update rate must be set between 1 and 20                
                    if 1 > varargin{1} || varargin{1} > 20
                        error('iRC:iRobotCreate', ['The update rate of the Roomba',...
                        'must be between 1 and 20']);
                    end
                    
                    iRC.update = varargin{1};
                    

                else
                    fprintf('The update rate has defaulted to 5 Hz \n');
                end

                % Starts a timer to ensure the update rate is maintained
                iRC.time = tic;
            else
                
                % Assigns the handles to point to the set of functions
                % specific to real mode
                iRC.connect_h = @iRC.connect_real;
                iRC.resumecontrol_h = @iRC.resumecontrol_real;
                iRC.beep_h = @iRC.beep_real;
                iRC.setvel_h = @iRC.setvel_real;
                iRC.directdrive_h = @iRC.directdrive_real;
                iRC.forward_h = @iRC.forward_real;
                iRC.rotate_h = @iRC.rotate_real;
                iRC.isbumped_h = @iRC.isbumped_real;
                iRC.distancesensor_h = @iRC.distancesensor_real;
                iRC.anglesensor_h = @iRC.anglesensor_real;
                iRC.delete_h = @iRC.delete_real;
               
                % Assigns the port to be used
                iRC.comnum = varargin{1};
                if iRC.autoconnect
                   iRC.connect();
                end
            end
        end

        function connect(iRC)
            %CONNECT Connects with the robot
            %   Only available in robot mode.
            iRC.connect_h();
        end
        
        function resumecontrol(iRC)
            %RESUMECONTROL Resumes control if the robot was picked up
            %   Only available in robot mode.
            iRC.resumecontrol_h();
        end
                
        function drawroomba(iRC, varargin)
            %DRAWROOMBA Draws the Roomba to the figure. 
            %   This may take in an argument that draws the Roomba to the 
            %   position specified at [x, y, theta]. If there is no
            %   argument, it will draw the Roomba at [0, 0, 0].
            iRC.drawroomba_h(varargin)
        end
        
        function moveroomba(iRC, pose)
            %MOVEROOMBA Moves the Roomba to position [x, y, theta]. 
            %   This requires an argument that specifies [x, y, theta].
            %   Only available in simulation mode.
            iRC.drawroomba_h(pose)
        end
        
        function beep(iRC)
            % BEEP Make the robot beep, like on startup.
            %   This function is only available in robot mode.
            iRC.beep_h();
        end
        
        function setnoise(iRC, noise, varargin)
            % SETNOISE Sets the amount of noise to be added to the simulation
            %	The simulation will default to no noise. Takes three arguments. 
            %   First whether or not there should be noise. Second, the 
            %   noise to be added to the linear velocity. Third, the noise 
            %   to be added to the angular velocity. If these are not 
            %   specified, the linear velocity and angular velocity will 
            %   default to 0.0152 and 0.5098 respectively.
            iRC.setnoise_h(noise, varargin);
        end
        
        function setvel(iRC, v, omega)
            % SETVEL Set the robots linear and angular velocity.
			%	Takes two arguments. First the linear velocity in m/s,
			%	second the angular velocity in radian/s. The seperate wheel
			%	speeds will then be set. Wheel speed is limited to 0.5m/s
			%	in either direction.
           iRC.setvel_h(v, omega);
        end
        
        function directdrive(iRC, v1, v2)
            % DIRECTDRIVE Set wheel velocities directly.
			%	Most likely used for direct driving of the robot.
            iRC.directdrive_h(v1, v2);
        end
        
        function forward(iRC,d)
            % FORWARD Move the robot forward.
			%	Takes one argument, the distance to move in meters. A
			%	negative distance will make the robot move backwards.
			%	Blocks until finished, uses the setvel method.
            iRC.forward_h(d);
        end
        
        function rotate(iRC, angle)
            % ROTATE Rotate the robot.
			%	Takes one argument, the angle to turn in radians. A 
			%	positive angle means anticlockwise rotation, a negative
			%	angle means clockwise rotation. The angle should be in the
			%	range of -pi to pi, if it is not, it will be translated to
			%	this range. Blocks until finished, uses the setvel method.
            iRC.rotate_h(angle)
        end
            
        function bool = isbumped(iRC)
			% ISBUMPED Checks the front bumper for a bump.
			%	Takes no arguments. Only detects bumper bumps at a given
			%	instant using the sensor. This function is only available
			%	in robot mode.
            bool = iRC.isbumped_h();
        end
        
        function setworkspace(iRC, space)
            % SETWORKSPACE Takes in the bounds of the workspace
            %   This will change the bounds of the workspace. This must be 
            %   inputted in the form ([XMin XMax YMin YMax]). Only 
            %   available in simulation mode         
            iRC.setworkspace_h(space);
        end
        
        function setworldframe(iRC, frame)
            % SETWORLDFRAME Changes frame to world frame.
            %   If the input to this function is 1, the frame will change
            %   to the world frame. Otherwise, it will be in robot frame.
            %   This function is only available in simulation mode.
            iRC.setworldframe_h(frame);
        end
        
        function setupdaterate(iRC, update)
            % SETUPDATERATE Sets the update rate.
            %   This function sets the update rate. Initially, the update
            %   rate defaults to 5 Hz, if not defined within the
            %   constructor.
            
            iRC.setupdaterate_h(update);
        end
        
        function trailsize(iRC, newlength)
            % TRAILSIZE Changes the trail length to the inputted number.
            %   Takes 1 argument, the new trail length. The trail length
            %   defaults to 100. However, this function can change that. If
            %   the user inputs inf as the trail length, the trail length
            %   will be infinite. This function is only available in
            %   simulation mode.
            iRC.trailsize_h(newlength);
        end
        
        function d = distancesensor(iRC)
			% DISTANCESENSOR Gets the distance from the distance sensor.
			%	Takes no arguments. The distance sensor is reset every time
			%	a value is read from it.
            d = iRC.distancesensor_h();
        end
        
        function a = anglesensor(iRC)
			% ANGLESENSOR Gets the angle from the angle sensor.
			%	Takes no arguments. The angle sensor is reset every time a
			%	value is read from it.            
            a = iRC.anglesensor_h();
        end
        
        function delete(iRC)
            %DELETE Deconstructs the object.
            iRC.delete_h();
        end
    end

    methods (Hidden, Access = private)
        
        function connect_sim(iRC)
            % CONNECT The GUI constructor
            %   Draws the roomba, in the event that this is accidentally
            %   called during simulation mode.
            drawroomba_sim(iRC);
        end
        
        function drawroomba_sim(iRC, varargin)
            % DRAWROOMBA The GUI constructor
            %	Creates the roomba object and graphs it at the location 
            %   specified by the roomba. If there is no input, it will 
            %   graph it at the origin with no angle. Usually, this 
            %   function does not need to be called.
            
            iRC.steps = iRC.steps + 1;
            scale = 1; % the scale (adjust how big it appears)

            rad = 0.3; % the radius
            N = 60; % number of lines in the circle approximation
            t = 0:2*pi/N:2*pi;
            r.const_body_x = rad * cos(t) * scale;
            r.const_body_y = rad * sin(t) * scale;

            const_arrow_x = [ -.3 * rad , -.3 * rad , rad * 1] .* scale;
            const_arrow_y = [ -6/10 * rad, 6/10 * rad, 0] .* scale;
            
            % Checks if there was a previously drawn image, by seeing if
            % the pose was passed into the DRAWROOMBA.

            if (nargin > 1)
                pose = varargin{1};
            else
                pose = [0,0,0];
            end
            
            % Keeps track of the location and angle here.
            iRC.loc = [pose(1) pose(2)];
            iRC.angle = pose(3);

            % The arrow is rotated around the body to indicate direction

            new_arrow_x = [cos(pose(3)), -sin(pose(3))] * ...
                [const_arrow_x; const_arrow_y];
            new_arrow_y = [sin(pose(3)), cos(pose(3))] * ...
                [const_arrow_x; const_arrow_y];

            % The body need not be rotated, as it is a circle.

            poly_body_x = r.const_body_x + pose(1);
            poly_body_y = r.const_body_y + pose(2);

            % The arrow is moved to the new location

            poly_arrow_x = new_arrow_x + pose(1);
            poly_arrow_y = new_arrow_y + pose(2);

            % Checks if there are as many points as trail length. If there are
            % deletes the first, so another can be added.
            
            if iRC.traillength < inf
                if gt(iRC.steps, iRC.traillength)
                    iRC.trail = iRC.trail(2:iRC.traillength,:);
                    iRC.trail(iRC.traillength, :) = [pose(1), pose(2)];
                else
                    iRC.trail(iRC.steps,:) = [pose(1), pose(2)];
                end
            else
                iRC.trail(iRC.steps,:) = [pose(1), pose(2)];
            end
            

            % Plots the trail and body.

            if (iRC.steps == 1)
                axis (iRC.dimensions);
                axis square;
                hold on;
                iRC.h_trail = plot(iRC.trail(:,1),iRC.trail(:,2),'k.');
                iRC.h_body = fill(poly_body_x, poly_body_y, 'w');
                iRC.h_arrow = fill(poly_arrow_x, poly_arrow_y, 'r');
            end
            
            % With each redrawing, the axis are changed depending on
            % whether it should be drawn in the world frame or robot
            if iRC.worldfr
                axis (iRC.dimensions);
            else
                % Centers the figure on the robot
                newaxis = ([(iRC.loc(1) - 1/2*(iRC.dimensions(2) - iRC.dimensions(1)))...
                    (iRC.loc(1) + 1/2*(iRC.dimensions(2) - iRC.dimensions(1)))...
                    (iRC.loc(2) - 1/2*(iRC.dimensions(4) - iRC.dimensions(3)))...
                    (iRC.loc(2) + 1/2*(iRC.dimensions(4) - iRC.dimensions(3)))]);
                axis (newaxis);
            end
            
            % Does not redraw the figure, sets the data to a new coordinate
            set(iRC.h_trail, 'XData', iRC.trail(:,1), 'YData', iRC.trail(:,2));
            set(iRC.h_arrow, 'XData', poly_arrow_x, 'YData', poly_arrow_y);
            set(iRC.h_body, 'XData', poly_body_x, 'YData', poly_body_y);
        
        end
        
        function moveroomba_sim(iRC, pose)
            % MOVEROOMBA Moves the graphical Roomba from the current spot to
            %   the specified position.
            drawroomba_sim(iRC, pose);
        end
        
        function setnoise_sim(iRC, noise, varargin)
            % SETNOISE Sets the amount of noise in the simulation
            %	Can take three arguments. The first argument specifies whether
            %   or not the noise should be turned on. The second and third
            %   argument are the noise on linear velocity and noise on angular 
            %   velocity, respectively. The measured inputs for this
            %	function should be (0.0152, .5098). If the linear and
            %	angular velocity are not specified, it will default to the
            %	measured values.

            if (noise == 1 && ~isempty(varargin{1}))
                % Sets the noise in the object
                iRC.sigma_v = varargin{1}{1};
                iRC.sigma_omega = varargin{1}{2};
            
            elseif (noise == 1 && isempty(varargin{1}))
                % Sets the noise on the measured values
                iRC.sigma_v = 0.0152;
                iRC.sigma_omega = 0.5098;
                
            elseif (noise == 0 && isempty(varargin{1}))
                %Turns off the noise
                iRC.sigma_v = 0;
                iRC.sigma_omega = 0;
            
            else
                error('iRC:noise', ['SETNOISE requires three inputs - the first', ...
                    ' to determine if noise is to be on/off. The second and third', ...
                    ' determine noise that will be added to linear and angular', ...
                    ' velocities, respectively. If noise is set to 1, the linear', ...
                    ' and angular velocity are not specified, they will be set to,' ...
                    ' 0.0152 and 0.5098, respectively']);
            end
        end

        function setvel_sim(iRC, v, omega)
            % SETVEL Sets the velocity of the simulation Roomba. 
            %	Takes two arguments, linear and angular velocity.
            %	The speeds cannot exceed 0.5 m/s.
            
            % Checks if the velocity is too large for the robot to handle
            wheel = int16([0.5, 0.5; 1/iRC.d, -1/iRC.d] ...
                \ [v; omega] .* 1000);
            if -500 > wheel(1) || wheel(1) > 500 || ...
                    -500 > wheel(2) || wheel(2) > 500
                error('iRC:setvel', ['the speed of each wheel cannot', ...
                    ' exceed 0.5m/s (consider both v and omega)']);
            end
            % Measures the time elapsed so the simulation is true to the update rate
            telapsed = toc(iRC.time);
            
            % Corrupt the velocities if necessary
            % sigma_v and sigma_omega are 0 by default
            v_hat = v + ( iRC.sigma_v * v * randn);
            omega_hat = omega + ( iRC.sigma_omega * omega * randn );

            % Calculate the displacement in the robot frame and the rotation 
            dx = v_hat/iRC.update;
            dtheta = omega_hat/iRC.update;

            % Calculate the displacement in the world frame
            xdisp = dx*cos( iRC.angle + dtheta/2 );
            ydisp = dx*sin( iRC.angle + dtheta/2 );

            % Update the robot pose
            pose = [(xdisp + iRC.loc(1)), (ydisp + iRC.loc(2)), (dtheta + iRC.angle)];

            % Update the robot counters 
            iRC.distance = iRC.distance + sqrt(xdisp^2 + ydisp^2);
            iRC.anglecount = iRC.anglecount + dtheta;
            iRC.totaldist = sqrt(xdisp^2 + ydisp^2) + iRC.totaldist;
            iRC.totalangle = iRC.totalangle + dtheta;
            iRC.angle = iRC.angle + dtheta;

            % Move the robot
            iRC.moveroomba(pose);

            % Pause to simulate real-time ops and grab the new time
	    % Blocks until the update rate wanted is achieved.
            pause((1/iRC.update - telapsed));
            iRC.time = tic;
        end
        
        function directdrive_sim(iRC, v1, v2)
            % DIRECTDRIVE Sets the differential velocity of the Roomba.
            %   Takes two arguments, one for each wheel. This translates
            %   the velocities to a linear and angular velocity.
            
            % Convert the velocity of each wheel into a linear and angular velocity
            linearv = (v1 + v2)/2;
            angv = (v1 - v2)/iRC.wheelbase;
            
            % Determines if the velocities are too big            
            wheel = int16([0.5, 0.5; 1/iRC.d, -1/iRC.d] ...
                \ [linearv ; angv] .* 1000);
            
            if -500 > wheel(1) || wheel(1) > 500 || ...
                    -500 > wheel(2) || wheel(2) > 500
                error('iRC:directdrive', ['the speed of each wheel cannot', ...
                    ' exceed 0.5m/s (consider both v and omega)']);
            end
            

            % Setvel command is run now that we know the linear and angular velocity
            iRC.setvel(linearv,angv);
        end
        
        function forward_sim(iRC, d)
            % FORWARD Makes the robot move forward.
            %   Takes one argument, the distance to move forward. A
            %   positive distance means it is moving forward. A negative
            %   distance indicates movement backwards. Blocks until
            %   finished, uses the setvel method
            
            % check distance
            if abs(d) > 10
                error('iRC:forward', 'distance is too large');
            end
            
            %Finds final destination of this command
            finald = [(d*cos(iRC.angle)), (d*sin(iRC.angle))];
            
            displacement = 0;

            % Sets the velocity of the forward command
            speed = 0.5;
            % Runs the setvel command at 0.5 m/s until it has gone the correct distance.
            while ((norm(finald) - 0.01) > displacement)
                
                    %Finds current traversed distance to be used for counter
                    initdist = iRC.totaldist;
                    iRC.setvel(sign(d)*speed, 0);
                    
                    % Update counters
                    displacement = displacement + iRC.totaldist - initdist;
            end
        end
        
        function rotate_sim(iRC, angle)
            % ROTATE Rotate the robot.
			%	Takes one argument, the angle to turn in radians. A 
			%	positive angle means anticlockwise rotation, a negative
			%	angle means clockwise rotation. The angle should be in the
			%	range of -pi to pi, if it is not, it will be translated to
			%	this range. Blocks until finished, uses the setvel method.
            
            %Convert the angle to the simplest form.
            while pi < angle
                angle = angle - 2 * pi;
            end
            
            while angle < -pi
                angle = angle + 2 * pi;
            end
            
            % Calculates the final angle
            finalangle = iRC.anglecount + angle;

            % If the angle is positive, it will spin in the ccw direction
            if (angle > 0)
                while (finalangle > (iRC.anglecount + 0.01))
                    iRC.setvel(0, pi/8);                    
                end
	    
            % If the angle is negative, it will spin in the cw direction
            else
                while (finalangle < (iRC.anglecount - 0.01))
                    iRC.setvel(0, -pi/8);                    
                end
            end
        end
             
        function setworkspace_sim(iRC, dimensions)
            % SETWORKSPACE Takes in the bounds of the workspace
            %   This will change the bounds of the workspace. This must be 
            %   inputted in the form ([XMin XMax YMin YMax])
            %   If nothing is inputted, it will default to the original 
            %   workspace ([-2 12 -2 12])

            if (size(dimensions) == [1 4])
                iRC.dimensions = dimensions;
                axis(iRC.dimensions);
            else
                error('iRC:setworkspace', ['The inputted workspace is ', ...
                    'invalid. It must be in the form [XMin XMax YMin', ...
                    ' YMax].']);
            end
        end
        
        function setworldframe_sim(iRC, worldframe)
            % SETWORLDFRAME Changes frame to world frame.
            %   If the input to this function is 1, the frame will change
            %   to the world frame. Otherwise, it will be in robot frame.
            %   The default frame is the world frame.
            
            
            % Allows this to be accessible to the robot object
            iRC.worldfr = worldframe;
            
            % If the worldframe is active, it will change the axis to what
            % the user has defined, or else it will center on the robot.
            if worldframe
                axis (iRC.dimensions);
            else
                axis equal;
            end
        end
        
        function setupdaterate_sim(iRC, update)
            % SETUPDATERATE Sets the update rate.
            %   This function sets the update rate. Initially, the update
            %   rate defaults to 5 Hz, if not defined within the
            %   constructor.
            
            % The update rate must be set between 1 and 20                
                if 1 > update || update > 20
                error('iRC:setupdaterate', ['The update rate of the Roomba',...
                    'must be between 1 and 20']);
                end
            iRC.update = update;
        end
        
        function trailsize_sim(iRC, newlength)
            % TRAILSIZE Changes the trail length to the inputted number.
            %   Takes 1 argument, the new trail length. The trail length
            %   defaults to 100. However, this function can change that. If
            %   the user inputs inf as the trail length, the trail length
            %   will be infinite.
            
            iRC.traillength = newlength;
        end
        
        function d = distancesensor_sim(iRC)
            % DISTANCESENSOR Returns the distance the Roomba has moved since
            %   the last time the function was called.
            d = iRC.distance;
            iRC.distance = 0;
        end
        
        function a = anglesensor_sim(iRC)
            % ANGLESENSOR Returns the angle the Roomba has rotated since the
            %   the last time the function was called.
			
            a = iRC.anglecount;
            iRC.anglecount = 0;
        end
        
        function delete_sim(iRC)
            % DELETE Deconstructs the GUI
            
            delete(iRC.h_trail);
            delete(iRC.h_body);
            delete(iRC.h_arrow);
        end
        
        function connect_real(iRC)
            % CONNECT Connects with the robot
            try
				name = strcat('COM', round(num2str(iRC.comnum)));
				% check port availability
				if ~isempty(instrfind('port', name, 'status', 'open'))
					error('iRC:init', ['serial port does not exist', ...
						' or is already in use']);
				end
				% create port
				iRC.com = serial(name, ...
					'BaudRate', 57600, ...
					'Terminator','LF', ...
					'InputBufferSize',100, ...
					'Timeout', 0.1, ...
					'ByteOrder','bigEndian', ...
					'Tag', 'Roomba');
				% open the port
				fopen(iRC.com);
%                 get(iRC.com);
				% start the bot
				fwrite(iRC.com, [iRC.op.Start]);
				pause(0.1);
				% set control
				fwrite(iRC.com, [iRC.op.Full]);
				pause(0.1);
				% light LEDs
				fwrite(iRC.com, [iRC.op.LED, 10, 0, 128]);
				pause(0.1);
				% select song
				fwrite(iRC.com, [iRC.op.Song, 1, 1, 48, 20]);
				pause(0.1);
				% play song
				fwrite(iRC.com, [iRC.op.PlaySong, 1]);
				pause(0.1);
			catch err
				if ~isempty(iRC.com)
					fclose(iRC.com);
				end
				rethrow(err);
            end
        end
        
        function resumecontrol_real(iRC)
            %RESUMECONTROL Resumes control if the robot was picked up

            % set control
            fwrite(iRC.com, [iRC.op.Safe]);
            pause(0.1);
            % light LEDs
            fwrite(iRC.com, [iRC.op.LED, 10, 0, 128]);
            pause(0.1);
        end
        
        function beep_real(iRC)
            % BEEP Make the robot beep, like on startup.
			
            % select song
            fwrite(iRC.com, [iRC.op.Song, 1, 1, 48, 20]);
            pause(0.1);
            % play song
            fwrite(iRC.com, [iRC.op.PlaySong, 1]);
            pause(0.1);
        end

        function setvel_real(iRC, v, omega)
            % SETVEL Set the robots linear and angular velocity.
			%	Takes two arguments. First the linear velocity in m/s,
			%	second the angular velocity in radian/s. The seperate wheel
			%	speeds will then be set. Wheel speed is limited to 0.5m/s
			%	in either direction.
            
            % find wheel velocities
            wheel = int16([0.5, 0.5; 1/iRC.d, -1/iRC.d] ...
                \ [v; omega] .* 1000);
            if -500 > wheel(1) || wheel(1) > 500 || ...
                    -500 > wheel(2) || wheel(2) > 500
                error('iRC:setvel', ['the speed of each wheel cannot', ...
                    ' exceed 0.5m/s (consider both v and omega)']);
            end
            
            % calculate wheel velocities
            wheel = iRC.twoscompl16(wheel);
            w1h = iRC.highbyte(wheel(1));
            w1l = iRC.lowbyte(wheel(1));
            w2h = iRC.highbyte(wheel(2));
            w2l = iRC.lowbyte(wheel(2));
            % write to serial port
            fwrite(iRC.com, [iRC.op.DriveDirect, w1h, w1l, w2h, w2l]);
        end
        
        function directdrive_real(iRC, v1, v2)
            % DIRECTDRIVE Set wheel velocities directly.
			%	Most likely used for direct driving of the robot.
			
            wheel = int16([v1 * 1000, v2 * 1000]);
            if -500 > wheel(1) || wheel(1) > 500 || ...
                    -500 > wheel(2) || wheel(2) > 500
                error('iRC:directdrive', ['the speed of each wheel', ...
                    'cannot exceed 0.5m/s']);
            end
            
            % calculate wheel velocities
            wheel = iRC.twoscompl16(wheel);
            w1h = iRC.highbyte(wheel(1));
            w1l = iRC.lowbyte(wheel(1));
            w2h = iRC.highbyte(wheel(2));
            w2l = iRC.lowbyte(wheel(2));
            % write to serial port
            fwrite(iRC.com, [iRC.op.DriveDirect, w2h, w2l, w1h, w1l]);
        end
        
        function forward_real(iRC, d)
            % FORWARD Move the robot forward.
			%	Takes one argument, the distance to move in meters. A
			%	negative distance will make the robot move backwards.
			%	Blocks until finished, uses the setvel method.
			
            % resume control in case of pickup
            iRC.resumecontrol();
            % check distance
            if abs(d) > 10
                error('iRC:forward', 'distance is too large');
            end
            % stop the robot
            %fwrite(iRC.com, [iRC.op.Drive, 0, 0, 0, 0]);
            % set v
            v = iRC.twoscompl16(iRC.v * sign(d) * 1000);
            % initialize distance
            dr = 0;
            iRC.distancesensor();
            % drive forward (or backwards)
            fwrite(iRC.com, [iRC.op.Drive, iRC.highbyte(v), iRC.lowbyte(v), 128, 0]);
            % loop checking distance
            while abs(dr) < abs(d)
                dr = dr + iRC.distancesensor();
                if iRC.isbumped(), break; end
            end
            % stop the robot
            fwrite(iRC.com, [iRC.op.Drive, 0, 0, 0, 0]);
        end
        
        function rotate_real(iRC, angle)
            % ROTATE Rotate the robot.
			%	Takes one argument, the angle to turn in radians. A 
			%	positive angle means anticlockwise rotation, a negative
			%	angle means clockwise rotation. The angle should be in the
			%	range of -pi to pi, if it is not, it will be translated to
			%	this range. Blocks until finished, uses the setvel method.
			
            % resume control in case of pickup
            iRC.resumecontrol();
            % shift the angle into -pi to pi
            while pi < angle
                angle = angle - 2 * pi;real
            end
            
            while angle < -pi
                angle = angle + 2 * pi;
            end
            
            % get direction
            if angle < 0
                
                % turn clockwise
                turn = [255, 255];
            else
                
                % turn anticlockwise
                turn = [0, 1];
            end
            
            % set v
            v = iRC.twoscompl16(iRC.vturn * 1000);
            
            % initialize angle
            angler = 0;
            iRC.anglesensor();
            
            % rotate
            fwrite(iRC.com, [iRC.op.Drive, iRC.highbyte(v), iRC.lowbyte(v), turn]);
            
            % loop checking
            while abs(angler) < abs(angle)
                angler = angler + iRC.anglesensor();
                if iRC.isbumped(), break; end
            end
            % stop the robot
            fwrite(iRC.com, [iRC.op.Drive, 0, 0, 0, 0]);
        end
        
        function bool = isbumped_real(iRC)
			% ISBUMPED Checks the front bumper for a bump.
			%	Takes no arguments. Only detects bumper bumps at a given
			%	instant using the sensor.
				
            warning off MATLAB:serial:fread:unsuccessfulRead;
            fwrite(iRC.com, [iRC.op.Sensors, 7]);
            out = fread(iRC.com, 1, 'uint8');
            while isempty(out)
                out = fread(iRC.com, 1, 'uint8');
            end
            if bitand(out, uint8(3))
                bool = 1;
            else
                bool = 0;
            end
            warning on MATLAB:serial:fread:unsuccessfulRead;
        end
        
        function d = distancesensor_real(iRC)
			% DISTANCESENSOR Gets the distance from the distance sensor.
			%	Takes no arguments. The distance sensor is reset every time
			%	a value is read from it.
			
            warning off MATLAB:serial:fread:unsuccessfulRead;
            fwrite(iRC.com, [iRC.op.Sensors, 19]);
            out = fread(iRC.com, 2, 'uint8');
            while isempty(out)
                out = fread(iRC.com, 2, 'uint8');
            end
            d = iRC.mergebytes16(out(1), out(2));
            d = double(iRC.invtwoscompl16(d)) / 1000;
            warning on MATLAB:serial:fread:unsuccessfulRead;
        end
        
        function a = anglesensor_real(iRC)
			% ANGLESENSOR Gets the angle from the angle sensor.
			%	Takes no arguments. The angle sensor is reset every time a
			%	value is read from it.
			
            warning off MATLAB:serial:fread:unsuccessfulRead;
            fwrite(iRC.com, [iRC.op.Sensors, 20]);
            out = fread(iRC.com, 2, 'uint8');
            while isempty(out)
                out = fread(iRC.com, 2, 'uint8');
            end
            a = iRC.mergebytes16(out(1), out(2));
            a = double(iRC.invtwoscompl16(a)) * pi / 180;
            warning on MATLAB:serial:fread:unsuccessfulRead; 
        end
        
        function delete_real(iRC)
            % DELETE Deconstructs the object
            if ~isempty(iRC.com)
                fclose(iRC.com);
            end
        end
    end
 
 	methods (Access = public, Static)
		function n = mergebytes16(hb, lb)
			n = uint16(lb);
			n = n + bitshift(uint16(hb), 8);
		end
		function h = highbyte(word)
			h = bitshift(uint16(word), -8);
		end
		function l = lowbyte(word)
			l = bitand(uint16(word), 255);
		end
		function out = twoscompl16(in)
			in = double(in);
			valid = logical((-32768 <= in) .* (in <= 32767));
			in(~valid) = 0;
			mask = in < 0;
			in(mask) = 65535 + in(mask) + 1;
			out = uint16(in);
		end
		function out = invtwoscompl16(in)
			in = double(in);
			valid = logical((0 <= in) .* (in <= 65535));
			in(~valid) = 0;
			mask = in > 32767;
			in(mask) = in(mask) - 65535 - 1;
			out = int16(in);
		end
	end
end
