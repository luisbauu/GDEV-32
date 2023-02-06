/******************************************************************************
 * This is a minor modification of demo4.cpp that draws two instances of the
 * quadrilateral. (This uses the same support files as demo4.cpp.)
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 360
#define WINDOW_TITLE  "Hello Transform (use WASDQE and arrow keys; space bar to switch between objects)"
GLFWwindow *pWindow;

// define a vertex array to hold our vertices
float vertices[] =
{
    // position (x, y, z)    color (r, g, b)    texture coordinates (s, t)
    -0.50f, -0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
     0.50f, -0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
    -0.50f,  0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
     0.50f,  0.50f,  0.00f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f
};

// define the triangles as triplets of indices into the vertex array
GLuint indices[] =
{
    0, 1, 2,
    1, 3, 2
};

// define OpenGL object IDs to represent the vertex array, shader program, and texture in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint ebo;         // element buffer object (stores the indices of the vertices to form triangles)
GLuint shader;      // combined vertex and fragment shader
GLuint texture;     // texture object

// variables controlling the object's position, rotation, and scaling
struct modelInstance
{
    float x         = 0.0f;
    float y         = 0.0f;
    float z         = 0.0f;  // added for demo4b
    float rotation  = 0.0f;
    float scaling   = 1.0f;
};
modelInstance instances[2];
int current = 0;
double previousTime = 0.0;

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // generate the VAO, VBO, and EBO objects and store their IDs in vao, vbo, and ebo, respectively
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // upload our index array data to the newly-created EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 8 floats (8 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color) is also 3 floats but starts at the fourth float (offset 3 * sizeof(float))
    // - layout location 2 (texcoord) is 2 floats and starts at the seventh float (offset 6 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO, VBO, and EBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("demo4.vs", "demo4.fs");
    if (! shader)
        return false;

    // load our texture
    texture = gdevLoadTexture("demo4.png", GL_REPEAT, true, true);
    if (! texture)
        return false;

    // enable OpenGL blending so that texels with alpha values less than one are drawn transparent
    // (you can omit these lines if you don't use alpha)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // the next line will enable z-buffer depth testing to properly draw objects on top of each other
    // (but wait, will this actually work properly with this demo?...)
    //glEnable(GL_DEPTH_TEST);

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // find the elapsed time since the last frame
    double currentTime = glfwGetTime();
    double elapsedTime = (currentTime - previousTime);
    previousTime = currentTime;

    // handle key events
    if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
        current = (current + 1) % 2;  // switch between the two instances
    if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
        instances[current].x += elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
        instances[current].x -= elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
        instances[current].y += elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
        instances[current].y -= elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
        instances[current].z += elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
        instances[current].z -= elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        instances[current].rotation += elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        instances[current].rotation -= elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_UP) == GLFW_PRESS)
        instances[current].scaling += elapsedTime;
    if (glfwGetKey(pWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        instances[current].scaling -= elapsedTime;
    if (instances[current].scaling < 0.1f)
        instances[current].scaling = 0.1f;  // don't let the object scale all the way down to zero or negative

    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
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
    viewTransform = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),   // eye position
                                glm::vec3(0.0f, 0.0f, 0.0f),   // center position
                                glm::vec3(0.0f, 1.0f, 0.0f));  // up vector
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewTransform"),
                       1, GL_FALSE, glm::value_ptr(viewTransform));

    for (int i = 0; i < 2; i++)
    {
        // ... set up the model matrix...
        glm::mat4 modelTransform = glm::mat4(1.0f);  // set to identity first
        modelTransform = glm::translate(modelTransform,
                                        glm::vec3(instances[i].x, instances[i].y, instances[i].z)); // translate xyz
        modelTransform = glm::rotate(modelTransform,
                                     instances[i].rotation,
                                     glm::vec3(0.0f, 0.0f, 1.0f));                                  // rotate around z
        modelTransform = glm::scale(modelTransform,
                                    glm::vec3(instances[i].scaling, instances[i].scaling, 1.0f));   // scale x and y
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelTransform"),
                        1, GL_FALSE, glm::value_ptr(modelTransform));

        // ... set the active texture...
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // ... then draw our triangles
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    }
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
