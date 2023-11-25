#include <iostream>
#include <thread>
#include "../headers/image.h"

int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    Image test("../images/kity.jpg");
    test.displayImage();

    std::cout << "jbdkjas" << std::endl;

    Gtk::Main::run();

    return 0;
}