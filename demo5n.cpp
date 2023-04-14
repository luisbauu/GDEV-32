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
#define WINDOW_TITLE  "Exercise2 (use WASDQE keys for camera, IKJLUO keys for light, ZX keys for Ambient Intensity, CV keys for Specular Intensity, BN keys for Specular Power, 1-2 for Spotlight Cutoff, 3-4 for Spotlight Outer Angle, 5 Cull Back-Facing Triangles)"
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
GLuint vao, vao2;
GLuint vbo, vbo2;
GLuint shader;
GLuint texture[2], texturePlane[2];
bool backfaceCulled = false;


float vertices[] = 
{
    0.00f , 0.00f , 15.00f, 0.00f, 0.00f, 10.00f, 1.00f, 0.00f, 0.00f, 0.5f, 0.5f,

    5.00f , 0.00f , 0.00f, -0.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 
    4.99f , 0.31f , 0.00f, -0.06f, 1.00f, 0.00f, 1.00f, 0.06f, 0.00f, 0.01f, 1.00f, 
    4.96f , 0.63f , 0.00f, -0.13f, 0.99f, 0.00f, 0.99f, 0.13f, 0.00f, 0.02f, 1.00f, 
    4.91f , 0.94f , 0.00f, -0.19f, 0.98f, 0.00f, 0.98f, 0.19f, 0.00f, 0.03f, 1.00f, 
    4.84f , 1.24f , 0.00f, -0.25f, 0.97f, 0.00f, 0.97f, 0.25f, 0.00f, 0.04f, 1.00f, 
    4.76f , 1.55f , 0.00f, -0.31f, 0.95f, 0.00f, 0.95f, 0.31f, 0.00f, 0.05f, 1.00f, 
    4.65f , 1.84f , 0.00f, -0.37f, 0.93f, 0.00f, 0.93f, 0.37f, 0.00f, 0.06f, 1.00f, 
    4.52f , 2.13f , 0.00f, -0.43f, 0.90f, 0.00f, 0.90f, 0.43f, 0.00f, 0.07f, 1.00f, 
    4.38f , 2.41f , 0.00f, -0.48f, 0.88f, 0.00f, 0.88f, 0.48f, 0.00f, 0.08f, 1.00f, 
    4.22f , 2.68f , 0.00f, -0.54f, 0.84f, 0.00f, 0.84f, 0.54f, 0.00f, 0.09f, 1.00f, 
    4.05f , 2.94f , 0.00f, -0.59f, 0.81f, 0.00f, 0.81f, 0.59f, 0.00f, 0.10f, 1.00f, 
    3.85f , 3.19f , 0.00f, -0.64f, 0.77f, 0.00f, 0.77f, 0.64f, 0.00f, 0.11f, 1.00f, 
    3.64f , 3.42f , 0.00f, -0.68f, 0.73f, 0.00f, 0.73f, 0.68f, 0.00f, 0.12f, 1.00f, 
    3.42f , 3.64f , 0.00f, -0.73f, 0.68f, 0.00f, 0.68f, 0.73f, 0.00f, 0.13f, 1.00f, 
    3.19f , 3.85f , 0.00f, -0.77f, 0.64f, 0.00f, 0.64f, 0.77f, 0.00f, 0.14f, 1.00f, 
    2.94f , 4.05f , 0.00f, -0.81f, 0.59f, 0.00f, 0.59f, 0.81f, 0.00f, 0.15f, 1.00f, 
    2.68f , 4.22f , 0.00f, -0.84f, 0.54f, 0.00f, 0.54f, 0.84f, 0.00f, 0.16f, 1.00f, 
    2.41f , 4.38f , 0.00f, -0.88f, 0.48f, 0.00f, 0.48f, 0.88f, 0.00f, 0.17f, 1.00f, 
    2.13f , 4.52f , 0.00f, -0.90f, 0.43f, 0.00f, 0.43f, 0.90f, 0.00f, 0.18f, 1.00f, 
    1.84f , 4.65f , 0.00f, -0.93f, 0.37f, 0.00f, 0.37f, 0.93f, 0.00f, 0.19f, 1.00f, 
    1.55f , 4.76f , 0.00f, -0.95f, 0.31f, 0.00f, 0.31f, 0.95f, 0.00f, 0.20f, 1.00f, 
    1.24f , 4.84f , 0.00f, -0.97f, 0.25f, 0.00f, 0.25f, 0.97f, 0.00f, 0.21f, 1.00f, 
    0.94f , 4.91f , 0.00f, -0.98f, 0.19f, 0.00f, 0.19f, 0.98f, 0.00f, 0.22f, 1.00f, 
    0.63f , 4.96f , 0.00f, -0.99f, 0.13f, 0.00f, 0.13f, 0.99f, 0.00f, 0.23f, 1.00f, 
    0.31f , 4.99f , 0.00f, -1.00f, 0.06f, 0.00f, 0.06f, 1.00f, 0.00f, 0.24f, 1.00f, 
    0.00f , 5.00f , 0.00f, -1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.25f, 1.00f, 
    -0.31f , 4.99f , 0.00f, -1.00f, -0.06f, 0.00f, -0.06f, 1.00f, 0.00f, 0.26f, 1.00f, 
    -0.63f , 4.96f , 0.00f, -0.99f, -0.13f, 0.00f, -0.13f, 0.99f, 0.00f, 0.27f, 1.00f, 
    -0.94f , 4.91f , 0.00f, -0.98f, -0.19f, 0.00f, -0.19f, 0.98f, 0.00f, 0.28f, 1.00f, 
    -1.24f , 4.84f , 0.00f, -0.97f, -0.25f, 0.00f, -0.25f, 0.97f, 0.00f, 0.29f, 1.00f, 
    -1.55f , 4.76f , 0.00f, -0.95f, -0.31f, 0.00f, -0.31f, 0.95f, 0.00f, 0.30f, 1.00f, 
    -1.84f , 4.65f , 0.00f, -0.93f, -0.37f, 0.00f, -0.37f, 0.93f, 0.00f, 0.31f, 1.00f, 
    -2.13f , 4.52f , 0.00f, -0.90f, -0.43f, 0.00f, -0.43f, 0.90f, 0.00f, 0.32f, 1.00f, 
    -2.41f , 4.38f , 0.00f, -0.88f, -0.48f, 0.00f, -0.48f, 0.88f, 0.00f, 0.33f, 1.00f, 
    -2.68f , 4.22f , 0.00f, -0.84f, -0.54f, 0.00f, -0.54f, 0.84f, 0.00f, 0.34f, 1.00f, 
    -2.94f , 4.05f , 0.00f, -0.81f, -0.59f, 0.00f, -0.59f, 0.81f, 0.00f, 0.35f, 1.00f, 
    -3.19f , 3.85f , 0.00f, -0.77f, -0.64f, 0.00f, -0.64f, 0.77f, 0.00f, 0.36f, 1.00f, 
    -3.42f , 3.64f , 0.00f, -0.73f, -0.68f, 0.00f, -0.68f, 0.73f, 0.00f, 0.37f, 1.00f, 
    -3.64f , 3.42f , 0.00f, -0.68f, -0.73f, 0.00f, -0.73f, 0.68f, 0.00f, 0.38f, 1.00f, 
    -3.85f , 3.19f , 0.00f, -0.64f, -0.77f, 0.00f, -0.77f, 0.64f, 0.00f, 0.39f, 1.00f, 
    -4.05f , 2.94f , 0.00f, -0.59f, -0.81f, 0.00f, -0.81f, 0.59f, 0.00f, 0.40f, 1.00f, 
    -4.22f , 2.68f , 0.00f, -0.54f, -0.84f, 0.00f, -0.84f, 0.54f, 0.00f, 0.41f, 1.00f, 
    -4.38f , 2.41f , 0.00f, -0.48f, -0.88f, 0.00f, -0.88f, 0.48f, 0.00f, 0.42f, 1.00f, 
    -4.52f , 2.13f , 0.00f, -0.43f, -0.90f, 0.00f, -0.90f, 0.43f, 0.00f, 0.43f, 1.00f, 
    -4.65f , 1.84f , 0.00f, -0.37f, -0.93f, 0.00f, -0.93f, 0.37f, 0.00f, 0.44f, 1.00f, 
    -4.76f , 1.55f , 0.00f, -0.31f, -0.95f, 0.00f, -0.95f, 0.31f, 0.00f, 0.45f, 1.00f, 
    -4.84f , 1.24f , 0.00f, -0.25f, -0.97f, 0.00f, -0.97f, 0.25f, 0.00f, 0.46f, 1.00f, 
    -4.91f , 0.94f , 0.00f, -0.19f, -0.98f, 0.00f, -0.98f, 0.19f, 0.00f, 0.47f, 1.00f, 
    -4.96f , 0.63f , 0.00f, -0.13f, -0.99f, 0.00f, -0.99f, 0.13f, 0.00f, 0.48f, 1.00f, 
    -4.99f , 0.31f , 0.00f, -0.06f, -1.00f, 0.00f, -1.00f, 0.06f, 0.00f, 0.49f, 1.00f, 
    -5.00f , 0.00f , 0.00f, -0.00f, -1.00f, 0.00f, -1.00f, 0.00f, 0.00f, 0.50f, 1.00f, 
    -4.99f , -0.31f , 0.00f, 0.06f, -1.00f, 0.00f, -1.00f, -0.06f, 0.00f, 0.51f, 1.00f, 
    -4.96f , -0.63f , 0.00f, 0.13f, -0.99f, 0.00f, -0.99f, -0.13f, 0.00f, 0.52f, 1.00f, 
    -4.91f , -0.94f , 0.00f, 0.19f, -0.98f, 0.00f, -0.98f, -0.19f, 0.00f, 0.53f, 1.00f, 
    -4.84f , -1.24f , 0.00f, 0.25f, -0.97f, 0.00f, -0.97f, -0.25f, 0.00f, 0.54f, 1.00f, 
    -4.76f , -1.55f , 0.00f, 0.31f, -0.95f, 0.00f, -0.95f, -0.31f, 0.00f, 0.55f, 1.00f, 
    -4.65f , -1.84f , 0.00f, 0.37f, -0.93f, 0.00f, -0.93f, -0.37f, 0.00f, 0.56f, 1.00f, 
    -4.52f , -2.13f , 0.00f, 0.43f, -0.90f, 0.00f, -0.90f, -0.43f, 0.00f, 0.57f, 1.00f, 
    -4.38f , -2.41f , 0.00f, 0.48f, -0.88f, 0.00f, -0.88f, -0.48f, 0.00f, 0.58f, 1.00f, 
    -4.22f , -2.68f , 0.00f, 0.54f, -0.84f, 0.00f, -0.84f, -0.54f, 0.00f, 0.59f, 1.00f, 
    -4.05f , -2.94f , 0.00f, 0.59f, -0.81f, 0.00f, -0.81f, -0.59f, 0.00f, 0.60f, 1.00f, 
    -3.85f , -3.19f , 0.00f, 0.64f, -0.77f, 0.00f, -0.77f, -0.64f, 0.00f, 0.61f, 1.00f, 
    -3.64f , -3.42f , 0.00f, 0.68f, -0.73f, 0.00f, -0.73f, -0.68f, 0.00f, 0.62f, 1.00f, 
    -3.42f , -3.64f , 0.00f, 0.73f, -0.68f, 0.00f, -0.68f, -0.73f, 0.00f, 0.63f, 1.00f, 
    -3.19f , -3.85f , 0.00f, 0.77f, -0.64f, 0.00f, -0.64f, -0.77f, 0.00f, 0.64f, 1.00f, 
    -2.94f , -4.05f , 0.00f, 0.81f, -0.59f, 0.00f, -0.59f, -0.81f, 0.00f, 0.65f, 1.00f, 
    -2.68f , -4.22f , 0.00f, 0.84f, -0.54f, 0.00f, -0.54f, -0.84f, 0.00f, 0.66f, 1.00f, 
    -2.41f , -4.38f , 0.00f, 0.88f, -0.48f, 0.00f, -0.48f, -0.88f, 0.00f, 0.67f, 1.00f, 
    -2.13f , -4.52f , 0.00f, 0.90f, -0.43f, 0.00f, -0.43f, -0.90f, 0.00f, 0.68f, 1.00f, 
    -1.84f , -4.65f , 0.00f, 0.93f, -0.37f, 0.00f, -0.37f, -0.93f, 0.00f, 0.69f, 1.00f, 
    -1.55f , -4.76f , 0.00f, 0.95f, -0.31f, 0.00f, -0.31f, -0.95f, 0.00f, 0.70f, 1.00f, 
    -1.24f , -4.84f , 0.00f, 0.97f, -0.25f, 0.00f, -0.25f, -0.97f, 0.00f, 0.71f, 1.00f, 
    -0.94f , -4.91f , 0.00f, 0.98f, -0.19f, 0.00f, -0.19f, -0.98f, 0.00f, 0.72f, 1.00f, 
    -0.63f , -4.96f , 0.00f, 0.99f, -0.13f, 0.00f, -0.13f, -0.99f, 0.00f, 0.73f, 1.00f, 
    -0.31f , -4.99f , 0.00f, 1.00f, -0.06f, 0.00f, -0.06f, -1.00f, 0.00f, 0.74f, 1.00f, 
    -0.00f , -5.00f , 0.00f, 1.00f, -0.00f, 0.00f, -0.00f, -1.00f, 0.00f, 0.75f, 1.00f, 
    0.31f , -4.99f , 0.00f, 1.00f, 0.06f, 0.00f, 0.06f, -1.00f, 0.00f, 0.76f, 1.00f, 
    0.63f , -4.96f , 0.00f, 0.99f, 0.13f, 0.00f, 0.13f, -0.99f, 0.00f, 0.77f, 1.00f, 
    0.94f , -4.91f , 0.00f, 0.98f, 0.19f, 0.00f, 0.19f, -0.98f, 0.00f, 0.78f, 1.00f, 
    1.24f , -4.84f , 0.00f, 0.97f, 0.25f, 0.00f, 0.25f, -0.97f, 0.00f, 0.79f, 1.00f, 
    1.55f , -4.76f , 0.00f, 0.95f, 0.31f, 0.00f, 0.31f, -0.95f, 0.00f, 0.80f, 1.00f, 
    1.84f , -4.65f , 0.00f, 0.93f, 0.37f, 0.00f, 0.37f, -0.93f, 0.00f, 0.81f, 1.00f, 
    2.13f , -4.52f , 0.00f, 0.90f, 0.43f, 0.00f, 0.43f, -0.90f, 0.00f, 0.82f, 1.00f, 
    2.41f , -4.38f , 0.00f, 0.88f, 0.48f, 0.00f, 0.48f, -0.88f, 0.00f, 0.83f, 1.00f, 
    2.68f , -4.22f , 0.00f, 0.84f, 0.54f, 0.00f, 0.54f, -0.84f, 0.00f, 0.84f, 1.00f, 
    2.94f , -4.05f , 0.00f, 0.81f, 0.59f, 0.00f, 0.59f, -0.81f, 0.00f, 0.85f, 1.00f, 
    3.19f , -3.85f , 0.00f, 0.77f, 0.64f, 0.00f, 0.64f, -0.77f, 0.00f, 0.86f, 1.00f, 
    3.42f , -3.64f , 0.00f, 0.73f, 0.68f, 0.00f, 0.68f, -0.73f, 0.00f, 0.87f, 1.00f, 
    3.64f , -3.42f , 0.00f, 0.68f, 0.73f, 0.00f, 0.73f, -0.68f, 0.00f, 0.88f, 1.00f, 
    3.85f , -3.19f , 0.00f, 0.64f, 0.77f, 0.00f, 0.77f, -0.64f, 0.00f, 0.89f, 1.00f, 
    4.05f , -2.94f , 0.00f, 0.59f, 0.81f, 0.00f, 0.81f, -0.59f, 0.00f, 0.90f, 1.00f, 
    4.22f , -2.68f , 0.00f, 0.54f, 0.84f, 0.00f, 0.84f, -0.54f, 0.00f, 0.91f, 1.00f, 
    4.38f , -2.41f , 0.00f, 0.48f, 0.88f, 0.00f, 0.88f, -0.48f, 0.00f, 0.92f, 1.00f, 
    4.52f , -2.13f , 0.00f, 0.43f, 0.90f, 0.00f, 0.90f, -0.43f, 0.00f, 0.93f, 1.00f, 
    4.65f , -1.84f , 0.00f, 0.37f, 0.93f, 0.00f, 0.93f, -0.37f, 0.00f, 0.94f, 1.00f, 
    4.76f , -1.55f , 0.00f, 0.31f, 0.95f, 0.00f, 0.95f, -0.31f, 0.00f, 0.95f, 1.00f, 
    4.84f , -1.24f , 0.00f, 0.25f, 0.97f, 0.00f, 0.97f, -0.25f, 0.00f, 0.96f, 1.00f, 
    4.91f , -0.94f , 0.00f, 0.19f, 0.98f, 0.00f, 0.98f, -0.19f, 0.00f, 0.97f, 1.00f, 
    4.96f , -0.63f , 0.00f, 0.13f, 0.99f, 0.00f, 0.99f, -0.13f, 0.00f, 0.98f, 1.00f, 
    4.99f , -0.31f , 0.00f, 0.06f, 1.00f, 0.00f, 1.00f, -0.06f, 0.00f, 0.99f, 1.00f, 
    
    5.00f , 0.00f , 0.00f, -0.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f, 1.00f,
};

float planeVertices []={
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 4.0f,};

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

// SHADOW MAPPING CODE

#define SHADOW_SIZE 1024
GLuint shadowMapFbo;      // shadow map framebuffer object
GLuint shadowMapTexture;  // shadow map texture
GLuint shadowMapRbo;
GLuint shadowMapShader;   // shadow map shader

bool setupShadowMap()
{
    // create the FBO for rendering shadows
    glGenFramebuffers(1, &shadowMapFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);

    // attach a texture object to the framebuffer
    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);

    // check if we did everything right
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Could not create custom framebuffer.\n";
        return false;
    }

    // load the shader program for drawing the shadow map
    shadowMapShader = gdevLoadShader("demo8s.vs", "demo8s.fs");
    if (! shadowMapShader)
        return false;

    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    
    return true;
}

glm::mat4 renderShadowMap()
{
    // use the shadow framebuffer for drawing the shadow map
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);

    // the viewport should be the size of the shadow map
    glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);

    // clear the shadow map
    // (we don't have a color buffer attachment, so no need to clear that)
    glClear(GL_DEPTH_BUFFER_BIT);

    // using the shadow map shader...
    glUseProgram(shadowMapShader);

    // ... set up the light space matrix...
    // (note that if you use a spot light, the FOV and the center position
    // vector should be set to your spotlight's outer radius and focus point,
    // respectively)
    glm::mat4 lightTransform;
    lightTransform = glm::perspective(glm::radians(75.0f),       // fov
                                      1.0f,                      // aspect ratio
                                      0.1f,                      // near plane
                                      100.0f);                   // far plane
    lightTransform *= glm::lookAt(lightPosition,                 // eye position
                                  glm::vec3(0.0f, 0.0f, 0.0f),   // center position
                                  glm::vec3(0.0f, 1.0f, 0.0f));  // up vector
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "lightTransform"),
                       1, GL_FALSE, glm::value_ptr(lightTransform));

    // ... set up the model matrix... (just identity for this demo)
    glm::mat4 modelTransform = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shadowMapShader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (8 * sizeof(float)));


    // set the framebuffer back to the default onscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // before drawing the final scene, we need to set drawing to the whole window
    int width, height;
    glfwGetFramebufferSize(pWindow, &width, &height);
    glViewport(0, 0, width, height);



    // we will need the light transformation matrix again in the main rendering code
    return lightTransform;
}

// SHADOW MAPPING CODE
///////////////////////////////////////////////////////////////////////////////

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

    ///////////////////////////////////////////////////////////////////////////
    // setup shadow rendering
    if (! setupShadowMap())
        return false;
    ///////////////////////////////////////////////////////////////////////////


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

     ///////////////////////////////////////////////////////////////////////////
    // draw the shadow map
    glm::mat4 lightTransform = renderShadowMap();
    ///////////////////////////////////////////////////////////////////////////

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

    // HOLD KEY 5 TO CULL BACKFACES
    if (glfwGetKey(pWindow, GLFW_KEY_5) == GLFW_PRESS && !backfaceCulled)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

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

    ///////////////////////////////////////////////////////////////////////////
    // ... set up the light transformation (for looking up the shadow map)...
    glUniformMatrix4fv(glGetUniformLocation(shader, "lightTransform"),
                       1, GL_FALSE, glm::value_ptr(lightTransform));

    // ... set the active texture...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader, "shadowMap"),  1);
    ///////////////////////////////////////////////////////////////////////////

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
    glDrawArrays(GL_TRIANGLE_FAN, 0, 102);

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
