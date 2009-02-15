%Computes the |tri . |_2 operator (whatever the hell it is actually called)

function [ res ] = phase_mag( image )
w = size(image,1);
h = size(image,2);
s = sqrt(w * h);

%Compute f_phase (the triangle operator)
f_phase = zeros(w, h);
for d=0:2
for dx=0:d
    dy = d-dx;
    fd = deriv_psf(w, h, dx, dy);
    f_phase = f_phase + weight_func(dx, dy) .* conj(fd) .* fd;
end
end
f_phase = f_phase ./ s;

%Compute rescaled magnitude
tmp = conj(fft2(image)) .* tmp ./ s;
res = sum(sum(conj(tmp) .* tmp));

end
