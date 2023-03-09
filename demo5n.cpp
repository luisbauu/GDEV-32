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
GLuint vao;
GLuint vbo;
GLuint shader;
GLuint texture[2];


float vertices[] = 
{
    /*// ground plane
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 4.0f,*/
    /* ============= POINTS CALCULATED THROUGH CODE BELOW ===============
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
            
            // calculate the normal and tangent vectors
            double nx = -sin(angle);
            double ny = cos(angle);
            double nz = 0.0;
            double tx = cos(angle);
            double ty = sin(angle);
            double tz = 0.0;
            
            // output the point coordinates, normal vector, and tangent vector
            cout << fixed << setprecision(2) << x << "f , " << y << "f , " << z << "f, "
                << nx << "f, " << ny << "f, " << nz << "f, "
                << tx << "f, " << ty << "f, " << tz << "f, " << 
                "1.0f, 0.0f,"<< endl;
        }
        
        return 0;
    }*/
    //cone Verts
    0.00f , 0.00f , 5.00f, 0.00f, 0.00f, 10.00f, 1.00f, 0.00f, 0.00f, 1.0f, 0.0f,
    5.00f , 0.00f , 0.00f, -0.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.0f, 0.0f,
    4.05f , 2.94f , 0.00f, -0.59f, 0.81f, 0.00f, 0.81f, 0.59f, 0.00f, 1.0f, 0.0f,
    1.55f , 4.76f , 0.00f, -0.95f, 0.31f, 0.00f, 0.31f, 0.95f, 0.00f, 1.0f, 0.0f,
    -1.55f , 4.76f , 0.00f, -0.95f, -0.31f, 0.00f, -0.31f, 0.95f, 0.00f, 1.0f, 0.0f,
    -4.05f , 2.94f , 0.00f, -0.59f, -0.81f, 0.00f, -0.81f, 0.59f, 0.00f, 1.0f, 0.0f,
    -5.00f , 0.00f , 0.00f, -0.00f, -1.00f, 0.00f, -1.00f, 0.00f, 0.00f, 1.0f, 0.0f,
    -4.05f , -2.94f , 0.00f, 0.59f, -0.81f, 0.00f, -0.81f, -0.59f, 0.00f, 1.0f, 0.0f,
    -1.55f , -4.76f , 0.00f, 0.95f, -0.31f, 0.00f, -0.31f, -0.95f, 0.00f, 1.0f, 0.0f,
    1.55f , -4.76f , 0.00f, 0.95f, 0.31f, 0.00f, 0.31f, -0.95f, 0.00f, 1.0f, 0.0f,
    4.05f , -2.94f , 0.00f, 0.59f, 0.81f, 0.00f, 0.81f, -0.59f, 0.00f, 1.0f, 0.0f,
    5.00f , 0.00f , 0.00f, -0.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.0f, 0.0f,
};


const int NUM_SEGMENTS = 32; // number of segments to approximate the cone

/*void coneCoords() {
    for
    
};*/
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

float ambientIntensity = 0.1f;
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

    // load our shader program
    shader = gdevLoadShader("demo5n.vs", "demo5n.fs");
    if (! shader)
        return false;

    // since we now use multiple textures, we need to set the texture channel for each texture
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(shader, "normalMap"),  1);

    // load our textures
    texture[0] = gdevLoadTexture("exercise2diffusemap.png", GL_REPEAT, true, true);
    texture[1] = gdevLoadTexture("exercise2normalmap.png", GL_REPEAT, true, true);
    if (! texture[0] || ! texture[1])
        return false;

    // enable z-buffer depth testing and face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

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

    // ... set up the model matrix... (just identity for this demo)
    glm::mat4 modelTransform = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                       1, GL_FALSE, glm::value_ptr(modelTransform));

    // ... set up the light position...
    glUniform3fv(glGetUniformLocation(shader, "lightPosition"),
                 1, glm::value_ptr(lightPosition));

    glUniform1f(glGetUniformLocation(shader, "uniformAmbientIntensity"), ambientIntensity);
    glUniform1f(glGetUniformLocation(shader, "uniformSpecularIntensity"), specularIntensity);
    glUniform1f(glGetUniformLocation(shader, "uniformSpecularPower"), specularPower);

    glUniform1f(glGetUniformLocation(shader,"spotlightCutoff"),glm::cos(glm::radians(spotlightCutoff)));
    glUniform1f(glGetUniformLocation(shader,"spotlightOuterAngle"), glm::cos(glm::radians(spotlightOuterAngle)));

    // ... set the active textures...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(vertices));
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
