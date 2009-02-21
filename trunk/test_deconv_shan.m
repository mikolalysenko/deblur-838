
close all;

%Read image / kernel pair
blurredImg = im2double(imread('test4.bmp'));
kernel = im2double(imread('kernel4.bmp'));

figure, imshow(blurredImg);
title('Blurred Image');

figure, imshow(kernel);
title('Kernel');

img = deconv_shan( blurredImg, kernel, 0.0016, 140, 2.0 );

figure, imshow(img);
title('Deblurred Result');
