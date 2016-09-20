//
// Created by ye on 16-9-19.
//

#ifndef CAMERACOLLECTION_CIMAGETRANS_H
#define CAMERACOLLECTION_CIMAGETRANS_H

extern "C"{
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/pixfmt.h>
#include <libavutil/avconfig.h>
}
#include <stdimage.hxx>
#include <rgbvalue.hxx>
#include <sized_int.hxx>
#include <imageinfo.hxx>
#include <impex.hxx>

#include <iostream>
using namespace std;

class CImageTrans {
public:
    CImageTrans(unsigned int width, unsigned int height):m_width(width),m_height(height){
        m_pImgCtx = sws_getContext(m_width,m_height,AV_PIX_FMT_YUYV422,m_width,m_height,AV_PIX_FMT_RGB24,SWS_BILINEAR,0,0,0);
        m_pRGB24=new unsigned char[avpicture_get_size(AV_PIX_FMT_RGB24, width, height)];
        m_pImage = new vigra::BRGBImage(width,height);
    }

     ~CImageTrans(){
         sws_freeContext(m_pImgCtx);
         m_pImgCtx = NULL;
    }

    bool transform(const unsigned char * pYUV);

    void exportAImage(int i);

private:
    unsigned int m_width;
    unsigned int m_height;
    struct SwsContext* m_pImgCtx = NULL;
    vigra::BRGBImage *m_pImage;
    const unsigned char  *m_pRGB24;
};


#endif //CAMERACOLLECTION_CIMAGETRANS_H