//
// Created by Abdulkafi on 03/10/2024 A.
//
#include "Window.hpp"
#include <stdexcept>
#include <iostream>


namespace Application {

    void Window::run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    void Window::initWindow() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // الحصول على الشاشة الأساسية
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        if (!primaryMonitor) {
            glfwTerminate();
            throw std::runtime_error("Failed to get primary monitor");
        }

        // الحصول على معلومات الفيديو (الدقة) للشاشة الأساسية
        const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
        if (videoMode) {
            std::cout << "Screen Width: " << videoMode->width << std::endl;
            std::cout << "Screen Height: " << videoMode->height << std::endl;
            std::cout << "Refresh Rate: " << videoMode->refreshRate << " Hz" << std::endl;
        }

        // ضبط عرض وارتفاع النافذة ليكون نصف الشاشة
        WIDTH = videoMode->width;
        HEIGHT = videoMode->height;

        // إنشاء النافذة
        window = glfwCreateWindow(WIDTH / 2, HEIGHT / 2, "Abdulkafi", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }


        // جعل السياق الحالي للنافذة
        glfwMakeContextCurrent(window);

        // تعيين دالة رد الفعل للنقر بالماوس
        glfwSetMouseButtonCallback(window, mouse_button_callback);
    }

    void Window::initVulkan() {
        // مكان التهيئة الخاصة بـ Vulkan (إذا كنت تريد استخدام Vulkan)
    }

    // تعريف دالة رد الفعل (callback) الخاصة بالنقر بالماوس كدالة ثابتة
    void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            std::cout << "تم النقر بالزر الأيسر للماوس!" << std::endl;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            std::cout << "تم النقر بالزر الأيمن للماوس!" << std::endl;
        }
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
