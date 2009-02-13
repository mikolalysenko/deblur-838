function [ deblurredImgs ] = deconv_rl( corruptedImg, kernel, niters )
%DECONV_RL Richardson-Lucy method for image deconvolution. 
%
% This is an implementation of the Richardson-Lucy method as described in the following
% paper, which describes it in a remarkably simple way:
% Yuan, L., Sun, J., Quan, L., and Shum, H. 2008. Progressive inter-scale and intra-scale
% non-blind image deconvolution. In ACM SIGGRAPH 2008 Papers (Los Angeles, California, 
% August 11 - 15, 2008). SIGGRAPH '08. ACM, New York, NY, 1-10. 
%
% Model:
% corruptedImg = conv(deblurredImg, kernel)   (assumes no noise)
%
%
% INPUT:
% corruptedImg   bad (blurred) image.
% kernel         point spread function/blur kernel.
%
% OUTPUT:
% deblurredImg   I-dimensional cell array of images, where I is the number of iterations.
%                Each cell corresponds to the result after that iteration. 

% adjoint of kernel
adjKernel = kernel(end:-1:1, end:-1:1);

deblurredImgs = cell(niters,1);
deblurredImgs{1} = corruptedImg;
for it=1:niters-1
    
    den  = imfilter(deblurredImgs{it}, adjKernel, 'conv');
    frac = corruptedImg ./ den;
    mult = imfilter(frac, adjKernel, 'conv');
    
    deblurredImgs{it+1} = deblurredImgs{it} .* mult;
end

return;



%{

% The following is a lame (and highly inefficient) attempt at a direct
% implementation of Richardson's paper.

[I,J,nbands] = size(H);
[K,L] = size(S);

W = cell(niters,1);
W{1} = zeros(I,J,nbands);

for it=1:niters
    
    fprintf('Iteration %d / %d', it, niters);
    
    for band=1:nbands
    
        for i=1:I
            
            fprintf('.');
            
            e = min(i+K-1, I);

            for j=1:J

                f = min(j+L-1, J);
                value = 0;
                for m=i:e
                    
                    a = max(1, m-K+1);
                    b = min(m, I);            
                    for n=j:f
                        
                        c = max(1, n-L+1);
                        d = min(n, J);

                        numer = H(m,n,band)*S(m-i+1, n-j+1);
                        denom = 0;
                        for p=a:b
                            for q=c:d
                                if(it == 1)
                                    denom = denom + S(m-p+1, n-q+1);
                                else
                                    denom = denom + W{it-1}(p,q,band)*S(m-p+1, n-q+1);
                                end
                            end
                        end

                        value = value + numer / denom;
                    end
                end        

                if(it == 1)
                    W{it}(i,j,band) = value;
                else
                    W{it}(i,j,band) = W{it-1}(i,j,band)*value; 
                end
            end
        end
    
    end
    
    fprintf('done\n');
end
%}



