#include "main.hpp"

namespace RK4{

namespace singlePendulum{
    // calculate physics
    void eval(double x, std::vector<double> y, std::vector<double>& dydx);
}

namespace doublePendulum{
    // calculate physics
    void eval(double x, std::vector<double> y, std::vector<double>& dydx);
}

// do the math
void fixed(void (*eval)(double x, std::vector<double> y, std::vector<double>& dydx), double& x, std::vector<double>& y, double h);

}