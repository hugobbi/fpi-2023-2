#include <iostream>
#include "../headers/image.hpp"

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    Image img("../images/avo_e_pai.jpg");

    img.computeHistogram();
    img.displayHistogram();

    if (Image::getNumberWindows() > 0)
        gtk_main();

    return 0;
}