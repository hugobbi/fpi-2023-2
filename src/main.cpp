#include <iostream>
#include "../headers/image.h"

int main(int argc, char* argv[])
{
    // Display images in windows
    gtk_init(&argc, &argv);

    Image test("../images/kity.jpg");
    test.displayImage();

    std::cout << Image::getNumberWindows() << std::endl;

    Image shrek("../images/shrek.jpg");
    shrek.displayImage();

    std::cout << Image::getNumberWindows() << std::endl;

    Image av("../images/avo_e_pai.jpg");
    av.displayImage();

    std::cout << Image::getNumberWindows() << std::endl;

    // Displays images
    gtk_main();

    return 0;
}