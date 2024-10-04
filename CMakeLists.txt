cmake_minimum_required(VERSION 3.10)
project(Abdulkafi)

# إعداد الإصدار القياسي لـ C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

# إضافة مسار المشروع بالكامل إلى include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/header)
include_directories(${CMAKE_SOURCE_DIR}/stb)

# تحديد مسار Vulkan SDK
set(VULKAN_SDK "/Users/mac/VulkanSDK/macOS")

# إضافة مسار الـ include files (headers) لمكتبة Vulkan
include_directories(${VULKAN_SDK}/include)

# ربط مكتبة Vulkan
find_library(Vulkan_LIBRARY NAMES vulkan HINTS ${VULKAN_SDK}/lib)
if (NOT Vulkan_LIBRARY)
    message(FATAL_ERROR "Could not find Vulkan library")
endif()

# تحديد مسار MoltenVK ليشير إلى النسخة في /usr/local/lib
set(MOLTENVK_LIB "/usr/local/lib/libMoltenVK.dylib")
if (EXISTS ${MOLTENVK_LIB})
    target_link_libraries(Abdulkafi ${MOLTENVK_LIB})
else()
    message(WARNING "MoltenVK library not found at ${MOLTENVK_LIB}")
endif()

# تحديد مسار GLFW
set(GLFW_SDK "/Users/mac/glfw")

# إضافة مسار الـ include files لمكتبة GLFW
include_directories(${GLFW_SDK}/include)

# ربط مكتبة GLFW
find_library(GLFW_LIBRARY NAMES glfw3 HINTS ${GLFW_SDK}/lib)
if (NOT GLFW_LIBRARY)
    message(FATAL_ERROR "Could not find GLFW library")
endif()

# ربط مكتبات النظام الأساسية على macOS
target_link_libraries(Abdulkafi ${Vulkan_LIBRARY} ${GLFW_LIBRARY}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
)

# إعداد RPATH لمكتبات التشغيل (تأكد من أن Vulkan SDK يأتي أولاً)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${VULKAN_SDK}/lib;/usr/local/lib")

# إعداد الأيقونة لتطبيق macOS

set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/Resources/logo.icns)
set(MACOSX_BUNDLE_ICON_FILE "logo.icns")
set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

# إضافة متغير الأيقونة إلى Info.plist عن طريق تعيين الخصائص
set_target_properties(Abdulkafi PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info.plist
        RESOURCE "${APP_ICON_MACOSX}"
)
# نسخ مجلد Resources إلى داخل حزمة التطبيق بعد البناء
add_custom_command(
        TARGET Abdulkafi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/Abdulkafi.app/Contents/Resources
        COMMENT "Copying Resources to the application bundle"
)
# إعداد بيئة البناء بناءً على نوع البناء
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DNDEBUG)
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
