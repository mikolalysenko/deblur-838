function [ psi ] = estimate_psi( image, L, M, lambda1, lambda2, gamma )

%Constants for edge distribution stats
lambda = lambda1 / 255.0;
K = 2.7 * 255.0;
a = 6.1e-4 * 255.0;
b = 5.0;
lt = (K - sqrt(K^2 - 4.0 * a * b)) / (2.0 * a);

%Dimensions
w = size(image, 1);
h = size(image, 2);
s = sqrt(w * h);

%Construct results
psi = zeros(w, h, 2);

%Construct gradient kernel
dX = deriv_psf(w, h, 1, 0);
dY = deriv_psf(w, h, 0, 1);

%Compute image gradient
ftI = fft2(image);
dI = zeros(w, h, 2);
dI(:,:,1) = ifft2(ftI .* dX);
dI(:,:,2) = ifft2(ftI .* dY);

%Compute L gradient
ftL = fft2(L);
dL = zeros(w, h, 2);
dL(:,:,1) = ifft2(ftL .* dX);
dL(:,:,2) = ifft2(ftL .* dY);

%Construct coefficients
cI = zeros(w,h,2);
cI(:,:,1) = M .* lambda2;
cI(:,:,2) = M .* lambda2;

x_coord = zeros(w, h, 2, 6);
x_coord(:,:,:,2) = lt;
x_coord(:,:,:,3) = -lt;
x_coord(:,:,:,4) = (cI .* dI + gamma .* dL) ./ (cI + (gamma + lambda * a));
x_coord(:,:,:,5) = (cI .* dI + gamma .* dL + lambda * K / 2) ./ (cI + gamma);
x_coord(:,:,:,6) = (cI .* dI + gamma .* dL - lambda * K / 2) ./ (cI + gamma);

best_val = zeros(w,h,2,5);

for i=1:6
    x = x_coord(:,:,:,i);
    best_val(:,:,:,i) = cI .* (x - dI).^2 + gamma * (x - dL).^2 + lambda * min(abs(x) * K, a * x.^2 + b);
end

%Solve for gradients
for i=1:size(psi,1)
for j=1:size(psi,2)
for d=1:2
    
    best_c = best_val(i,j,d,1);
    best_x = 0;

    for v=1:6
        if(best_val(i,j,d,v) < best_c)
            best_c = best_val(i,j,d,v);
            best_x = x_coord(i,j,d,v);
        end
    end
    
    psi(i,j,d) = best_x;
end
end
end

end

%{
function [cost] = eval_x(x)

    global K a b lt vI vL cI cL best_x best_c lambda;

    cost = cI * (x - vI)^2 + cL * (x - vL)^2;

    if(abs(x) < lt)
        cost = cost + lambda * abs(x) * K;
    else
        cost = cost + lambda * (a * x^2 + b);
    end
    
    disp(x)
    disp(cost)

    if(cost < best_c)
        best_c = cost;
        best_x = x;
    end
end
%}