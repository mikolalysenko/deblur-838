%Weight function (taken from paper)
function [ res ] = weight_func( dx, dy )

if(dx == 0 && dy == 0)
    res = 1;
else
    res = 50.0 / 2.0^(dx+dy);
end

end
