%lambda1 is a tweak parameter in 0.002-0.5
%lambda2 is a tweak parameter in 10-25
%gamma is the convergence factor

function [ L ] = deconv_shan( image, psf, lambda1, lambda2, gamma )

w = size(image,1);
h = size(image,2);

%Construct constant map
M = zeros(w, h, 3);
for d=1:3
    M(:,:,d) = constant_map(image(:,:,d), 10, 10, 0.005);
end

%Initialize local vars
L = image;

while true    
    n_good = 0;
    for d=1:3
        %Estimate psi
        psi = estimate_psi(image(:,:,d), L(:,:,d), M(:,:,d), lambda1, lambda2, gamma);

        %Estimate L
        L(:,:,d) = get_L_star( psf, image(:,:,d), psi, gamma);

        %Check convergence
        if( phase_mag(L(:,:,d)) <= 1e-5 && ...
            phase_mag(psi(:,:,1)) <= 1e-5 && ...
            phase_mag(psi(:,:,2)) <= 1e-5 )
        
            n_good = n_good + 1;
        end
    end
    
    figure, imshow(psi(:,:,1))
    title('Psi-x');

    figure, imshow(psi(:,:,2))
    title('Psi-y');
    
    figure, imshow(L)
    title('L');
    
    %If all channels converged, then done
    if(n_good == 3)
        break;
    end
        
    %Scale tweak variables
    gamma = 2.0 * gamma;
    lambda1 = lambda1 / 1.1;
    lambda2 = lambda2 / 1.5;
end

end
