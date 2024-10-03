//
// Created by Abdulkafi on 03/10/2024 A.
//
#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

namespace Application {
    inline uint32_t WIDTH = 800;
    inline uint32_t HEIGHT = 600;

    class Window {
    public:
        void run();

    private:
        GLFWwindow *window;

        void initWindow();

        void initVulkan();

        void mainLoop();

        void cleanup();
    };
}
#endif //WINDOW_H
