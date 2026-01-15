cmake_minimum_required(VERSION 3.23)

project(vingt-et-un LANGUAGES CXX)

add_subdirectory(src)

option(ENABLE_TESTING "If true will generate tests" TRUE)
if(ENABLE_TESTING)
  include(CTest)
  include(cmake/FetchCatch2.cmake)
  add_subdirectory(test)
endif()

option(ENABLE_QML "Build QML frontend" OFF)
if(ENABLE_QML)
  find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml)
  qt_standard_project_setup()
  add_subdirectory(app/qml)
endif()
