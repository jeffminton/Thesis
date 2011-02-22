function [segment_masks imgGray] = segmentImage(img, segments)

%declare a conversion format
cform = makecform('srgb2lab');

%get grayscale image of input image
imgGray = rgb2gray(img);
%convert grayscale image to binary image
imgBW = im2bw(imgGray, .5);
%create white image
white = or(imgBW, ~imgBW);

%apply conversion format
imgLAB = applycform(img, cform);
%create array of doubles from last 2 elems of formatted image
ab = double(imgLAB(:,:,2:3));
%get rows and columns in image
nrows = size(ab, 1);
ncols = size(ab, 2);
%reshape image array to have same number of elements but in only 2 columns
%returns a matrix same size as ab, each element represents a
%pixels cluster number
ab = reshape(ab, nrows * ncols, 2);
%run kmeans clustering using ab and segments
[cluster_idx cluster_centers] = kmeansK(ab, segments);
%reshape cluster_idx to have orig rows and cols
pixel_labels = reshape(cluster_idx, nrows, ncols);
%make a cell conataining num of segments
segment_masks = cell(1, segments);

%get a scale to use when coloring segments
scale = 255 / segments;

%color in grayscale image so that each cluster has a different shade
for i = 1 : segments
    imgGray(pixel_labels == i) = i * scale;
end

%create segment masks so that cluster is white and rest is black
for k = 1:segments
    mask = white;
    %set all pixels not part of current cluster to 0
    mask(pixel_labels ~= k) = 0;
    %filter mask to reduce noise
    filt = ones(7,7)/49;
    maskFilt = imfilter(mask, filt);
    segment_masks{k} = maskFilt;
end

end