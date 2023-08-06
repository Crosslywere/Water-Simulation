#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Mesh.h"
#include <iostream>

struct WindowProps
{
    int Width, Height;
    std::string Title;
};

void frameBufferResizeFn(GLFWwindow* window, int width, int height)
{
    WindowProps* props = (WindowProps*)glfwGetWindowUserPointer(window);
    props->Width = width;
    props->Height = height;
    glViewport(0, 0, width, height);
}

int main(void)
{
    // Initializing GLFW
    glfwInit();
    // Configuring OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Configuring the window
    WindowProps props = { 1600, 900, "Water Simulation" };
    // Creating the Window
    GLFWwindow* window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
    // Testing that the Window was created
    if (!window) return -1;
    // Further window configuration
    glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowUserPointer(window, &props);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeFn);
    // Setting the Window to be the current render context
    glfwMakeContextCurrent(window);
    // Loading OpenGL functions via Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    std::cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    // Loading shader resource
    Shader shader({ "res/shaders/shader.frag", ShaderType::Fragment }, { "res/shaders/shader.vert", ShaderType::Vertex });
    Mesh mesh("res/meshes/test.obj");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // The application loop
    while (!glfwWindowShouldClose(window))
    {
        // Clearing the colors of the display
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // TODO: Draw mesh using shader
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)props.Width / (float)props.Height, 0.1f, 100.0f);
        shader.SetMat4("projection", projection);
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        shader.SetMat4("model", model);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.SetMat4("view", view);
        mesh.Draw(shader);
        // Swapping the buffers
        glfwSwapBuffers(window);
        // Updating the input states
        glfwPollEvents();
    }
    // Terminating the Window instance
    glfwTerminate();
}
