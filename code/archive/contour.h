#ifndef DISPLAYIMAGE_H
#define DISPLAYIMAGE_H


std::pair<double, int> findLargestContour(std::vector<std::vector<cv::Point> >& contours) {
  double largest_area = 0;
  int largest_area_idx = 0;
  
  for(int i = 0; i < contours.size(); ++i) {
    double area_of_contour_i = contourArea(contours[i],false);
    if(area_of_contour_i > largest_area) {


      // debug msg
      std::cout << "itr " << i << '\n';
      std::cout << "largest area of " << largest_area << " usurped by " << area_of_contour_i << "\n\n";

      largest_area = area_of_contour_i;
      largest_area_idx = i;
    }
  }

  return std::make_pair(largest_area, largest_area_idx);
}

// format the data from origin to destination required by the function which performs our transformation
void populateFourCorners(const std::vector<std::vector<cv::Point> >& origin, std::vector<cv::Point2f>& destination) {
  destination.push_back(cv::Point2f(origin[0][0].x, origin[0][0].y));
  destination.push_back(cv::Point2f(origin[0][1].x, origin[0][1].y));
  destination.push_back(cv::Point2f(origin[0][3].x, origin[0][3].y));
  destination.push_back(cv::Point2f(origin[0][2].x, origin[0][2].y));
}

void populateFourCorners(const cv::Rect& origin, std::vector<cv::Point2f>& destination) {
  destination.push_back(cv::Point2f(origin.x,origin.y));
  destination.push_back(cv::Point2f(origin.x,origin.y + origin.height));
  destination.push_back(cv::Point2f(origin.x + origin.height, origin.y));
  destination.push_back(cv::Point2f(origin.x + origin.height, origin.y + origin.height));
}
void printPoint(cv::Point p, std::string s) {
  std::cout << s << "vals: " << p.x << ", " << p.y << '\n';
}

void drawLines(cv::Mat& img,std::vector<std::vector<cv::Point> >& contours_poly) {
  cv::Point p1 = contours_poly[0][0];
  cv::Point p2 = contours_poly[0][1];
  cv::Point p3 = contours_poly[0][2];
  cv::Point p4 = contours_poly[0][3];

  printPoint(p1, "p1");
  printPoint(p2, "p2");
  printPoint(p3, "p3");
  printPoint(p4, "p4");

  cv::line(img, p1, p2, cv::Scalar(0,0,255), 1, CV_AA, 0); 
  cv::line(img, p2, p3, cv::Scalar(0,0,255), 1, CV_AA, 0); 
  cv::line(img, p3, p4, cv::Scalar(0,0,255), 1, CV_AA, 0); 
  cv::line(img, p4, p1, cv::Scalar(0,0,255), 1, CV_AA, 0); 
}

void drawRectangles();


#endif
