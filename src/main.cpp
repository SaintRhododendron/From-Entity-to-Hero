#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#define PROGRAMM_NAME "From-Entity-to-Hero"
#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640

int g_windowSizeX = WINDOW_WIDTH;
int g_windowSizeY = WINDOW_HEIGHT;

GLfloat points[] = {
    0.f,   0.5f,  0.f,
    0.5f, -0.5f,  0.f,
   -0.5f, -0.5f,  0.f,
   0.0f, 0.2f, 0.0f,
   0.2f, -0.2f, 0.0f,
   -0.2f, -0.2f, 0.0f
};

GLfloat colors[] = {
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,
  1.f, 0.f, 0.f,
   0.f, 1.f, 0.f,
    0.f, 0.f, 1.f
};

const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 3) in vec3 vertex_color;"
"out vec3 color;"
"void main(){"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 fragment_color;"
"void main(){"
"   fragment_color = vec4(color, 1.0);"
"}";

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

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);       //Create vertex shader. ID: vs
    glShaderSource(vs, 1, &vertex_shader, nullptr);     //Send a code to the shader. (ID, lines?, our code, size of code?) 
    glCompileShader(vs);                                //Compile our code

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);     //Create fragment shader. ID: fs
    glShaderSource(fs, 1, &fragment_shader, nullptr);   //Send a code to the shader Same as vertex
    glCompileShader(fs);                                //Compile as vertex

    GLuint sp = glCreateProgram();                      //Create shader program. ID: sp
    glAttachShader(sp, vs);                             //Attach vertex shader
    glAttachShader(sp, fs);                             //Attach fragment shader
    glLinkProgram(sp);                                  //Link them together

    glDeleteShader(vs);                                 //Delete vs to free memory
    glDeleteShader(fs);                                 //Same with the fs

    GLuint points_vbo = 0;                              //Vertex Buffer Object. ID: 0
    glGenBuffers(1, &points_vbo);                       //Gen a buffer (count?, address of ID. ID will be given by video driver)
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);          //Bind points_vbo to make it current
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW); //fill current array buffer with data (buffer, size of data, data, hint for videocard

    GLuint colors_vbo = 0;                              //Now make it for colors
    glGenBuffers(1, &colors_vbo);                       //Same
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);          //Same
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); //Same

    GLuint vao = 0;                     //Vertex Array Object - complex of data that will be send to videocard?
    glGenVertexArrays(1, &vao);         //As Gen buffer but gen VAO
    glBindVertexArray(vao);             //Bind vao. I think we dont have any other VertexArray that is why we dont need array as attribute

    glEnableVertexAttribArray(0);               //This is location in videocard memory. We need enable it first
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);  //we bind buffer again
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //send data from binded buffer to binded array (location, size of portion of data, data type, normalize it, stride, where from start reading?)

    glEnableVertexAttribArray(3);                   //Now location 1 is activated
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);      //Same
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //Color data to location 1

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(sp);                       //what shader program we use
        glBindVertexArray(vao);                 //Bind VAO that will give data to videocard
        glDrawArrays(GL_TRIANGLES, 0, 3);      //Ask videocard to draw triangles (Type of primitive, what index will be first, verticis)
        glDrawArrays(GL_TRIANGLES, 3, 3);
        glDrawArrays(GL_TRIANGLES, 2, 3);
                                               /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}