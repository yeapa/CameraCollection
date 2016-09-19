cmake_minimum_required(VERSION 3.6)
project(CameraCollection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

INCLUDE_DIRECTORIES(/usr/share/qt4/mkspecs/linux-g++-64 . /usr/include/qt4/QtCore /usr/include/qt4/QtGui /usr/include/qt4)
LINK_DIRECTORIES(-L/usr/lib/x86_64-linux-gnu)
LINK_LIBRARIES(-lQtGui -lQtCore -lpthread)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/Desktop/myworkspace/CameraCollection")
set(SOURCE_FILES main.cpp CCamera.cpp CCamera.h CImageWidget.cpp CImageWidget.h CBuffer.h)
add_executable(CameraCollection ${SOURCE_FILES})