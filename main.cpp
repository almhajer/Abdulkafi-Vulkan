#include"header/Window.hpp"


int main() {
    Application::WIDTH=1200;
    Application::HEIGHT=1200;

    Application::Window app;

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
