function [ l_star ] = get_L_star( kernel, image, psi, gamma )

%FFT input signals
ks = fft2(kernel);
is = fft2(image);
pxs = fft2(psi(:,1));
pys = fft2(psi(:,2));

%Point spread function for that weird operator
f_phase = zeros(w, h);
for d=0:2
for dx=0:d
for dy=0:(d-dx)
    fd = deriv_psf(w, h, dx, dy);
    f_phase = f_phase + weight_func(dx, dy) * conj(fd) * fd;
end
end

%Point spread functions for gradient
f_dx = deriv_psf(size(image,1), size(image,2), dx, dy));
f_dy = deriv_psf(size(image,1), size(image,2), dx, dy));

%Taken from Eq. 10
l_star = ifft2( ...
    (conj(ks) .* is .* f_phase + gamma * (conj(f_dx) .* pxs + conj(f_dy) .* pys)) ...
    ./ (conj(ks) .* ks .* f_phase + gamma * (conj(f_dx) .* f_dx + conj(f_dy) .* f_dy)));

end
