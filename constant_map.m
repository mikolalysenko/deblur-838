function [ M ] = constant_map( image, nx, ny, threshold )

M = zeros(size(image,1), size(image,2));

for i=1:size(image,1)
for j=1:size(image,2)
    
    %Check if neighborhood around (i,j) is constant
    v = std(std(image(max(1,i-nx):min(size(image,1),i+nx), ...
                      max(1,j-nx):min(size(image,2),j+ny))));
    
    %Check for threshold
    if(v <= threshold)
        M(i,j) = 1;
    end
end
end


figure, imshow(M)
title('Constant Map');

end
