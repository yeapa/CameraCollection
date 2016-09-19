//
// Created by ye on 16-9-15.
//

/*v4l2操作流程
1.打开设备文件。 int fd=open(”/dev/video2″,O_RDWR);
2.取得设备的capability，看看设备具有什么功能，比如是否具有视频输入,或者音频输入输出等。VIDIOC_QUERYCAP,struct v4l2_capability
3.设置视频的制式和帧格式，制式包括PAL，NTSC，帧的格式个包括宽度和高度等。
VIDIOC_S_STD,VIDIOC_S_FMT,struct v4l2_std_id,struct v4l2_format
4.向驱动申请帧缓冲，一般不超过5个。struct v4l2_requestbuffers
5.将申请到的帧缓冲映射到用户空间，这样就可以直接操作采集到的帧了，而不必去复制。mmap
6.将申请到的帧缓冲全部入队列，以便存放采集到的数据.VIDIOC_QBUF,struct v4l2_buffer
7.开始视频的采集。VIDIOC_STREAMON
8.出队列以取得已采集数据的帧缓冲，取得原始采集数据。VIDIOC_DQBUF
9.将缓冲重新入队列尾,这样可以循环采集。VIDIOC_QBUF
10.停止视频的采集。VIDIOC_STREAMOFF
11.关闭视频设备。close(fd);
*/
#ifndef CAMERACOLLECTION_CCAMERA_H
#define CAMERACOLLECTION_CCAMERA_H
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/ioctl.h>
#include<iostream>
#include "CImageWidget.h"
#include "CBuffer.h"
using namespace std;


class CCamera {
public:
    CCamera(string deviceName,unsigned int width, unsigned int height) :m_deviceName(deviceName),m_width(width),m_height(height){
        m_widget =new CImageWidget(m_width,m_height);
        m_widget->show();
    }
    int init();
    int queryCapability();
    int queryFormatDesc();
    int setFormat(v4l2_format &format);
    int getCurrentFormat();
    bool tryFormat(v4l2_format &format,string formatDesc);
    int getCrop();
    int setCrop();
    int setMmap();
    void startCapture();
    int readFrame();
    int showPicture();
    CImageWidget *m_widget=NULL;


private:
    string m_deviceName;
    int m_fd=-1;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_bufferSize=4;



    Buffer *m_buffers;
    Buffer *m_imagePaintBuffer;

};




#endif //CAMERACOLLECTION_CCAMERA_H
