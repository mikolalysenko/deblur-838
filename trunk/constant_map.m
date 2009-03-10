function [ M ] = constant_map( image, kw, kh, threshold )

lo_x = -ceil(kw/2);
hi_x = ceil(kw/2);
lo_y = -ceil(kw/2);
hi_y = ceil(kw/2);

M = zeros(size(image,1), size(image,2), 3);

for d=1:3
for i=1:size(image,1)
for j=1:size(image,2)
    
    %Check if neighborhood around (i,j) is constant
    t = image(max(1,i+lo_x):min(size(image,1),i+hi_x), ...
                      max(1,j+lo_y):min(size(image,2),j+hi_y),d);
    v = std(reshape(t, size(t,1) * size(t,2), 1));
    
    %Check for threshold
    if(v <= threshold)
        M(i,j,d) = 1;
    end
end
end
end

end
