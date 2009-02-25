function [ output ] = kernel_exporter( kernel, filename )
%KERNEL_EXPORTER Save a kernel image as a textfile in the following format:
% 
% height
% width
% (y1, x1) value
% (y2, x1) value
% (y3, x1) value
% ...
% (yh, x1) value
% (y1, x2) value
% ...
% (yh, xw) value

[height, width, nbands] = size(kernel);

output = zeros(height*width+2, 1);

output(1) = height;
output(2) = width;

c = 3;
for x=1:width
    for y=1:height
        output(c) = kernel(y,x);
        c = c + 1;
    end
end

fid = fopen(filename, 'w+');

fprintf(fid, '%d\n%d\n', height, width);
for i=3:length(output)
    fprintf(fid, '%.10f\n', output(i));
end

fclose(fid);

