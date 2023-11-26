#include <iostream>
#include "../headers/image.hpp"

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    Image img("../images/kity.jpg");

    img.applyLuminance();
    img.display();

    img.computeHistogram();
    img.displayHistogram();

    img.histogramEqualization();
    img.displayHistogram();
    img.display();
 
    if (Image::getNumberWindows() > 0)
        gtk_main();

    return 0;
}