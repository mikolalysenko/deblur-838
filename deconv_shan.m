%lambda1 is a tweak parameter in 0.002-0.5
%lambda2 is a tweak parameter in 10-25
%gamma is the convergence factor

function [ L ] = deconv_shan( image, psf, M, lambda1, lambda2 )

%Dimensions
w = size(image,1);
h = size(image,2);

%Taper edges
image = edgetaper(image, psf);
L = image;

%Place gamma
gamma = 2.0;

for i=1:15

    %Per component solve for L_star
    for d=1:3
        psi = estimate_psi(image(:,:,d), L(:,:,d), M(:,:,d), lambda1, lambda2, gamma);
        L(:,:,d) = get_L_star( psf, image(:,:,d), psi, gamma);
    end
    
    %Scale tweak variables
    gamma   = gamma * 2.0;
    lambda1 = lambda1 / 1.5;
    lambda2 = lambda2 / 1.1;
end

L = 2.0 .* L;

end
