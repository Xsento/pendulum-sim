#pragma once

#include "main.hpp"
#include "shader.h"
#include "RK4.hpp"
#include "globals.hpp"

class Pendulum{
    GLuint VAO, VBO;
    glm::vec3 color;

    //double length;
    double angle;
    double angularVelocity;
    double mass;

    float vertices[6];

    public:
    Pendulum(double angle, double angularVelocity, glm::vec3 color);

    void setupBuffers();
    void updateAndDraw(Shader& shaderProgram, double t, double h);
};