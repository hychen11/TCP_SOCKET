#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

class Potato
{
public:
    int hop{};
    std::vector<int> path;
    Potato() : path(512) {}
    Potato(int num_hops) : hop(num_hops),path(512) {}

};
