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
    int index{};
    //weird bug : change this to vector will cause segment fault, and valgrind cannot find out why?
    // maybe the object move copy will cause vector error!
    // int path[512];
    std::vector<int> path{};
    Potato(){}
    Potato(int num_hops) : hop(num_hops){
        //memset(path,0,sizeof(path));
        path.resize(num_hops);
        }

};
