#include <iostream>
#include "../headers/image.h"

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    Image img("../images/kity.jpg");

    img.computeHistogram();

    for(int i = 0; i < 256; i++)
        std::cout << i << ": " << img.histogram[i] << std::endl;

    img.displayImage();

    if (Image::getNumberWindows() > 0)
        gtk_main();

    return 0;
}