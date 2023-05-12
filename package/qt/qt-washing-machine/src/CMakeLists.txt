cmake_minimum_required(VERSION 3.4)

PROJECT(qt-washing-machine)
INCLUDE(GNUInstallDirs)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Qml)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_ADD_RESOURCES(_RCC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/qml.qrc)
QT5_ADD_RESOURCES(_RCC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/images/images.qrc)

list(APPEND SOURCES
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
	${_MOC_OUTFILES}
	${_UI_OUTFILES}
	${_RCC_OUTFILES})

foreach(dt5_dep Core Gui Widgets Qml)
  add_definitions(${Qt5${dt5_dep}_DEFINITIONS})
  include_directories(${Qt5${dt5_dep}_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${Qt5${dt5_dep}_LIBRARIES})
endforeach()

if(HAVE_QT_OPENGL)
  add_definitions(${Qt5OpenGL_DEFINITIONS})
  include_directories(${Qt5OpenGL_INCLUDE_DIRS})
  list(APPEND HIGHGUI_LIBRARIES ${Qt5OpenGL_LIBRARIES})
endif()

ADD_EXECUTABLE(qt-washing-machine ${SOURCES})
TARGET_LINK_LIBRARIES(qt-washing-machine ${LIBRARIES} pthread dl)
INSTALL(TARGETS qt-washing-machine
	RUNTIME DESTINATION ${CMAKE_INSTALL_SBINDIR})
