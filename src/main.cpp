#include <iostream>
#include "../headers/image.h"

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    Image img("../images/kity.jpg");
    img.displayImage();

    img.applyLuminance();
    img.displayImage();

    if (Image::getNumberWindows() > 0)
        gtk_main();

    return 0;
}