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
dX = deriv_psf(w, h, 1, 0) ./ s;
dY = deriv_psf(w, h, 0, 1) ./ s;


%Compute image gradient
ftI = fft2(image) ./ s;
dI = zeros(w, h, 2);
dI(:,:,1) = ifft2(s .* conj(dX) .* ftI);
dI(:,:,2) = ifft2(s .* conj(dY) .* ftI);
dI = dI * 256;

%Compute L gradient
ftL = fft2(L) ./ s;
dL = zeros(w, h, 2);
dL(:,:,1) = ifft2(s .* conj(dX) .* ftL);
dL(:,:,2) = ifft2(s .* conj(dY) .* ftL);
dL = dL * 256;


%Solve for gradients
for i=1:size(psi,1)
for j=1:size(psi,2)
for d=1:2
    
    %Optimize psi(i,j) in direction d
   
    %For now, do this by brute force search (can actually be done in
    % constant time by taking derivatives)

    best_v = 1.0e32;
    best_x = 0;
    
    for x=-256:256
        
        v = lambda2 * M(i,j) * (x - dI(i,j,d))^2 + ...
            gamma * (x - dL(i,j,d))^2;
        
        if abs(x) <= lt
            v = v + lambda1 * K * abs(x);
        else
            v = v + lambda1 * (a * x^2 + b);
        end
        
        if(v < best_v)
            best_v = v;
            best_x = x;
        end
    end
    
    psi(i,j,d) = best_x;
end
end
end

%Rescale
psi = psi ./ 512.0;

end
