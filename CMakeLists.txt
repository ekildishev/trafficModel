cmake_minimum_required(VERSION 3.0)
project(traffic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TrafficModel)

find_package(
  Qt5
  HINTS "$ENV{QTDIR}"
  REQUIRED COMPONENTS
  Core
  Widgets
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
)
set(
  ${PROJECT_NAME}_SOURCE
  sources/MainWindow.cpp
  sources/TrafficLight.cpp
  sources/TrafficLightSystem.cpp
  sources/TrafficPath.cpp
  sources/TrafficPathPoint.cpp
  sources/Bus.cpp
  sources/Car.cpp
  sources/PedSystem.cpp
  sources/main.cpp
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

add_executable(
  ${PROJECT_NAME}
  ${GUI_TYPE}
  ${${PROJECT_NAME}_INCLUDE}
  ${${PROJECT_NAME}_SOURCE}
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

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Widgets)