cmake_minimum_required(VERSION 3.16)

project(gpio_stmp_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appgpio_stmp_demo
    main.cpp
)

qt_add_qml_module(appgpio_stmp_demo
    URI gpio_stmp_demo
    QML_FILES
        Main.qml
        SOURCES gpio/igpiopin.h
        SOURCES gpio/mockgpiopin.h gpio/mockgpiopin.cpp
        SOURCES gpio/realgpiopin.h gpio/realgpiopin.cpp
        SOURCES gpio/gpiofactory.h gpio/gpiofactory.cpp
        RESOURCES go_fonts/Go-Bold-Italic.ttf go_fonts/Go-Bold.ttf go_fonts/Go-Italic.ttf go_fonts/Go-Medium-Italic.ttf go_fonts/Go-Medium.ttf go_fonts/Go-Regular.ttf go_fonts/README
)

# Optional: real GPIO (libgpiod) - off by default for easy desktop build
option(USE_REAL_GPIO "Use real libgpiod (requires libgpiod-dev on target/desktop)" OFF)

if(USE_REAL_GPIO)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBGPIOD REQUIRED libgpiod)
    target_link_libraries(appgpio_stmp_demo PRIVATE ${LIBGPIOD_LIBRARIES})
    target_include_directories(appgpio_stmp_demo PRIVATE ${LIBGPIOD_INCLUDE_DIRS})
    target_compile_definitions(appgpio_stmp_demo PRIVATE USE_REAL_GPIO)
    message(STATUS "Building with REAL GPIO (libgpiod)")
else()
    message(STATUS "Building with MOCK GPIO (perfect for Windows/WSL desktop)")
endif()


target_link_libraries(appgpio_stmp_demo
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appgpio_stmp_demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
