function [edges sig sigImg rgb_segment_mask] = findObjects(img, segments)

%copy input image
imgBak = img;

%set segments to find
%segments = 5;

%write the original image to disk
imwrite(img, 'imgout/segOrig.jpg');

%segment the image
[segment_masks gray] = segmentImage(img, segments);

%write the grayscale segmented image to disk
imwrite(gray, 'imgout\segGray.jpg');

%initialize the edges struct
edges = segment_masks;

sigImg = cell(1, size(segment_masks, 2));
sig = cell(1, size(segment_masks, 2));

%perform task on each segment mask returned
for i = 1:size(segment_masks, 2);
    %copy image to segImg for masking
    segImg = imgBak;
    %turn the segment_mask at i into a 3 dimensional matrix
    rgb_segment_mask = repmat(segment_masks{i}, [1, 1, 3]);
    %create signature img
    sig{i} = imgBak(rgb_segment_mask ~= 0);
    pixCount = size(sig{i}, 1) / 3;
    %create signature array
    sigImg{i} = zeros(1, pixCount, 3);
    start = 1;
    sigImg{i}(1, :, 1) = sig{i}(start:pixCount);
    start = start + pixCount;
    sigImg{i}(1, :, 2) = sig{i}(start:pixCount * 2);
    start = start + pixCount;
    sigImg{i}(1, :, 3) = sig{i}(start:pixCount * 3);
    sigImg{i} = uint8(sigImg{i});
    %write segmented color file to disk
    imwrite(segImg, ['imgout\segColor', num2str(i), '.jpg']);
    %perform edge detection on segment mask i
    edges{i} = edge(segment_masks{i});
    %write edge image to disk
    imwrite(edges{i}, ['imgout\edges', num2str(i), '.jpg']);
    %write mask to disk
    imwrite(segment_masks{i}, ['imgout\segMasks', num2str(i), '.jpg']);
end
