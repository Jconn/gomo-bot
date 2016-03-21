CC = g++

OPENCV_LIBS = -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal

LIBS = $(OPENCV_LIBS)

OPENCV_FLAGS = -I/usr/local/include/opencv -I/usr/local/include

CFLAGS = -g -Wall -Werror -Wextra -std=c++11

default: main 


main: Gomoku.cpp
	$(CC) $(CFLAGS) Gomoku.cpp -o $@

####### fiveHorizontally tests
# left side, win, most recent = leftmost
test1: fiveHorizontallyTest1.cpp 
	$(CC) $(CFLAGS) fiveHorizontallyTest1.cpp -o $@

# left side, no win, most recent = leftmost 
test1-1: fiveHorizontallyTest1-1.cpp
	$(CC) $(CFLAGS) fiveHorizontallyTest1-1.cpp -o $@

# left side, win, most recent = center 
test2: fiveHorizontallyTest2.cpp
	$(CC) $(CFLAGS) fiveHorizontallyTest2.cpp -o $@

# left side, win, adjusted center
test2-1: fiveHorizontallyTest2-1.cpp
	$(CC) $(CFLAGS) fiveHorizontallyTest2-1.cpp -o $@

# left side, most recent = white, loss
test2-2: fiveHorizontallyTest2-2.cpp
	$(CC) $(CFLAGS) fiveHorizontallyTest2-2.cpp -o $@

<<<<<<< HEAD
# 5 in a row: right of board
test3: fiveHorizontallyTest2.cpp
	$(CC) $(CFLAGS) fiveHorizontallyTest3.cpp -o $@

######### fiveVertically tests

test4: fiveVerticallyTest1.cpp
	$(CC) $(CFLAGS) fiveVerticallyTest1.cpp -o $@

test4-1: fiveVerticallyTest1-1.cpp
	$(CC) $(CFLAGS) fiveVerticallyTest1-1.cpp -o $@

test4-2: fiveVerticallyTest1-2.cpp
	$(CC) $(CFLAGS) fiveVerticallyTest1-2.cpp -o $@


######### fiveDiagonally tests

test5: fiveDiagonallyTest1.cpp
	$(CC) $(CFLAGS) fiveDiagonallyTest1.cpp -o $@

test5-1: fiveDiagonallyTest1-2.cpp
	$(CC) $(CFLAGS) fiveDiagonallyTest1-2.cpp -o $@


#### fiveAntiDiagonally tests

test6: fiveAntiDiagonallyTest1.cpp
	$(CC) $(CFLAGS) fiveAntiDiagonallyTest1.cpp -o $@

clean:
	rm -rf *.o Gomoku test1 test1-1 test2 test2-1 test 2-2 test3 test4 test4-1 test4-2 test5 test5-1 test6
=======
#
>>>>>>> origin/master
