#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include <iostream>
#include <array>

struct WindowProps
{
    // The Windows width and height
    int Width, Height;
    // The Windows title
    std::string Title;
    // The Camera (stored as a pointer to optimize for space)
    Camera* ptrCamera;
};

void frameBufferResizeFn(GLFWwindow* window, int width, int height)
{
    WindowProps* props = (WindowProps*)glfwGetWindowUserPointer(window);
    props->Width = width;
    props->Height = height == 0 ? 1 : height;
    glViewport(0, 0, width, height);
}

void keyFn(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_1:
    case GLFW_KEY_F1:
        if (action == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLFW_KEY_2:
    case GLFW_KEY_F2:
        if (action == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case GLFW_KEY_3:
    case GLFW_KEY_F3:
        if (action == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    }
}

void processInput(GLFWwindow* window, Camera& camera, float delta)
{
    static float speed = 2.5f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = 5.0f;
    else
        speed = 2.5f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Move(camera.GetFront() * speed * delta);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Move(-camera.GetFront() * speed * delta);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Move(-camera.GetRight() * speed * delta);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Move(camera.GetRight() * speed * delta);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.Move(Camera::WorldUp * speed * delta);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.Move(-Camera::WorldUp * speed * delta);
}

void cursorPosFn(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    Camera& camera = *((WindowProps*)glfwGetWindowUserPointer(window))->ptrCamera;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        static float xlast = 0.0f;
        static float ylast = 0.0f;
        static float past = 0.0f;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (firstMouse)
        {
            xlast = static_cast<float>(xpos);
            ylast = static_cast<float>(ypos);
            firstMouse = false;
        }
        float xoffset = static_cast<float>(xpos) - xlast;
        float yoffset = ylast - static_cast<float>(ypos);
        camera.PitchBy(yoffset * 0.1f);
        camera.YawBy(xoffset * 0.1f);
        xlast = static_cast<float>(xpos);
        ylast = static_cast<float>(ypos);
    }
    else
    {
        firstMouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void scrollFn(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera& camera = *((WindowProps*)glfwGetWindowUserPointer(window))->ptrCamera;
    camera.DecFOV(yoffset);
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
    WindowProps props = { 1280, 720, "Water Simulation", new Camera(glm::vec3(0.0f, 6.0f, -20.0f), -30.0f, 90.0f, 45.0f) };
    Camera& camera = *props.ptrCamera;
    // Creating the Window
    GLFWwindow* window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
    // Testing that the Window was created
    if (!window) return -1;
    // Further window configuration
    glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowUserPointer(window, &props);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeFn);
    glfwSetKeyCallback(window, keyFn);
    glfwSetCursorPosCallback(window, cursorPosFn);
    glfwSetScrollCallback(window, scrollFn);
    // Setting the Window to be the current render context
    glfwMakeContextCurrent(window);
    // Loading OpenGL functions via Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    std::cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    glfwSwapInterval(1);
    // Loading shader resource
    Shader shader({ "res/shaders/shader.frag", ShaderType::Fragment }, { "res/shaders/shader.vert", ShaderType::Vertex }, { "res/shaders/shader.geom", ShaderType::Geometry });
    Mesh mesh("res/meshes/plane.obj");
    // The application loop
    std::array<glm::vec3, 9> transforms =
    {
        glm::vec3(0.0f),
        glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3(10.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -10.0f),
        glm::vec3(0.0f, 0.0f, -10.0f),
        glm::vec3(-10.0f, 0.0f, 0.0f),
        glm::vec3(-10.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3(0.0f, 0.0f, 10.0f),
    };
    float past = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window))
    {
        // Calculating the time delta
        float now = static_cast<float>(glfwGetTime());
        float delta = now - past;
        past = now;
        // Clearing the colors of the display
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window, camera, delta);
        // Setting the shaders uniforms
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), static_cast<float>(props.Width) / static_cast<float>(props.Height), 0.1f, 100.0f);
        shader.SetMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        shader.SetMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        shader.SetFloat("time", (float)glfwGetTime());
        // Drawing the mesh
        for (int i = 0; i < transforms.size(); i++)
        {
            model = glm::translate(model, transforms[i]);
            shader.SetMat4("model", model);
            mesh.Draw(shader);
        }
        // Swapping the buffers
        glfwSwapBuffers(window);
        // Updating the input states
        glfwPollEvents();
    }
    // Terminating the Window instance
    glfwTerminate();
}
