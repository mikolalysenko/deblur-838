%Computes the |tri . |_2 operator (whatever the hell it is actually called)

function [ res ] = phase_mag( image )

%Dimensions
w = size(image,1);
h = size(image,2);

%Point spread function for that weird operator
f_phase = zeros(w, h);
for d=0:2
for dx=0:d
    dy = d-dx;
    fd = deriv_psf(w, h, dx, dy);
    f_phase = f_phase + weight_func(dx, dy) .* conj(fd) .* fd;
end
end

%Compute rescaled magnitude
tmp = ifft2(fft2(image) .* f_phase);
res = sqrt(sum(sum(tmp .^ 2)) / (w * h));


end
