//
// Created by Abdulkafi on 03/10/2024 A.
//
#include "Window.hpp"

namespace Application {
    void Window::run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }


    void Window::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Abdulkafi", nullptr, nullptr);
    }


    void Window::initVulkan() {
    }


    void Window::mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }


    void Window::cleanup() {
        glfwDestroyWindow(window);

        glfwTerminate();
    }
};
