#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include "shader.h"

#define PI 3.14159265359f

// Window dimensions
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    // GLFW settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SolarSim2D - Circle", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL function pointers with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // Load shaders
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");

    // Generate circle vertices (center + perimeter)
    std::vector<float> vertices;
    float r = 0.5f, cx = 0.0f, cy = 0.0f;
    int seg = 100;

    // Center vertex
    vertices.push_back(cx);
    vertices.push_back(cy);

    for (int i = 0; i <= seg; i++) {
        float theta = 2.0f * PI * float(i) / float(seg);
        float x = cx + r * cosf(theta);
        float y = cy + r * sinf(theta);
        vertices.push_back(x);
        vertices.push_back(y);
    }

    // Setup buffers
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, seg + 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Adjust viewport on resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Close window on ESC
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
