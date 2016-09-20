cmake_minimum_required(VERSION 3.6)
project(CameraCollection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#qt
include_directories(/usr/share/qt4/mkspecs/linux-g++-64 . /usr/include/qt4/QtCore /usr/include/qt4/QtGui /usr/include/qt4)
link_directories(-L/usr/lib/x86_64-linux-gnu)
link_libraries(-lQtGui -lQtCore -lpthread)

#ffmpeg
include_directories(/usr/local/ffmpeg/include /usr/include/vigra)
link_directories(/usr/local/ffmpeg/lib  /usr/lib)
link_libraries(avformat avcodec avutil avdevice swresample swscale vigraimpex)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/Desktop/myworkspace/CameraCollection")
set(SOURCE_FILES main.cpp CCamera.cpp CCamera.h CImageWidget.cpp CImageWidget.h CBuffer.h CImageTrans.cpp CImageTrans.h)
add_executable(CameraCollection ${SOURCE_FILES})