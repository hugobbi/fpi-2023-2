#include <iostream>
#include "../headers/image.h"

int main(int argc, char* argv[])
{
    std::cout << "Starting" << std::endl;

    Image test("../images/kity.jpg");

    test.quantize(5);

    test.write("yee.jpg");

    return 0;
}