//
// Created by ye on 16-9-13.
//

#ifndef V4L2CPLUS_IMAGEWIDGET_H
#define V4L2CPLUS_IMAGEWIDGET_H


#include <qt4/QtGui/QApplication>
#include <qt4/QtGui/QWidget>
#include <qt4/QtGui/QImage>
#include <qt4/QtGui/QPainter>
#include <qt4/QtGui/QPixmap>
#include <qt4/QtGui/QPaintEvent>
#include <qt4/QtGui/QColor>
#include "CBuffer.h"
class CImageWidget : public QWidget {
public:
    QImage* m_pImage;
    int m_width;
    int m_height;
    struct Buffer * m_buffer;
    CImageWidget(int width,int height) {
        m_width=width;
        m_height=height;
        m_pImage = NULL;
        setAutoFillBackground(false);
        m_buffer=NULL;
        resize(m_width,m_height);
    }
    void setBuf(Buffer *buf){
        m_buffer=buf;
    }
    Buffer * getBuf(){
        return m_buffer;
    }

    void paintEvent(QPaintEvent* event);

};
#endif //V4L2CPLUS_IMAGEWIDGET_H
