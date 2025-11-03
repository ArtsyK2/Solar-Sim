#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Circle - OpenGL", NULL, NULL);
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
    shader.use();

    // Generate circle vertices
    std::vector<float> vertices;
    float radius = 50.0f, cx = 0.0f, cy = 0.0f;
    int segments = 100;


    // Center vertex
    vertices.push_back(cx);
    vertices.push_back(cy);

    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * PI * float(i) / float(segments);
        float x = cx + radius * cosf(theta);
        float y = cy + radius * sinf(theta);
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

    // Projection matrix (normalized space)
    glm::mat4 projection = glm::ortho(
    -(float)WIDTH/2, (float)WIDTH/2,    // left, right
    -(float)HEIGHT/2, (float)HEIGHT/2,    // bottom, top
    -1.0f, 1.0f      // near, far
    );
    shader.setMat4("projection", projection);

    // Model and view
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(200.0f, 0.0f, 0.0f));

    shader.setMat4("model", model);
    shader.setMat4("view", view);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Resize window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// ESC to close
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
