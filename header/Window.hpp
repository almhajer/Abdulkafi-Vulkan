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
#include<vector>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif



namespace Application {
    class Window {
    public:
        void run();

    private:


        GLFWwindow *window;
        uint32_t WIDTH = 800;
        uint32_t HEIGHT = 600;
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        const std::vector<const char *> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        void initWindow();

        void initVulkan();

        void mainLoop() const;

        void cleanup() const;

        void setupDebugMessenger();

        //تهيئة مثيل فولكان
        void createInstance();

        //الحصول علي الاضافات
        std::vector<const char *> requiredExtensions(bool includePortabilityExtension = true);

        void extensionProperties() const;

        // التحقق من الطبقات
        bool checkValidationLayerSupport();

        static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        // لا تضع Window:: هنا
    };
}

#endif // WINDOW_HPP
