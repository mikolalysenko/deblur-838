%This function constructs the kernel for the derivative wrt x,y
function [res] = deriv_psf( w, h, dx, dy )

s = sqrt(w*h);

%Construct derivative maps incrementally
res = psf2otf([1], [w,h]);
for i=1:dy
    res = res .* psf2otf([1,-1], [w,h]);
end
for j=1:dx
    res = res .* psf2otf([1;-1], [w,h]);
end

res = res .* (w*h / sqrt(sum(sum(res .* conj(res)))));

end
