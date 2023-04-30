/******************************************************************************
 * This demo is a modification of demo5.cpp to implement normal mapping,
 * simulating bumpy surfaces.
 *
 * The vertex data now includes tangent vectors (in addition to normals), and
 * the texture code is upgraded to load a diffuse map and a normal map at the
 * same time.
 *
*  (Note that the shader code is also updated -- see demo5n.vs and demo5n.fs.)
 *
 * TIP: To help you understand the code better, I highly recommend that you
 * view the changes between demo5 and demo5n in VS Code by doing the following:
 *
 * 1. Right-click demo5.cpp in VS Code's Explorer pane and click
 *    "Select for Compare".
 * 2. Right-click the demo5n.cpp and click "Compare with Selected".
 *
 * (Do the same for demo5.vs/demo5n.vs and demo5.fs/demo5n.fs.)
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>
#include <vector>

// change this to your desired window attributes
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 360
#define WINDOW_TITLE  "Exercise2 (use WASDQE keys for camera, IKJLUO keys for light, ZX keys for Ambient Intensity, CV keys for Specular Intensity, BN keys for Specular Power, 1-2 for Spotlight Cutoff, 3-4 for Spotlight Outer Angle)"
GLFWwindow *pWindow;
/*
// model
float vertices[] =
{
    // position (x, y, z)    normal (x, y, z)     tangent (x, y, z)    texture coordinates (s, t)

    // ground plane
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 4.0f,

    // UNCOMMENT VERTICES BELOW TO PRESERVE PLANE BUT REMOVE CUBE
    // cube top
    -1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f,  1.00f,  1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f,  1.00f, -1.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube bottom
    -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f, -1.00f, -1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f, -1.00f,  1.00f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube front
    -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f, -1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.00f,  1.00f,  1.00f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube back
     1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.00f, -1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.00f,  1.00f, -1.00f,  0.0f,  0.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    // cube right
     1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     1.00f, -1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     1.00f,  1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1.00f, -1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     1.00f,  1.00f, -1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1.00f,  1.00f,  1.00f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

    // cube left
    -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    -1.00f, -1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
    -1.00f,  1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -1.00f, -1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    -1.00f,  1.00f,  1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -1.00f,  1.00f, -1.00f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f
};
*/
// OpenGL object IDs
GLuint vao, vao2, vao3;
GLuint vbo, vbo2, vbo3;
GLuint shader;
GLuint texture[2], texturePlane[2];

/* ============= CONE POINTS CALCULATED THROUGH CODE BELOW ===============
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double PI = 3.14159265358979323846;

int main()
{
    double radius = 5.0; // radius of the circle
    double centerX = 0.0; // x-coordinate of the center of the circle
    double centerY = 0.0; // y-coordinate of the center of the circle
    double centerZ = 0.0; // z-coordinate of the center of the circle
    
    int numPoints = 10; // number of points to generate
    
    // loop through and generate the coordinates of the points on the circle
    for (int i = 0; i < numPoints; i++)
    {
        double angle = (2.0 * PI * i) / numPoints;
        double x = centerX + (radius * cos(angle));
        double y = centerY + (radius * sin(angle));
        double z = centerZ;
        double radius = 5.0; // radius of the circle
        double centerX = 0.0; // x-coordinate of the center of the circle
        double centerY = 0.0; // y-coordinate of the center of the circle
        double centerZ = 0.0; // z-coordinate of the center of the circle
        
        int numPoints = 100; // number of points to generate
        
        // calculate the normal and tangent vectors
        double nx = -sin(angle);
        double ny = cos(angle);
        double nz = 0.0;
        double tx = cos(angle);
        double ty = sin(angle);
        double tz = 0.0;
        // loop through and generate the coordinates of the points on the circle
        for (int i = 0; i < numPoints; i++)
        {
            double angle = (2.0 * PI * i) / numPoints;
            double x = centerX + (radius * cos(angle));
            double y = centerY + (radius * sin(angle));
            double z = centerZ;
            
            // calculate the normal and tangent vectors
            double normx = -sin(angle);
            double normy = cos(angle);
            double normz = 0.0;
            double tanx = cos(angle);
            double tany = sin(angle);
            double tanz = 0.0;
            double s = angle/(2*PI);
            double t = (5 - z) / 5;

            // output the point coordinates, normal vector, and tangent vector
            cout << fixed << setprecision(2) << x << "f , " << y << "f , " << z << "f, "
                << normx << "f, " << normy << "f, " << normz << "f, "
                << tanx << "f, " << tany << "f, " << tanz << "f, "<< 
                s <<"f, "<< t << "f, "<< endl;
        }
        
        // output the point coordinates, normal vector, and tangent vector
        cout << fixed << setprecision(2) << x << "f , " << y << "f , " << z << "f, "
            << nx << "f, " << ny << "f, " << nz << "f, "
            << tx << "f, " << ty << "f, " << tz << "f, " << 
            "1.0f, 0.0f,"<< endl;
        return 0;
    }
    
    return 0;
}*/

/*============= SPHERE POINTS CALCULATED THROUGH CODE BELOW ===============
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double PI = 3.14159265358979323846;

int main()
{
    int numCircles = 10; // number of circles to generate
    double initialRadius = 5.0; // initial radius of the circles
    double radiusShrinkFactor = 1 / (numCircles + 0.5); // factor by which radius shrinks for each circle

    int numPoints = 10; // number of points to generate

    // loop through each circle
    for (int j = 0; j < numCircles; j++)
    {
        double angle1 = PI * (j + 0.5) / numCircles; // calculate the angle for latitude
        double radius1 = initialRadius * sin(angle1); // calculate the radius for latitude
        double z = initialRadius * cos(angle1); // calculate the z-coordinate

        // loop through and generate the coordinates of the points on the circle
        for (int i = 0; i < numPoints; i++)
        {
            double angle2 = (2.0 * PI * i) / numPoints; // calculate the angle for longitude
            double x = radius1 * cos(angle2); // calculate x-coordinate
            double y = radius1 * sin(angle2); // calculate y-coordinate
            z = abs(z); // take the absolute value of z

            // calculate the normal and tangent vectors
            double normx = x / initialRadius;
            double normy = y / initialRadius;
            double normz = z / initialRadius;
            double tanx = -sin(angle2);
            double tany = cos(angle2);
            double tanz = 0.0;
            double s = angle2 / (2 * PI);
            double t = angle1 / PI;

            // output the point coordinates, normal vector, and tangent vector
            cout << fixed << setprecision(2) << x << "f , " << y << "f , " << z << "f, "
                 << normx << "f, " << normy << "f, " << normz << "f, "
                 << tanx << "f, " << tany << "f, " << tanz << "f, " << s << "f, " << t << "f, " << endl;
        }

        cout << endl;
    }

    return 0;
}
*/
float vertices[] = 
{
0.39f , 0.00f , 4.98f, 0.08f, 0.00f, 1.00f, -0.00f, 1.00f, 0.00f, 0.00f, 0.03f, 
0.32f , 0.23f , 4.98f, 0.06f, 0.05f, 1.00f, -0.59f, 0.81f, 0.00f, 0.10f, 0.03f,
0.12f , 0.37f , 4.98f, 0.02f, 0.07f, 1.00f, -0.95f, 0.31f, 0.00f, 0.20f, 0.03f,
-0.12f , 0.37f , 4.98f, -0.02f, 0.07f, 1.00f, -0.95f, -0.31f, 0.00f, 0.30f, 0.03f,
-0.32f , 0.23f , 4.98f, -0.06f, 0.05f, 1.00f, -0.59f, -0.81f, 0.00f, 0.40f, 0.03f,
-0.39f , 0.00f , 4.98f, -0.08f, 0.00f, 1.00f, -0.00f, -1.00f, 0.00f, 0.50f, 0.03f,
-0.32f , -0.23f , 4.98f, -0.06f, -0.05f, 1.00f, 0.59f, -0.81f, 0.00f, 0.60f, 0.03f,
-0.12f , -0.37f , 4.98f, -0.02f, -0.07f, 1.00f, 0.95f, -0.31f, 0.00f, 0.70f, 0.03f,
0.12f , -0.37f , 4.98f, 0.02f, -0.07f, 1.00f, 0.95f, 0.31f, 0.00f, 0.80f, 0.03f,
0.32f , -0.23f , 4.98f, 0.06f, -0.05f, 1.00f, 0.59f, 0.81f, 0.00f, 0.90f, 0.03f,

1.17f , 0.00f , 4.86f, 0.23f, 0.00f, 0.97f, -0.00f, 1.00f, 0.00f, 0.00f, 0.07f,
0.94f , 0.69f , 4.86f, 0.19f, 0.14f, 0.97f, -0.59f, 0.81f, 0.00f, 0.10f, 0.07f,
0.36f , 1.11f , 4.86f, 0.07f, 0.22f, 0.97f, -0.95f, 0.31f, 0.00f, 0.20f, 0.07f,
-0.36f , 1.11f , 4.86f, -0.07f, 0.22f, 0.97f, -0.95f, -0.31f, 0.00f, 0.30f, 0.07f,
-0.94f , 0.69f , 4.86f, -0.19f, 0.14f, 0.97f, -0.59f, -0.81f, 0.00f, 0.40f, 0.07f,
-1.17f , 0.00f , 4.86f, -0.23f, 0.00f, 0.97f, -0.00f, -1.00f, 0.00f, 0.50f, 0.07f,
-0.94f , -0.69f , 4.86f, -0.19f, -0.14f, 0.97f, 0.59f, -0.81f, 0.00f, 0.60f, 0.07f,
-0.36f , -1.11f , 4.86f, -0.07f, -0.22f, 0.97f, 0.95f, -0.31f, 0.00f, 0.70f, 0.07f,
0.36f , -1.11f , 4.86f, 0.07f, -0.22f, 0.97f, 0.95f, 0.31f, 0.00f, 0.80f, 0.07f,
0.94f , -0.69f , 4.86f, 0.19f, -0.14f, 0.97f, 0.59f, 0.81f, 0.00f, 0.90f, 0.07f, 

1.91f , 0.00f , 4.62f, 0.38f, 0.00f, 0.92f, -0.00f, 1.00f, 0.00f, 0.00f, 0.12f,
1.55f , 1.12f , 4.62f, 0.31f, 0.22f, 0.92f, -0.59f, 0.81f, 0.00f, 0.10f, 0.12f,
0.59f , 1.82f , 4.62f, 0.12f, 0.36f, 0.92f, -0.95f, 0.31f, 0.00f, 0.20f, 0.12f,
-0.59f , 1.82f , 4.62f, -0.12f, 0.36f, 0.92f, -0.95f, -0.31f, 0.00f, 0.30f, 0.12f,
-1.55f , 1.12f , 4.62f, -0.31f, 0.22f, 0.92f, -0.59f, -0.81f, 0.00f, 0.40f, 0.12f,
-1.91f , 0.00f , 4.62f, -0.38f, 0.00f, 0.92f, -0.00f, -1.00f, 0.00f, 0.50f, 0.12f,
-1.55f , -1.12f , 4.62f, -0.31f, -0.22f, 0.92f, 0.59f, -0.81f, 0.00f, 0.60f, 0.12f,
-0.59f , -1.82f , 4.62f, -0.12f, -0.36f, 0.92f, 0.95f, -0.31f, 0.00f, 0.70f, 0.12f,
0.59f , -1.82f , 4.62f, 0.12f, -0.36f, 0.92f, 0.95f, 0.31f, 0.00f, 0.80f, 0.12f,
1.55f , -1.12f , 4.62f, 0.31f, -0.22f, 0.92f, 0.59f, 0.81f, 0.00f, 0.90f, 0.12f,

2.61f , 0.00f , 4.26f, 0.52f, 0.00f, 0.85f, -0.00f, 1.00f, 0.00f, 0.00f, 0.17f,
2.11f , 1.54f , 4.26f, 0.42f, 0.31f, 0.85f, -0.59f, 0.81f, 0.00f, 0.10f, 0.17f,
0.81f , 2.48f , 4.26f, 0.16f, 0.50f, 0.85f, -0.95f, 0.31f, 0.00f, 0.20f, 0.17f,
-0.81f , 2.48f , 4.26f, -0.16f, 0.50f, 0.85f, -0.95f, -0.31f, 0.00f, 0.30f, 0.17f,
-2.11f , 1.54f , 4.26f, -0.42f, 0.31f, 0.85f, -0.59f, -0.81f, 0.00f, 0.40f, 0.17f,
-2.61f , 0.00f , 4.26f, -0.52f, 0.00f, 0.85f, -0.00f, -1.00f, 0.00f, 0.50f, 0.17f,
-2.11f , -1.54f , 4.26f, -0.42f, -0.31f, 0.85f, 0.59f, -0.81f, 0.00f, 0.60f, 0.17f,
-0.81f , -2.48f , 4.26f, -0.16f, -0.50f, 0.85f, 0.95f, -0.31f, 0.00f, 0.70f, 0.17f,
0.81f , -2.48f , 4.26f, 0.16f, -0.50f, 0.85f, 0.95f, 0.31f, 0.00f, 0.80f, 0.17f,
2.11f , -1.54f , 4.26f, 0.42f, -0.31f, 0.85f, 0.59f, 0.81f, 0.00f, 0.90f, 0.17f,

3.25f , 0.00f , 3.80f, 0.65f, 0.00f, 0.76f, -0.00f, 1.00f, 0.00f, 0.00f, 0.23f,
2.63f , 1.91f , 3.80f, 0.53f, 0.38f, 0.76f, -0.59f, 0.81f, 0.00f, 0.10f, 0.23f, 
1.00f , 3.09f , 3.80f, 0.20f, 0.62f, 0.76f, -0.95f, 0.31f, 0.00f, 0.20f, 0.23f,
-1.00f , 3.09f , 3.80f, -0.20f, 0.62f, 0.76f, -0.95f, -0.31f, 0.00f, 0.30f, 0.23f,
-2.63f , 1.91f , 3.80f, -0.53f, 0.38f, 0.76f, -0.59f, -0.81f, 0.00f, 0.40f, 0.23f,
-3.25f , 0.00f , 3.80f, -0.65f, 0.00f, 0.76f, -0.00f, -1.00f, 0.00f, 0.50f, 0.23f,
-2.63f , -1.91f , 3.80f, -0.53f, -0.38f, 0.76f, 0.59f, -0.81f, 0.00f, 0.60f, 0.23f,
-1.00f , -3.09f , 3.80f, -0.20f, -0.62f, 0.76f, 0.95f, -0.31f, 0.00f, 0.70f, 0.23f,
1.00f , -3.09f , 3.80f, 0.20f, -0.62f, 0.76f, 0.95f, 0.31f, 0.00f, 0.80f, 0.23f,
2.63f , -1.91f , 3.80f, 0.53f, -0.38f, 0.76f, 0.59f, 0.81f, 0.00f, 0.90f, 0.23f,

3.80f , 0.00f , 3.25f, 0.76f, 0.00f, 0.65f, -0.00f, 1.00f, 0.00f, 0.00f, 0.27f,
3.08f , 2.23f , 3.25f, 0.62f, 0.45f, 0.65f, -0.59f, 0.81f, 0.00f, 0.10f, 0.27f,
1.17f , 3.62f , 3.25f, 0.23f, 0.72f, 0.65f, -0.95f, 0.31f, 0.00f, 0.20f, 0.27f,
-1.17f , 3.62f , 3.25f, -0.23f, 0.72f, 0.65f, -0.95f, -0.31f, 0.00f, 0.30f, 0.27f,
-3.08f , 2.23f , 3.25f, -0.62f, 0.45f, 0.65f, -0.59f, -0.81f, 0.00f, 0.40f, 0.27f,
-3.80f , 0.00f , 3.25f, -0.76f, 0.00f, 0.65f, -0.00f, -1.00f, 0.00f, 0.50f, 0.27f,
-3.08f , -2.23f , 3.25f, -0.62f, -0.45f, 0.65f, 0.59f, -0.81f, 0.00f, 0.60f, 0.27f,
-1.17f , -3.62f , 3.25f, -0.23f, -0.72f, 0.65f, 0.95f, -0.31f, 0.00f, 0.70f, 0.27f,
1.17f , -3.62f , 3.25f, 0.23f, -0.72f, 0.65f, 0.95f, 0.31f, 0.00f, 0.80f, 0.27f,
3.08f , -2.23f , 3.25f, 0.62f, -0.45f, 0.65f, 0.59f, 0.81f, 0.00f, 0.90f, 0.27f,

4.26f , 0.00f , 2.61f, 0.85f, 0.00f, 0.52f, -0.00f, 1.00f, 0.00f, 0.00f, 0.33f,
3.45f , 2.51f , 2.61f, 0.69f, 0.50f, 0.52f, -0.59f, 0.81f, 0.00f, 0.10f, 0.33f,
1.32f , 4.05f , 2.61f, 0.26f, 0.81f, 0.52f, -0.95f, 0.31f, 0.00f, 0.20f, 0.33f, 
-1.32f , 4.05f , 2.61f, -0.26f, 0.81f, 0.52f, -0.95f, -0.31f, 0.00f, 0.30f, 0.33f,
-3.45f , 2.51f , 2.61f, -0.69f, 0.50f, 0.52f, -0.59f, -0.81f, 0.00f, 0.40f, 0.33f,
-4.26f , 0.00f , 2.61f, -0.85f, 0.00f, 0.52f, -0.00f, -1.00f, 0.00f, 0.50f, 0.33f,
-3.45f , -2.51f , 2.61f, -0.69f, -0.50f, 0.52f, 0.59f, -0.81f, 0.00f, 0.60f, 0.33f,
-1.32f , -4.05f , 2.61f, -0.26f, -0.81f, 0.52f, 0.95f, -0.31f, 0.00f, 0.70f, 0.33f,
1.32f , -4.05f , 2.61f, 0.26f, -0.81f, 0.52f, 0.95f, 0.31f, 0.00f, 0.80f, 0.33f,
3.45f , -2.51f , 2.61f, 0.69f, -0.50f, 0.52f, 0.59f, 0.81f, 0.00f, 0.90f, 0.33f,

4.62f , 0.00f , 1.91f, 0.92f, 0.00f, 0.38f, -0.00f, 1.00f, 0.00f, 0.00f, 0.38f,
3.74f , 2.72f , 1.91f, 0.75f, 0.54f, 0.38f, -0.59f, 0.81f, 0.00f, 0.10f, 0.38f,
1.43f , 4.39f , 1.91f, 0.29f, 0.88f, 0.38f, -0.95f, 0.31f, 0.00f, 0.20f, 0.38f,
-1.43f , 4.39f , 1.91f, -0.29f, 0.88f, 0.38f, -0.95f, -0.31f, 0.00f, 0.30f, 0.38f,
-3.74f , 2.72f , 1.91f, -0.75f, 0.54f, 0.38f, -0.59f, -0.81f, 0.00f, 0.40f, 0.38f,
-4.62f , 0.00f , 1.91f, -0.92f, 0.00f, 0.38f, -0.00f, -1.00f, 0.00f, 0.50f, 0.38f,
-3.74f , -2.72f , 1.91f, -0.75f, -0.54f, 0.38f, 0.59f, -0.81f, 0.00f, 0.60f, 0.38f,
-1.43f , -4.39f , 1.91f, -0.29f, -0.88f, 0.38f, 0.95f, -0.31f, 0.00f, 0.70f, 0.38f,
1.43f , -4.39f , 1.91f, 0.29f, -0.88f, 0.38f, 0.95f, 0.31f, 0.00f, 0.80f, 0.38f,
3.74f , -2.72f , 1.91f, 0.75f, -0.54f, 0.38f, 0.59f, 0.81f, 0.00f, 0.90f, 0.38f,

4.86f , 0.00f , 1.17f, 0.97f, 0.00f, 0.23f, -0.00f, 1.00f, 0.00f, 0.00f, 0.42f,
3.93f , 2.86f , 1.17f, 0.79f, 0.57f, 0.23f, -0.59f, 0.81f, 0.00f, 0.10f, 0.42f,
1.50f , 4.62f , 1.17f, 0.30f, 0.92f, 0.23f, -0.95f, 0.31f, 0.00f, 0.20f, 0.42f,
-1.50f , 4.62f , 1.17f, -0.30f, 0.92f, 0.23f, -0.95f, -0.31f, 0.00f, 0.30f, 0.42f,
-3.93f , 2.86f , 1.17f, -0.79f, 0.57f, 0.23f, -0.59f, -0.81f, 0.00f, 0.40f, 0.42f, 
-4.86f , 0.00f , 1.17f, -0.97f, 0.00f, 0.23f, -0.00f, -1.00f, 0.00f, 0.50f, 0.42f,
-3.93f , -2.86f , 1.17f, -0.79f, -0.57f, 0.23f, 0.59f, -0.81f, 0.00f, 0.60f, 0.42f,
-1.50f , -4.62f , 1.17f, -0.30f, -0.92f, 0.23f, 0.95f, -0.31f, 0.00f, 0.70f, 0.42f,
1.50f , -4.62f , 1.17f, 0.30f, -0.92f, 0.23f, 0.95f, 0.31f, 0.00f, 0.80f, 0.42f,
3.93f , -2.86f , 1.17f, 0.79f, -0.57f, 0.23f, 0.59f, 0.81f, 0.00f, 0.90f, 0.42f,

4.98f , 0.00f , 0.39f, 1.00f, 0.00f, 0.08f, -0.00f, 1.00f, 0.00f, 0.00f, 0.47f,
4.03f , 2.93f , 0.39f, 0.81f, 0.59f, 0.08f, -0.59f, 0.81f, 0.00f, 0.10f, 0.47f,
1.54f , 4.74f , 0.39f, 0.31f, 0.95f, 0.08f, -0.95f, 0.31f, 0.00f, 0.20f, 0.47f,
-1.54f , 4.74f , 0.39f, -0.31f, 0.95f, 0.08f, -0.95f, -0.31f, 0.00f, 0.30f, 0.47f,
-4.03f , 2.93f , 0.39f, -0.81f, 0.59f, 0.08f, -0.59f, -0.81f, 0.00f, 0.40f, 0.47f,
-4.98f , 0.00f , 0.39f, -1.00f, 0.00f, 0.08f, -0.00f, -1.00f, 0.00f, 0.50f, 0.47f,
-4.03f , -2.93f , 0.39f, -0.81f, -0.59f, 0.08f, 0.59f, -0.81f, 0.00f, 0.60f, 0.47f,
-1.54f , -4.74f , 0.39f, -0.31f, -0.95f, 0.08f, 0.95f, -0.31f, 0.00f, 0.70f, 0.47f,
1.54f , -4.74f , 0.39f, 0.31f, -0.95f, 0.08f, 0.95f, 0.31f, 0.00f, 0.80f, 0.47f,
4.03f , -2.93f , 0.39f, 0.81f, -0.59f, 0.08f, 0.59f, 0.81f, 0.00f, 0.90f, 0.47f,

4.98f , 0.00f , -0.39f, 1.00f, 0.00f, -0.08f, -0.00f, 1.00f, 0.00f, 0.00f, 0.53f,
4.03f , 2.93f , -0.39f, 0.81f, 0.59f, -0.08f, -0.59f, 0.81f, 0.00f, 0.10f, 0.53f,
1.54f , 4.74f , -0.39f, 0.31f, 0.95f, -0.08f, -0.95f, 0.31f, 0.00f, 0.20f, 0.53f,
-1.54f , 4.74f , -0.39f, -0.31f, 0.95f, -0.08f, -0.95f, -0.31f, 0.00f, 0.30f, 0.53f,
-4.03f , 2.93f , -0.39f, -0.81f, 0.59f, -0.08f, -0.59f, -0.81f, 0.00f, 0.40f, 0.53f,
-4.98f , 0.00f , -0.39f, -1.00f, 0.00f, -0.08f, -0.00f, -1.00f, 0.00f, 0.50f, 0.53f,
-4.03f , -2.93f , -0.39f, -0.81f, -0.59f, -0.08f, 0.59f, -0.81f, 0.00f, 0.60f, 0.53f,
-1.54f , -4.74f , -0.39f, -0.31f, -0.95f, -0.08f, 0.95f, -0.31f, 0.00f, 0.70f, 0.53f, 
1.54f , -4.74f , -0.39f, 0.31f, -0.95f, -0.08f, 0.95f, 0.31f, 0.00f, 0.80f, 0.53f,
4.03f , -2.93f , -0.39f, 0.81f, -0.59f, -0.08f, 0.59f, 0.81f, 0.00f, 0.90f, 0.53f,

4.86f , 0.00f , -1.17f, 0.97f, 0.00f, -0.23f, -0.00f, 1.00f, 0.00f, 0.00f, 0.57f,
3.93f , 2.86f , -1.17f, 0.79f, 0.57f, -0.23f, -0.59f, 0.81f, 0.00f, 0.10f, 0.57f,
1.50f , 4.62f , -1.17f, 0.30f, 0.92f, -0.23f, -0.95f, 0.31f, 0.00f, 0.20f, 0.57f,
-1.50f , 4.62f , -1.17f, -0.30f, 0.92f, -0.23f, -0.95f, -0.31f, 0.00f, 0.30f, 0.57f,
-3.93f , 2.86f , -1.17f, -0.79f, 0.57f, -0.23f, -0.59f, -0.81f, 0.00f, 0.40f, 0.57f,
-4.86f , 0.00f , -1.17f, -0.97f, 0.00f, -0.23f, -0.00f, -1.00f, 0.00f, 0.50f, 0.57f,
-3.93f , -2.86f , -1.17f, -0.79f, -0.57f, -0.23f, 0.59f, -0.81f, 0.00f, 0.60f, 0.57f,
-1.50f , -4.62f , -1.17f, -0.30f, -0.92f, -0.23f, 0.95f, -0.31f, 0.00f, 0.70f, 0.57f,
1.50f , -4.62f , -1.17f, 0.30f, -0.92f, -0.23f, 0.95f, 0.31f, 0.00f, 0.80f, 0.57f,
3.93f , -2.86f , -1.17f, 0.79f, -0.57f, -0.23f, 0.59f, 0.81f, 0.00f, 0.90f, 0.57f,

4.62f , 0.00f , -1.91f, 0.92f, 0.00f, -0.38f, -0.00f, 1.00f, 0.00f, 0.00f, 0.62f,
3.74f , 2.72f , -1.91f, 0.75f, 0.54f, -0.38f, -0.59f, 0.81f, 0.00f, 0.10f, 0.62f,
1.43f , 4.39f , -1.91f, 0.29f, 0.88f, -0.38f, -0.95f, 0.31f, 0.00f, 0.20f, 0.62f,
-1.43f , 4.39f , -1.91f, -0.29f, 0.88f, -0.38f, -0.95f, -0.31f, 0.00f, 0.30f, 0.62f,
-3.74f , 2.72f , -1.91f, -0.75f, 0.54f, -0.38f, -0.59f, -0.81f, 0.00f, 0.40f, 0.62f,
-4.62f , 0.00f , -1.91f, -0.92f, 0.00f, -0.38f, -0.00f, -1.00f, 0.00f, 0.50f, 0.62f,
-3.74f , -2.72f , -1.91f, -0.75f, -0.54f, -0.38f, 0.59f, -0.81f, 0.00f, 0.60f, 0.62f,
-1.43f , -4.39f , -1.91f, -0.29f, -0.88f, -0.38f, 0.95f, -0.31f, 0.00f, 0.70f, 0.62f, 
1.43f , -4.39f , -1.91f, 0.29f, -0.88f, -0.38f, 0.95f, 0.31f, 0.00f, 0.80f, 0.62f,
3.74f , -2.72f , -1.91f, 0.75f, -0.54f, -0.38f, 0.59f, 0.81f, 0.00f, 0.90f, 0.62f,

4.26f , 0.00f , -2.61f, 0.85f, 0.00f, -0.52f, -0.00f, 1.00f, 0.00f, 0.00f, 0.68f,
3.45f , 2.51f , -2.61f, 0.69f, 0.50f, -0.52f, -0.59f, 0.81f, 0.00f, 0.10f, 0.68f,
1.32f , 4.05f , -2.61f, 0.26f, 0.81f, -0.52f, -0.95f, 0.31f, 0.00f, 0.20f, 0.68f,
-1.32f , 4.05f , -2.61f, -0.26f, 0.81f, -0.52f, -0.95f, -0.31f, 0.00f, 0.30f, 0.68f,
-3.45f , 2.51f , -2.61f, -0.69f, 0.50f, -0.52f, -0.59f, -0.81f, 0.00f, 0.40f, 0.68f,
-4.26f , 0.00f , -2.61f, -0.85f, 0.00f, -0.52f, -0.00f, -1.00f, 0.00f, 0.50f, 0.68f,
-3.45f , -2.51f , -2.61f, -0.69f, -0.50f, -0.52f, 0.59f, -0.81f, 0.00f, 0.60f, 0.68f,
-1.32f , -4.05f , -2.61f, -0.26f, -0.81f, -0.52f, 0.95f, -0.31f, 0.00f, 0.70f, 0.68f,
1.32f , -4.05f , -2.61f, 0.26f, -0.81f, -0.52f, 0.95f, 0.31f, 0.00f, 0.80f, 0.68f,
3.45f , -2.51f , -2.61f, 0.69f, -0.50f, -0.52f, 0.59f, 0.81f, 0.00f, 0.90f, 0.68f,

3.80f , 0.00f , -3.25f, 0.76f, 0.00f, -0.65f, -0.00f, 1.00f, 0.00f, 0.00f, 0.72f,
3.08f , 2.23f , -3.25f, 0.62f, 0.45f, -0.65f, -0.59f, 0.81f, 0.00f, 0.10f, 0.72f,
1.17f , 3.62f , -3.25f, 0.23f, 0.72f, -0.65f, -0.95f, 0.31f, 0.00f, 0.20f, 0.72f,
-1.17f , 3.62f , -3.25f, -0.23f, 0.72f, -0.65f, -0.95f, -0.31f, 0.00f, 0.30f, 0.72f,
-3.08f , 2.23f , -3.25f, -0.62f, 0.45f, -0.65f, -0.59f, -0.81f, 0.00f, 0.40f, 0.72f,
-3.80f , 0.00f , -3.25f, -0.76f, 0.00f, -0.65f, -0.00f, -1.00f, 0.00f, 0.50f, 0.72f,
-3.08f , -2.23f , -3.25f, -0.62f, -0.45f, -0.65f, 0.59f, -0.81f, 0.00f, 0.60f, 0.72f,
-1.17f , -3.62f , -3.25f, -0.23f, -0.72f, -0.65f, 0.95f, -0.31f, 0.00f, 0.70f, 0.72f,
1.17f , -3.62f , -3.25f, 0.23f, -0.72f, -0.65f, 0.95f, 0.31f, 0.00f, 0.80f, 0.72f, 
3.08f , -2.23f , -3.25f, 0.62f, -0.45f, -0.65f, 0.59f, 0.81f, 0.00f, 0.90f, 0.72f,

3.25f , 0.00f , -3.80f, 0.65f, 0.00f, -0.76f, -0.00f, 1.00f, 0.00f, 0.00f, 0.78f,
2.63f , 1.91f , -3.80f, 0.53f, 0.38f, -0.76f, -0.59f, 0.81f, 0.00f, 0.10f, 0.78f,
1.00f , 3.09f , -3.80f, 0.20f, 0.62f, -0.76f, -0.95f, 0.31f, 0.00f, 0.20f, 0.78f,
-1.00f , 3.09f , -3.80f, -0.20f, 0.62f, -0.76f, -0.95f, -0.31f, 0.00f, 0.30f, 0.78f,
-2.63f , 1.91f , -3.80f, -0.53f, 0.38f, -0.76f, -0.59f, -0.81f, 0.00f, 0.40f, 0.78f,
-3.25f , 0.00f , -3.80f, -0.65f, 0.00f, -0.76f, -0.00f, -1.00f, 0.00f, 0.50f, 0.78f,
-2.63f , -1.91f , -3.80f, -0.53f, -0.38f, -0.76f, 0.59f, -0.81f, 0.00f, 0.60f, 0.78f,
-1.00f , -3.09f , -3.80f, -0.20f, -0.62f, -0.76f, 0.95f, -0.31f, 0.00f, 0.70f, 0.78f,
1.00f , -3.09f , -3.80f, 0.20f, -0.62f, -0.76f, 0.95f, 0.31f, 0.00f, 0.80f, 0.78f,
2.63f , -1.91f , -3.80f, 0.53f, -0.38f, -0.76f, 0.59f, 0.81f, 0.00f, 0.90f, 0.78f,

2.61f , 0.00f , -4.26f, 0.52f, 0.00f, -0.85f, -0.00f, 1.00f, 0.00f, 0.00f, 0.82f,
2.11f , 1.54f , -4.26f, 0.42f, 0.31f, -0.85f, -0.59f, 0.81f, 0.00f, 0.10f, 0.82f,
0.81f , 2.48f , -4.26f, 0.16f, 0.50f, -0.85f, -0.95f, 0.31f, 0.00f, 0.20f, 0.82f,
-0.81f , 2.48f , -4.26f, -0.16f, 0.50f, -0.85f, -0.95f, -0.31f, 0.00f, 0.30f, 0.82f,
-2.11f , 1.54f , -4.26f, -0.42f, 0.31f, -0.85f, -0.59f, -0.81f, 0.00f, 0.40f, 0.82f,
-2.61f , 0.00f , -4.26f, -0.52f, 0.00f, -0.85f, -0.00f, -1.00f, 0.00f, 0.50f, 0.82f,
-2.11f , -1.54f , -4.26f, -0.42f, -0.31f, -0.85f, 0.59f, -0.81f, 0.00f, 0.60f, 0.82f,
-0.81f , -2.48f , -4.26f, -0.16f, -0.50f, -0.85f, 0.95f, -0.31f, 0.00f, 0.70f, 0.82f,
0.81f , -2.48f , -4.26f, 0.16f, -0.50f, -0.85f, 0.95f, 0.31f, 0.00f, 0.80f, 0.82f,
2.11f , -1.54f , -4.26f, 0.42f, -0.31f, -0.85f, 0.59f, 0.81f, 0.00f, 0.90f, 0.82f,

1.91f , 0.00f , -4.62f, 0.38f, 0.00f, -0.92f, -0.00f, 1.00f, 0.00f, 0.00f, 0.88f, 
1.55f , 1.12f , -4.62f, 0.31f, 0.22f, -0.92f, -0.59f, 0.81f, 0.00f, 0.10f, 0.88f,
0.59f , 1.82f , -4.62f, 0.12f, 0.36f, -0.92f, -0.95f, 0.31f, 0.00f, 0.20f, 0.88f,
-0.59f , 1.82f , -4.62f, -0.12f, 0.36f, -0.92f, -0.95f, -0.31f, 0.00f, 0.30f, 0.88f,
-1.55f , 1.12f , -4.62f, -0.31f, 0.22f, -0.92f, -0.59f, -0.81f, 0.00f, 0.40f, 0.88f,
-1.91f , 0.00f , -4.62f, -0.38f, 0.00f, -0.92f, -0.00f, -1.00f, 0.00f, 0.50f, 0.88f,
-1.55f , -1.12f , -4.62f, -0.31f, -0.22f, -0.92f, 0.59f, -0.81f, 0.00f, 0.60f, 0.88f,
-0.59f , -1.82f , -4.62f, -0.12f, -0.36f, -0.92f, 0.95f, -0.31f, 0.00f, 0.70f, 0.88f,
0.59f , -1.82f , -4.62f, 0.12f, -0.36f, -0.92f, 0.95f, 0.31f, 0.00f, 0.80f, 0.88f,
1.55f , -1.12f , -4.62f, 0.31f, -0.22f, -0.92f, 0.59f, 0.81f, 0.00f, 0.90f, 0.88f,

1.17f , 0.00f , -4.86f, 0.23f, 0.00f, -0.97f, -0.00f, 1.00f, 0.00f, 0.00f, 0.93f,
0.94f , 0.69f , -4.86f, 0.19f, 0.14f, -0.97f, -0.59f, 0.81f, 0.00f, 0.10f, 0.93f,
0.36f , 1.11f , -4.86f, 0.07f, 0.22f, -0.97f, -0.95f, 0.31f, 0.00f, 0.20f, 0.93f,
-0.36f , 1.11f , -4.86f, -0.07f, 0.22f, -0.97f, -0.95f, -0.31f, 0.00f, 0.30f, 0.93f,
-0.94f , 0.69f , -4.86f, -0.19f, 0.14f, -0.97f, -0.59f, -0.81f, 0.00f, 0.40f, 0.93f,
-1.17f , 0.00f , -4.86f, -0.23f, 0.00f, -0.97f, -0.00f, -1.00f, 0.00f, 0.50f, 0.93f,
-0.94f , -0.69f , -4.86f, -0.19f, -0.14f, -0.97f, 0.59f, -0.81f, 0.00f, 0.60f, 0.93f,
-0.36f , -1.11f , -4.86f, -0.07f, -0.22f, -0.97f, 0.95f, -0.31f, 0.00f, 0.70f, 0.93f,
0.36f , -1.11f , -4.86f, 0.07f, -0.22f, -0.97f, 0.95f, 0.31f, 0.00f, 0.80f, 0.93f,
0.94f , -0.69f , -4.86f, 0.19f, -0.14f, -0.97f, 0.59f, 0.81f, 0.00f, 0.90f, 0.93f,

0.39f , 0.00f , -4.98f, 0.08f, 0.00f, -1.00f, -0.00f, 1.00f, 0.00f, 0.00f, 0.97f,
0.32f , 0.23f , -4.98f, 0.06f, 0.05f, -1.00f, -0.59f, 0.81f, 0.00f, 0.10f, 0.97f, 
0.12f , 0.37f , -4.98f, 0.02f, 0.07f, -1.00f, -0.95f, 0.31f, 0.00f, 0.20f, 0.97f,
-0.12f , 0.37f , -4.98f, -0.02f, 0.07f, -1.00f, -0.95f, -0.31f, 0.00f, 0.30f, 0.97f,
-0.32f , 0.23f , -4.98f, -0.06f, 0.05f, -1.00f, -0.59f, -0.81f, 0.00f, 0.40f, 0.97f,
-0.39f , 0.00f , -4.98f, -0.08f, 0.00f, -1.00f, -0.00f, -1.00f, 0.00f, 0.50f, 0.97f,
-0.32f , -0.23f , -4.98f, -0.06f, -0.05f, -1.00f, 0.59f, -0.81f, 0.00f, 0.60f, 0.97f,
-0.12f , -0.37f , -4.98f, -0.02f, -0.07f, -1.00f, 0.95f, -0.31f, 0.00f, 0.70f, 0.97f,
0.12f , -0.37f , -4.98f, 0.02f, -0.07f, -1.00f, 0.95f, 0.31f, 0.00f, 0.80f, 0.97f,
0.32f , -0.23f , -4.98f, 0.06f, -0.05f, -1.00f, 0.59f, 0.81f, 0.00f, 0.90f, 0.97f,
};

float planeVertices []={
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 4.0f,};

float coneVertices []={
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 4.0f,};
/* ===== INDICES GENERATED THROUGH CODE BELOW =====
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double PI = 3.14159265358979323846;

int main()
{
    int numCircles = 20; // number of circles to generate
    int numPoints = 10; // number of points to generate

    int numIndices = (numCircles-1) * numPoints * 6; // number of indices needed for all triangles
    int* indices = new int[numIndices]; // allocate memory for indices array

    int idx = 0;
    for (int j = 0; j < numCircles-1; j++) // loop through each circle except the last one
    {
        for (int i = 0; i < numPoints; i++) // loop through each point on the circle
        {
            int k = j * numPoints + i; // index of current point
            int next_k = (j + 1) * numPoints + i; // index of corresponding point on next circle

            // indices of the four vertices of the current quad
            int v1 = k;
            int v2 = k + 1;
            int v3 = next_k;
            int v4 = next_k + 1;

            // wrap around for last point on circle
            if (i == numPoints - 1) {
                v2 -= numPoints;
                v4 -= numPoints;
            }

            // add indices for first triangle
            indices[idx++] = v1;
            indices[idx++] = v3;
            indices[idx++] = v2;

            // add indices for second triangle
            indices[idx++] = v2;
            indices[idx++] = v3;
            indices[idx++] = v4;
        }
    }

    // output the indices array with 3 points per line
    for (int i = 0; i < numIndices; i++) {
        cout << indices[i] << ",";
        if ((i+1) % 3 == 0) {
            cout << endl; // start a new line after every third index
        }
    }

    delete[] indices; // free memory allocated for indices array
    return 0;
}

*/
GLuint planeIndices[] =
{
    0,10,1,
    1,10,11,
    1,11,2,
    2,11,12,
    2,12,3,
    3,12,13,
    3,13,4,
    4,13,14,
    4,14,5,
    5,14,15,
    5,15,6,
    6,15,16,
    6,16,7,
    7,16,17,
    7,17,8,
    8,17,18,
    8,18,9,
    9,18,19,
    9,19,0,
    0,19,10,
    10,20,11,
    11,20,21,
    11,21,12,
    12,21,22,
    12,22,13,
    13,22,23,
    13,23,14,
    14,23,24,
    14,24,15,
    15,24,25,
    15,25,16,
    16,25,26,
    16,26,17,
    17,26,27,
    17,27,18,
    18,27,28,
    18,28,19,
    19,28,29,
    19,29,10,
    10,29,20,
    20,30,21,
    21,30,31,
    21,31,22,
    22,31,32,
    22,32,23,
    23,32,33,
    23,33,24,
    24,33,34,
    24,34,25,
    25,34,35,
    25,35,26,
    26,35,36,
    26,36,27,
    27,36,37,
    27,37,28,
    28,37,38,
    28,38,29,
    29,38,39,
    29,39,20,
    20,39,30,
    30,40,31,
    31,40,41,
    31,41,32,
    32,41,42,
    32,42,33,
    33,42,43,
    33,43,34,
    34,43,44,
    34,44,35,
    35,44,45,
    35,45,36,
    36,45,46,
    36,46,37,
    37,46,47,
    37,47,38,
    38,47,48,
    38,48,39,
    39,48,49,
    39,49,30,
    30,49,40,
    40,50,41,
    41,50,51,
    41,51,42,
    42,51,52,
    42,52,43,
    43,52,53,
    43,53,44,
    44,53,54,
    44,54,45,
    45,54,55,
    45,55,46,
    46,55,56,
    46,56,47,
    47,56,57,
    47,57,48,
    48,57,58,
    48,58,49,
    49,58,59,
    49,59,40,
    40,59,50,
    50,60,51,
    51,60,61,
    51,61,52,
    52,61,62,
    52,62,53,
    53,62,63,
    53,63,54,
    54,63,64,
    54,64,55,
    55,64,65,
    55,65,56,
    56,65,66,
    56,66,57,
    57,66,67,
    57,67,58,
    58,67,68,
    58,68,59,
    59,68,69,
    59,69,50,
    50,69,60,
    60,70,61,
    61,70,71,
    61,71,62,
    62,71,72,
    62,72,63,
    63,72,73,
    63,73,64,
    64,73,74,
    64,74,65,
    65,74,75,
    65,75,66,
    66,75,76,
    66,76,67,
    67,76,77,
    67,77,68,
    68,77,78,
    68,78,69,
    69,78,79,
    69,79,60,
    60,79,70,
    70,80,71,
    71,80,81,
    71,81,72,
    72,81,82,
    72,82,73,
    73,82,83,
    73,83,74,
    74,83,84,
    74,84,75,
    75,84,85,
    75,85,76,
    76,85,86,
    76,86,77,
    77,86,87,
    77,87,78,
    78,87,88,
    78,88,79,
    79,88,89,
    79,89,70,
    70,89,80,
    80,90,81,
    81,90,91,
    81,91,82,
    82,91,92,
    82,92,83,
    83,92,93,
    83,93,84,
    84,93,94,
    84,94,85,
    85,94,95,
    85,95,86,
    86,95,96,
    86,96,87,
    87,96,97,
    87,97,88,
    88,97,98,
    88,98,89,
    89,98,99,
    89,99,80,
    80,99,90,
    90,100,91,
    91,100,101,
    91,101,92,
    92,101,102,
    92,102,93,
    93,102,103,
    93,103,94,
    94,103,104,
    94,104,95,
    95,104,105,
    95,105,96,
    96,105,106,
    96,106,97,
    97,106,107,
    97,107,98,
    98,107,108,
    98,108,99,
    99,108,109,
    99,109,90,
    90,109,100,
    100,110,101,
    101,110,111,
    101,111,102,
    102,111,112,
    102,112,103,
    103,112,113,
    103,113,104,
    104,113,114,
    104,114,105,
    105,114,115,
    105,115,106,
    106,115,116,
    106,116,107,
    107,116,117,
    107,117,108,
    108,117,118,
    108,118,109,
    109,118,119,
    109,119,100,
    100,119,110,
    110,120,111,
    111,120,121,
    111,121,112,
    112,121,122,
    112,122,113,
    113,122,123,
    113,123,114,
    114,123,124,
    114,124,115,
    115,124,125,
    115,125,116,
    116,125,126,
    116,126,117,
    117,126,127,
    117,127,118,
    118,127,128,
    118,128,119,
    119,128,129,
    119,129,110,
    110,129,120,
    120,130,121,
    121,130,131,
    121,131,122,
    122,131,132,
    122,132,123,
    123,132,133,
    123,133,124,
    124,133,134,
    124,134,125,
    125,134,135,
    125,135,126,
    126,135,136,
    126,136,127,
    127,136,137,
    127,137,128,
    128,137,138,
    128,138,129,
    129,138,139,
    129,139,120,
    120,139,130,
    130,140,131,
    131,140,141,
    131,141,132,
    132,141,142,
    132,142,133,
    133,142,143,
    133,143,134,
    134,143,144,
    134,144,135,
    135,144,145,
    135,145,136,
    136,145,146,
    136,146,137,
    137,146,147,
    137,147,138,
    138,147,148,
    138,148,139,
    139,148,149,
    139,149,130,
    130,149,140,
    140,150,141,
    141,150,151,
    141,151,142,
    142,151,152,
    142,152,143,
    143,152,153,
    143,153,144,
    144,153,154,
    144,154,145,
    145,154,155,
    145,155,146,
    146,155,156,
    146,156,147,
    147,156,157,
    147,157,148,
    148,157,158,
    148,158,149,
    149,158,159,
    149,159,140,
    140,159,150,
    150,160,151,
    151,160,161,
    151,161,152,
    152,161,162,
    152,162,153,
    153,162,163,
    153,163,154,
    154,163,164,
    154,164,155,
    155,164,165,
    155,165,156,
    156,165,166,
    156,166,157,
    157,166,167,
    157,167,158,
    158,167,168,
    158,168,159,
    159,168,169,
    159,169,150,
    150,169,160,
    160,170,161,
    161,170,171,
    161,171,162,
    162,171,172,
    162,172,163,
    163,172,173,
    163,173,164,
    164,173,174,
    164,174,165,
    165,174,175,
    165,175,166,
    166,175,176,
    166,176,167,
    167,176,177,
    167,177,168,
    168,177,178,
    168,178,169,
    169,178,179,
    169,179,160,
    160,179,170,
    170,180,171,
    171,180,181,
    171,181,172,
    172,181,182,
    172,182,173,
    173,182,183,
    173,183,174,
    174,183,184,
    174,184,175,
    175,184,185,
    175,185,176,
    176,185,186,
    176,186,177,
    177,186,187,
    177,187,178,
    178,187,188,
    178,188,179,
    179,188,189,
    179,189,170,
    170,189,180,
    180,190,181,
    181,190,191,
    181,191,182,
    182,191,192,
    182,192,183,
    183,192,193,
    183,193,184,
    184,193,194,
    184,194,185,
    185,194,195,
    185,195,186,
    186,195,196,
    186,196,187,
    187,196,197,
    187,197,188,
    188,197,198,
    188,198,189,
    189,198,199,
    189,199,180,
    180,199,190,
};
// helper struct for defining spherical polar coordinates
struct polar
{
    float radius      =   8.0f;   // distance from the origin
    float inclination = -20.0f;   // angle on the YZ vertical plane
    float azimuth     =  45.0f;   // angle on the XZ horizontal plane

    // sanity ranges to prevent strange behavior like flipping axes, etc.
    // (you can change these as you see fit)
    static constexpr float minRadius      =   0.1f;
    static constexpr float maxRadius      =  20.0f;
    static constexpr float minInclination = -89.0f;
    static constexpr float maxInclination =  89.0f;

    // restricts the coordinates to sanity ranges
    void clamp()
    {
        if (radius < minRadius)
            radius = minRadius;
        if (radius > maxRadius)
            radius = maxRadius;
        if (inclination < minInclination)
            inclination = minInclination;
        if (inclination > maxInclination)
            inclination = maxInclination;
    }

    // converts the spherical polar coordinates to a vec3 in Cartesian coordinates
    glm::vec3 toCartesian()
    {
        glm::mat4 mat = glm::mat4(1.0f);  // set to identity first
        mat = glm::rotate(mat, glm::radians(azimuth), glm::vec3(0.0f, 1.0f, 0.0f));
        mat = glm::rotate(mat, glm::radians(inclination), glm::vec3(1.0f, 0.0f, 0.0f));
        return mat * glm::vec4(0.0f, 0.0f, radius, 1.0f);
    }
};

// variables for tracking camera and light position
polar camera;
glm::vec3 lightPosition = glm::vec3(0.0f, 10.0f, 0.0f);

float spotlightCutoff = 75.0f;
float spotlightOuterAngle = 200.0f;

float ambientIntensity = 0.75f;
float specularIntensity = 5.0f;
float specularPower = 50.0f;

double previousTime = 0.0;

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // upload the model to the GPU (explanations omitted for brevity)
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (9 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

        // upload the model to the GPU (explanations omitted for brevity)
    glGenVertexArrays(1, &vao2);
    glGenBuffers(1, &vbo2);
    glBindVertexArray(vao2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (9 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glGenVertexArrays(1, &vao3);
    glGenBuffers(1, &vbo3);
    glBindVertexArray(vao3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coneVertices), coneVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (9 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // load our shader program
    shader = gdevLoadShader("demo5n.vs", "demo5n.fs");
    if (! shader)
        return false;

    // since we now use multiple textures, we need to set the texture channel for each texture
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader, "normalMap"),  1);

    // load our textures
    texture[0] = gdevLoadTexture("cone.jpeg", GL_REPEAT, true, true);
    texture[1] = gdevLoadTexture("conenormalmap.png", GL_REPEAT, true, true);
    if (! texture[0] || ! texture[1])
        return false;

    texturePlane[0] = gdevLoadTexture("exercise2diffusemap.png", GL_REPEAT, true, true);
    texturePlane[1] = gdevLoadTexture("exercise2normalmap.png", GL_REPEAT, true, true);
    if (! texturePlane[0] || ! texturePlane[1])
        return false;

    // enable z-buffer depth testing and face culling
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // find the elapsed time since the last frame
    double currentTime = glfwGetTime();
    double elapsedTime = (currentTime - previousTime);
    previousTime = currentTime;

    // define how much to rotate and translate according to time
    float rotationAmount = 100.0f * elapsedTime;
    float translationAmount = 10.0f * elapsedTime;

    // handle key events for camera
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera.radius -= translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
        camera.radius += translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera.azimuth -= rotationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera.azimuth += rotationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
        camera.inclination += rotationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
        camera.inclination -= rotationAmount;
    camera.clamp();

    // get the Cartesian coordinates of the camera
    glm::vec3 cameraPosition = camera.toCartesian();

    // get a "forward" vector for controlling the light position
    glm::vec3 lightForward = glm::normalize(glm::vec3(-cameraPosition.x, 0.0f, -cameraPosition.z));

    if (glfwGetKey(pWindow, GLFW_KEY_I) == GLFW_PRESS)
        lightPosition += lightForward * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_K) == GLFW_PRESS)
        lightPosition -= lightForward * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_J) == GLFW_PRESS)
        lightPosition -= glm::cross(lightForward, glm::vec3(0.0f, 1.0f, 0.0f)) * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_L) == GLFW_PRESS)
        lightPosition += glm::cross(lightForward, glm::vec3(0.0f, 1.0f, 0.0f)) * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_U) == GLFW_PRESS)
        lightPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_O) == GLFW_PRESS)
        lightPosition += glm::vec3(0.0f, 1.0f, 0.0f) * translationAmount;
    if (glfwGetKey(pWindow, GLFW_KEY_Z) == GLFW_PRESS)
        ambientIntensity += 0.01f;
    if (glfwGetKey(pWindow, GLFW_KEY_X) == GLFW_PRESS)
        ambientIntensity -= 0.01f;
    if (glfwGetKey(pWindow, GLFW_KEY_C) == GLFW_PRESS)
        specularIntensity += 0.01f;
    if (glfwGetKey(pWindow, GLFW_KEY_V) == GLFW_PRESS)
        specularIntensity -= 0.01f;
    if (glfwGetKey(pWindow, GLFW_KEY_B) == GLFW_PRESS)
        specularPower+= 0.1f;
    if (glfwGetKey(pWindow, GLFW_KEY_N) == GLFW_PRESS)
        specularPower -= 0.1f;
    if (glfwGetKey(pWindow, GLFW_KEY_1) == GLFW_PRESS)
        spotlightCutoff += 0.1f;
    if (glfwGetKey(pWindow, GLFW_KEY_2) == GLFW_PRESS)
        spotlightCutoff -= 0.1f;
    if (glfwGetKey(pWindow, GLFW_KEY_3) == GLFW_PRESS)
        spotlightOuterAngle += 0.1f;
    if (glfwGetKey(pWindow, GLFW_KEY_4) == GLFW_PRESS)
        spotlightOuterAngle -= 0.1f;


    // clear the whole frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);

    // ... set up the projection matrix...
    glm::mat4 projectionTransform;
    projectionTransform = glm::perspective(glm::radians(45.0f),                   // fov
                                           (float) WINDOW_WIDTH / WINDOW_HEIGHT,  // aspect ratio
                                           0.1f,                                  // near plane
                                           100.0f);                               // far plane
    glUniformMatrix4fv(glGetUniformLocation(shader, "projectionTransform"),
                       1, GL_FALSE, glm::value_ptr(projectionTransform));

    // ... set up the view matrix...
    glm::mat4 viewTransform;
    viewTransform = glm::lookAt(cameraPosition,                // eye position
                                glm::vec3(0.0f, 0.0f, 0.0f),   // center position
                                glm::vec3(0.0f, 1.0f, 0.0f));  // up vector
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewTransform"),
                       1, GL_FALSE, glm::value_ptr(viewTransform));
    
    // ... set up the light position...
    glUniform3fv(glGetUniformLocation(shader, "lightPosition"),
                 1, glm::value_ptr(lightPosition));

    glUniform1f(glGetUniformLocation(shader, "uniformAmbientIntensity"), ambientIntensity);
    glUniform1f(glGetUniformLocation(shader, "uniformSpecularIntensity"), specularIntensity);
    glUniform1f(glGetUniformLocation(shader, "uniformSpecularPower"), specularPower);

    glUniform1f(glGetUniformLocation(shader,"spotlightCutoff"),glm::cos(glm::radians(spotlightCutoff)));
    glUniform1f(glGetUniformLocation(shader,"spotlightOuterAngle"), glm::cos(glm::radians(spotlightOuterAngle)));

    // CONE MODEL
    glm::mat4 modelTransform = glm::mat4(1.0f);
    modelTransform = glm::scale(modelTransform, glm::vec3(0.75f, 0.75f, 0.75f));
    modelTransform = glm::translate(modelTransform, glm::vec3(0.0f,3.0f, 0.0f));
    modelTransform = glm::rotate(modelTransform, glm::radians((float) 145.0f), glm::vec3(1.0f,0.0f,0.0f));
    modelTransform = glm::rotate(modelTransform, glm::radians((float) currentTime * 90), glm::vec3(0.0f,0.0f,1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBindVertexArray(vao);
    
    /*for (int i = 0; i < 2; i++)
    {
        glDrawArrays(GL_LINE_LOOP, i * 10, 10);
    }*/
    
    
    glDrawElements(GL_TRIANGLES, 1500, GL_UNSIGNED_INT, planeIndices);
    glDrawArrays(GL_TRIANGLE_FAN, 190, 10);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
    //glDrawArrays(GL_LINE_LOOP, 1, sizeof(vertices));

    glm::mat4 planeTransform = glm::mat4(1.0f);
    planeTransform = glm::translate(planeTransform, glm::vec3(0.0f,-5.0f, 0.0f));
    planeTransform = glm::scale(planeTransform, glm::vec3(1.5f, 0.0f,1.5f));
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(planeTransform));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturePlane[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texturePlane[1]);

    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(planeVertices) / (8 * sizeof(float)));

    glDrawArrays(GL_TRIANGLE_FAN, 190, 10);
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
}

// handler called by GLFW when the window is resized
void handleResize(GLFWwindow* pWindow, int width, int height)
{
    // tell OpenGL to do its drawing within the entire "client area" (area within the borders) of the window
    glViewport(0, 0, width, height);
}

// main function
int main(int argc, char** argv)
{
    // initialize GLFW and ask for OpenGL 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create a GLFW window with the specified width, height, and title
    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (! pWindow)
    {
        // gracefully terminate if we cannot create the window
        std::cout << "Cannot create the GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    // enable vertical sync and aspect-ratio correction on the GLFW window,
    // and make the window the current context of subsequent OpenGL commands
    glfwSwapInterval(1);
    glfwSetWindowAspectRatio(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwMakeContextCurrent(pWindow);

    // set up callback functions to handle window system events
    glfwSetKeyCallback(pWindow, handleKeys);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setup())
    {
        // do rendering in a loop until the user closes the window
        while (! glfwWindowShouldClose(pWindow))
        {
            // render our next frame
            // (by default, GLFW uses double-buffering with a front and back buffer;
            // all drawing goes to the back buffer, so the frame does not get shown yet)
            render();

            // swap the GLFW front and back buffers to show the next frame
            glfwSwapBuffers(pWindow);

            // process any window events (such as moving, resizing, keyboard presses, etc.)
            glfwPollEvents();
        }
    }

    // gracefully terminate the program
    glfwTerminate();
    return 0;
}
