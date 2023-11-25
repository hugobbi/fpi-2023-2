#include <iostream>
#include <thread>
#include "../headers/image.h"

int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create();

    Image test("../images/kity.jpg");
    test.displayImage();

    test.applyLuminance();

    std::cout << "jbdkjas" << std::endl;

    app->run();

    return 0;
}