function [segment_masks, imgGray] = doKmeans(img, nColors)

cform = makecform('srgb2lab');

imgGray = rgb2gray(img);
imgBW = im2bw(imgGray, .5);
white = or(imgBW, ~imgBW);

imgLAB = applycform(img, cform);
ab = double(imgLAB(:,:,2:3));
nrows = size(ab, 1);
ncols = size(ab, 2);
ab = reshape(ab, nrows * ncols, 2);
[cluster_idx cluster_centers] = kmeans(ab, nColors);
% [cluster_idx] = litekmeans(ab, nColors);
pixel_labels = reshape(cluster_idx, nrows, ncols);
segment_masks = cell(1, nColors);

scale = 255 / nColors;

for i = 1 : nColors
    imgGray(pixel_labels == i) = i * scale;
end

for k = 1:nColors
    mask = white;
    mask(pixel_labels ~= k) = 0;
    filt = ones(7,7)/49;
    maskFilt = imfilter(mask, filt);
    segment_masks{k} = maskFilt;
end

% for i = 1 : nColors
%     figure, imshow(segmented_images{i}), title(['segment', num2str(i)]);
% end
% mean_cluster_value = mean(cluster_center, 1);
% [tmp, idx] = sort(mean_cluster_value);
% 
% idx

% for i = 1:size(idx, 1)
%     red_cluster_num = idx(i);
%     L = imgLAB(:,:,1);
%     red_idx = find(pixel_labels == red_cluster_num);
%     l_red = L(red_idx);
%     is_red = im2bw(l_red, graythresh(l_red));
%     cup_labels = repmat(uint8(0), [nrows ncols]);
%     cup_labels(red_idx(is_red == false)) = 1;
%     cup_labels = repmat(cup_labels, [1 1 3]);
%     red_cup = img;
%     red_cup(cup_labels ~= 1) = 0;
%     figure, imshow(red_cup);
% end
end