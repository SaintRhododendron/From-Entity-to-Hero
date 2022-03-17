#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#define PROGRAMM_NAME "From-Entity-to-Hero"
#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640

int g_windowSizeX = WINDOW_WIDTH;
int g_windowSizeY = WINDOW_HEIGHT;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
{
    
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "[ERR] Fatal Error! GLFW failed to be initialized!" << std::endl;
        return -1;
    }
    /* Create a windowed mode window and its OpenGL context */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, PROGRAMM_NAME, nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "[ERR] Fatal Error! Window " << PROGRAMM_NAME << " failed to be created!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    /*Setting the callbacks*/
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    if (!gladLoadGL())
    {
        std::cout << "[ERR] Fatal Error! GLAD failed to be loaded!" << std::endl;
        return -1;
    }
    
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0, 0.3922, 0, 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}