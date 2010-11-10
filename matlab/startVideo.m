function [vid] = startVideo()
    vid = videoinput('winvideo', 1);
    triggerconfig(vid, 'manual');
    start(vid);
end