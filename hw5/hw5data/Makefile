# Makefile for Computer Vision HW5
# Comments begin with a # (like this line)

# Your UNI
UNI = kas2221

# VARIOUS VARIABLES (you will need to change some of these)
# The compiler. If you're writing code using c++, change gcc to g++
CC = g++

# Compile time flags. If you want to compile with debugging turned off, remove the -g
CFLAGS = -g

# Libraries to link against. You should only need to link against the math library (-lm).
LFLAGS = -lm

# Enter the names of your source files here. If you have header files as well,
# then do not include them here (they will be autoincluded).
P1SRC = flow.cpp

# Step parameter for choosing how fine of a normal map to get.
# You can decrease this for better quality outputs, at the
# expense of computation time, or increase it to get quick results
# (at reduced quality). Set this to an appropriate value
STEP = 10

# This contains a list of various images and other files generated from 
# running your programs. By default, it contains the filenames of all files
# generated from running the 'test' target. If you generate any other files,
# make sure this is correct. This list of files is deleted when 'make clean'
# or 'make submit' are run (to prevent submitting all these files).
OUTPUTS_TO_CLEAN = simpleout.pgm flowout12.pgm flowout23.pgm flowout34.pgm flowout45.pgm flowout56.pgm *.cpp~ *.h~ *.obj *.c~ *.png

# THE BUILD RULES (you shouldn't need to edit any of these)
# 'make test' builds the program and tests it
# You should check to see that the final output images are correct
test: flow
	./flow simple1.pgm simple2.pgm $(STEP) simpleout.pgm
	./flow flow1.pgm flow2.pgm $(STEP) flowout12.pgm
	./flow flow2.pgm flow3.pgm $(STEP) flowout23.pgm
	./flow flow3.pgm flow4.pgm $(STEP) flowout34.pgm
	./flow flow4.pgm flow5.pgm $(STEP) flowout45.pgm
	./flow flow5.pgm flow6.pgm $(STEP) flowout56.pgm

# The utilities file. Leave this as is.
UTILS = vision_utilities.c

#my methods
METHODS = vision_methods.cpp
METHODSH = vision_methods.h

# 'make flow' builds the program
flow: $(P1SRC) $(UTILS) $(METHODS)
	$(CC) $(CFLAGS) -o $@ $(P1SRC) $(METHODS) $(UTILS) $(LFLAGS)

# the rule for building the vision utilities
vision_utilities.o: vision_utilities.c vision_utilities.h
	$(CC) -c -Wall -pedantic -o $@ vision_utilities.c

vision_methods.o: vision_methods.cpp vision_methods.h
	$(CC) -c -Wall -pedantic -o $@ vision_methods.cpp

# 'make submit' creates a tar of all files in the directory
# NOTE 1: It also runs 'make clean' before submission.
 submit: clean
	tar -czvf $(UNI)-hw5.tar.gz $(P1SRC) $(UTILS) $(UTILSH) Makefile README.txt *.cpp *.c *.h

# 'make clean' removes all output and temporary files
clean:
	-rm -f flow vision_utilities.o $(OUTPUTS_TO_CLEAN) core CORE flow.exe

#image MAGIK! 
convert:
	mogrify -format png *.pgm
	mogrify -format png *.ppm
	open *.png
