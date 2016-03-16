clear all; close all;

%load image and convert to grayscale, increase contrast
I = imread('board_rpi.jpg'); %clean board, board_pieces_rpi2
%I = imread('board_pieces.jpg'); %board with pieces
BW = rgb2gray(I);
BW = imsharpen(BW,'Amount',1);
figure, imshow(BW)

% edge detection
edgeI = edge(BW);
figure, imshow(edgeI)

%improving edge picture
BW2 = bwmorph(edgeI,'thick');
BW3 = bwmorph(BW2,'thick');
BW4 = bwmorph(BW3,'bridge');
BW5 = bwmorph(BW4, 'thin');
BW5 = bwmorph(BW5, 'thin');
BW6 = bwmorph(BW5, 'majority');

figure, imshow(BW6)

%detecting boundaries
B = bwboundaries(edgeI);
figure, imshow(BW)
hold on
visboundaries(B);

%putting the boundaries into a useable array
grid_edges = BW6;
[x,y] = size(BW6);
array = zeros(x,y);
x0 = 0;
y0 = 0;
for i =round(x/2):x
    for j = round(y/2):y
        if grid_edges(j:j+10,i:i+10) == 0
            x0 = i;
            y0 = j;
            break
        end
    end
    if x0 ~=0 && y0 ~= 0
        break
    end
end

for i = 1:y
    if grid_edges(y0,i) == 1
        array(:,i) = 1;
        i = i+20;
    end
end

for j = 1:x
    
    if grid_edges(j,x0) == 1
        array(j,:) = 1;
        j = j+20;
    end
end
figure
imshow(array)