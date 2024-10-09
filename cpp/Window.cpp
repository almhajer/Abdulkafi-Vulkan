//
// Created by Abdulkafi on 03/10/2024 A.
//
#include "Window.hpp"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>

namespace Application {

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT meesageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackDate,
            void *pUserData) {
        std::cerr << "validation layer: " << pCallbackDate->pMessage << std::endl;
        return VK_FALSE;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                               "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
                                                                                "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

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
        createInstance();
        setupDebugMessenger();
    }

    // تعريف دالة رد الفعل (callback) الخاصة بالنقر بالماوس كدالة ثابتة
    void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            std::cout << "تم النقر بالزر الأيسر للماوس!" << std::endl;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            std::cout << "تم النقر بالزر الأيمن للماوس!" << std::endl;
        }
    }

    void Window::mainLoop() const {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void Window::cleanup() const {
        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }


    void Window::createInstance() {
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }
        //معلومات التطبيق
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Abdulkafi BK Game";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "no Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);


        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &appInfo;


        instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        std::vector<const char *> requiredextensions = requiredExtensions();

        instanceCreateInfo.enabledExtensionCount = (uint32_t) requiredextensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = requiredextensions.data();
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
            populateDebugMessengerCreateInfo(debugCreateInfo);
            instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

        } else {
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }


        if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create instance");
        }
#ifdef DEBUG_MODE
        std::cout << "VkInstance created successfully. Handle value: " << reinterpret_cast<uintptr_t>(instance)
                  << std::endl;
        extensionProperties();
#endif
    }


    std::vector<const char *> Window::requiredExtensions(bool includePortabilityExtension) {
        // الحصول على قائمة ملحقات GLFW المطلوبة
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        if (!glfwExtensions) {
            throw std::runtime_error("Failed to get required GLFW extensions");
        }

        // استخدام مجموعة للتحقق من التكرار وتجنب الإضافات المكررة
        std::set<const char *> requiredExtensionsSet(glfwExtensions, glfwExtensions + glfwExtensionCount);

        // إذا كان التضمين مطلوبًا، تحقق من عدم التكرار قبل الإضافة
        if (includePortabilityExtension) {
            requiredExtensionsSet.insert(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        }

        // تحويل مجموعة الامتدادات إلى `std::vector` للعودة كقائمة
        std::vector<const char *> requiredExtensions(requiredExtensionsSet.begin(), requiredExtensionsSet.end());
        if (enableValidationLayers) {
            requiredExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
#ifdef DEBUG_MODE
        // الطباعة للتأكد من الامتدادات المطلوبة (إذا كان وضع التصحيح مفعلًا)
        std::cout << "Required extensions count: " << requiredExtensions.size() << std::endl;
        for (const auto &extension: requiredExtensions) {
            std::cout << extension << std::endl;
        }
#endif

        return requiredExtensions;
    }


    // خصائص الاضافات
    void Window::extensionProperties() const {
        // الحصول على عدد الامتدادات المتوفرة
        uint32_t extensionCount = 0;
        VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate instance extension properties.");
        }

        // إنشاء المتجه لتخزين خصائص الامتدادات
        std::vector<VkExtensionProperties> extensions(extensionCount);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate instance extension properties.");
        }

        // طباعة الامتدادات
        std::cout << "Available extensions: " << extensionCount << std::endl;
        for (const auto &extension: extensions) {
            std::cout << "  Name: " << extension.extensionName
                      << " | Version: " << extension.specVersion << std::endl;
        }
    }

    bool Window::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        for (const char *layerName: validationLayers) {
            bool layerFound = false;
            for (const auto &layerProperties: availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) {
                return false;
            }
        }
        return true;
    }

    void Window::setupDebugMessenger() {
        if (!enableValidationLayers) return;
        VkDebugUtilsMessengerCreateInfoEXT createInfoExt{};
        createInfoExt.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfoExt.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfoExt.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfoExt.pfnUserCallback = debugCallback;
        createInfoExt.pUserData = nullptr;//optional

    }


};
