% Assume your videoinput object has already been created and otherwise configured.
triggerconfig(vid, 'manual');
start(vid);

% Data is now being sent from the device, but is not logged to memory.

for ii = 1:1000
   curFrame = getsnapshot(vid);
   myProcessing(curFrame);
end

stop(vid)