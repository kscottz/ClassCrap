HW2 README
Katherine Scott
KAS2221

(0)
General Notes. 
I broke my code out into a separate file called vision_methods.h.
This makes it easier to share methods between programs and means
I have a reusable product at the end of the day. 

Just in case something goes wrong I processed all of the provided
images and stuck them in an images.zip file. 

I tested this on my mac and see no reason why it shouldn't run
in a cunix environment.

The source code can also be found here: 
https://github.com/kscottz/ClassCrap

(1) 
I set the threshold value to 120 in the
makefile. The default threshold of 100 worked fairly well but caused
some edge effects (i.e. there was a blob/contour on the side of the 
image) in one of the images so I raised it slightly to 
eliminate the edge effects.

(2)
A modified the algorithm slightly to count the number of 
objects and then divide the color space evenly. This makes it 
much easier to see the images in the resulting file. It
also means I don't need to change the image header. I used
templated types like map and set to make the 
labeling process a lot easier to implement and understand.

(3)
I initially didn't have the nodes for class so I went by
the wikipedia entry on image moment:
http://en.wikipedia.org/wiki/Image_moment

In the code I calculate all of the raw moments, the first
six central moments and the first Hu moments for each
object. 
The moment I provide in the output database is the
Hu Moment (which is rotation invariant). I also calculate
roundness as the ratio between the square roots of the min
eigen value over the max eigen value. This should yield
a similar result. I also tried calculating the angle 
of rotation using the class method and the wikipedia
method. There were some descrpancies between the two, so
I opted to use my method. You can find the results
in the vision_methods.cpp file at line 252. All of the 
statistics start at around line 230 in the methods file. 

I included an example of each approach 
The notes way: notes.png
Wikipedia way: MyWay.png

My final output database entires look like this:
<lable - string> <x_center> <y_center> <Hu Moment> <angle - degrees>
<roundness> <area>

I could have expanded it to include all of the raw, central, and Hu
moments but I was lazy. The basic data type for the database
is a vector of structs called SObjetLabel. 

(4)
For this problem I used the average percent differences of 
Hu moment and the roundness between the database and the test
objects. The Hu moment is rotation invariant so it makes 
a good choice for matching. The roundness metric is even
beter as it should be scale and rotation invariant. I take
the percent difference of these two values and then average
them to get my decision criteria. I set the threshold at
10% (0.1) and I seem to get acceptable results.
