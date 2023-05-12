cmake_minimum_required(VERSION 3.4)

PROJECT(qt-pinyin)
INCLUDE(GNUInstallDirs)

find_package(Qt5Core)
find_package(Qt5Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/share/*.cpp)
file(GLOB_RECURSE SRC_FILES_1 ${CMAKE_CURRENT_LIST_DIR}/command/*.cpp)
file(GLOB_RECURSE HEAD_FILES ${CMAKE_CURRENT_LIST_DIR}/include/*.h)

foreach(dt5_dep Core Gui Widgets)
  add_definitions(${Qt5${dt5_dep}_DEFINITIONS})
  include_directories(${Qt5${dt5_dep}_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${Qt5${dt5_dep}_LIBRARIES})
endforeach()

if(HAVE_QT_OPENGL)
  add_definitions(${Qt5OpenGL_DEFINITIONS})
  include_directories(${Qt5OpenGL_INCLUDE_DIRS})
  list(APPEND HIGHGUI_LIBRARIES ${Qt5OpenGL_LIBRARIES})
endif()

ADD_LIBRARY(qt-pinyin STATIC ${SRC_FILES} ${SRC_FILE_1} ${HEAD_FILES})
install(FILES  ${PROJECT_SOURCE_DIR}/libqt-pinyin.a
        DESTINATION ${PROJECT_SOURCE_DIR}/../lib/)
