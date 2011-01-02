% Show the border of a fairly segmented image
% Input image is RGB given as a file name

function mid = shoBorder(imgFile);
im = imread(imgFile);
figure,imshow(im)

bwf = im2bw(im,35/256);
h = ones(7,7)/49;
for i = 1:5
   bwf = imfilter(bwf,h);
end
bw = ~bwf;
imshow(bw)
cc = bwconncomp(bw,4);
itmData = regionprops(cc,'basic');
[unused, order] = sort([itmData(:).Area],'descend');
newDat = itmData(order);
rc = newDat(1).Centroid;
col = round(rc(2));
row = min(find(bwf(:,col)))

boundary = bwtraceboundary(bwf,[row, col],'E');
figure,imshow(im)
hold on;
plot(boundary(:,2),boundary(:,1),'g','LineWidth',3);
BW_filled = imfill(bw,'holes');
boundaries = bwboundaries(BW_filled);

for k=1:cc.NumObjects
    b = boundaries{k};
    plot(b(:,2),b(:,1),'g','LineWidth',3);
end

end


