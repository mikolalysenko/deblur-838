
close all;

%Read image / kernel pair
blurredImg = im2double(imread('test4.bmp'));
kernel = im2double(imread('kernel4.bmp'));

figure, imshow(blurredImg);
title('Blurred Image');

figure, imshow(kernel);
title('Kernel');

img = deconv_shan( blurredImg, ...
        kernel(:,:,1), ...
        constant_map(blurredImg, size(kernel,1), size(kernel,2), 5.0/255.0), ...
        0.016, 40 );

figure, imshow(img);
title('Deblurred Result');
