#include <iostream>
#include "easy/net/EventLoop.h"


void print()
{
    std::cout << "heelo" <<std::endl;
}


int main()
{
    easy::EventLoop loop;
    loop.runAfter(5, print);
    loop.loop();
    return 0;
}
