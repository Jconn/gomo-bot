clear all; close all;

%load image and convert to grayscale, increase contrast
I = imread('actual_board2.jpg'); %clear board
%I = imread('board_pieces.jpg'); %board with pieces
BW = rgb2gray(I);
BW = imsharpen(BW,'Amount',1);
figure, imshow(BW)

% edge detection
edgeI = edge(BW);
figure, imshow(edgeI)

%improving edge picture
BW2 = bwmorph(edgeI,'spur');
figure, imshow(BW2)

% detecting boundaries
B = bwboundaries(edgeI);
figure, imshow(BW)
hold on
visboundaries(B);