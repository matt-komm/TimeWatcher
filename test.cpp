#include "TimeWatcher.hpp"

#include <iostream>

int main()
{
    TimeWatcher::getInstance().runAll();
    return 0;
}

WATCH(Test, bla)
{
}
WATCH(Test, blub)
{
}
