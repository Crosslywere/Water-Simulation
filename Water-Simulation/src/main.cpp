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

void keyFn(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLFW_KEY_2:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case GLFW_KEY_3:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    }
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
    WindowProps props = { 1280, 720, "Water Simulation", nullptr };
    // Creating the Window
    GLFWwindow* window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
    // Testing that the Window was created
    if (!window) return -1;
    // Further window configuration
    glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowUserPointer(window, &props);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeFn);
    glfwSetKeyCallback(window, keyFn);
    // Setting the Window to be the current render context
    glfwMakeContextCurrent(window);
    // Loading OpenGL functions via Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    std::cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    glfwSwapInterval(1);
    // Loading shader resource
    Shader shader({ "res/shaders/shader.frag", ShaderType::Fragment }, { "res/shaders/shader.vert", ShaderType::Vertex });
    Mesh mesh("res/meshes/plane.obj");
    // The application loop
    while (!glfwWindowShouldClose(window))
    {
        // Clearing the colors of the display
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Setting the shaders uniforms
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)props.Width / (float)props.Height, 0.1f, 100.0f);
        shader.SetMat4("projection", projection);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 3.0f, -13.0f), glm::vec3(0.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
        shader.SetMat4("view", view);
        shader.SetMat4("model", glm::rotate(glm::mat4(1.0f), glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        shader.SetFloat("time", (float)glfwGetTime());
        shader.SetVec3("right", glm::vec3(1.0f, 0.0f, 0.0f));
        shader.SetVec3("lightDir", glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f)));
        // Drawing the mesh
        mesh.Draw(shader);
        // Swapping the buffers
        glfwSwapBuffers(window);
        // Updating the input states
        glfwPollEvents();
    }
    // Terminating the Window instance
    glfwTerminate();
}
