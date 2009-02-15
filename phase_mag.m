%Computes the |tri . |_2 operator (whatever the hell it is actually called)

function [ res ] = phase_mag( image )
w = size(image,1);
h = size(image,2);

tmp = zeros(w, h);
for dx=0:2
for dy=0:2
    D = conj(fft2(image)) .* deriv_psf(w, h, dx, dy);
    tmp = tmp + weight_func(dx, dy) .* conj(D) .* D;
end
end

tmp = ifft2(tmp);
res = sum(sum(conj(tmp) .* tmp));
end
