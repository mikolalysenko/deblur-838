function [ res ] = nifft2( f )

res = ifft2( f .* sqrt(size(f,1) * size(f,2)));

end
