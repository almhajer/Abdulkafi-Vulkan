cmake_minimum_required(VERSION 3.10)
project(Abdulkafi)

# إعداد الإصدار القياسي لـ C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# جمع جميع ملفات .cpp و .hpp من الجذر وكل المجلدات الفرعية
file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/*.cpp"
        "${CMAKE_SOURCE_DIR}/*.hpp"
        "${CMAKE_SOURCE_DIR}/*.h"
)

# استبعاد الملفات الموجودة في مجلد CMakeFiles
file(GLOB_RECURSE EXCLUDE_FILES "${CMAKE_BINARY_DIR}/CMakeFiles/*.cpp")
list(REMOVE_ITEM SOURCES ${EXCLUDE_FILES})

# إضافة ملفات المصدر إلى الهدف التنفيذي (executable target)
# تحديد المجلدات التي تحتوي على الملفات
file(GLOB SOURCES
        "src/*.cpp"        # جميع ملفات CPP في مجلد src
        "cpp/*.cpp"        # جميع ملفات CPP في مجلد cpp
        "header/*.h"       # جميع ملفات HEADER في مجلد header
)

# إضافة الهدف التنفيذي
add_executable(Abdulkafi ${SOURCES} main.cpp)

# إضافة مسار المشروع بالكامل إلى include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/header/)

# تحديد مسار Vulkan SDK
set(VULKAN_SDK "/Users/abdulkafi/VulkanSDK/macOS/")

# إضافة مسار الـ include files (headers) لمكتبة Vulkan
include_directories(${VULKAN_SDK}/Include)


# ربط مكتبة Vulkan من /usr/local/lib
find_library(Vulkan_LIBRARY NAMES vulkan HINTS /usr/local/lib)
target_link_libraries(Abdulkafi ${Vulkan_LIBRARY})

# تحديد مسار MoltenVK ليشير إلى النسخة في /usr/local/lib
set(MOLTENVK_LIB "/usr/local/lib/libMoltenVK.dylib")

# ربط مكتبة MoltenVK مع المشروع
target_link_libraries(Abdulkafi ${MOLTENVK_LIB})

# تحديد مسار GLFW (المسار إلى مكتبة GLFW)
set(GLFW_SDK "/Users/abdulkafi/glfw")

# إضافة مسار الـ include files (headers) لمكتبة GLFW
include_directories(${GLFW_SDK}/include)

# ربط مكتبة GLFW
find_library(GLFW_LIBRARY NAMES glfw3 HINTS ${GLFW_SDK}/lib)
target_link_libraries(Abdulkafi ${GLFW_LIBRARY})

# ربط مكتبات النظام الأساسية على macOS
target_link_libraries(Abdulkafi
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
)

# إعداد RPATH لمكتبات التشغيل
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "/usr/local/lib;/Users/abdulkafi/VulkanSDK/macOS/lib")

# إعداد بيئة البناء بناءً على نوع البناء
if (CMAKE_BUILD_TYPE)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-DDEBUG)  # تفعيل DEBUG في بيئة التصحيح
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_definitions(-DNDEBUG)  # تفعيل NDEBUG في بيئة الإصدار
    endif()
else()
    message(STATUS "CMAKE_BUILD_TYPE is not set. Defaulting to Release mode.")
    set(CMAKE_BUILD_TYPE "Release")
    add_definitions(-DNDEBUG)
endif()

# تشغيل ملف سكربت بعد بناء المشروع
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env bash ${CMAKE_SOURCE_DIR}/shaders/compile.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Executing shader compile script after build..."
)

# نسخ محتويات مجلد shaders إلى مجلد التنفيذ بعد تشغيل الكود
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
        COMMENT "Copying shaders to the build directory..."
)

# إضافة الهدف لتشغيل سكربت معين عند بناء المشروع
add_custom_target(run_script ALL
        COMMAND ${CMAKE_COMMAND} -E env bash ${CMAKE_SOURCE_DIR}/shaders/compile.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running shader compile script..."
)
