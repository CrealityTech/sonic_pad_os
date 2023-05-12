cmake_minimum_required(VERSION 3.4)

PROJECT(qt-minimal)
INCLUDE(GNUInstallDirs)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5 COMPONENTS WebEngineWidgets REQUIRED)
find_package(Qt5 COMPONENTS WebEngine REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -Wall -ggdb")
add_definitions("-DQT_NO_DEBUG_OUTPUT")

QT5_WRAP_CPP(_MOC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/webview.h)

list(APPEND SOURCES
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/watchdog.cpp
	${CMAKE_CURRENT_LIST_DIR}/webview.cpp
	${_MOC_OUTFILES}
	${_UI_OUTFILES}
	${_RCC_OUTFILES})

foreach(dt5_dep Core Gui Widgets WebEngineWidgets) 
  add_definitions(${Qt5${dt5_dep}_DEFINITIONS})
  include_directories(${Qt5${dt5_dep}_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${Qt5${dt5_dep}_LIBRARIES})
endforeach()

if(HAVE_QT_OPENGL)
  add_definitions(${Qt5OpenGL_DEFINITIONS})
  include_directories(${Qt5OpenGL_INCLUDE_DIRS})
  list(APPEND HIGHGUI_LIBRARIES ${Qt5OpenGL_LIBRARIES})
endif()

ADD_EXECUTABLE(qt-minimal ${SOURCES})
TARGET_LINK_LIBRARIES(qt-minimal ${LIBRARIES} pthread dl rt resolv Qt5::WebEngine)
INSTALL(TARGETS qt-minimal
	RUNTIME DESTINATION ${CMAKE_INSTALL_SBINDIR})
