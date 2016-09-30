#ifndef CMSGQUEUE_H
#define CMSGQUEUE_H
#include <sys/msg.h>
#include <string>
#include "Message.h"
using namespace std;
class CMsgQueue
{
public:

    CMsgQueue(key_t keyMsgQueue);
    ~CMsgQueue(){Delete();}
    int Create();
    int sendMsg(Message *p);
    int receiveMsg(Message *p,long int msgtype=0);
    int Delete();
    static int cmdChangeMsgmax(int size);
    static int cmdChangeMsgmnb(int size);

protected:

    key_t m_keyMsgQueue;
    int m_iMsgQueueID;
    string m_strError;
    char *m_pMsgQueue;
};
#endif
