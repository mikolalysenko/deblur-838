%lambda1 is a tweak parameter in 0.002-0.5
%lambda2 is a tweak parameter in 10-25
%gamma is the convergence factor

function [ L ] = deconv_shan( image, psf, lambda1, lambda2 )

%Dimensions
w = size(image,1);
h = size(image,2);

gamma = 2.0 / (255.0^2);

%Construct constant map
M = zeros(w, h, 3);
for i=1:3
    M(:,:,i) = constant_map(image(:,:,i), psf, 5.0/255.0);
end

%Initialize local vars
L = image;

for i=1:15
    
    ngood = 0;
    
    %Per component solve for L_star
    for d=1:3
        psi = estimate_psi(image(:,:,d), L(:,:,d), M(:,:,d), lambda1, lambda2, gamma);
        L(:,:,d) = get_L_star( psf, image(:,:,d), psi, gamma);

        %If converged, then done
        if( phase_mag(L(:,:,d)) <= 1e-5 && ...
            phase_mag(psi(:,:,1)) <= 1e-5 && ...
            phase_mag(psi(:,:,2)) <= 1e-5 )
            ngood = ngood + 1;
        end

    end
    
    
    figure,imshow(L);
    title('L');
    
    if(ngood == 3)
        break;
    end
       
    %Scale tweak variables
    gamma   = gamma * 2.0;
    lambda1 = lambda1 / 1.1;
    lambda2 = lambda2 / 1.5;
end

end