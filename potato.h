#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

class Potato
{
    int hop{};
    std::vector<int> path;
    Potato() : path(512) {}
};