#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>
#include "MoveType.h"
#include "Gomoku.h"
using namespace cv;
using namespace std;
static const Size dispSize(640, 480);
static const Size inputSize(1024, 860);
static const Size GS(13,13);


vector <compositeCircle> observePieces(string filename, int expectedPieces)
{
	cout << "expecting to see " << expectedPieces << " pieces " << endl;
  Mat3b img = imread(filename, 1);
  Mat3b blurred;
  Mat dispImg;
  resize(img,dispImg,dispSize);
  cout << "showing image" << endl; 
  imshow("image", dispImg);
	waitKey(0);
	vector<Mat> dispVec;
	split(dispImg, dispVec);
	cout << "showing image channel 0" << endl; 
	imshow("image", dispVec[0]);
	waitKey(0);
	cout << "showing image channel 1" << endl; 
	imshow("image", dispVec[1]);
	waitKey(0);
	cout << "showing image channel 2" << endl; 
	imshow("image", dispVec[2]);
	waitKey(0);





	Mat origImg; 
	cvtColor(img, origImg, CV_RGB2GRAY,1);
	Mat cimg;
	vector <compositeCircle> known_circles; 
	for(int j =3; j < 25; j+=2) { 
		cout << "Trying gaussian blur of size " << j << endl;
		if((int)known_circles.size() == expectedPieces)
			break;	
		for (int i = 1; i < 4; i+=1){
			GaussianBlur(origImg, cimg,Size(j,j), i,i);
			vector<Vec3f> circles;
			HoughCircles(cimg, circles, HOUGH_GRADIENT, 1, 10,
					100, 30, 30, 60 // change the last two parameters
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
	}
	int meanblack = 25;
  int meanwhite = 100;
  int numwhite = 0;
  int numblack = 0;
  int whiteval = 0;
  int blackval = 0;
  for( size_t i = 0; i < known_circles.size(); i++ )
  {
    Vec3i c = known_circles[i].circle;
    int pixval = origImg.at<unsigned char>(Point(c[0],c[1]));
    cout << " pixval is " << pixval << endl; 
    if (abs(pixval-meanwhite) < abs(pixval-meanblack)){
      numwhite++;
      whiteval+=pixval; 
    }
    else
    {
      numblack++;
      blackval+=pixval;
    }
  }

  meanblack = numblack > 0 ? blackval/numblack : -255;
  meanwhite = numwhite > 0 ? whiteval/numwhite : 255;
  int  threshold_value = (meanwhite+meanblack)/2;
  cout << "thresh value is " << threshold_value << endl; 
  for( size_t i = 0; i < known_circles.size(); i++ )
  {
    Vec3i c = known_circles[i].circle;
    int pixval = origImg.at<unsigned char>(Point(c[0],c[1]));
    if(pixval> threshold_value)
    {
      known_circles[i].color = white;
      circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
      circle( cimg, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
      //   numwhite++;
    }
    else {
      known_circles[i].color = black;
      circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
      circle( cimg, Point(c[0], c[1]), 2, Scalar(255,255,255), 3, LINE_AA);
      //  numblack++;
    }

  }
	//resize(cimg, cimg, dispSize);

	//namedWindow( "imOutput", WINDOW_AUTOSIZE ); // Create a window for display.
	//imshow("imOutput", cimg);
  //waitKey(0);
  cout << "num circles is: " << known_circles.size() << endl; 
  return known_circles;
} 
