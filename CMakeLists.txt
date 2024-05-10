cmake_minimum_required(VERSION 3.23)

project(vingt-et-un LANGUAGES CXX)

add_subdirectory(src)

option(ENABLE_TESTING "If true will generate tests" TRUE)
if(ENABLE_TESTING)
  include(cmake/FetchCatch2.cmake)
  include(CTest)
  add_subdirectory(test)
endif()
