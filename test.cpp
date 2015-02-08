#include "TimeWatcher.hpp"

#include <iostream>
#include <cmath>

int main()
{
    TimeWatcher::getInstance().runAll();
    return 0;
}

WATCH(Test, cos)
{
    double i = std::cos(INTERATION/1000.0);
    (void)i;
}

WATCH(Test, sin)
{
    double i = std::sin(INTERATION/1000.0);
    (void)i;
}

WATCH(Test, exp)
{
    double i = std::exp(INTERATION/1000.0);
    (void)i;
}

WATCH(Test, pow)
{
    double i = std::pow(2.0,INTERATION/1000.0);
    (void)i;
}
