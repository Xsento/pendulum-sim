#pragma once

#include "main.hpp"
#include "shader.h"
#include "RK4.hpp"
#include "globals.hpp"

class DoublePendulum{
    GLuint VAO, VBO;
    glm::vec3 color;

    //double length;
    double angle1;
    double angle2;
    double angularVelocity1;
    double angularVelocity2;

    float vertices[9];

    public:

    DoublePendulum(double angle1, double angle2, double angularVelocity1, double angularVelocity2, glm::vec3 color);

    void setupBuffers();
    void updateAndDraw(Shader& shaderProgram, double t, double h);
};