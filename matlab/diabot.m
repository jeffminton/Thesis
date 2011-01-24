function [ output_args ] = diabot( input_args )
	% diabot version 0.0
	%   funcction used for controlling a dialog based robot

	%function forward(d)
	%move robot forward d meters
	%function is part of iRobotCreate

	%function rotate(r)
	%rotate the robot r radians
	%function part of iRobotCreate

    function knowledge = getKnowledge()
        knowledge = {};
        
        knowledge 
    end

    function commands = splitComms(comStr)
        commands = textscan(comStr, '%s');
    end

	function funcOut = interperateCommand(command)
	
		if(strcmp(command, 'find'))
			funcOut = find;
		elseif(strcmp(command, 'describe'))
			funcOut = describe;
		elseif(strcmp(command, 'classify'))
			funcOut = classify;
		end
		    
	end

	function find(obj)
	end
		
	function describe()
	end
		
	function classify(obj)
	end
end
