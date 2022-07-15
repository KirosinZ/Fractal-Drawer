#include <iostream>
#include <iomanip>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "program/shader.h"

const float wnd_width = 800.0f;//1920.0f;
const float wnd_height = 600.0f;//1080.0f;

GLuint QVAO = 0;
GLuint CVBO = 0;

float quad_coordinates[] = {
        -1.0f, 1.0f, // NW corner
        1.0f, 1.0f,  // NE corner
        -1.0f, -1.0f,// SW corner
        1.0f, -1.1f  // SE corner
};

float zoom = 1.0f;
glm::vec2 pivot = glm::vec2(0.0f);

glm::dvec2 lastmouse = glm::vec2(0.0);
glm::dvec2 mouse = glm::dvec2(0.0);

void mouse_callback(GLFWwindow* wnd, double xpos, double ypos)
{
    glm::vec2 screenspace = glm::vec2(xpos / wnd_width - 0.5f, 0.5f - ypos / wnd_height);
    screenspace.x *= wnd_width / wnd_height;

    mouse = pivot + 2.0f * screenspace * zoom;

    if(glfwGetMouseButton(wnd, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glm::dvec2 diff = lastmouse - mouse;
        pivot += diff;
        mouse = lastmouse;
    }
    lastmouse = mouse;
}

void mouse_scroll_callback(GLFWwindow* wnd, double xoffset, double yoffset)
{
    float ampl = glm::pow(0.8, yoffset);
    zoom *= ampl;
    pivot = (glm::vec2)mouse + (pivot - (glm::vec2)mouse) * ampl;
}

bool tabheld = false;
bool shoder = true;



void keyboard(GLFWwindow* wnd)
{
    if(glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(wnd, GLFW_TRUE);

    if(glfwGetKey(wnd, GLFW_KEY_TAB) == GLFW_PRESS && !tabheld)
    {
        tabheld = true;
        shoder = !shoder;
    }
    else if (glfwGetKey(wnd, GLFW_KEY_TAB) == GLFW_RELEASE && tabheld)
    {
        tabheld = false;
    }
}

GLFWwindow* init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wnd_width, wnd_height, "OpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        const char* err;
        glfwGetError(&err);
        std::cerr << "Window could not be opened: " << err << std::endl;
        glfwTerminate();
        return window;
    }
    glfwMakeContextCurrent(window);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    glewInit();
    glViewport(0, 0, wnd_width, wnd_height);

    //glfwMaximizeWindow(window);

    return window;
}

int main() {

    GLFWwindow* wnd = init();
    if(wnd == nullptr) return -1;

    glGenBuffers(1, &CVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_coordinates), quad_coordinates, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &QVAO);
    glBindVertexArray(QVAO);

    glBindBuffer(GL_ARRAY_BUFFER, CVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Shader s("../shaders/vert.vert", "../shaders/frag.frag");
    Shader s1("../shaders/vert.vert", "../shaders/frag2.frag");

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    while(!glfwWindowShouldClose(wnd))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        keyboard(wnd);

        (shoder ? s : s1).use();
        s.setVec2("Z0", pivot);
        s.setFloat("zoom", zoom);
        s.setInt("maxIterations", 8000);
        s.setFloat("aspect", wnd_width / wnd_height);
        glBindVertexArray(QVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }


}
