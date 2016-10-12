#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <cstring>
#include <iostream>
#include "CBuffer.h"
using namespace std;

struct Message
{
public:
    long int m_msgType;
    char m_data[4147200];
    unsigned int m_length;
    Message(unsigned int length=61440, long int msgType=1)
    {
        m_msgType=msgType;
        m_length=length;
    }
};
#endif
