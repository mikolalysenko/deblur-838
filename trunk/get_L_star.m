function [ l_star ] = get_L_star( kernel, image, psi, gamma )

w = size(image, 1);
h = size(image, 2);
s = sqrt(w * h);

%FFT input signals
ks = psf2otf(kernel, [w,h]);
is = fft2(image);
pxs = fft2(psi(:,:,1));
pys = fft2(psi(:,:,2));

%Correct the scaling on the kernel
scale = s / sqrt(sum(sum(conj(ks) .* ks)));
ks = ks .* scale^2;

%Point spread function for that weird operator
f_phase = zeros(w, h);
for d=0:2
for dx=0:d
    dy = d-dx;
    fd = deriv_psf(w, h, dx, dy);
    f_phase = f_phase + (50 / 2^d) * conj(fd) .* fd;
end
end

%Point spread functions for gradient
f_dx = deriv_psf(w, h, 1, 0);
f_dy = deriv_psf(w, h, 0, 1);

%Taken from Eq. 10
numer = (conj(ks) .* is .* f_phase) + gamma .* (conj(f_dx) .*  pxs + conj(f_dy) .*  pys);
denom = (conj(ks) .* ks .* f_phase) + gamma .* (conj(f_dx) .* f_dx + conj(f_dy) .* f_dy); 

l_star = ifft2( (numer ./ denom) );

end