cmake_minimum_required(VERSION 3.4)

PROJECT(qt-keyboard)
INCLUDE(GNUInstallDirs)

find_package(Qt5Core)
find_package(Qt5Widgets)
file(MAKE_DIRECTORY lib/)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(pinyin)
add_subdirectory(qt5softkeyboard)

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
install(FILES  ${PROJECT_SOURCE_DIR}/qt5softkeyboard/libqtsoftkeyboard.so
        DESTINATION ${CMAKE_INSTALL_SBINDIR}/usr/lib)
