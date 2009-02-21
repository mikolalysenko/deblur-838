function [res] = nfft2( f )

res = fft2(f) ./ sqrt(size(f,1) * size(f,2));

end
