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
    int count{};
    int path[512];
    Potato(){}
    Potato(int num_hops) : hop(num_hops) {memset(path, 0, sizeof(path));}

};
