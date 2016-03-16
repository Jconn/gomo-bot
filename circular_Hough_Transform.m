
A = imread('white_pieces.png');

%Converts the image to grayscale
im = double(A);
im = 0.299*im(:,:,1) + 0.587*im(:,:,2) + 0.114*im(:,:,3);
im = uint8(im);

%Converts the image to black and white
level = .7;
im2bw(im, level);
bwImg = false(size(im));
bwImg(im == 117) = true;


imshow(bwImg)

Rmin = 35;
Rmax = 65;

[centersBright, radiiBright] = imfindcircles(im,[Rmin Rmax],'ObjectPolarity','bright');
[centersDark, radiiDark] = imfindcircles(im,[Rmin Rmax],'ObjectPolarity','dark');
viscircles(centersBright, radiiBright,'EdgeColor','b');

viscircles(centersDark, radiiDark,'LineStyle','--');