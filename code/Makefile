CC = g++

OPENCV_LIBS = -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core 
LIBS = $(OPENCV_LIBS)
OPENCV_FLAGS = -I/usr/local/include/opencv -I/usr/local/include
CFLAGS = -g -Wall -Werror -std=c++11 $(OPENCV_FLAGS)

default:	observe 
ai:  main.cpp grid.cpp Gomoku.cpp Coordinate.cpp threats.cpp observepieces.cpp 
	$(CC) $(CFLAGS) main.cpp grid.cpp Gomoku.cpp Coordinate.cpp threats.cpp observepieces.cpp -o $@ $(LIBS) 
observe: observepieces.cpp
	$(CC) $(CFLAGS) observepieces.cpp -o $@ $(LIBS)
grid: hough_lines.cpp
	$(CC) $(CFLAGS) hough_lines.cpp -o $@ $(LIBS)
fill: grid_fill.cpp
	$(CC) $(CFLAGS) grid_fill.cpp -o $@ $(LIBS)


clean:
	rm -rf *.o svm test
