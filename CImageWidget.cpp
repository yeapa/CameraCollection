//
// Created by ye on 16-9-13.
//

#include "CImageWidget.h"
#include <iostream>


void CImageWidget::paintEvent(QPaintEvent* event) {
    if(m_buffer==NULL){
        perror("m_buffer==NULL");
        std::cout<<"hahah"<<std::endl;
    }
    if (!m_pImage)
        m_pImage = new QImage(m_width,m_height, QImage::Format_RGB888);

    const unsigned char* ptr = (const unsigned char*)m_buffer->start;
    int x = 0;
    int y = 0;
    while (y<m_height) {
        int y1;
        int pb;
        int pr;
        int r;
        int g;
        int b;

        int Y1;
        int Cb;
        int Cr;

        Y1 = *ptr;
        Cb = *(ptr+1);
        Cr = *(ptr+3);
        y1 = (255 * (Y1 -  16)) / 219;
        pb = (255 * (Cb - 128)) / 224;
        pr = (255 * (Cr - 128)) / 224;
        r = y1 + (0    * pb + 1402 * pr)/1000;
        g = y1 + (-344 * pb - 714  * pr)/1000;
        b = y1 + (1772 * pb + 0    * pr)/1000;
        r = r>255?255:r;
        r = r<0?0:r;
        g = g>255?255:g;
        g = g<0?0:g;
        b = b>255?255:b;
        b = b<0?0:b;
        m_pImage->setPixel(x,y, r<<16 | g<<8 | b);

        Y1 = *ptr+2;
        Cb = *(ptr+1);
        Cr = *(ptr+3);
        y1 = (255 * (Y1 -  16)) / 219;
        pb = (255 * (Cb - 128)) / 224;
        pr = (255 * (Cr - 128)) / 224;
        r = y1 + (0    * pb + 1402 * pr)/1000;
        g = y1 + (-344 * pb - 714  * pr)/1000;
        b = y1 + (1772 * pb + 0    * pr)/1000;
        r = r>255?255:r;
        r = r<0?0:r;
        g = g>255?255:g;
        g = g<0?0:g;
        b = b>255?255:b;
        b = b<0?0:b;
        m_pImage->setPixel(x+1,y, r<<16 | g<<8 | b);

        x += 2;
        if (x > m_width-1) {
            x = 0;
            y++;
        }
        ptr += 4;
    }

    QPainter p(this);
    p.drawImage(QPoint(0,0),*m_pImage);
    p.end();
}