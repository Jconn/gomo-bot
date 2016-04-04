CC = g++

OPENCV_LIBS = -L/home/cfregan/usr/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core 
LIBS = $(OPENCV_LIBS)
OPENCV_FLAGS = -I/home/cfregan/usr/include/opencv -I/home/cfregan/usr/include
CFLAGS = -g -Wall -Werror -std=c++11 $(OPENCV_FLAGS)

default:	observe 

observe: observepieces.cpp
	$(CC) $(CFLAGS) observepieces.cpp -o $@ $(LIBS)
grid: hough_lines.cpp
	$(CC) $(CFLAGS) hough_lines.cpp -o $@ $(LIBS)
fill: grid_fill.cpp
	$(CC) $(CFLAGS) grid_fill.cpp -o $@ $(LIBS)


clean:
	rm -rf *.o svm test
