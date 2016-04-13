#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>
#include "MoveType.h"
#include "Gomoku.h"
#include <algorithm>
#include <vector>
using namespace cv;
using namespace std;
static const Size dispSize(640, 480);
static const Size inputSize(1024, 860);
static const Size GS(13,13);

float ED(Vec3b vec1, Vec3b vec2){ // euclidean distance
  return sqrt(
            (vec1[0]-vec2[0]) *(vec1[0]-vec2[0]) +
            (vec1[1]-vec2[1]) *(vec1[1]-vec2[1]) + 
            (vec1[2]-vec2[2]) *(vec1[2]-vec2[2])
            ); 

}

vector <compositeCircle> observePieces(string filename, int expectedPieces)
{
	//cout << "expecting to see " << expectedPieces << " pieces " << endl;
  Mat3b img = imread(filename, 1);
  Mat3b blurred;
  Mat dispImg;
	//  resize(img,dispImg,dispSize);
	//cout << "showing image" << endl; 
	//imshow("image", dispImg);
	//waitKey(0);
	//  vector<Mat> dispVec;
	//  split(dispImg, dispVec);

  /*	
      cout << "showing image channel 0" << endl; 
      imshow("image", dispVec[0]);
      waitKey(0);
      cout << "showing image channel 1" << endl; 
      imshow("image", dispVec[1]);
      waitKey(0);
      cout << "showing image channel 2" << endl; 
      imshow("image", dispVec[2]);
      waitKey(0);

*/

  vector<Mat> imageVec;
  split(img,imageVec);
  Mat curImg; 
  //cvtColor(img, curImg, CV_RGB2GRAY,1);
  Mat cimg;
  vector <compositeCircle> known_circles; 

	for(int j =3; j < 25; j+=2) { 
		for(int v = 0; v < 3; ++v){	
			curImg = imageVec[v]; 

			cout << "Trying gaussian blur of size " << j << endl;
			cout << " number of detected pieces is " << known_circles.size() << endl;
			if((int)known_circles.size() == expectedPieces)
				break;
			if(j==9 && expectedPieces < 0)
				break;
			for (int i = 1; i < 4; i+=1){
				GaussianBlur(curImg, cimg,Size(j,j), i,i);
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
	}

	Vec3b meanBlack = {25,25,25};
  Vec3b meanWhite = {150,150,150};
  Vec3b meanGreen = {0,160,85};
  Vec3b meanRed = {0,75,150};
  float EDs[4]; 

  /*
  for( size_t i = 0; i < known_circles.size(); i++ )
  {
    Vec3i c = known_circles[i].circle;
    Vec3b pixel = img.at<Vec3b>(Point(c[0],c[1]));
    //find the array of distances
    //cout << " blue pixval is " << (int)pixel[0] << endl;
    //cout << " green pixval is " << (int)pixel[1] << endl; 
    //cout << " red pixval is " << (int)pixel[2] << endl;  


  }
*/
 
  for( size_t i = 0; i < known_circles.size(); i++ )
  {
    Vec3i c = known_circles[i].circle;
    Vec3b pixel = img.at<Vec3b>(Point(c[0],c[1]));
    //cout << " blue pixval is " << (int)pixel[0] << endl;
    //cout << " green pixval is " << (int)pixel[1] << endl; 
    //cout << " red pixval is " << (int)pixel[2] << endl;  

 
    EDs[0] = ED(pixel,meanBlack);
    EDs[1] = ED(pixel,meanWhite);
    EDs[2] = ED(pixel,meanGreen);
    EDs[3] = ED(pixel,meanRed);
    //cout << EDs[0] << "\n" << EDs[1] << "\n" << EDs[2] << "\n" << EDs[3] << "\n\n";
    int minIndex = 0; 
    float miniDist = 3000000;
    //cout << "in a mindist loop" << endl;  
    for(int i = 0; i <4; ++i){
      if(EDs[i] < miniDist){
        miniDist = EDs[i];

        //cout << " found new minDist = " << miniDist;
        minIndex = i;
      }
    }
    int color = minIndex;
    //cout << " color is " << color<< endl; 

    //distance(EDs.begin(), min_element(EDs.begin(),EDs.end()));
    known_circles[i].color = static_cast<MoveType>(color);
    if(color ==0)
    {
      //black
      circle( img, Point(c[0], c[1]), 2, Scalar(255,255,255), 3, LINE_AA);
      circle( img, Point(c[0], c[1]), c[2], Scalar(255,255,255), 3, LINE_AA); 

      //   numwhite++;
    }
    else if(color==1) {
      //white 
      circle( img, Point(c[0], c[1]), 2, Scalar(0,0,0), 3, LINE_AA);
      circle( img, Point(c[0], c[1]), c[2], Scalar(0,0,0), 3, LINE_AA); 
      //  numblack++;
    }
    else if(color==2) {
      //green 
      circle( img, Point(c[0], c[1]), 2, Scalar(0,0,255), 3, LINE_AA);
      circle( img, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA); 
      //  numblack++;
    }
    else if(color==3) {
      //red 
      circle( img, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
      circle( img, Point(c[0], c[1]), c[2], Scalar(0,255,0), 3, LINE_AA); 
      //  numblack++;
    }
	}
	//namedWindow( "imOutput", WINDOW_AUTOSIZE ); // Create a window for display.
	if(expectedPieces < 0 ) {

		resize(img, img, dispSize);
		imshow("imOutput", img);
		waitKey(0);
	}
	cout << "detected: " << known_circles.size()<< " circles " << endl; 
  return known_circles;
} 
