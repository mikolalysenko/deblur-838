
close all;

gndTruthImg = imread('lichtenstein.png');
gndTruthImg = im2double(gndTruthImg(50:149, 50:149, :));

% the blur kernel
kernel = fspecial('gaussian', 7,7);

blurredImg = imfilter(gndTruthImg, kernel, 'conv', 'full');


figure, imshow(gndTruthImg);
title('Ground Truth Image');

figure, imshow(blurredImg);
title('Blurred Image');

niters = 50;
[ W ] = deconv_rl( blurredImg, kernel, niters );


for it=1:5:length(W)
    img = W{it};
    
    figure, imshow(img);
    title(sprintf('Deblurred Result, Iteration %d', it));
end
