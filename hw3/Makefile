# Makefile for computer vision HW3
# Comments begin with a # (like this line)

# Your UNI
UNI = abc1234

# VARIOUS VARIABLES (you will need to change some of these)
# The compiler. If you're writing code using c++, change gcc to g++
CC = g++

# Compile time flags. If you want to compile with debugging turned off, remove the -g
CFLAGS = -g

# Libraries to link against. You should only need to link against the math library (-lm).
LFLAGS = -lm

# The utilities file. Leave this as is.
UTILS = vision_utilities.c
UTILSH = vision_utilities.h

# Enter the names of your source files here. If you have header files as well,
# then do not include them here (they will be autoincluded).
P1SRC = p1.cpp
P2SRC = p2.cpp
P3SRC = p3.cpp
P4SRC = p4.cpp

# Edge Threshold for getting strong edges on all images. 
# Change this.
EDGE_THRESHOLD = 0

# Hough Threshold for getting consistent lines on the 
# hough_simple images. Change this.
HOUGH_SIMPLE_1_THRESHOLD = 0
HOUGH_SIMPLE_2_THRESHOLD = 0

# Hough Threshold for getting consistent lines on the 
# hough_complex_1 image. Change this.
HOUGH_COMPLEX_THRESHOLD = 0

# This contains a list of various images and other files generated from 
# running your programs. By default, it contains the filenames of all files
# generated from running the 'test' target. If you generate any other files,
# make sure this is correct. This list of files is deleted when 'make clean'
# or 'make submit' are run (to prevent submitting all these files).
OUTPUTS_TO_CLEAN = p11.pgm p12.pgm p13.pgm p21.pgm p22.pgm p23.pgm h21.pgm h22.pgm h23.pgm p31.pgm p32.pgm p33.pgm p41.pgm p42.pgm p43.pgm

# THE BUILD RULES (you shouldn't need to edit any of these)
# 'make test' builds all 4 programs and then tests them.
# You should check to see that the final output images are correct
test: test1 test2 test3 test4
	
# 'make test1' tests only program 1
test1: p1
	p1 hough_simple_1.pgm p11.pgm
	p1 hough_simple_2.pgm p12.pgm
	p1 hough_complex_1.pgm p13.pgm

# 'make test2' tests only program 2
test2: p2
	p2 p11.pgm $(EDGE_THRESHOLD) p21.pgm h21.pgm
	p2 p12.pgm $(EDGE_THRESHOLD) p22.pgm h22.pgm
	p2 p13.pgm $(EDGE_THRESHOLD) p23.pgm h23.pgm

# 'make test3' tests only program 3
test3: p3
	p3 hough_simple_1.pgm h21.pgm $(HOUGH_SIMPLE_1_THRESHOLD) p31.pgm
	p3 hough_simple_2.pgm h22.pgm $(HOUGH_SIMPLE_2_THRESHOLD) p32.pgm
	p3 hough_complex_1.pgm h23.pgm $(HOUGH_COMPLEX_THRESHOLD) p33.pgm

# 'make test4' tests only program 4
test4: p4
	p4 hough_simple_1.pgm h21.pgm p21.pgm $(HOUGH_SIMPLE_THRESHOLD) p41.pgm
	p4 hough_simple_2.pgm h22.pgm p22.pgm $(HOUGH_SIMPLE_THRESHOLD) p42.pgm
	p4 hough_complex_1.pgm h23.pgm p23.pgm $(HOUGH_COMPLEX_THRESHOLD) p43.pgm

# 'make all' builds all 4 programs (but doesn't run them)
all: p1 p2 p3 p4

# 'make p1' builds p1 only
p1: $(P1SRC) $(UTILS)
	$(CC) $(CFLAGS) -o $@ $(P1SRC) $(UTILS) $(LFLAGS)

# 'make p2' builds p2 only
p2: $(P2SRC) $(UTILS)
	$(CC) $(CFLAGS) -o $@ $(P2SRC) $(UTILS) $(LFLAGS)

# 'make p3' builds p3 only
p3: $(P3SRC) $(UTILS)
	$(CC) $(CFLAGS) -o $@ $(P3SRC) $(UTILS) $(LFLAGS)

# 'make p4' builds p4 only
p4: $(P4SRC) $(UTILS)
	$(CC) $(CFLAGS) -o $@ $(P4SRC) $(UTILS) $(LFLAGS)

# the rule for building the vision utilities
vision_utilities.o: vision_utilities.c vision_utilities.h
	$(CC) -c -Wall -pedantic -o $@ vision_utilities.c

# 'make submit' creates a tar of all files in the directory
# NOTE 1: It also runs 'make clean' before submission.
 submit: clean
	tar -czvf $(UNI)-hw3.tar.gz $(P1SRC) $(P2SRC) $(P3SRC) $(P4SRC) $(UTILS) $(UTILSH) Makefile README

# 'make extract' extracts all files into a new folder named UNI
extract: $(UNI).tar.gz
	mkdir -p $(UNI)
	tar -xvf $(UNI)-hw3.tar.gz -C $(UNI)/

	
# 'make clean' removes all output and temporary files
clean:
	-rm -f p1 p2 p3 p4 vision_utilities.o $(OUTPUTS_TO_CLEAN) core CORE


