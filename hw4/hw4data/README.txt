Katherine Scott
KAS2221@columbia.edu

1. Nada

2. See calculateLightingVector() @ line 759
I calculate the surface normal as follows:
x = (brightestpoint_x-center.x)/radius
y = (brightestpoint_y-center.y)/radius
z = sqrt(radius^2-(x^2+y^2)/radius

Assuming that the brightest spot is the surface normal is safe because
the surface is lambertian. The reflectance is only a function on the
lighting and not the view direction.

3. Nada

4. 
Surface Normals.
I used a vvector.h to do my vector and matrix math. vvector.h is part
of OpenGL and it is just a bunch of nice macros. 

I had some issues with using the best of three lighting vectors that
caused a "bright spot" in my normal vector image. I changed over to
using just the first three vectors and this seemed to work a bit
better. I left this approach in the code, but the best three of five
approach is commented out. See createNormalMap (line 898) and
constructNormal (line 1017)

With respect to the pq vector I store everything in a single vector
data structure and then just pipe it out to file. I reconstruct it in
part five. I hope this okay.

5. My seed files are included. I just picked between 6 and 12 points
on the image in all of the quadrants. I used the method described in
the class notes. 
