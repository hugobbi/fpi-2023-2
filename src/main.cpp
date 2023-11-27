#include <iostream>
#include "../headers/image.hpp"

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    Image img1("../images/kity.jpg");
    img1.computeLuminance();
    img1.applyLuminance();
    img1.display();
    img1.computeHistogram();
    img1.displayHistogram();

    if (Image::getNumberWindows() > 0)
        gtk_main();

    return 0;
}