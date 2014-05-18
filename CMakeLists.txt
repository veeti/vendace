project(vendace)
cmake_minimum_required(VERSION 2.8)

# Find files
file(GLOB_RECURSE VENDACE_SOURCES src/*.c src/*.cpp)
file(GLOB_RECURSE VENDACE_HEADERS src/*.h src/*.hpp)
file(GLOB_RECURSE VENDACE_FORMS forms/*.ui)

# Find Qt
find_package(Qt5Widgets REQUIRED)

# Qt build steps
qt5_wrap_cpp(VENDACE_HEADERS_MOC ${VENDACE_HEADERS})
qt5_wrap_ui(VENDACE_FORMS_HEADERS ${VENDACE_FORMS})

# Include generated files
include_directories(${CMAKE_CURRENT_SOURCEDIR} ${CMAKE_CURRENT_BINARY_DIR})

# Add executable
add_executable(vendace ${VENDACE_SOURCES} ${VENDACE_HEADERS} ${VENDACE_HEADERS_MOC} ${VENDACE_FORMS_HEADERS})
qt5_use_modules(vendace Widgets Network)
