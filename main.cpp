#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "program/shader.h"

GLuint QVAO = 0;
GLuint CVBO = 0;

float qc[] = {
        -1.0f, 1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -2.56f, 1.44f,
        2.56f, 1.44f,
        -2.56f, -1.44f,
        2.56f, -1.44f
};

glm::dvec2 mouse = glm::dvec2(0.0);

void mouse_callback(GLFWwindow* wnd, double xpos, double ypos)
{
    glm::dvec2 screenspace = glm::dvec2((double)xpos, ypos) / glm::dvec2(2560.0, 1440.0);
    screenspace.y = 1.0 - screenspace.y;
    glm::dvec2 uu = glm::dvec2(qc[10], qc[11]);
    glm::dvec2 ll = glm::dvec2(qc[12], qc[13]);
    mouse = ll + (uu - ll) * screenspace;
}

void mouse_scroll_callback(GLFWwindow* wnd, double xoffset, double yoffset)
{
    float m;
    if ( yoffset > 0) m = 0.8f;
    else m = 1.0f / 0.8f;
    for(int i = 4; i < 8; i++)
    {
        qc[2 * i] = m * (qc[2 * i] - mouse.x) + mouse.x;
        qc[2 * i + 1] = m * (qc[2 * i + 1] - mouse.y) + mouse.y;
    }
}

void keyboard(GLFWwindow* wnd)
{
    if(glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(wnd, GLFW_TRUE);
}

GLFWwindow* init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(2560, 1440, "OpenGL", glfwGetPrimaryMonitor(), nullptr);
    if (window == nullptr)
    {
        std::cerr << "Window could not be opened." << std::endl;
        glfwTerminate();
        return window;
    }
    glfwMakeContextCurrent(window);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    glewInit();
    glViewport(0, 0, 2560, 1440);

    //glfwMaximizeWindow(window);

    return window;
}

int main() {

    GLFWwindow* wnd = init();
    if(wnd == nullptr) return -1;

    glGenBuffers(1, &CVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(qc), qc, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &QVAO);
    glBindVertexArray(QVAO);

    glBindBuffer(GL_ARRAY_BUFFER, CVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(8 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Shader s("../shaders/vert.vert", "../shaders/frag.frag");

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    while(!glfwWindowShouldClose(wnd))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        keyboard(wnd);

        glBindBuffer(GL_ARRAY_BUFFER, CVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(qc), qc, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        s.use();
        s.setVec2("Z0", glm::vec2(0.0f));
        s.setInt("maxIterations", 1280);
        glBindVertexArray(QVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }


}
