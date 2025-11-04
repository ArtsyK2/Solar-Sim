#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>

#include "shader.h"
#include "objs.h"

#define PI 3.14159265359f

// Window dimensions
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
    // --- Initialize GLFW ---
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // --- Create window ---
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Circle - OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --- Initialize GLAD ---
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // --- Enable blending (optional for alpha) ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // --- Load shaders ---
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");

    // --- Generate Circle Entity ---
    C_Entity circle(0.0f, 0.0f, 50.0f, 100);

    // --- Set up matrices ---
    glm::mat4 projection = glm::ortho(
        -(float)WIDTH / 2, (float)WIDTH / 2,
        -(float)HEIGHT / 2, (float)HEIGHT / 2,
        -1.0f, 1.0f
    );

    glm::mat4 view = glm::mat4(1.0f);
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // --- Circle physics setup ---
    glm::vec3 cir_pos = glm::vec3(0.0f, 100.0f, 0.0f);
    glm::vec3 cir_velocity = glm::vec3(0.0f, 100.0f, 0.0f);
    glm::vec3 cir_acceleration = glm::vec3(0.0f, -650.0f, 0.0f); // gravity downward!

    float lastFrame = glfwGetTime();

    // --- Render loop ---
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // --- Physics update ---
        cir_velocity += cir_acceleration * deltaTime;
        cir_pos += cir_velocity * deltaTime;

        // --- Bounce check ---
        /*float floorY = -HEIGHT / 2 + 50.0f;

        if (cir_pos.y < floorY) {
            cir_pos.y = floorY;
            cir_velocity.y = -cir_velocity.y * 0.8f;

            // Small damping to prevent infinite bounces
            if (fabs(cir_velocity.y) < 5.0f){
                cir_velocity.y = 0.0f;
            }
        }*/


        // --- Render ---
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cir_pos);
        shader.setMat4("model", model);

        circle.Draw(); // Make sure C_Entity::Draw() binds VAO and uses glDrawArrays

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


//g++ src/main.cpp src/glad.c -Iinclude -Llib -lglfw3 -lopengl32 -lgdi32 -o app.exe