cmake_minimum_required(VERSION 3.4)

PROJECT(qt-textures)
INCLUDE(GNUInstallDirs)

find_package(Qt5Core)
find_package(Qt5Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_ADD_RESOURCES(_RCC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/textures.qrc)
QT5_WRAP_CPP(_MOC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/window.h)
QT5_WRAP_CPP(_MOC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/glwidget.h)

list(APPEND SOURCES
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/window.cpp
	${CMAKE_CURRENT_LIST_DIR}/glwidget.cpp
	${_MOC_OUTFILES}
	${_UI_OUTFILES}
	${_RCC_OUTFILES})

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

ADD_EXECUTABLE(qt-textures ${SOURCES})
TARGET_LINK_LIBRARIES(qt-textures ${LIBRARIES} pthread dl rt)
INSTALL(TARGETS qt-textures
	RUNTIME DESTINATION ${CMAKE_INSTALL_SBINDIR})
