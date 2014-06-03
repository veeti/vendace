project(vendace)
cmake_minimum_required(VERSION 2.8)

# Find files
file(GLOB_RECURSE VENDACE_SOURCES src/*.c src/*.cpp)
file(GLOB_RECURSE VENDACE_HEADERS src/*.h src/*.hpp)
file(GLOB_RECURSE VENDACE_FORMS forms/*.ui)
file(GLOB_RECURSE VENDACE_RESOURCES forms/*.qrc)

# Find Qt
find_package(Qt5Widgets REQUIRED)

# Qt build steps
qt5_wrap_cpp(VENDACE_HEADERS_MOC ${VENDACE_HEADERS})
qt5_wrap_ui(VENDACE_FORMS_HEADERS ${VENDACE_FORMS})
qt5_add_resources(VENDACE_RESOURCES_CPP ${VENDACE_RESOURCES})

# Include generated files
include_directories(${CMAKE_CURRENT_SOURCEDIR} ${CMAKE_CURRENT_BINARY_DIR} src/)

# Add executable
add_executable(vendace ${VENDACE_SOURCES} ${VENDACE_HEADERS} ${VENDACE_HEADERS_MOC} ${VENDACE_FORMS_HEADERS} ${VENDACE_RESOURCES_CPP})
qt5_use_modules(vendace Widgets Network)

# Generate desktop file
configure_file(share/vendace.desktop "${CMAKE_CURRENT_BINARY_DIR}/vendace.desktop")

# Installation
install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/vendace" DESTINATION bin/)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/vendace.desktop" DESTINATION share/applications/)
install(FILES share/icons/vendace.svg DESTINATION share/icons/hicolor/scalable/apps/)
install(FILES share/icons/vendace-64.png DESTINATION share/icons/hicolor/64x64/apps/ RENAME vendace.png)
install(FILES share/icons/vendace-128.png DESTINATION share/icons/hicolor/128x128/apps/ RENAME vendace.png)
install(FILES share/icons/vendace-256.png DESTINATION share/icons/hicolor/256x256/apps/ RENAME vendace.png)
install(FILES share/icons/vendace-512.png DESTINATION share/icons/hicolor/512x512/apps/ RENAME vendace.png)
