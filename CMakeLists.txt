cmake_minimum_required(VERSION 3.23)

project(vingt-et-un
  VERSION 0.2.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate version header
configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/version.h.in"
  "${CMAKE_BINARY_DIR}/generated/version.h"
)

add_subdirectory(src)

option(ENABLE_TESTING "If true will generate tests" TRUE)
if(ENABLE_TESTING)
  include(CTest)
  include(cmake/FetchCatch2.cmake)
  add_subdirectory(test)
endif()

option(ENABLE_QML "Build QML frontend" OFF)
if(ENABLE_QML)
  find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml QuickControls2)
  qt_standard_project_setup()
  add_subdirectory(app/qml)
endif()
