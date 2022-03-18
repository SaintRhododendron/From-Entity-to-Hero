#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

#include"Renderer/ShaderProgram.h"
#include"Resources/ResourceManager.h"
#include"Renderer/Texture.h"
#include"Renderer/Sprite.h"

#define PROGRAM_NAME "From-Entity-to-Hero"
#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

/*
class Shader Program

contructor (string VertexShader, string FragmentShader)

bool isCompiled - returns flag "_isCompiled"

.use() - use this shaderProgram
*/
/*
1   1 - 2
| \   \ |
2 - 3   3
*/

/*GLfloat points[] = {
   0.f, 0.f, 0.f,
   500.f, 800.f, 0.f,
   1000.f,  0.f, 0.f
};

GLfloat colors[] = {
    1.f, 0.99f, 0.29f, 1.f,
    1.f, 0.99f, 0.29f, 1.f,
    1.f, 0.99f, 0.29f, 1.f,
};

GLfloat textures[] = {
    0.f, 0.5f,
    0.5f,  1.f,
    1.f,  0.5f,
}; */

glm::ivec2 g_windowSize(//pVideoMode->height, pVideoMode->width);
                        WINDOW_WIDTH, WINDOW_HEIGHT);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x, g_windowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

}

int main(int argc, char** argv)
{

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "[ERR] Fatal Error! GLFW failed to be initialized!" << std::endl;
        return -1;
    }

    GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
    if (!pMonitor)
    {
        std::cerr << "[ERR] Fatal Error! Failed to detect Primal Monitor!" << std::endl;
        return -1;
    }

    const GLFWvidmode* pVideoMode = glfwGetVideoMode(pMonitor);
    g_windowSize.x = pVideoMode->width;
    g_windowSize.y = pVideoMode->height;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, PROGRAM_NAME, nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "[ERR] Fatal Error! Window " << PROGRAM_NAME << " failed to be created!" << std::endl;
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

    ResourceManager g_resourceManager(argv[0]);
    /*auto pDefaultShaderProgram = g_resourceManager.loadShaderProgram("DefaultShaderProgram", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "[ERR] Fatal Error! Default Shader program is failed to load!" << std::endl;
        return -1;
    }
    auto pTextureShaderProgram = g_resourceManager.loadShaderProgram("TextureShaderProgram", "res/shaders/vertexTexture.txt", "res/shaders/fragmentTexture.txt");
    if (!pTextureShaderProgram)
    {
        std::cerr << "[ERR] Fatal Error! Texture Shader program is failed to load!" << std::endl;
        return -1;
    }
    

    GLuint points_vbo = 0;                              //Vertex Buffer Object. ID: 0
    glGenBuffers(1, &points_vbo);                       //Gen a buffer (count?, address of ID. ID will be given by video driver)
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);          //Bind points_vbo to make it current
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW); //fill current array buffer with data (buffer, size of data, data, hint for videocard

    GLuint colors_vbo = 0;                              //Now make it for colors
    glGenBuffers(1, &colors_vbo);                       //Same
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);          //Same
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); //Same

    GLuint texture_vbo = 0;
    glGenBuffers(1, &texture_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);

    GLuint vao = 0;                     //Vertex Array Object - complex of data that will be send to videocard?
    glGenVertexArrays(1, &vao);         //As Gen buffer but gen VAO
    glBindVertexArray(vao);             //Bind vao. I think we dont have any other VertexArray that is why we dont need array as attribute

    glEnableVertexAttribArray(0);               //This is location in videocard memory. We need enable it first
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);  //we bind buffer again
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //send data from binded buffer to binded array (location, size of portion of data, data type, normalize it, stride, where from start reading?)

    glEnableVertexAttribArray(3);                   //Now location 1 is activated
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);      //Same
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, nullptr); //Color data to location 1

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glm::mat4 modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(100.f, 100.f, 0.f));
    

    pTextureShaderProgram->use();
    pTextureShaderProgram->setInt("tex", 0);
    pTextureShaderProgram->setMatrix4("projectionMatrix", projectionMatrix); */
    
    std::vector<std::string> subTexturesNames = { "Miadz1" };
    auto pTexture = g_resourceManager.loadTextureAtlas("Stone", "res/textures/miadzel.png", subTexturesNames, 32, 32);

    auto pSpriteShaderProgram = g_resourceManager.loadShaderProgram("SpriteShaderProgram", "res/shaders/vertexSprite.txt", "res/shaders/fragmentSprite.txt");
    
    auto pSprite = g_resourceManager.loadSprite("Miadzel", "Stone", "SpriteShaderProgram", 150, 180, "Miadz1");
    pSprite->setPosition(glm::vec2(50, 50));
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMatrix", projectionMatrix);
    /* Loop until the user closes the window */

    int x = 100;
    int y = 200;
    int xx = 10;
    int yy = 10;


    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        pSprite->setPosition(glm::vec2(x, y));
        pSprite->render();
        x += xx;
        y += yy;
        if (x >= (g_windowSize.x - pSprite->getSize().x) || x <= 0) { xx = -xx; };
        if (y >= (g_windowSize.y - pSprite->getSize().y) || y <= 0) { yy = -yy; };
        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);


        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}