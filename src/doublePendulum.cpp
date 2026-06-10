#include "doublePendulum.hpp"

DoublePendulum::DoublePendulum(double angle1, double angle2, double angularVelocity1, double angularVelocity2, glm::vec3 color) : 
    angle1(angle1), angle2(angle2), angularVelocity1(angularVelocity1), angularVelocity2(angularVelocity2), color(color)
{
    DoublePendulum::setupBuffers();
}

void DoublePendulum::updateAndDraw(Shader& shaderProgram, double t, double h){
    std::vector<double> y = {angle1, angularVelocity1, angle2, angularVelocity2};
    RK4::fixed(RK4::doublePendulum::eval, t, y, h);

    angle1 = y[0];
    angularVelocity1 = y[1];
    angle2 = y[2];
    angularVelocity2 = y[3];

    vertices[0] = 0.f;
    vertices[1] = 0.f;
    vertices[2] = 0.f;
    vertices[3] = static_cast<float>(L * sin(angle1));
    vertices[4] = static_cast<float>(-L * cos(angle1));
    vertices[5] = 0.f;
    vertices[6] = static_cast<float>(vertices[3] + L * sin(angle2));
    vertices[7] = static_cast<float>(vertices[4] - L * cos(angle2));
    vertices[8] = 0.f;

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
 
    shaderProgram.use();
    shaderProgram.setVec3("uniColor", color);
    glDrawArrays(GL_LINE_STRIP, 0, 3);
}

void DoublePendulum::setupBuffers(){
    for (int i = 0; i <= 8; i++){
        vertices[i] = 0.f;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}