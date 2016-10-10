//
// Created by ye on 16-9-15.
//

#include <linux/videodev2.h>
#include "CCamera.h"
#include "CImageTrans.h"

int CCamera::init(){
    m_fd =  open(m_deviceName.c_str(), O_RDWR);
    if (-1==m_fd){
        perror("error in open device %s");
        exit(EXIT_FAILURE);
        return -1;

    }

    m_widget =new CImageWidget(m_width,m_height);
    m_widget->show();
    pImageTrans=new CImageTrans(m_width,m_height,m_path);
    m_msgQueue=new CMsgQueue(1124);
    m_deviceNum=m_deviceName.at(m_deviceName.size()-1)-'0'+1;
    cout<<m_videoID<<endl;
//    m_msgQueue->cmdChangeMsgmax(614404);
//    m_msgQueue->cmdChangeMsgmnb(6144040);


    v4l2_format fmt;
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = m_width;
    fmt.fmt.pix.height      = m_height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    setFormat(fmt);

    setMmap();
    startCapture();

    return 0;
}

int CCamera::queryCapability() {
    if (-1==m_fd){
        perror("camera has not been open");
        exit(EXIT_FAILURE);
    }
    struct v4l2_capability cap;
    if (-1 == ioctl(m_fd, VIDIOC_QUERYCAP, &cap)) {
        perror("query capbility fault");
        return -1;
    }
    printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n", cap.driver, cap.card, cap.bus_info,
           (cap.version >> 16) & 0XFF, (cap.version >> 8) & 0XFF, cap.version & 0xFF);

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf(stderr, "Device is not a video capture device\n");
    }


    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        fprintf(stdout, "Device does  not support streaming i/o\n");
    }
    return 0;
}

int CCamera::queryFormatDesc() {
    if (-1==m_fd){
        perror("camera has not been open");
        return -1;
    }
    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index=0;
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support format:\n");
    while(ioctl(m_fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
    {
        printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
        fmtdesc.index++;
    }
    return 0;
}

int CCamera::getCurrentFormat(){
    if (-1==m_fd){
        perror("camera has not been open");
        exit(EXIT_FAILURE);
    }
    struct v4l2_format fmt;
    fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1==ioctl(m_fd,VIDIOC_G_FMT,&fmt)){
        perror("get format faild");
        return -1;
    }
    printf("Current data format information:\n\twidth:%d\n\theight:%d\n",fmt.fmt.pix.width,fmt.fmt.pix.height);
    v4l2_fmtdesc fmtdesc;
    fmtdesc.index=0;
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while(ioctl(m_fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
    {
        if(fmtdesc.pixelformat & fmt.fmt.pix.pixelformat)
        {
            printf("\tformat:%s\n",fmtdesc.description);
            break;
        }
        fmtdesc.index++;
    }
    return 0;
}

int CCamera::setFormat(v4l2_format &format){
    if (-1==m_fd){
        perror("camera has not been open");
        exit(EXIT_FAILURE);
    }
    if (-1 == ioctl (m_fd, VIDIOC_S_FMT, &format)) {
        perror ("VIDIOC_S_FMT");
        exit(EXIT_FAILURE);
    }else{
        cout<<"successed set format"<<endl;
    }

    return 0;
}


bool CCamera::tryFormat(v4l2_format &format,string formatDesc){
    if (-1==m_fd){
        perror("camera has not been open");
        exit(EXIT_FAILURE);
    }
    if(ioctl(m_fd,VIDIOC_TRY_FMT,&format)==-1)
    {
        if(errno==EINVAL)
            printf("not support format %s!\n",formatDesc.c_str());
        return false;
    }else{
        printf("support format %s!\n",formatDesc.c_str());
        return true;
    }

}

int CCamera::setCrop() {
    v4l2_cropcap cropcap;
//    memset(&cropcap, 0, sizeof(cropcap));
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == ioctl (m_fd, VIDIOC_CROPCAP, &cropcap)) {
        v4l2_crop crop;
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if (-1 == ioctl (m_fd, VIDIOC_S_CROP, &crop)) {
            perror("set crop faild");
            switch (errno) {
                case EINVAL:
                    /* Cropping not supported. */

                    break;
                default:
                    /* Errors ignored. */
                    break;
            }
        } else{
            cout<<"type"<<crop.type<<"height"<<crop.c.height<<"width"<<crop.c.width;
        }
    } else {
        /* Errors ignored. */
        perror("VIDIOC_CROPCAP  faild");
        return -1;
    }
    return 0;
}

int CCamera::getCrop() {
    v4l2_crop crop;
    if (-1 == ioctl (m_fd, VIDIOC_G_CROP, &crop)) {
        perror("VIDIOC_G_CROP faild");
    }else{
        cout<<crop.c.width<<"  "<<crop.c.height <<"  "<<crop.c.left<<"  "<<crop.c.left;
    }
    return 0;
}

int CCamera::setMmap(){
    struct v4l2_requestbuffers req;

//    memset(&req, 0, sizeof(req));

    req.count  = m_bufferSize;// 缓存数量，也就是说在缓存队列里保持多少张照片
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;// 数据流类型，必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE
    req.memory = V4L2_MEMORY_MMAP;// V4L2_MEMORY_MMAP 或 V4L2_MEMORY_USERPTR

    if (-1 == ioctl (m_fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            fprintf (stderr, "Device does not support memory mapping\n");
            exit (EXIT_FAILURE);
        } else {
            perror ("VIDIOC_REQBUFS");
            exit(EXIT_FAILURE);
        }
    }

    if (req.count < 2) {
        fprintf (stderr, "Insufficient buffer memory on device\n");
        exit (EXIT_FAILURE);
    }

//    m_buffers = (Buffer*)calloc (req.count, sizeof (*m_buffers));
    m_buffers = new Buffer[req.count];
    if (!m_buffers) {
        fprintf (stderr, "Out of memory\n");
        exit (EXIT_FAILURE);
    }

    for (unsigned int i = 0; i < req.count; ++i) {
        v4l2_buffer buf;

//        memset(&buf, 0, sizeof(buf));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;

        if (-1 == ioctl (m_fd, VIDIOC_QUERYBUF, &buf)) {
            perror ("VIDIOC_QUERYBUF faild");
            exit(EXIT_FAILURE);
        }

        m_buffers[i].length = buf.length;
        m_buffers[i].start =
                mmap (NULL /* start anywhere */,
                      buf.length,
                      PROT_READ | PROT_WRITE /* required */,
                      MAP_SHARED /* recommended */,
                      m_fd, buf.m.offset);

        if (MAP_FAILED == m_buffers[i].start) {
            perror ("mmap faild");
            return -1;
        }
    }
    return 0;
}

void CCamera::startCapture(){
    unsigned int i;
    enum v4l2_buf_type type;

    for (i = 0; i < m_bufferSize; ++i) {
        struct v4l2_buffer buf;

//        memset(&buf, 0, sizeof(buf));
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;

        if (-1 == ioctl (m_fd, VIDIOC_QBUF, &buf)) {
            perror ("VIDIOC_QBUF");
            exit(EXIT_FAILURE);
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == ioctl (m_fd, VIDIOC_STREAMON, &type)) {
        perror ("VIDIOC_STREAMON");
        exit(EXIT_FAILURE);
    }
    cout<<"start capture pic"<<endl;
}

int CCamera::readFrame() {
    v4l2_buffer buf;

//    memset(&buf, 0, sizeof(buf));

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (-1 == ioctl (m_fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
            case EAGAIN:
                return 0;
            case EIO:
                /* Could ignore EIO, see spec. */
                /* fall through */
            default:
                perror ("VIDIOC_DQBUF");
                exit(EXIT_FAILURE);
        }
    }

    assert (buf.index < m_bufferSize);
    printf ("%d %d: \n", buf.index, buf.bytesused);
    printf("thread %ld read a image\n" ,m_threadID);

    m_imagePaintBuffer =&m_buffers[buf.index];
    showPicture();

    Message msg(m_buffers[buf.index].length,m_deviceNum);
    memcpy(msg.m_data,m_buffers[buf.index].start,m_buffers[buf.index].length);
    m_msgQueue->sendMsg(&msg);

    pImageTrans->transform((unsigned char*)m_buffers[buf.index].start);
    pImageTrans->exportAImage();




    if (-1 == ioctl (m_fd, VIDIOC_QBUF, &buf)) {
        perror ("VIDIOC_QBUF");
        exit(EXIT_FAILURE);
    }
    return 1;
}

int CCamera::showPicture() {
    m_widget->setBuf(m_imagePaintBuffer);

    if(m_widget->getBuf()==NULL) return -1;
    m_widget->update();
    QApplication::processEvents();
    return 0;
}

int CCamera::getParm(){
    struct v4l2_streamparm stream_Parm;
    stream_Parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(m_fd, VIDIOC_G_PARM, &stream_Parm)==-1){

        return -1;
    }else{
        int denominator=stream_Parm.parm.capture.timeperframe.denominator;
        int numerator=stream_Parm.parm.capture.timeperframe.numerator;
        cout << "帧数：  "<<denominator/numerator<<"fps"<<endl;
        return 0;
    }
}

int CCamera::setParm(unsigned int Denominator,unsigned int Numerator){
    if(Denominator<Numerator){
        perror("Denominator must bigger than Numerator");
        exit(EXIT_FAILURE);
    }
    struct v4l2_streamparm stream_Parm;
//    memset(&stream_Parm, 0, sizeof(struct v4l2_streamparm));
    stream_Parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    stream_Parm.parm.capture.timeperframe.denominator =Denominator;;
    stream_Parm.parm.capture.timeperframe.numerator = Numerator;
    if(ioctl(m_fd, VIDIOC_S_PARM, &stream_Parm)==-1){
        return -1;
    } else{
        return 0;
    }
}

int CCamera::getCtrl(){
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_EXPOSURE_AUTO;

    if(ioctl(m_fd, VIDIOC_G_CTRL, &ctrl)==-1){
        perror("get V4L2_CID_EXPOSURE_AUTO faild");
        return -1;
    }else{
        cout<<"Exposure Type：  "<<ctrl.value<<endl;
    }

    ctrl.id = V4L2_CID_EXPOSURE_ABSOLUTE;
    if(ioctl(m_fd, VIDIOC_G_CTRL, &ctrl)==-1){
        perror("get V4L2_CID_EXPOSURE_AUTO faild");
        return -1;
    }else{
        cout<<"EXPOSURE：  "<<ctrl.value<<endl;
    }


    ctrl.id = V4L2_CID_AUTO_WHITE_BALANCE;
    if(ioctl(m_fd, VIDIOC_G_CTRL, &ctrl)==-1){
        perror("get V4L2_CID_AUTO_WHITE_BALANCE faild");
        return -1;
    }else{
        cout<<"WHITE_BALANCE：  "<<ctrl.value<<endl;
    }

    ctrl.id = V4L2_CID_GAIN;
    if(ioctl(m_fd, VIDIOC_G_CTRL, &ctrl)==-1){
        perror("get V4L2_CID_GAIN faild");
        return -1;
    }else{
        cout<<"GAIN：  "<<ctrl.value<<endl;
    }

    return 0;
}

int CCamera::setCtrl(){

    struct v4l2_queryctrl  Setting;
    Setting.id = V4L2_CID_GAIN;
    if(ioctl(m_fd, VIDIOC_QUERYCTRL, &Setting)==-1){
        perror("set  VIDIOC_QUERYCTRL:V4L2_CID_GAIN failed");
        return -1;
    }else{

    }

    unsigned int Gain =0;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_GAIN;
    ctrl.value = Gain;
    if(ioctl(m_fd, VIDIOC_S_CTRL, &ctrl)==-1){
        perror("set V4L2_CID_GAIN failed");
        return -1;
    }else{

    }

    return 0;
}

void CCamera::stopcapturing()
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl (m_fd, VIDIOC_STREAMOFF, &type))
        perror("VIDIOC_STREAMOFF faild");
}