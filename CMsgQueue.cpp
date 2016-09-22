#include "CMsgQueue.h"
#include <errno.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

CMsgQueue::CMsgQueue(key_t keyMsgQueue)
{
    m_keyMsgQueue=keyMsgQueue;
    m_iMsgQueueID=-1;
    Create();
}

int CMsgQueue::Create()
{
    m_iMsgQueueID = msgget(m_keyMsgQueue, 0666 | IPC_CREAT);
    if (m_iMsgQueueID==-1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        return -1;
    }
    return 0;
}

int CMsgQueue::sendMsg(Message *p)
{

    //msgsnd最后一个变量为msgflag,msgflag为发送标识,取值如下
    //MSG_NOERROR:若返回的消息比nbytes字节多,则消息就会截短到nbytes字节,且不通知消息发送进程.
    //IPC_NOWAIT:调用进程会立即返回.若没有收到消息则返回-1.
    //0:msgrcv调用阻塞直到条件满足为止.
    if(msgsnd(m_iMsgQueueID, (void *)(p), sizeof(p->m_msgBuffer), 0) == -1)
    {
        perror("send message faild");
//        cout<<"start address: "<<p->m_msgBuffer->start<<endl;
        fprintf(stderr, "msgsnd failed\n");
        return -1;
    }
    return 0;
}

int CMsgQueue::receiveMsg(Message *p,long int msgtype)
{
    if(msgrcv(m_iMsgQueueID, p, 10, msgtype, 0) == -1)
    {
        fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
        return -1;
    }
    return 0;
}

int CMsgQueue::Delete()
{
    if(msgctl(m_iMsgQueueID, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        return -1;
    }
    return 0;
}

int CMsgQueue::cmdChangeMsgmax(int size)
{
    int status ;
    char * cmd=new char[50];
    sprintf(cmd, "sudo sysctl -w kernel.msgmax=%d > /dev/null",size);
    status=system(cmd);

    if(status == -1){
        // printf("system error");
        return -1;
    }
    else
    {
        if(WIFEXITED(status)){
            if(WEXITSTATUS(status) == 0){
                // printf("run command successful\n");
                return 0;
            }
            else{
                // printf("run command fail and exit code is %d\n",WEXITSTATUS(status));
                return -1;
            }
        }
        else{
            // printf("exit status = %d\n",WEXITSTATUS(status));
            return -1;
        }
    }
}

int CMsgQueue::cmdChangeMsgmnb(int size)
{
    int status ;
    char * cmd=new char[50];
    sprintf(cmd, "sudo sysctl -w kernel.msgmnb=%d > /dev/null",size);
    status=system(cmd);

    if(status == -1){
        // printf("system error");
        return -1;
    }
    else
    {
        if(WIFEXITED(status)){
            if(WEXITSTATUS(status) == 0){
                // printf("run command successful\n");
                return 0;
            }
            else{
                // printf("run command fail and exit code is %d\n",WEXITSTATUS(status));
                return -1;
            }
        }
        else{
            // printf("exit status = %d\n",WEXITSTATUS(status));
            return -1;
        }
    }
}
