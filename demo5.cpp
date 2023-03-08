/******************************************************************************
 * This demo implements simple Phong lighting with one point-light source.
 *
 * The model's vertex data replaces the layout location for vertex color
 * with per-vertex normal vectors, which allows for lighting calculations.
 *
 * In addition, the camera position in this demo is specified via spherical
 * polar coordinates. This is ideal for the demo since we typically want to
 * look at a model from different angles to see the light's effect.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>
#include <list>

// change this to your desired window attributes
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 360
#define WINDOW_TITLE  "Exercise2 (use WASDQE keys for camera, IKJLUO keys for light)"
GLFWwindow *pWindow;
int current = 0;

// model
float vertices[] =
{
    // position (x, y, z)    normal (x, y, z)     texture coordinates (s, t)
    // ground plane
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  4.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f,  8.00f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  4.0f, 4.0f,
    -8.00f, -2.00f, -8.00f,  0.0f,  1.0f,  0.0f,  0.0f, 4.0f,
};

struct Vertex
{
    float x, y, z;
    float r, g, b;
};

struct Triangle
{
    Vertex a, b, c;
};

struct modelInstance
{
    float x         = 0.0f;
    float y         = 0.0f;
    float z         = 0.0f;  // added for demo4b
    float rotation  = 0.0f;
    float scaling   = 1.0f;
};

Vertex midpoint(Vertex x, Vertex y)
{
    Vertex z;
    z.x = (x.x + y.x) / 2;
	z.y = (x.y + y.y) / 2;
	z.z = (x.z + y.z) / 2;
	return z; 
}

int recursionLvl = 3;
const int verNum = 20 * 3 * pow(4, recursionLvl);
Vertex sphere[3840];
void vertSphere()
{
    float t = (1.0f + sqrtf(5.0f))/2.0f;
    Vertex idn[12];
    // RECT1
	idn[0].x = -1.0f;	idn[0].y = t;	idn[0].z = 0.0f;
	idn[0].r = 1.0f;	idn[0].g = 0.0f;		idn[0].b = 0.0f,
	idn[1].x = 1.0f;	idn[1].y = t;	idn[1].z = 0.0f;
	idn[1].r = 1.0f;	idn[1].g = 0.0f;		idn[1].b = 0.0f;
	idn[2].x = -1.0f;	idn[2].y = -t;	idn[2].z = 0.0f;
	idn[2].r = 1.0f;	idn[2].g = 0.0f;		idn[2].b = 0.0f;
	idn[3].x = 1.0f;	idn[3].y = -t;	idn[3].z = 0.0f;
	idn[3].r = 1.0f;	idn[3].g = 0.0f;		idn[3].b = 0.0f;
	idn[4].x = 0.0f;	idn[4].y = -1.0f;	idn[4].z = t;
	idn[4].r = 1.0f;	idn[4].g = 0.0f;		idn[4].b = 0.0f;
	idn[5].x = 0.0f;	idn[5].y = 1.0f;	idn[5].z = t;
	idn[5].r = 1.0f;	idn[5].g = 0.0f;		idn[5].b = 0.0f;
	idn[6].x = 0.0f;	idn[6].y = -1.0f;	idn[6].z = -t;
	idn[6].r =  1.0f;	idn[6].g = 0.0f;		idn[6].b = 0.0f;
	idn[7].x = 0.0f;	idn[7].y = 1.0f;	idn[7].z = -t;
	idn[7].r =  1.0f;	idn[7].g = 0.0f;		idn[7].b = 0.0f;
	idn[8].x = t;	idn[8].y = 0.0f;	idn[8].z = -1.0F;
	idn[8].r =  1.0f;	idn[8].g = 0.0f;		idn[8].b = 0.0f;
	idn[9].x = t;	idn[9].y = 0.0f;	idn[9].z = 1.0F;
	idn[9].r =  1.0f;	idn[9].g = 0.0f;		idn[9].b = 0.0f;
	idn[10].x = -t;	idn[10].y = 0.0f;	idn[10].z = -1.0F;
	idn[10].r =  1.0f;	idn[10].g = 0.0f;		idn[10].b = 0.0f;
	idn[11].x = -t;	idn[11].y = 0.0f;	idn[11].z = 1.0F;
	idn[11].r =  1.0f;	idn[11].g = 0.0f;		idn[11].b = 0.0f;

    Triangle tri[20];
	tri[0].a = idn[0];
	tri[0].b = idn[11];
	tri[0].c = idn[5];
	tri[1].a = idn[0];
	tri[1].b = idn[5];
	tri[1].c = idn[1];
	tri[2].a = idn[0];
	tri[2].b = idn[1];
	tri[2].c = idn[7];
	tri[3].a = idn[0];
	tri[3].b = idn[7];
	tri[3].c = idn[10];
	tri[4].a = idn[0];
	tri[4].b = idn[10];
	tri[4].c = idn[11];
	tri[5].a = idn[1];
	tri[5].b = idn[5];
	tri[5].c = idn[9];
	tri[6].a = idn[5];
	tri[6].b = idn[11];
	tri[6].c = idn[4];
	tri[7].a = idn[11];
	tri[7].b = idn[10];
	tri[7].c = idn[2];
	tri[8].a = idn[10];
	tri[8].b = idn[7];
	tri[8].c = idn[6];
	tri[9].a = idn[7];
	tri[9].b = idn[1];
	tri[9].c = idn[8];
	tri[10].a = idn[3];
	tri[10].b = idn[9];
	tri[10].c = idn[4];
	tri[11].a = idn[3];
	tri[11].b = idn[4];
	tri[11].c = idn[2];
	tri[12].a = idn[3];
	tri[12].b = idn[2];
	tri[12].c = idn[6];
	tri[13].a = idn[3];
	tri[13].b = idn[6];
	tri[13].c = idn[8];
	tri[14].a = idn[3];
	tri[14].b = idn[8];
	tri[14].c = idn[9];
	tri[15].a = idn[4];
	tri[15].b = idn[9];
	tri[15].c = idn[5];
	tri[16].a = idn[2];
	tri[16].b = idn[4];
	tri[16].c = idn[11];
	tri[17].a = idn[6];
	tri[17].b = idn[2];
	tri[17].c = idn[10];
	tri[18].a = idn[8];
	tri[18].b = idn[6];
	tri[18].c = idn[7];
	tri[19].a = idn[9];
	tri[19].b = idn[8];
	tri[19].c = idn[1];

    std::list<Triangle> triangles;

	for(int i=0; i<20; i++)
	{
		triangles.push_back(tri[i]);
	}
	recursionLvl = 3;
	
	for (int i =0; i<recursionLvl; i++)
	{
		std::list<Triangle> triangles2;
		for (Triangle triangle : triangles)
		{
			Vertex a = midpoint(triangle.a, triangle.b);
			Vertex b = midpoint(triangle.b, triangle.c);
			Vertex c = midpoint(triangle.c, triangle.a);

			Triangle triangle1, triangle2, triangle3, triangle4;
			triangle1.a = triangle.a;
			triangle1.b = a;
			triangle1.c = c;

			triangle2.a = triangle.b;
			triangle2.b = b;
			triangle2.c = a;

			triangle3.a = triangle.c;
			triangle3.b = c;
			triangle3.c = b;

			triangle4.a = a;
			triangle4.b = b;
			triangle4.c = c;

			triangles2.push_back(triangle1);
			triangles2.push_back(triangle2);
			triangles2.push_back(triangle3);
			triangles2.push_back(triangle4);

		}
		triangles = triangles2;
	}
	int counter = 0;
	for (Triangle triangle : triangles)
	{
		
		sphere[counter] = triangle.a;
		sphere[counter + 1] = triangle.b;
		sphere[counter + 2] = triangle.c;
		counter += 3;
		
	}
	for (int i = 0; i<verNum; i++)
	{
		float length = sqrtf(sphere[i].x * sphere[i].x + sphere[i].y * sphere[i].y + sphere[i].z * sphere[i].z);
		sphere[i].x /= length;
		sphere[i].y /= length;
		sphere[i].z /= length;
		sphere[i].r = 1.0f;
		sphere[i].g = 1.0f;
		sphere[i].b = 1.0f;
	}
}
// OpenGL object IDs
GLuint vao, vao2;         // vertex array object (stores the render state for our vertex array)
GLuint vbo, vbo2;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint ebo, ebo2; 
GLuint shader;
GLuint texture, grass;

GLuint vaoSphere;
GLuint vboSphere;

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
glm::vec3 spotlightFocusPosition = glm::vec3(0.0f, -5.0f, 0.0f);


float uniformAmbientIntensityValue = 1.0f;
float uniformSpecularIntensityValue = 10.0f;
float uniformSpecularPowerValue = 32.0f;
double previousTime = 0.0;

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // upload the model to the GPU (explanations omitted for brevity)
    {
        glGenVertexArrays(1, &vaoSphere);
        glGenBuffers(1, &vboSphere);
        glBindVertexArray(vaoSphere);
        glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, x));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, r));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        /*glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);*/
    }

    // load our shader program
    shader = gdevLoadShader("demo5n.vs", "demo5n.fs");
    if (! shader)
        return false;

    // load our texture
    texture = gdevLoadTexture("demo5.png", GL_REPEAT, true, true);
    if (! texture)
        return false;
    
    grass = gdevLoadTexture("grass.png", GL_REPEAT, true, true);
    if (! grass)
        return false;

    // enable z-buffer depth testing and face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return true;
}
modelInstance sphereInstance;
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
        uniformAmbientIntensityValue += 0.01f;
    if (glfwGetKey(pWindow, GLFW_KEY_X) == GLFW_PRESS)
        uniformAmbientIntensityValue -= 0.01f;
    if (glfwGetKey(pWindow, GLFW_KEY_C) == GLFW_PRESS)
        uniformSpecularIntensityValue += 0.01f;
    if (glfwGetKey(pWindow, GLFW_KEY_V) == GLFW_PRESS)
        uniformSpecularIntensityValue -= 0.01f;
    if (glfwGetKey(pWindow, GLFW_KEY_B) == GLFW_PRESS)
        uniformSpecularPowerValue += 0.01f;
    if (glfwGetKey(pWindow, GLFW_KEY_N) == GLFW_PRESS)
        uniformSpecularPowerValue -= 0.01f;
        
    // clear the whole frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);

    glUniform1f(glGetUniformLocation(shader, "uniformAmbientIntensity"), uniformAmbientIntensityValue);
    glUniform1f(glGetUniformLocation(shader, "uniformSpecularIntensity"), uniformSpecularIntensityValue);
    glUniform1f(glGetUniformLocation(shader, "uniformSpecularPower"), uniformSpecularPowerValue);

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

    glUniform3fv(glGetUniformLocation(shader,"spotlightFocusPosition"),1,glm::value_ptr(spotlightFocusPosition));

    glUniform1f(glGetUniformLocation(shader,"spotLightCutoff"),glm::cos(glm::radians(3.0f)));

    // ... set up the light position...
    glUniform3fv(glGetUniformLocation(shader, "lightPosition"),
                 1, glm::value_ptr(lightPosition));

    // ... set the active texture...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grass);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (8 * sizeof(float)));

    float rotationTime = sphereInstance.rotation * currentTime;
    glUniform1f(glGetUniformLocation(shader, "shaderNormal"), 1.0f);
    sphereInstance.rotation = 1.0f;
    sphereInstance.scaling = 0.75f;
    sphereInstance.x = 0.0f;
    sphereInstance.y = 0.0f;
    sphereInstance.z = 0.0f;
    modelTransform = glm::translate(modelTransform, glm::vec3(sphereInstance.x, sphereInstance.y, sphereInstance.z));
    modelTransform = glm::rotate(modelTransform, rotationTime, glm::vec3(0.0f, 1.0f, 0.0f));

    sphereInstance.z = 3.0f;
    modelTransform = glm::translate(modelTransform, glm::vec3(sphereInstance.x, sphereInstance.y, sphereInstance.z));
    modelTransform = glm::rotate(modelTransform, rotationTime, glm::vec3(1.0f, 0.0f, 1.0f));
    modelTransform = glm::scale(modelTransform, glm::vec3(sphereInstance.scaling, sphereInstance.scaling, sphereInstance.scaling));
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),1, GL_FALSE, glm::value_ptr(modelTransform));
    

    glm::mat4 normModel = transpose(inverse(modelTransform));
    glUniformMatrix4fv(glGetUniformLocation(shader, "normat"), 1, GL_FALSE, glm::value_ptr(normModel));

    glBindVertexArray(vaoSphere);
    for(int i = 0; i<verNum; i+=3)
    {
        glDrawArrays(GL_TRIANGLE_FAN, i, 3);
    }

    // s = 12.8f;
    // sUniformLocation = glGetUniformLocation(shader, "s");
    // glUniform1f(sUniformLocation, s);
    // Ka = glm::vec4(0.135f, 0.2225f, 0.1575f, 0.95f);
    // Kd = glm::vec4(0.54f, 0.89f, 0.63f, 0.95f);
    // Ks = glm::vec4(0.316228f, 0.316228f, 0.316228f, 0.95f);
    // KaUniformLocation = glGetUniformLocation(shader, "Ka");
    // glUniform4fv(KaUniformLocation, 1, glm::value_ptr(Ka));
    // KdUniformLocation = glGetUniformLocation(shader, "Kd");
    // glUniform4fv(KdUniformLocation, 1, glm::value_ptr(Kd));
    // KsUniformLocation = glGetUniformLocation(shader, "Ks");
    // glUniform4fv(KsUniformLocation, 1, glm::value_ptr(Ks));
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
