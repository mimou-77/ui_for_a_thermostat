# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)





set(EXTRA_COMPONENT_DIRS ~/esp-box/components/)

add_compile_options(-fdiagnostics-color=always)

set(LVGL_CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build/config/sdkconfig.h")




project(ui3_thermostat)
