#include <iostream>
#include "CCamera.h"
#include "CImageWidget.h"
#include <stdio.h>
using namespace std;
int main(int argc, char** argv) {
    QApplication qapp(argc,argv);
//    ImageWidget * widget = NULL;

//阻塞方式
    CCamera camera("/dev/video0",1920,1080,"./image/");
    camera.createAThread();

//    CCamera camera1("/dev/video1",1920,1080,"./image1/");
//    camera1.createAThread();

    camera.joinThread();
//    camera1.joinThread();
//非阻塞方式
//    CCamera camera("/dev/video0",1920,1080,"./image/");
//    camera.init();
//    CCamera camera1("/dev/video1",1920,1080,"./image1/");
//    camera1.init();
//    fd_set readFds;
//
//    int maxfd = camera.getFd()>camera1.getFd()? camera.getFd():camera1.getFd();
//    struct timeval timeout;
//    timeout.tv_sec=1;
//    timeout.tv_usec=0;
//
//    while (true){
//        FD_ZERO(&readFds);
//        FD_SET(camera.getFd(),&readFds);
//        FD_SET(camera1.getFd(),&readFds);
//
//        int ret = select(maxfd+1,&readFds,NULL,NULL,&timeout);
//        if (ret<0){
//            perror("selsct error");
//        }
//        else if(ret==0){
//            cout<<"time out"<<endl;
//        }
//        else{
//            if(FD_ISSET(camera.getFd(),&readFds)){
//                camera.readFrame();
//            }
//            if(FD_ISSET(camera1.getFd(),&readFds)){
//                camera1.readFrame();
//            }
//        }
//    }


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

//    camera.getParm();
//    cout<<camera.setParm(20,2)<<endl;
//    camera.getParm();

//    while(true){
//        camera.readFrame();
//        if(camera.m_widget->isHidden()) break;
//    }
//    cout<<"get current format:"<<endl;
//    camera.getCurrentFormat();
}