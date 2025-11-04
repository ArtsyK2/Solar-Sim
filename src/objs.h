#pragma once
#include <glad/glad.h>
#include <vector>
#include <cmath>

#define PI 3.14159265359f

class C_Entity {
public:
    unsigned int VAO, VBO;
    std::vector<float> vertices;

    C_Entity(float cx = 0.0f, float cy = 0.0f, float radius = 50.0f, int segments = 100) {
        // Generate circle vertices (triangle fan)
        vertices.push_back(cx);
        vertices.push_back(cy);

        for (int i = 0; i <= segments; i++) {
            float theta = 2.0f * PI * float(i) / float(segments);
            float x = cx + radius * cosf(theta);
            float y = cy + radius * sinf(theta);
            vertices.push_back(x);
            vertices.push_back(y);
        }

        // Setup VAO and VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Draw() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
        glBindVertexArray(0);
    }

    ~C_Entity() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};
