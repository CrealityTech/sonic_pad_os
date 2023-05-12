cmake_minimum_required(VERSION 3.4)

PROJECT(qt-softkeyboard)
INCLUDE(GNUInstallDirs)

find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(../pinyin/include)
include_directories($ENV{STAGING_DIR}/usr/include/QtCore/5.12.9/QtCore/private/)
include_directories($ENV{STAGING_DIR}/usr/include/QtGui/5.12.9/)
include_directories($ENV{STAGING_DIR}/usr/include/QtCore/5.12.9)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../pinyin)

file(GLOB_RECURSE HEAD_FILES ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

QT5_WRAP_UI(_UI_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/NormalKeyboard.ui)
QT5_WRAP_UI(_UI_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/NumKeyboard.ui)
QT5_WRAP_UI(_UI_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/SymbolKeyboard.ui)
QT5_WRAP_CPP(_MOC_OUTFILES, ${HEAD_FILES})
QT5_ADD_RESOURCES(_RCC_OUTFILES ${CMAKE_CURRENT_LIST_DIR}/res.qrc)
list(APPEND SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/CandidatesListWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/inputcontext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/KeyboardContainer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_CandidatesListWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_inputcontext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_KeyboardContainer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_NormalKeyboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_NumKeyboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_pinyindecoderservice_p.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_platforminputcontextplugin.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_QAbstractInputMethod.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_QKeyboardLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_QPinyinInputMethod.cpp
    ${CMAKE_CURRENT_LIST_DIR}/moc_SymbolKeyboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/NormalKeyboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/NumKeyboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pinyindecoderservice.cpp
    ${CMAKE_CURRENT_LIST_DIR}/platforminputcontextplugin.cpp
    ${CMAKE_CURRENT_LIST_DIR}/QAbstractInputMethod.cpp
    ${CMAKE_CURRENT_LIST_DIR}/QKeyboardLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/QPinyinInputMethod.cpp
    ${CMAKE_CURRENT_LIST_DIR}/qrc_res.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SymbolKeyboard.cpp
    ${_RCC_OUTFILES}
    ${_MOC_OUTFILES}
    ${_UI_OUTFILES}
    )

foreach(dt5_dep Core Gui Widgets)
  add_definitions(${Qt5${dt5_dep}_DEFINITIONS})
  include_directories(${Qt5${dt5_dep}_INCLUDE_DIRS})
  message(${Qt5${dt5_dep}_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${Qt5${dt5_dep}_LIBRARIES})
endforeach()

if(HAVE_QT_OPENGL)
  add_definitions(${Qt5OpenGL_DEFINITIONS})
  include_directories(${Qt5OpenGL_INCLUDE_DIRS})
  list(APPEND HIGHGUI_LIBRARIES ${Qt5OpenGL_LIBRARIES})
endif()

add_library(qtsoftkeyboard SHARED ${SOURCES})
target_link_libraries(qtsoftkeyboard ${LIBRARIES} libqt-pinyin.a pthread dl rt)
