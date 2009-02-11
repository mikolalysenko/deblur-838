%This function constructs the kernel for the derivative wrt x,y
function [res] = deriv_psf( w, h, dx, dy )

%Construct derivative maps incrementally
arr = [1];
for i=1:dx
    arr = conv2(arr, [1,-1]);
end
for j=1:dy
    arr = conv2(arr, [1;-1]);
end

%Done
res = psf2otf(arr, [w,h]);

end
