function [ psi ] = estimate_psi( image, L, M, lambda1, lambda2, gamma )

%Constants for edge distribution stats
K = 2.7;
a = 6.1e-4;
b = 5.0;
lt = 2.5;        %This may not be the right number...

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
ftI = nfft2(image);
dI = zeros(w, h, 2);
dI(:,:,1) = nifft2(dX .* ftI);
dI(:,:,2) = nifft2(dY .* ftI);

%Compute L gradient
ftL = nfft2(L);
dL = zeros(w, h, 2);
dL(:,:,1) = nifft2(dX .* ftL);
dL(:,:,2) = nifft2(dY .* ftL);

%Solve for gradients
for i=1:size(psi,1)
for j=1:size(psi,2)
for d=1:2
    
    %Optimize psi(i,j) in direction d
   
    %For now, do this by brute force search (can actually be done in
    % constant time by taking derivatives)

    best_v = 1.0e32;
    best_x = 0;
    
    cI = lambda2 * M(i,j);
    cL = gamma;
    
    best_x = (cI * dI(i,j,d) + cL * dL(i,j,d)) / (cI + cL);
    
    psi(i,j,d) = best_x;
end
end
end


end
