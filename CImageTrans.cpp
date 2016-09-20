//
// Created by ye on 16-9-19.
//

#include "CImageTrans.h"
void CImageTrans::exportAImage(int i){
    char * name=new char[100];
    sprintf(name,"./image/hello%d.jpg",i);
    string picName(name);
    exportImage(srcImageRange(*m_pImage), vigra::ImageExportInfo(picName.c_str()).setCompression("80"));
}

bool CImageTrans::transform(const unsigned char * pYUV)
{
    if (m_width < 1 || m_height < 1 || pYUV == NULL || m_pRGB24 == NULL)
        return false;
    AVPicture pFrameYUV,pFrameRGB;
    avpicture_fill(&pFrameYUV,pYUV,AV_PIX_FMT_YUYV422,m_width,m_height);
    avpicture_fill(&pFrameRGB,m_pRGB24,AV_PIX_FMT_RGB24,m_width,m_height);
    if (m_pImgCtx != NULL){
        sws_scale(m_pImgCtx,(const uint8_t* const*)pFrameYUV.data,pFrameYUV.linesize,0,m_height,pFrameRGB.data,pFrameRGB.linesize);

//            FILE *output=fopen("out.rgb","wb");
//            fwrite(pFrameRGB.data[0],(m_width)*(height)*3,1,output);

        int x=0;
        int y=0;
        for(int i = 0; i<m_width*m_height*3;i+=3){
            vigra::RGBValue<vigra::UInt8>  rgbPoint;
            rgbPoint.setRed(pFrameRGB.data[0][i]);
            rgbPoint.setGreen(pFrameRGB.data[0][i+1]);
            rgbPoint.setBlue(pFrameRGB.data[0][i+2]);
            (*m_pImage)[x][y]=rgbPoint;
            y++;
            if (y>=m_width){
                y=0;
                x++;
            }
        }
//          cout<<endl<<"linesize:"<<pFrameRGB.linesize[0]<<endl;
        return true;
    }else{
        return false;
    }
}
