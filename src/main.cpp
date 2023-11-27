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

    Image img2("../images/shrek.jpg");
    img2.computeLuminance();
    img2.applyLuminance();
    img2.display();
    img2.computeHistogram();
    img2.displayHistogram();

    img1.histogramMatching(img2);
    img1.display();
    img1.computeHistogram();
    img1.displayHistogram();

    if (Image::getNumberWindows() > 0)
        gtk_main();

    return 0;
}