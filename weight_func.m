%Weight function (taken from paper)
function [ res ] = weight_func( dx, dy )

res = 50.0 / 2.0^(dx+dy);

end
