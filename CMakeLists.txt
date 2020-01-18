cmake_minimum_required(VERSION 3.0)
project(traffic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS Off)

project(TrafficModel)

find_package(
  Qt5
  HINTS "$ENV{QTDIR}"
  REQUIRED COMPONENTS
  Core
  Widgets
  Test
)

set(
  ${PROJECT_NAME}_INCLUDE
  include/MainWindow.h
  include/TrafficLight.h
  include/TrafficLightSystem.h
  include/TrafficPath.h
  include/TrafficPathPoint.h
  include/Bus.h
  include/Car.h
  include/PedSystem.h
  include/CarFactory.h
  include/Interface.h
)
set(
  ${PROJECT_NAME}_SOURCE
  sources/TrafficLight.cpp
  sources/TrafficLightSystem.cpp
  sources/TrafficPath.cpp
  sources/TrafficPathPoint.cpp
  sources/Bus.cpp
  sources/Car.cpp
  sources/PedSystem.cpp
  sources/CarFactory.cpp
)
set(
  ${PROJECT_NAME}_UI
  sources/MainWindow.ui
)
set(
  ${PROJECT_NAME}_RESOURCE
  resources/resource.qrc
)

set(GUI_TYPE)
if (WIN32)
  set(GUI_TYPE WIN32)
endif ()

# Core

add_library(
  ${PROJECT_NAME}Core
  ${${PROJECT_NAME}_SOURCE}
)

target_link_libraries(${PROJECT_NAME}Core PUBLIC
  Qt5::Core
  Qt5::Widgets
  )

# Demo

add_executable(
  ${PROJECT_NAME}
  ${GUI_TYPE}

  sources/main.cpp
  sources/MainWindow.cpp

  ${${PROJECT_NAME}_INCLUDE}
  ${${PROJECT_NAME}_UI}
  ${${PROJECT_NAME}_RESOURCE}
)

target_include_directories(
  ${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(include)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOUIC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC TRUE)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${PROJECT_NAME}Core
  )

# Tests
if (BUILD_TESTS)

  add_executable(tests
    tests/tests.cpp sources/MainWindow.cpp

    ${${PROJECT_NAME}_INCLUDE}
    ${${PROJECT_NAME}_UI}
    ${${PROJECT_NAME}_RESOURCE}
    )
  add_test(tests tests)

  set_target_properties(tests PROPERTIES AUTOMOC TRUE)
  set_target_properties(tests PROPERTIES AUTOUIC TRUE)
  set_target_properties(tests PROPERTIES AUTORCC TRUE)

  target_link_libraries(tests PUBLIC Qt5::Test ${PROJECT_NAME}Core)

endif ()