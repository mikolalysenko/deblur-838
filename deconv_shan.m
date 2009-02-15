%lambda1 is a tweak parameter in 0.002-0.5
%lambda2 is a tweak parameter in 10-25
%gamma is the convergence factor

function [ L ] = deconv_shan( image, psf, lambda1, lambda2, gamma )

%Dimensions
w = size(image,1);
h = size(image,2);

%Grayscale image
g_image = rgb2gray(image);

%Construct constant map
M = constant_map(g_image, 10, 10, 0.0025);

%Initialize local vars
L = image;

while true    
    
    %Estimate psi
    psi = estimate_psi(g_image, rgb2gray(L), M, lambda1, lambda2, gamma);
    
    %Per component solve for L_star
    for d=1:3
        L(:,:,d) = get_L_star( psf, image(:,:,d), psi, gamma);
    end
    
    %Display L
    figure, imshow(L)
    title('L');
    
    %If converged, then done
    if( phase_mag(rgb2gray(L)) <= 1e-5 && ...
        phase_mag(psi(:,:,1)) <= 1e-5 && ...
        phase_mag(psi(:,:,2)) <= 1e-5 )
        break;
    end
        
    %Scale tweak variables
    gamma = 2.0 * gamma;
    lambda1 = lambda1 / 1.1;
    lambda2 = lambda2 / 1.5;
end

end
