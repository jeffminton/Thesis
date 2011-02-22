function [segment_masks, cluster_idx] = doKmeans(img)

cform = makecform('srgb2lab');

[y, x, z] = size(img);
white = ones(y, x);
imshow(white);

imgLAB = applycform(img, cform);
ab = double(imgLAB(:,:,2:3));
nrows = size(ab, 1);
ncols = size(ab, 2);
ab = reshape(ab, nrows * ncols, 2);
nColors = 5;
% [cluster_idx] = kmeans(ab, nColors, 'distance','sqEuclidean', 'Replicates', 2);
[cluster_idx] = litekmeans(ab, nColors);
size(cluster_idx)
pixel_labels = reshape(cluster_idx, nrows, ncols);
segment_masks = cell(1, nColors);

for k = 1:nColors
    mask = white;
    mask(pixel_labels ~= k) = 0;
    filt = ones(7,7)/49;
    maskFilt = imfilter(mask, filt);
    for i = 1:5
        maskFilt = imfilter(maskFilt,filt);
    end
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