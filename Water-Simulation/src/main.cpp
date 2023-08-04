#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
    std::cout << "Hello, Water Sim!" << std::endl;
    // The application loop
    while (!glfwWindowShouldClose(window))
    {
        // Clearing the colors of the display
        glClear(GL_COLOR_BUFFER_BIT);
        // Swapping the buffers
        glfwSwapBuffers(window);
        // Updating the input states
        glfwPollEvents();
    }
    // Terminating the Window instance
    glfwTerminate();
    return 0;
}