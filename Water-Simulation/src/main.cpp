#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>

int main(void)
{
    // Initializing GLFW
    glfwInit();
    // Configuring OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Creating the Window
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Water Simulation", nullptr, nullptr);
    // Testing that the Window was created
    if (!window) return -1;
    // Setting the Window to be the current render context
    glfwMakeContextCurrent(window);
    // Loading OpenGL functions via Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    std::cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    // Loading shader resource
    Shader s({ "res/shader.frag", ShaderType::Fragment });
    // Creating a test triangle
    float vertices[] = {
        0.0f, 0.5f, 0.0f,
        -.5f, -.5f, 0.0f,
        0.5f, -.5f, 0.0f,
    };
    // Creating the Vertex Array and Vertex Buffer
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Adding the vertices to the Vertex Attay
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);
    // Setting up the vertex position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // The application loop
    while (!glfwWindowShouldClose(window))
    {
        // Clearing the colors of the display
        glClear(GL_COLOR_BUFFER_BIT);
        // Draw the mesh using the shader
        s.Use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Swapping the buffers
        glfwSwapBuffers(window);
        // Updating the input states
        glfwPollEvents();
    }
    // Terminating the Window instance
    glfwTerminate();
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    return 0;
}