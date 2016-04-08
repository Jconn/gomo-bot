#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>
#include "MoveType.h"
#include "Gomoku.h"
#include <stdio.h>
using namespace cv;
using namespace std;


int UI(string filename, Coordinate coords, spot (&board)[GRID_LENGTH][GRID_LENGTH])
{
    
    
    Mat src;
    if( !(src=imread(filename, 0)).data)
        return 0;
    
    Point2f pt(0,0);
    //get board locations from grid
    
    int i = coords.x;
    int j = coords.y;
    pt = board[i][j].loc;
    
    circle(src, Point(pt.x, pt.y), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
    imshow("Image", src);
    waitKey(0);
    return 0;
    
}
