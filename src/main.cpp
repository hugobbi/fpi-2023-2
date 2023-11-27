#include <iostream>
#include "../headers/image.hpp"

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    Image img("../images/kity.jpg");
    img.display();
    img.zoomOut(8, 8);
    img.zoomIn();
    img.display();

    if (Image::getNumberWindows() > 0)
        gtk_main();

    return 0;
}