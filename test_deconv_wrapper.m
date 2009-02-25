
img = 'release/input_blurry.ppm';
kernel = fspecial('gaussian', 7,7);
options.method = 'RL';
options.numIters = 30;
options.regFactor = 0.2;

outputImg = deconv_wrapper( img, kernel, options );

figure, imshow(outputImg);