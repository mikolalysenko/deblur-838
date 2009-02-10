function [ psi ] = estimate_psi( image, L, M, lambda1, lambda2, gamma )

%Constants for edge distribution stats
K = 2.7;
a = 6.1e-4;
b = 5.0;

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
    
    
    
end
end
end


end
