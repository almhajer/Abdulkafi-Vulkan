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


    class Window {
    public:
        void run();

    private:
        GLFWwindow *window;
         uint32_t WIDTH = 800;
         uint32_t HEIGHT = 600;

        void initWindow();

        void initVulkan();

        void mainLoop();

        void cleanup();

        static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        // لا تضع Window:: هنا
    };
}

#endif // WINDOW_HPP
