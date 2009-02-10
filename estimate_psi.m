function [ psi ] = estimate_psi( image, L, M, gamma, lambda1, lambda2 )

%Constants for edge distribution stats
K = 2.7;
a = 6.1e-4;
b = 5.0;
lt = 10;        %This may not be the right number...

%Construct results
psi = zeros(size(image,1), size(image,2), 2);

%Construct gradient maps
dI = zeros(size(image,1), size(image,2), 2);
dI(:,:,1) = diff(image);
dI(:,:,2) = diff(image,1,2);

dL = zeros(size(L,1), size(L,2), 2);
dL(:,:,1) = diff(image);
dL(:,:,2) = diff(image,1,2);

%Solve for gradients
for i=1:size(psi,1)
for j=1:size(psi,2)
for d=1:2
    
    %Optimize psi(i,j) in direction d
   
    %For now, do this by brute force search (can actually be done in
    % constant time by taking derivatives)

    best_v = 1e32;
    best_x = 0;
    
    for x=-256:256
        
        v = lambda_2 * M(i,j) * (x - dI(i,j,d))^2 + ...
            gamma * (x - dL(i,j,d)^2;
        
        if abs(x) <= lt
            v = v + k * abs(x);
        else
            v = v + (a * x^2 + b);
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


end
