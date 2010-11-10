%--------------------------------------------------------% Code to invoke Matlab
clc; % Clear command window.
clear all; % Get rid of variables from prior run of this m-file.
im = imread('cup.png');
imr = im(:,:,1);
imwrite(imr,'cup-in-reds.png');
figure,imshow(imr);
img = im(:,:,2);
imwrite(img,'cup-in-greens.png');
figure,imshow(img);
imb = im(:,:,3);
imwrite(imb,'cup-in-blues.png');
figure,imshow(imb);
imrg = (imr + img)/2;
imwrite(imrg,'cup-in-rg.png');
figure,imshow(imrg);
                                                                     
level = graythresh(img);
bw = im2bw(img,level);
figure,imshow(bw);
imwrite(bw,'bwcupg.png');
h = ones(5,5) / 25;
bwf = imfilter(bw,h);
for i = 1:5
    bwf = imfilter(bwf,h);
end;
figure,imshow(bwf);
imwrite(bwf,'bwfCup.png');

cc = bwconncomp(bwf,4);
disp(cc)

item = false(size(bwf));
for i=1:7
    item(cc.PixelIdxList{i}) = true;
    if i == 2
        imwrite(item,'item2.png')
    end;
    figure,imshow(item);
end;

%labeled = labelmatrix(cc);
%RGB_label = label2rgb(labeled, @copper, 'c', 'shuffle');
%imshow(RGB_label,'InitialMagnification','fit')

itemdata = regionprops(bw,'basic');
for i = 1:7
    disp(itemdata(i));
end;    
 