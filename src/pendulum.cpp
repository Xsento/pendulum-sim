#include "pendulum.hpp"

Pendulum::Pendulum(double angle, double angularVelocity, glm::vec3 color) : angle(angle), angularVelocity(angularVelocity), color(color){
    Pendulum::setupBuffers();
}

void Pendulum::updateAndDraw(Shader& shaderProgram, double t, double h){
    std::vector<double> y = {angle, angularVelocity};
    RK4::fixed(RK4::singlePendulum::eval, t, y, h);

    angle = y[0];
    angularVelocity = y[1];

    while (angle > 2*M_PI){
        angle -= 2*M_PI;
    }

    vertices[0] = 0.f;
    vertices[1] = 0.f;
    vertices[2] = 0.f;
    vertices[3] = static_cast<float>(L * sin(angle));
    vertices[4] = static_cast<float>(-L * cos(angle));
    vertices[5] = 0.f;

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
 
    shaderProgram.use();
    shaderProgram.setVec3("uniColor", color);
    glDrawArrays(GL_LINES, 0, 2);
}

void Pendulum::setupBuffers(){
    for (int i = 0; i <= 6; i++){
        vertices[i] = 0.f;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}