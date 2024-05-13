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
    Potato(){}
    Potato(int num_hops) : hop(num_hops),path(vector<int>(512)) {}

};
