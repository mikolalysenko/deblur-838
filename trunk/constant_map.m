function [ M ] = constant_map( image, psf, threshold )

lo_x = size(psf,1);
hi_x = 0;
lo_y = size(psf, 2);
hi_y = 0;

for i=1:size(psf,1)
for j=1:size(psf,2)
    
    if(psf(i,j) > 0)
        lo_x = min(lo_x, i);
        hi_x = max(hi_x, i);
        lo_y = min(lo_y, j);
        hi_y = max(hi_y, j);
    end
end
end

lo_x = 2* (lo_x - floor(size(psf,1)/2));
hi_x = 2 * (hi_x - floor(size(psf,1)/2));
lo_y = 2* (lo_y - floor(size(psf,2)/2));
hi_y = 2 * (hi_y - floor(size(psf,2)/2));


M = zeros(size(image,1), size(image,2));

for i=1:size(image,1)
for j=1:size(image,2)
    
    %Check if neighborhood around (i,j) is constant
    t = image(max(1,i+lo_x):min(size(image,1),i+hi_x), ...
                      max(1,j+lo_y):min(size(image,2),j+hi_y));
    v = std(reshape(t, size(t,1) * size(t,2), 1));
    
    %Check for threshold
    if(v <= threshold)
        M(i,j) = 1;
    end
end
end


figure, imshow(M)
title('Constant Map');

end
