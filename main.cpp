#include <iostream>
#include "CCamera.h"
#include "CImageWidget.h"
using namespace std;
int main(int argc, char** argv) {
    QApplication qapp(argc,argv);
//    ImageWidget * widget = NULL;
    CCamera camera("/dev/video0",640,480);
    camera.init();
//    camera.queryCapability();
//    cout<<"format description"<<endl;
//    camera.queryFormatDesc();
//
//    cout<<"get current format:"<<endl;
//    camera.getCurrentFormat();

//    v4l2_format fmt;
//    fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YUYV;
//    string formatDesc="V4L2_PIX_FMT_RGB332";
//    camera.tryFormat(fmt,formatDesc);

    while(true){
        camera.readFrame();
        if(camera.m_widget->isHidden()) break;
    }
//    cout<<"get current format:"<<endl;
//    camera.getCurrentFormat();
}