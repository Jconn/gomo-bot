//
//  grid_with_centroid.cpp
//
//
//  Created by Meredith Ostrowski on 3/30/16.
//
//
#include "grid_with_centroid.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

int MAX_DIFF_Y = 130;
int MAX_DIFF_X = 20;
static const Size finalSize(1024, 860);

struct compositeCircle{
    Vec3i circle;
    int numCombines;
};

struct comp_x{
    bool operator()(const Point2f &a, const Point2f &b)
    { return (a.x < b.x); }
};

static void help()
{
    cout << "\nThis program demonstrates line finding with the Hough transform.\n"
    "Usage:\n"
    "./hough_lines <image_name>, Default is ../data/board.jpg\n" << endl;
}

//assume all points on grid have been extrapolated
void calc_Position(vector<Point2f>corners, vector<Point2f>centroids){
    
    cout << corners.at(0) << endl;
    cout << centroids.at(0) << endl;
    
    float x1, x2, y1, y2, dist;
    float min = 10000;
    for(int i = 0; i < corners.size(); ++i){
        for (int i = 0; i < centroids.size(); ++i){
            x1 = corners.at(i).x;
            y1 = corners.at(i).y;
            x2 = centroids.at(i).x;
            y2 = centroids.at(i).y;
            
            dist = sqrt(pow((x1-x2), 2) + pow((y1-y2), 2));
            
            if(dist < min){
                min = dist;
            }
        }
        cout << min << endl;
    }
    
    return;
}

int main(int argc, char** argv)
{
    
    cv::CommandLineParser parser(argc, argv,
                                 "{help h ||}{@image|../data/board.jpg|}"
                                 );
    if (parser.has("help"))
    {
        help();
        return 0;
        cout << "help" << endl;
    }
    
    
    string img1 = argv[1];//CHANGE: both lines
    string img2 = argv[2];
    cout << img1 << " " << img2 << endl;
    
    if (img1.empty() || img2.empty()) //CHANGE: both
    {
        help();
        cout << "no image_name provided" << endl;
        return -1;
    }
    
    Mat src, dst, dst2, color_dst, mask, tmp, sharp_im;
    if( argc != 3 || !(src=imread(argv[1], 0)).data) //CHANGE: 2 to 3
        return 0;
    
    cvtColor(dst,color_dst,CV_GRAY2BGR);
    
    //display original image
    cout << "Before resize" << endl;
    resize(src,tmp,finalSize);
    cout << "After resize" << endl;
    imshow("Original Image",tmp);
    
    //blur and sharpen image
    medianBlur(src,src, 11);
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(2);
    clahe->apply(src,src);
    src.convertTo(src,-1,1.5,-100);
    
    sharp_im =src;
    

    /*
     //display sharpened image
     namedWindow("Sharpened Image", 1);
     resize(sharp_im,sharp_im,finalSize);
     imshow("Sharpened Image", sharp_im);
     */
    //corner detection
    vector<Point2f>corners;
    vector<Point2f>corners_x;
    vector<Point2f>corners_y;
    goodFeaturesToTrack(src,corners,370, 0.1, 50, mask, 18, 10, 0.1);
    
    for(size_t i = 0; i < corners.size(); i++)
    {
        circle(src, corners[i], 10, Scalar(0.), -1);
    }
    
    //display detected corners
    namedWindow("Corners",1);
    resize(src, src, finalSize);
    imshow("Corners", src);
    
    // putting corners into useful format
    
    //cout<< corners << endl;
    //vector<Point2f>sorted_corners;
    //sorted_corners = corners;
    
    
    comp_x comp_ab;
    std::sort(corners.begin(), corners.end(), comp_ab);
    
    corners_x = corners;
    //cout << corners << endl;
    
    std::sort(corners.begin(), corners.end(), comp_ab);
    
    corners_y = corners;
    //cout << corners << " ";
    
    
    vector<double> sum_coord;
    for(size_t i = 0; i<corners_x.size(); i ++){
        sum_coord.push_back(corners_x[i].x + corners_x[i].y);
    }
    
    double min = 10000000;
    int index = 0;
    for(size_t i = 0; i < corners_x.size(); i ++){
        if(sum_coord[i] < min){
            min = sum_coord[i];
            index = i;
        }
    }
    
    Mat_<Point2f> grid(19,19, Point2f(0,0)) ;
    //Point2f starting_pt = corners_x[index];
    Point2f prev(0,0) ;
    prev.x = corners_x[index].x;
    prev.y = corners_x[index].y;
    Point2f prev2(0,0);
    vector<Point2f> final_points;
    Point2f extrap(0,0);
    Point2f last_point(9999,9999);
    corners_x.push_back(last_point);
    //grid.at<Point2f>(0,0) = prev;
    //final_points.push_back(prev);
    int col = 0;
    int row = 0;
    for(size_t i = 0; i < corners_x.size(); i ++){
        if((corners_x[i].x - prev.x) < MAX_DIFF_X){
            final_points.push_back(corners_x[i]);
            prev.x = corners_x[i].x;
            prev.y = corners_x[i].y;
            cout << "Pushed Back Point: " << corners_x[i] << endl;
        }
        else{
            cout << "Point not pushed: " << corners_x[i] << endl;
            prev.x = corners_x[i].x;
            prev.y = corners_x[i].y;
            //col = col + 1;
            //row = 0;
            
            std::sort(final_points.begin(), final_points.end(), comp_ab);
            prev2.x = final_points[0].x;
            prev2.y = final_points[0].y;
            for(size_t j = 0; j < final_points.size(); j++){
                cout << "Current Point: " << final_points[j] << "   Prev Point: " << prev2 << endl;
                if((final_points[j].y - prev2.y) < MAX_DIFF_Y){
                    if(prev2.y < final_points[j].y){
                        prev2.x = final_points[j].x;
                        prev2.y = final_points[j].y;
                    }
                    cout << "Point is in the right order: " << final_points[j] << endl;
                    grid.at<Point2f>(col,row) = final_points[j];
                    //final_points.push_back(corners_x[i]);
                    row = row + 1;
                }
                else{
                    extrap.x = prev2.x;
                    extrap.y = prev2.y + 100;
                    //row = row + 1;
                    prev2.x = extrap.x;
                    prev2.y = extrap.y;
                    j = j -1;
                    grid.at<Point2f>(col,row) = prev2;
                    row = row + 1;
                    cout << "Extrapolated point: " << extrap << endl;
                    final_points.push_back(prev2);
                }
                /*
                 for(size_t i = 0; i < final_points.size(); i++){
                 grid.at<Point2f>(col,row) = final_points[i];
                 row = row + 1;
                 }*/
            }
            
            col = col + 1;
            row = 0;
            final_points.clear();
            final_points.push_back(prev);
            //grid.at<Point2f>(col,row) = prev;
        }
    }
    
    
    for(int i = 0; i < grid.rows; i++){
        for(int j = 0; j<grid.cols; j++){
            cout << "Column " << i << " Row " << j << " " << grid.at<Point2f>(i,j) << endl;
        }
        
    }
    cout << "Min " << min << " at index " << index << endl;
    cout << "starting point: " << corners_x[index] << endl;
    
    
    //End of grid, beginning of observe
   
    Mat3b img = imread(img2, 1); //CHANGE: img2
    resize(img, img, finalSize);
    Mat3b blurred;
    
    cout << "showing image" << endl;
    imshow("image", img);
    waitKey(0);
    Mat origImg;
    cvtColor(img, origImg, CV_RGB2GRAY,1);
    
    Mat cimg;
    vector <compositeCircle> known_circles;
    for (int i = 1; i < 13; i+=2){
        medianBlur(origImg, cimg, i);
        vector<Vec3f> circles;
        HoughCircles(cimg, circles, HOUGH_GRADIENT, 1, 10,
                     100, 30, 10, 30 // change the last two parameters
                     // (min_radius & max_radius) to detect larger circles
                     );
        
        for(size_t i = 0; i< circles.size(); ++i){
            bool merged = false;
            Vec3i curCircle = circles[i];
            for(size_t j = 0; j < known_circles.size(); ++j){
                Vec3i knownCircle = known_circles[j].circle;
                float centerDist = sqrt(pow(curCircle[0] - knownCircle[0],2) +
                                        pow(curCircle[1] - knownCircle[1],2));
                if(centerDist < curCircle[2] || centerDist < knownCircle[2]){
                    //the circles are the same, combine them
                    int nc = known_circles[j].numCombines;
                    knownCircle[0] = (nc*knownCircle[0]+curCircle[0])/(nc+1.0);
                    knownCircle[1] = (nc*knownCircle[1]+curCircle[1])/(nc+1.0);
                    knownCircle[2] = (nc*knownCircle[2]+curCircle[2])/(nc+1.0);
                    known_circles[j].numCombines = nc + 1;
                    known_circles[j].circle = knownCircle;
                    merged = true;
                    break;
                }
                
            }
            if(!merged) {
                compositeCircle newCircle;
                newCircle.circle = curCircle;
                newCircle.numCombines = 1;
                known_circles.push_back(newCircle);
            }
        }
    }
    for( size_t i = 0; i < known_circles.size(); i++ )
    {
        Vec3i c = known_circles[i].circle;
        circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
        circle( cimg, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
    }
    namedWindow( "imOutput", WINDOW_AUTOSIZE ); // Create a window for display.
    
    imshow("imOutput", cimg);
    cout << "num circles is: " << known_circles.size() << endl;
    waitKey(0);

}
