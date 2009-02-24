function [ psi ] = estimate_psi( image, L, M, lambda1, lambda2, gamma )

%Constants for edge distribution stats
global K a b lt vI vL cI cL best_x best_c;

K = lambda1 * 2.7 / 255.0;
a = lambda1 * 6.1e-4 / (255.0^2);
b = lambda1 * 5.0;
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
dI(:,:,1) = ifft2(dX .* ftI);
dI(:,:,2) = ifft2(dY .* ftI);

%Compute L gradient
ftL = fft2(L);
dL = zeros(w, h, 2);
dL(:,:,1) = ifft2(dX .* ftL);
dL(:,:,2) = ifft2(dY .* ftL);

%Solve for gradients
for i=1:size(psi,1)
for j=1:size(psi,2)
for d=1:2
    
    %Optimize psi(i,j) in direction d
   
    %For now, do this by brute force search (can actually be done in
    % constant time by taking derivatives)

    vI = dI(i,j,d);
    vL = dL(i,j,d);
    cI = lambda2 * M(i,j) / (255.0^2);
    cL = gamma;

    best_x = 0;
    best_c = 1e32;
    
    eval_x(0);
    eval_x(lt);
    eval_x(-lt);
    eval_x( (cI * vI + cL * vL) / (cI + cL + a) );
    eval_x( (cI * vI + cL * vL + K/2) / (cI + cL) );
    eval_x( (cI * vI + cL * vL - K/2) / (cI + cL) );    
    
    psi(i,j,d) = best_x;
end
end
end

clear global K a b lt vI vL cI cL best_x best_c;

end

function [cost] = eval_x(x)

    global K a b lt vI vL cI cL best_x best_c;

    cost = cI * (x - vI)^2 + cL * (x - vL)^2;

    if(abs(x) < lt)
        cost = cost + abs(x) * K;
    else
        cost = cost + (a * x^2 + b);
    end

    if(cost < best_c)
        best_c = cost;
        best_x = x;
    end
end
