%Computes the |tri . |_2 operator (whatever the hell it is actually called)

function [ res ] = phase_mag( image )
tmp = zeros(size(image,1), size(image,2));
for dx=0:2
for dy=0:2
    D = fft2(diff(diff(image, dx, 1), dy, 2));
    tmp = tmp + weight_func(dx, dy) .* conj(D) .* D;
end
tmp = ifft2(tmp);
res = sum(sum(conj(tmp) .* tmp));
end
