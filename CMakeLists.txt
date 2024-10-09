cmake_minimum_required(VERSION 3.10)
project(Abdulkafi)

# إعداد الإصدار القياسي لـ C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# تحديد نوع البناء الافتراضي (إذا لم يتم تحديده)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# تحديد مسارات الـ GLFW و Vulkan SDK
set(GLFW_SDK "/Users/mac/glfw")
set(VULKAN_SDK "/Users/mac/VulkanSDK/macOS")
set(MoltenVK_LIB "/Users/mac/VulkanSDK/macOS")

# التحقق من صحة مسار Vulkan SDK
if (NOT VULKAN_SDK OR NOT EXISTS "${VULKAN_SDK}")
    message(FATAL_ERROR "VULKAN_SDK environment variable is not set correctly. Please set it to the location of your Vulkan SDK.")
endif()

# جمع جميع ملفات .cpp و .hpp و .h من المشروع والمجلدات الفرعية
file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/cpp/*.cpp"
        "${CMAKE_SOURCE_DIR}/header/*.hpp"
        "${CMAKE_SOURCE_DIR}/header/*.h"
        "${CMAKE_SOURCE_DIR}/main.cpp"  # تضمين main.cpp بشكل صريح
)

# إضافة الهدف التنفيذي
add_executable(Abdulkafi MACOSX_BUNDLE ${SOURCES})

# إضافة مسارات include directories
target_include_directories(Abdulkafi PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/header
        ${CMAKE_SOURCE_DIR}/stb
        "${VULKAN_SDK}/include"
        "${GLFW_SDK}/include"
)

# ربط مكتبة Vulkan
find_library(Vulkan_LIBRARY NAMES vulkan HINTS "${VULKAN_SDK}/lib")
if (NOT Vulkan_LIBRARY)
    message(FATAL_ERROR "Could not find Vulkan library. Make sure Vulkan SDK is installed and the VULKAN_SDK path is correct.")
endif()

# تحديد مسار MoltenVK (اختياري)
find_library(MoltenVK_LIB NAMES libMoltenVK HINTS "${MoltenVK_LIB}/lib")
if (NOT Vulkan_LIBRARY)
    message(FATAL_ERROR "Could not find libMoltenVK library. Make sure Vulkan SDK is installed and the VULKAN_SDK path is correct.")
endif()

# ربط مكتبة GLFW
find_library(GLFW_LIBRARY NAMES glfw glfw3 HINTS "${GLFW_SDK}/lib")
if (NOT GLFW_LIBRARY)
    message(FATAL_ERROR "Could not find GLFW library. Make sure GLFW is built and the GLFW_SDK path is correct.")
endif()

# ربط المكتبات والأطر المطلوبة
target_link_libraries(Abdulkafi PRIVATE
        ${Vulkan_LIBRARY}
        ${GLFW_LIBRARY}
        ${Vulkan_LIBRARY}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
)

# إعداد RPATH لمكتبات التشغيل
set_target_properties(Abdulkafi PROPERTIES
        INSTALL_RPATH "${VULKAN_SDK}/lib;/usr/local/lib"
        BUILD_WITH_INSTALL_RPATH TRUE
)

# إعداد الأيقونة لتطبيق macOS (اختياري)
set(APP_ICON_MACOSX "${CMAKE_CURRENT_SOURCE_DIR}/Resources/logo.icns")
if (EXISTS "${APP_ICON_MACOSX}")
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(Abdulkafi PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_ICON_FILE "logo.icns"
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info.plist"
            RESOURCE "${APP_ICON_MACOSX}"
    )
endif()

# نسخ مجلد Resources إلى داخل حزمة التطبيق بعد البناء
add_custom_command(
        TARGET Abdulkafi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/Abdulkafi.app/Contents/Resources"
        COMMENT "Copying Resources to the application bundle"
)

# إعداد بيئة البناء بناءً على نوع البناء (Debug أو Release)
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE is not set. Defaulting to Release mode.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# إضافة تعاريف الترجمة بناءً على نوع البناء
target_compile_definitions(Abdulkafi PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

# إعداد التحسينات عند بناء إصدار Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# تشغيل ملف سكربت بعد بناء المشروع لتجميع الـ shaders
add_custom_command(
        TARGET Abdulkafi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env bash "${CMAKE_SOURCE_DIR}/shaders/compile.sh"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Executing shader compile script after build..."
)

# نسخ محتويات مجلد shaders إلى مجلد التنفيذ بعد البناء
add_custom_command(
        TARGET Abdulkafi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders"
        COMMENT "Copying shaders to the build directory..."
)
# التحقق من نوع البناء
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_MODE)  # تعريف المتغير DEBUG_MODE في حالة Debug
endif()

# أو يمكن استخدام target_compile_definitions لتعريفه على مستوى الهدف (Target Level)
target_compile_definitions(Abdulkafi PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE>  # تعريف DEBUG_MODE فقط عند البناء في وضع Debug
)


# تضمين CPack لإنشاء الحزم (اختياري)
include(CPack)
