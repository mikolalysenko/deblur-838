function [ l_star ] = get_L_star( kernel, image, psi, gamma )

w = size(image, 1);
h = size(image, 2);
s = sqrt(w * h);

%FFT input signals
ks = psf2otf(kernel(:,:,1), [w,h]) / s;
is = fft2(image) / s;
pxs = fft2(psi(:,:,1)) / s;
pys = fft2(psi(:,:,2)) / s;

%Point spread function for that weird operator
f_phase = zeros(w, h);
for d=0:2
for dx=0:d
    dy = d-dx;
    fd = deriv_psf(w, h, dx, dy);
    f_phase = f_phase + weight_func(dx, dy) .* conj(fd) .* fd;
end
end

f_phase = f_phase / s;

%Point spread functions for gradient
f_dx = deriv_psf(w, h, 1, 0) / s;
f_dy = deriv_psf(w, h, 0, 1) / s;

%Taken from Eq. 10
numer = conj(ks) .* is .* f_phase + gamma .* (conj(f_dx) .*  pxs + conj(f_dy) .*  pys);
denom = conj(ks) .* ks .* f_phase + gamma .* (conj(f_dx) .* f_dx + conj(f_dy) .* f_dy); 

l_star = ifft2( s .* (numer ./ denom) );

end
