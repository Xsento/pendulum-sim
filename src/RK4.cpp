#include "RK4.hpp"
#include "globals.hpp"

void RK4::singlePendulum::eval(double x, std::vector<double> y, std::vector<double>& dydx){
    dydx[0] = y[1];    // y[1] - angular velocity, dydx[0] - first derivative of the angle
    
    // angular acceleration
    dydx[1] = -g/L * sin(y[0]) - q * y[1] + F_drive * sin(Omega * x);
}

void RK4::doublePendulum::eval(double x, std::vector<double> y, std::vector<double>& dydx){
    // the two motion equations are expressed as a matrix in order to simplify the calculations of the second derivatives
    
    double A1 = 1/2. * L * cos(y[0] - y[2]);
    double A2 = 1/3. * L;
    double B1 = 4/3. * L;
    double B2 = 1/2. * L * cos(y[0] - y[2]);

    glm::mat2 M;
    M[0][0] = A1;   M[1][0] = A2;
    M[0][1] = B1;   M[1][1] = B2;

    glm::vec2 X; // second derivatives vector

    // right hand side vector
    glm::vec2 RHS(
        1/2. * L * y[1]*y[1] * sin(y[0] - y[2]) - 1/2. * g * sin(y[2]),
        -1/2. * L * y[3]*y[3] * sin(y[0] - y[2]) - 3/2. * g * sin(y[0])
    );

    X = glm::inverse(M) * RHS;

    dydx[0] = y[1];
    dydx[1] = X[0];
    dydx[2] = y[3];
    dydx[3] = X[1];
}

void RK4::fixed(void (*eval)(double x, std::vector<double> y, std::vector<double>& dydx), double& x, std::vector<double>& y, double h){
    int n = y.size();
    std::vector<double> k1(n), k2(n), k3(n), k4(n), f(n), dydx(n);
    
    // f is a vector of intermediate states of y used in calculations

    // Zeroth intermediate step
    eval(x, y, dydx);
    for (int j = 0; j < n; j++)
    {
        k1[j] = h * dydx[j];
        f[j] = y[j] + k1[j] / 2.;
    }
    
    // First intermediate step
    eval(x + h / 2., f, dydx);
    for (int j = 0; j < n; j++)
    {
        k2[j] = h * dydx[j];
        f[j] = y[j] + k2[j] / 2.;
    }
    
    // Second intermediate step
    eval(x + h / 2., f, dydx);
    for (int j = 0; j < n; j++)
    {
        k3[j] = h * dydx[j];
        f[j] = y[j] + k3[j];
    }
    
    // Third intermediate step
    eval(x + h, f, dydx);
    for (int j = 0; j < n; j++)
    {
       k4[j] = h * dydx[j];
    }
    
    // Actual step
    for (int j = 0; j < n; j++)
    {
        y[j] += k1[j] / 6. + k2[j] / 3. + k3[j] / 3. + k4[j] / 6.;
    }
    x += h;

    return;
}