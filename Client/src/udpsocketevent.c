/** 
 * @file udpsocketevent
 * @brief 可靠udp
 * @author ljc jc.liuworld@hotmail.com
 * Copyright (c) 2019 liuworld. All rights reserved.
 * @date 2019-01-10
 */
#define udpsocketeventl_c
#define LUA_LIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>

#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#endif
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#if defined(LUA_BUILD_AS_DLL)   /* { */
#if defined(LUA_CORE) || defined(LUA_LIB)   /* { */
#define LUA_API __declspec(dllexport)
#else                       /* }{ */
#define LUA_API __declspec(dllimport)
#endif                      /* } */
#else               /* }{ */
#define LUA_API     extern
#endif              /* } */
#define LUALIB_API  LUA_API
#define LUAMOD_API  LUALIB_API


#define LUA_SOCKETEVENT_UDP_HANDLE          "SOCKETEVENT_UDP*"
#define LUA_SOCKETEVENT_UDP_BUFFER_SIZE         0x4000
#define LUA_SOCKETEVENT_UDP_MESSAGE_HEAD_SIZE       4
#define LUA_SOCKETEVENT_UDP_MESSAGE_MAX_SIZE        0x100000
#define LUA_SOCKETEVENT_UDP_STATE_CONNECT       0x1
#define LUA_SOCKETEVENT_UDP_STATE_THREAD        0x2
#define LUA_SOCKETEVENT_UDP_STATE_CLOSE         0x4


#define MIN_RTO 100
#define UDP_RD_SIZE 17024
#define UDP_WR_SIZE 68096

#ifdef _WIN32
#define pthread_t int
#endif

#ifdef _WIN32
#pragma warning(disable: 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _WIN32
 typedef SOCKET sock_t;
#else
 typedef int sock_t;
#endif

#define UDP_BUFSIZE 548

#define CONN_KEY "cA8ZKLoo"

enum FrameFlag
{
    F_NONE,//空包
    F_NORMAL,//普通数据包
    F_ACK,//确认包
    F_FEC,//校验
    F_CONN,//连接
};
#pragma pack(1)
typedef struct  stUPDHeader
{
    unsigned short flag;// 0 普通数据包 标记位 1 重传请求包  2 FEC校验包    data = dataID xor (dataID + 1) 3 ack 确认包
    unsigned int orderID; //udp 顺序编号 用于计算RTT
    unsigned int connID;//连接ID
    unsigned int dataID;//数据编号
    unsigned long  long timestamp;//时戳
} UDPHeader;

typedef struct stUDPFrame
{
    UDPHeader head;
    unsigned short dataLen;
    unsigned char data[0];
}UDPFRAME;

#pragma pack()


/**
 * @brief checklen
 */
static const CHECK_LEN = sizeof(unsigned int);

/**
* @brief head size
*/
static const int UDP_HEADSIZE =  sizeof(UDPHeader) + sizeof( unsigned short);

/**
* @brief data size
*/
const static  int UDP_DATASIZE = UDP_BUFSIZE - sizeof(UDPHeader) - sizeof( unsigned short);

/**
* @brief data head len
*/

const static int PH_LEN = sizeof(unsigned int);


typedef struct lua_SocketEventUDP {
    // lua State
    lua_State *L;

    // state 0 default  1 connect 2 clsose 
    int state;

    int isTerminate;

    // pthread
    pthread_t thread;

    // socket
    sock_t socket;
    const char *host;
    const char *ip;

    lua_Integer port;

    char * send_data_buffer;
    int send_buffer_size;
    int send_buffer_use;

    int data_buffer_size;
    int data_buffer_use;
    char *data_buffer;

    unsigned int  win_size;
    // message buffer
    int message_buffer_size;
    int message_buffer_use;
    char *message_buffer;
    int message_len;

    unsigned int  connectID;
    unsigned int rdBufCount;
    unsigned int wrBufCount;
    unsigned int checkCount;
    unsigned int fecBufCount;

    unsigned int srtt;
    unsigned int mdev;
    unsigned int mdev_max;
    unsigned int rttvar;
    unsigned int rtovar;


    unsigned int  udp_send_check_index;
    unsigned int *udp_send_check_stat; //接收到的ack数据包的状态

    unsigned int udp_recv_index; //udp 数据 接收标记
    unsigned int udp_send_index; //udp 数据 发送标记

    unsigned int udp_sendok_order;//最近一个发送确认的包
    unsigned int udp_sended_order;//最近一个已经发送的包的index

    unsigned int udp_recv_max; //udp 接收顺序标记
    unsigned int udp_send_order; //udp 发送顺寻标记

    unsigned int last_fec_order;

    unsigned int * udp_recv_check_stat;// 接受到的数据包的check状态

    unsigned char  * udp_send_buff[UDP_BUFSIZE];//重发缓存
    unsigned char  * udp_recved_buff[UDP_BUFSIZE];//接收缓存
    unsigned char * udp_fec_buff[UDP_BUFSIZE];//FEC缓存

    unsigned char _udp_recv_tmp_queue[UDP_BUFSIZE];

    // event
    int event_connect;
    int event_close;
    int event_error;
    int event_message;

    #ifdef _WIN32
    HANDLE mutex;
    #else
    pthread_mutex_t mutex;
    #endif

} LSocketEventUDP;

unsigned long long now();

int udp_unpack(LSocketEventUDP *sock);
int refresh_recv_buf(LSocketEventUDP *sock);
int send_frame(sock_t sock ,UDPFRAME  * frame , struct sockaddr * addr, int socklen);
int udp_pack(unsigned int connectID,unsigned int dataID,const void * in,int iSize,UDPFRAME * frame);

bool fec_recover(LSocketEventUDP *sock);
bool check_recv_state(LSocketEventUDP *sock, unsigned int index);
bool check_send_state(LSocketEventUDP *sock, unsigned int index);

void udp_check_resend(LSocketEventUDP *sock);
void shift_check_state(LSocketEventUDP *sock);
void calc_rttrvo(LSocketEventUDP *sock,unsigned int mrtt);
void set_check_state(LSocketEventUDP *sock,unsigned int index);
void send_ack(LSocketEventUDP *sock, unsigned int orderID,unsigned long long dwTime);
void cal_fec(unsigned int connectID,unsigned int  udp_send_order,UDPFRAME * frameFec,UDPFRAME * frame1,UDPFRAME * frame2);

unsigned int send_raw_data(LSocketEventUDP *sock);
unsigned int cal_udp_index(LSocketEventUDP *sock, unsigned int index);


static int socketevent_udp(lua_State *L)
{
    // create tcp sock handle
    LSocketEventUDP *sock = (LSocketEventUDP *)lua_newuserdata(L, sizeof(LSocketEventUDP));
#if LUA_VERSION_NUM == 501
    luaL_getmetatable(L, LUA_SOCKETEVENT_UDP_HANDLE);
    lua_setmetatable(L, -2);
#else
    luaL_setmetatable(L, LUA_SOCKETEVENT_UDP_HANDLE);
#endif

    // lua State
    sock->L = L;

    // state
    sock->state = 0;
    sock->isTerminate = 0;

#ifdef _WIN32
     sock->mutex = CreateMutex(NULL,FALSE,NULL);
#else
    pthread_mutexattr_t attr;
    ::pthread_mutexattr_init(&attr);
    ::pthread_mutexattr_settype(&attr,kind);
    ::pthread_mutex_init(&sock->mutex,&attr);
#endif

    sock->send_buffer_size = LUA_SOCKETEVENT_UDP_BUFFER_SIZE;
    sock->send_buffer_use = 0;
    sock->send_data_buffer = (char *) malloc(sock->send_buffer_size  + 1);
    memset(sock->send_data_buffer, 0, sock->send_buffer_size  + 1);


    sock->data_buffer_size = LUA_SOCKETEVENT_UDP_BUFFER_SIZE;
    sock->data_buffer_use = 0;
    sock->data_buffer = (char *)malloc(sock->data_buffer_size + 1);
    memset(sock->data_buffer, 0, sock->data_buffer_size + 1);


    // message buffer
    sock->message_buffer_size = LUA_SOCKETEVENT_UDP_BUFFER_SIZE;
    sock->message_buffer_use = 0;
    sock->message_buffer = (char *)malloc(sock->message_buffer_size + 1);
    memset(sock->message_buffer, 0, sock->message_buffer_size + 1);
    sock->message_len = 0;

    // socket
    sock->socket = -1;
    sock->host = NULL;
    sock->ip = NULL;
    sock->port = -1;

    sock->srtt = 0;
    sock->mdev = 0;
    sock->mdev_max = 0;
    sock->rttvar = 0;
    sock->rtovar = 0;

    
    sock->udp_recv_index = 0 ;
    sock->udp_send_index = 0;

    sock->udp_sendok_order = 0;
    sock->udp_sended_order = 0;

    sock->udp_recv_max = 0;
    sock->udp_send_check_index = 0;
    sock->udp_send_order = 0;

    
    sock->wrBufCount = UDP_WR_SIZE / UDP_DATASIZE;
    sock->rdBufCount = UDP_RD_SIZE / UDP_DATASIZE;
    sock->fecBufCount = sock->wrBufCount / 2 + 1;
    sock->checkCount = sock->rdBufCount / CHECK_LEN + 1;
    sock->win_size = sock->rdBufCount;

    sock->udp_recv_check_stat = (unsigned int *) malloc (sizeof( int ) * sock->checkCount);
    memset(sock->udp_recv_check_stat, 0,sizeof(int) * sock->checkCount);

    sock->udp_send_check_stat = (unsigned int *) malloc (sizeof( int ) * sock->checkCount);
    memset(sock->udp_send_check_stat, 0,sizeof(int) * sock->checkCount);

    for(unsigned int i = 0 ; i < sock->wrBufCount; ++i )
    {
        sock->udp_send_buff[i] = (unsigned char *) malloc (UDP_BUFSIZE);
        memset(sock->udp_send_buff[i], 0,UDP_BUFSIZE );
    }
    
    for(unsigned int i = 0 ; i < sock->rdBufCount; ++i )
    {
        sock->udp_recved_buff[i] = malloc(UDP_BUFSIZE);
        memset(sock->udp_recved_buff[i], 0,UDP_BUFSIZE );
        
    }
    
    for(unsigned int i = 0 ; i < sock->fecBufCount; ++i )
    {
        sock->udp_fec_buff[i] = malloc(UDP_BUFSIZE);
        memset(sock->udp_fec_buff[i], 0,UDP_BUFSIZE );
    }
   
    // event
    sock->event_connect = -1;
    sock->event_close = -1;
    sock->event_error = -1;
    sock->event_message = -1;

    return 1;
}

void socketevent_udp_trigger_connect(LSocketEventUDP *  sock,lua_State *L)
{
    if(sock->event_connect < 0)
        return;
    // trigger lua connect handle
    lua_rawgeti(L, LUA_REGISTRYINDEX, sock->event_connect);
    lua_newtable(L);

    int result = lua_pcall(L, 1, 0, 0);
    if (0 != result) {
        luaL_error(L, "connect event call error: %d", result);
    }
}

void socketevent_udp_trigger_message(LSocketEventUDP *sock, lua_State *L) {
    if (sock->event_message < 0) {
        return;
    }

    lua_rawgeti(L, LUA_REGISTRYINDEX, sock->event_message);
    lua_newtable(L);
    lua_pushliteral(L, "data");
    lua_pushlstring(L, sock->message_buffer + PH_LEN, sock->message_len);
    lua_settable(L, -3);

    int result = lua_pcall(L, 1, 0, 0);
    if (0 != result) {
        luaL_error(L, "message event call error: %d", result);
    }
}

void socketevent_udp_trigger_close(LSocketEventUDP *sock, lua_State *L) {
    if (sock->event_close < 0) {
        return;
    }

    // check close state
    if (sock->state != 1) {
        return;
    }

    // trigger lua close handle
    lua_rawgeti(L, LUA_REGISTRYINDEX, sock->event_close);
    lua_newtable(L);

    int result = lua_pcall(L, 1, 0, 0);
    if (0 != result) {
        luaL_error(L, "close event call error: %d", result);
    }
}

void socketevent_udp_trigger_error(LSocketEventUDP *sock, lua_State *L, int line, int err, const char *message) {
    if (sock->event_error < 0) {
        return;
    }

    // trigger lua error handle
    lua_rawgeti(L, LUA_REGISTRYINDEX, sock->event_error);
    lua_newtable(L);
    lua_pushliteral(L, "line");
    lua_pushinteger(L, line);
    lua_settable(L, -3);
    lua_pushliteral(L, "error");
    lua_pushinteger(L, err);
    lua_settable(L, -3);
    lua_pushliteral(L, "message");
    lua_pushstring(L, message);
    lua_settable(L, -3);

    int result = lua_pcall(L, 1, 0, 0);
    if (0 != result) {
        luaL_error(L, "error event call error: %d", result);
    }
}

#define bufsize 1024
char buffer[bufsize];

LUALIB_API int socketevent_udp_trigger_log (lua_State *L, const char *fmt, ...) {
  
  lua_getglobal(L, "print");
  va_list argptr;
  va_start(argptr, fmt);
  vsnprintf(buffer,bufsize ,fmt, argptr);
  va_end(argptr);
  lua_pushstring(L, buffer);
  
  lua_call(L, 1 ,0);

  return 0;
}

void *socketevent_udp_data(void *psock) {

    // tcp sock struct
    LSocketEventUDP *sock = (LSocketEventUDP *)psock;

    // create new thread State
    lua_State *L = lua_newthread(sock->L);

    // server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    if (inet_pton(AF_INET, (const char *)sock->ip, &server_addr.sin_addr) <= 0) {
        socketevent_udp_trigger_error(sock, sock->L, __LINE__, 12, "domain error");
        return 0;
    }

    server_addr.sin_port = htons((u_short)sock->port);

     if ((sock->socket  = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        socketevent_udp_trigger_error(sock, sock->L, __LINE__, 12, "create error");
        return 0;
     }

    char conn_buf[UDP_BUFSIZE];
    memset(conn_buf,0,UDP_BUFSIZE);

    UDPFRAME * frame = (UDPFRAME * )conn_buf;
    frame->head.flag = F_CONN;
    frame->dataLen = sizeof(CONN_KEY);

    strncpy(frame->data,CONN_KEY,frame->dataLen);
 
    sendto(sock->socket,conn_buf, frame->dataLen + UDP_HEADSIZE ,0, (const struct sockaddr *)&server_addr,sizeof(struct sockaddr_in));

    bool init = false;

    int addrlen = sizeof(server_addr);

    while(!init && sock->isTerminate == 0)
    {
        int recv_len = recvfrom(sock->socket, sock->data_buffer,  sock->data_buffer_size,0,(struct sockaddr *)&server_addr, &addrlen );
        if(recv_len > 0)
        {
            sock->port = ntohs(server_addr.sin_port);
            UDPFRAME * frame = (UDPFRAME * ) sock->data_buffer;
            sock->connectID = frame->head.connID;

            init = true;
        }   
    }
    
    struct  fd_set  rfds,wfds;
    
    struct timeval  tv;

    tv.tv_sec=tv.tv_usec=0;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(sock->socket,&rfds);
    FD_SET(sock->socket,&wfds);

    // trigger connect handle
    socketevent_udp_trigger_connect(sock, sock->L);
    int times = 10;

    // recv data
    while (sock->isTerminate == 0) {
#ifdef _WIN32   
        WaitForSingleObject(sock->mutex,INFINITE);
#else
        ::pthread_mutex_lock(&sock->mutex);
#endif
        
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_SET(sock->socket,&rfds);
        FD_SET(sock->socket,&wfds);

        switch(select((int)sock->socket+1,&rfds,&wfds,NULL,NULL))
        {// recv deal
            case -1:  exit(-1);break; //select error
            case  0:  break;
            default:
            {
                
                if(FD_ISSET(sock->socket,&rfds))
                {
                    sock->data_buffer_use = recvfrom(sock->socket, sock->data_buffer, sock->data_buffer_size, 0,(struct sockaddr *)&server_addr,  &addrlen);
                    
                    // check error
                    if (sock->data_buffer_use <= 0)
                        break;

                    udp_unpack(sock);
                }

                udp_check_resend(sock);

                send_raw_data(sock);

                unsigned int index = sock->udp_sended_order;
                for (unsigned int i = index; i < sock->udp_send_order;++i)
                {

                    if(FD_ISSET(sock->socket,&wfds) == 0)
                        break;
                    
                    unsigned int dwIndex = i % sock->wrBufCount;
                    UDPFRAME * frame = ( UDPFRAME * )sock->udp_send_buff[dwIndex];
                    send_frame(sock->socket,frame,(struct sockaddr *)&server_addr,  addrlen);
                    sock->udp_sended_order = i+1;   
                    
                }
                
            }
        }

#ifdef _WIN32       
        ReleaseMutex(sock->mutex);
        Sleep(0);
#else
        ::pthread_mutex_unlock(&sock->mutex);
        usleep(1000);
#endif
        
    }
    // trigger close handle
    socketevent_udp_trigger_close(sock, L);

    // socket state
    sock->state = 2;

    return NULL;
}

int udp_unpack(LSocketEventUDP *sock)
{
    unsigned char * in = sock->data_buffer;
    int iSize = sock->data_buffer_use;
    UDPFRAME * frame = (UDPFRAME *)in;
    switch (frame->head.flag)
    {
        case F_NORMAL:
            {
                if(check_recv_state(sock,frame->head.dataID))//重复的不需要处理
                    break;
                int buf_index = frame->head.dataID % sock->rdBufCount;
                memcpy(sock->udp_recved_buff[buf_index],in, iSize);
                int ret = refresh_recv_buf(sock);
                send_ack( sock,frame->head.orderID,frame->head.timestamp );

                int  diff = (int)frame->head.dataID - (int)sock->udp_recv_max ;
                if(diff >= 0)
                {
                   sock->udp_recv_max = frame->head.dataID;
                   sock->win_size = sock->rdBufCount -  ((int) sock->udp_recv_max - (int)sock->udp_recv_index ) - 1;//更新窗口大小
                }

                return ret;

            }break;
        case F_FEC:
            {

                int index = frame->head.dataID % sock->fecBufCount;
                memcpy(sock->udp_fec_buff[index], in, iSize);
                if(fec_recover(sock))
                {
                    int ret = refresh_recv_buf(sock);
                    send_ack( sock,frame->head.orderID,frame->head.timestamp );
                    int  diff = (int)frame->head.dataID - (int)sock->udp_recv_max ;
                    if(diff >= 0)
                    {
                        sock->udp_recv_max = frame->head.dataID;
                        sock->win_size = sock->rdBufCount -  ((int) sock->udp_recv_max - (int)sock->udp_recv_index ) - 1;//更新窗口大小
                    }
                    return ret;
                }

            }break;
        case F_ACK:
            {
                 if(frame->head.dataID >= sock->udp_send_check_index)
                 {
                    unsigned long long cur = now();
                    sock->udp_send_check_index = frame->head.dataID;
                    memcpy(sock->udp_send_check_stat ,frame->data, CHECK_LEN * sock->checkCount );
                    unsigned int mrtt = (unsigned int)(cur - frame->head.timestamp);
                    calc_rttrvo(sock,mrtt);
                 }
                
                return 0;
            }break;

        default:
            break;
    }

    return 0;

}

int refresh_recv_buf(LSocketEventUDP *sock)
{
    int ret = 0;

    for (unsigned int i = 0; i < sock->rdBufCount; ++i)
    {
        unsigned int  buf_index = sock->udp_recv_index % sock->rdBufCount;

        UDPFRAME * frame = (UDPFRAME * )sock->udp_recved_buff[buf_index];
        if (frame->head.flag != F_NONE)
        {
            if (frame->dataLen > (int)LUA_SOCKETEVENT_UDP_MESSAGE_MAX_SIZE)
                break;

            if (sock->message_buffer_size - sock->message_buffer_use < frame->dataLen) {
                while (sock->message_buffer_size - sock->message_buffer_use < frame->dataLen) {
                    sock->message_buffer_size <<= 1;
                }

                /*if (sock->message_buffer_size > LUA_SOCKETEVENT_UDP_MESSAGE_MAX_SIZE) {
                    sock->message_buffer_size = LUA_SOCKETEVENT_UDP_MESSAGE_MAX_SIZE;
                }*/ //缓冲不限制大小
                sock->message_buffer = (char *)realloc((void *)sock->message_buffer, sock->message_buffer_size + 1);
                sock->message_buffer[sock->message_buffer_size] = 0;
            }

            memcpy(&sock->message_buffer[sock->message_buffer_use],frame->data,  frame->dataLen);

            ret += frame->dataLen;
            sock->message_buffer_use += frame->dataLen;

            sock->udp_recv_index++;
            shift_check_state(sock);
            memset(frame,0, frame->dataLen + UDP_HEADSIZE);
        }

    }

    return ret;
}

void calc_rttrvo(LSocketEventUDP *sock,unsigned int mrtt)
{
     long m = mrtt;
    if(m == 0)
        m = 1;

    if(sock->srtt != 0)
    {
        m -= sock->srtt >> 3;
        sock->srtt += m;

        if(m < 0)
        {
            m = -m;
            m -= (sock->mdev>>2);

            if(m > 0)
                m >>= 3;

        }else
        {
            m -= (sock->mdev>> 2);

        }

        sock->mdev += m;

        if(sock->mdev > sock->mdev_max)
        {
            sock->mdev_max = sock->mdev;
            if(sock->mdev_max > sock->rttvar)
                sock->rttvar = sock->mdev_max;
        }

    }
    else
    {
        sock->srtt = m << 3;
        sock->mdev = m << 1;
        sock->mdev_max = sock->rttvar = sock->mdev >  MIN_RTO ?  sock->mdev : MIN_RTO;
    }

    sock->rtovar  =  sock->srtt +  (sock->mdev <<  2);
}

void shift_check_state(LSocketEventUDP *sock)
{
    for ( unsigned int i = 0; i < sock->checkCount; ++i)
        sock->udp_recv_check_stat[i] = (sock->udp_recv_check_stat[i] >> 1);
}

void send_ack(LSocketEventUDP *sock,unsigned int orderID,unsigned long long dwTime)
{
    int buf_count  = sock->wrBufCount - ((int)sock->udp_send_order - (int)sock->udp_sendok_order);

    if(buf_count <= 0)
        return;
    UDPFRAME * frame = (UDPFRAME *)sock->udp_send_buff[sock->udp_send_order];

    frame->head.flag = F_ACK;
    frame->head.connID = sock->connectID;
    frame->head.dataID = sock->udp_recv_index;
    frame->dataLen = sock->checkCount * CHECK_LEN;
    frame->head.timestamp = dwTime;
    frame->head.orderID = orderID;
    memcpy(frame->data,sock->udp_recv_check_stat,frame->dataLen);
    sock->udp_send_order++;
}

unsigned int cal_udp_index(LSocketEventUDP *sock, unsigned int index)
{
    int  recv_index = (int)sock->udp_recv_index;
    int  cal_index = (int)index;

    return  cal_index - recv_index;
}

bool check_recv_state( LSocketEventUDP *sock, unsigned int index)
{

    unsigned int offset = cal_udp_index(sock,index);

    if (offset > sock->rdBufCount);
    {
        //error
        return false;
    }

    return sock->udp_recv_check_stat[offset / CHECK_LEN] & (1 << (offset % CHECK_LEN));

}

bool check_send_state(LSocketEventUDP *sock, unsigned int index)
{
    int offset = (int) index - (int) sock->udp_send_check_index;

    if(offset < 0)
        return true;
    if (offset > (int)sock->wrBufCount)
    {
        return true;
    }

    return sock->udp_send_check_stat[offset / CHECK_LEN] & (1 << (offset % CHECK_LEN));
}

void set_check_state( LSocketEventUDP *sock,unsigned int index)
{
    unsigned int offset = cal_udp_index(sock,index);

    if (offset > sock->rdBufCount);
    {
        //error
        return ;
    }

    sock->udp_recv_check_stat[ offset / CHECK_LEN ] |= (1 << (offset % CHECK_LEN));

}

bool fec_recover(LSocketEventUDP *sock)
{
    bool recover = false;
    for (unsigned int  i = 0; i < sock->fecBufCount; ++i)
    {
        UDPFRAME * frame = (UDPFRAME *)sock->udp_fec_buff[i];

        if (frame->head.flag != F_NONE)
        {
            int index = frame->head.orderID * 2;

            bool hasFirst  = check_recv_state(sock,index);
            bool hasSecond = check_recv_state(sock,index + 1);
            if ( (hasFirst == false && hasSecond == true ) || ( hasFirst == true && hasSecond == false ) )
            {
                recover = true;
                unsigned int index_miss =  hasFirst ? index+1 : index;
                unsigned int index_exist = hasFirst ?  index : index + 1;

                index = hasFirst ? index + 1 : index ;

                UDPFRAME * frameiss = (UDPFRAME *)sock->udp_recved_buff[index_miss % sock->rdBufCount];
                UDPFRAME * frameExist = (UDPFRAME *)sock->udp_recved_buff[index_exist% sock->rdBufCount];

                frameiss->head.flag = F_NORMAL;
                frameiss->head.connID = sock->connectID;
                frameiss->head.orderID = 0;
                frameiss->head.dataID = index;
                frameiss->dataLen = frameExist->dataLen ^ frame->dataLen;

                for (int i =  0; i < frameiss->dataLen; ++i)
                {
                    frameiss->data[i] = frame->data[i] ^ frameExist->data[i];
                }

                set_check_state(sock,index_miss);
                memset(frame, 0,UDP_BUFSIZE);
            }

        }
    }

    return recover;
}

#if defined(_WIN32)
    typedef union {
        unsigned long long ft_scalar;
        FILETIME ft_struct;
    } FT;

    // Number of 100 nanosecond units from 1/1/1601 to 1/1/1970
#define EPOCH_BIAS  116444736000000000i64
#endif 

unsigned long long now()
{

#if defined(_WIN32)
    unsigned long long tim;
    FT nt_time;
    GetSystemTimeAsFileTime(&(nt_time.ft_struct));

    tim = (unsigned long long)((nt_time.ft_scalar - EPOCH_BIAS) / 10000);

    return tim;
#else
    unsigned long long retval = 0LL;

    struct timespec tv;

    clock_gettime(CLOCK_REALTIME,&tv);

    retval = tv.tv_sec;

    retval *= 1000LL;

    retval += ( tv.tv_nsec / 1000000L );

    return retval;
    
#endif
}

int send_frame(sock_t sock ,UDPFRAME  * frame , struct sockaddr * addr, int socklen)
{
    int iSize  = 0;
   
    iSize = frame->dataLen + UDP_HEADSIZE;

    if(frame->head.flag != F_ACK)
        frame->head.timestamp = now();

    int retcode = sendto(sock, (const char *)frame, iSize, 0, addr, socklen);
    return retcode;
}

unsigned int send_raw_data(LSocketEventUDP *sock)
{

    if(sock->send_buffer_use == 0)
        return 0;
    
    int buf_count = sock->wrBufCount - ((int)sock->udp_send_order - (int)sock->udp_sendok_order);
    if(buf_count <= 0 )
        return 0;

    int buf_size = buf_count * UDP_DATASIZE - PH_LEN;


    char * pBuffer = sock->send_data_buffer;
    int iSize = sock->send_buffer_use;

    int maxSize = (int)iSize > buf_size ? buf_size : iSize;
    int size = 0;
    int len = 0;

    socketevent_udp_trigger_log(sock->L,"send_raw_data maxSize %d",maxSize);

    while (true)
    {
        
        UDPFRAME * frame = (UDPFRAME *)sock->udp_send_buff[sock->udp_send_order];

        if (maxSize > UDP_DATASIZE)
            size = UDP_DATASIZE;
        else
            size = maxSize;

        udp_pack(sock->connectID,sock->udp_send_index,&(((char *)pBuffer)[len]), size, frame);

        frame->head.orderID = sock->udp_send_order;
        len += size;
        maxSize -= size;

        if (sock->udp_send_index % 2 == 1)
        {
            ++sock->udp_send_order;

            UDPFRAME * frameFec = (UDPFRAME *)sock->udp_send_buff[sock->udp_send_order%sock->wrBufCount];
            UDPFRAME * frame1 = (UDPFRAME *)sock->udp_send_buff[sock->last_fec_order%sock->fecBufCount];
            UDPFRAME * frame2 = frame;

            cal_fec(sock->connectID,sock->udp_send_order,frameFec, frame1, frame2);
        }
        else
        {
            sock->last_fec_order = sock->udp_send_order;
        }

        ++sock->udp_send_order;
        ++sock->udp_send_index;

        if (maxSize == 0)
        {
            break;
        }
    }

    memmove(sock->send_data_buffer, sock->send_data_buffer + len, sock->send_buffer_use - len);

    sock->send_buffer_use -= len;

    return len;
}

void udp_check_resend(LSocketEventUDP *sock)
{
    unsigned long long cur = now();

    int sept = (int)sock->udp_sended_order - (int)sock->udp_sendok_order  ;

    if(sept <= 0 )
        return;

    bool  increase = true;

    for(int i = 0 ; i <= sept; ++i)
    {
        int frameID = (sock->udp_sendok_order + i) % sock->wrBufCount;

        UDPFRAME *  frame_buf = (UDPFRAME *)sock->udp_send_buff[frameID];

        if(frame_buf->head.flag == F_NORMAL)
        {
            if(check_send_state(sock,frame_buf->head.dataID ))
            {
                memset(frame_buf, 0,UDP_BUFSIZE);
            }
            else
            {
                    int buf_count  = sock->wrBufCount - ((int)sock->udp_send_order - (int)sock->udp_sendok_order);

                    if(buf_count <= 0)
                        return;

                unsigned int  mrtt =  (unsigned int )(cur - frame_buf->head.timestamp);
                if(mrtt > sock->rtovar && sock->rtovar != 0)
                {
                   
                    UDPFRAME * newFrame = (UDPFRAME *)(sock->udp_send_buff[sock->udp_send_order% sock->wrBufCount]);
                    memcpy(newFrame,frame_buf,  UDP_BUFSIZE);
                    newFrame->head.orderID = sock->udp_send_order;
                    sock->udp_send_order ++;
                    memset(frame_buf,0, UDP_BUFSIZE);
                }
                else
                {
                    increase = false;
                }

            }

        }
        else if(frame_buf->head.flag != F_NONE)
        {
            memset(frame_buf, 0,UDP_BUFSIZE);
        }

        if(increase)
            sock->udp_sendok_order ++;
    }
}

#if defined(_WIN32)
void socketevent_udp_data_win(void *psock) {
    socketevent_udp_data(psock);
}
#endif

size_t pack_data(const unsigned char * pData,  size_t size,unsigned char * out)
{

    size_t iSize = size ;

    memcpy(&(out[PH_LEN]),pData,iSize);

    (*(unsigned int *)out) = (int)iSize;

    return iSize + PH_LEN;
}

unsigned int packet_size(const unsigned char * in) 
{
    return PH_LEN + (*((unsigned int *)in));
}

int udp_pack(unsigned int connectID,unsigned int dataID,const void * in, int iSize, UDPFRAME * frame)
{
    frame->head.flag = F_NORMAL;
    frame->head.connID = connectID;
    frame->head.orderID = 0;
    frame->head.dataID = dataID;
    frame->dataLen = iSize;
    frame->head.timestamp = 0;

    memcpy(frame->data,in, iSize);

    return UDP_HEADSIZE + iSize;
}


void cal_fec( unsigned int connectID,unsigned int  udp_send_order,UDPFRAME * frameFec, UDPFRAME * frame1, UDPFRAME *frame2)
{
    memset(frameFec, 0,UDP_BUFSIZE);
    frameFec->head.flag = F_FEC;
    frameFec->head.connID = connectID;
    frameFec->head.orderID = udp_send_order;
    frameFec->head.dataID = (frame1->head.dataID / 2);

    int max = frame1->dataLen > frame2->dataLen ?  frame1->dataLen : frame2->dataLen;

    for (int i = 0; i < max; ++i)
    {
        frameFec->data[i] = frame1->data[i] ^ frame2->data[i];
    }

    frameFec->dataLen = frame1->dataLen ^ frame2->dataLen;

}

static int socketevent_udp_connect(lua_State *L) {
    // sock struct
    LSocketEventUDP *sock = luaL_checkudata(L, 1, LUA_SOCKETEVENT_UDP_HANDLE);


    // check connect state
    if (sock->state != 0) {
        socketevent_udp_trigger_error(sock, sock->L, __LINE__, 6, "socket has connect");
        return 0;
    }

    sock->state = 1;

    // get params
    const char *host = luaL_checkstring(L, 2);
    lua_Integer port = luaL_checkinteger(L, 3);

#if defined(_WIN32)
    WSADATA wsa;
    // WinSock Startup
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        socketevent_udp_trigger_error(sock, sock->L, __LINE__, 2, "c WSAStartup function error!");
        return 0;
    }
#endif
    
    sock->host = host;
    if (-1 == inet_addr(host)) {
        struct hostent *hostinfo;
        if ((hostinfo = (struct hostent*)gethostbyname(host)) == NULL) {
#if defined(_WIN32)
            socketevent_udp_trigger_error(sock, sock->L, __LINE__, 18, "domain not found!");
#else
            socketevent_udp_trigger_error(sock, sock->L, __LINE__, h_errno, hstrerror(h_errno));
#endif
            
            return 0;
        }

        if (hostinfo->h_addrtype == AF_INET && hostinfo->h_addr_list != NULL) {
#if defined(_WIN32)
            char ipstr[16];
            char * ipbyte = *(hostinfo->h_addr_list);
            sprintf(ipstr, "%d.%d.%d.%d", *ipbyte, *(ipbyte++), *(ipbyte+2), *(ipbyte+3));
            sock->ip = ipstr;
#else
            char ipstr[16];
            inet_ntop(hostinfo->h_addrtype, *(hostinfo->h_addr_list), ipstr, sizeof(ipstr));
            sock->ip = ipstr;
        
#endif
        } else {
            socketevent_udp_trigger_error(sock, sock->L, __LINE__, 3, "not support ipv6!");
            
            return 0;
        }
    } else {
        sock->ip = host;
    }
    sock->port = port;
    
    // create socket
    if ((sock->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        return 0;
    }

    // start thread
#if defined(_WIN32)
    _beginthread(socketevent_udp_data_win, 0, sock);
#else
    int retval = pthread_create(&sock->thread, NULL, socketevent_udp_data, sock);
    if (retval != 0) {
        socketevent_udp_trigger_error(sock, sock->L, __LINE__, retval, strerror(retval));
        return 0;
    }
#endif
    
    return 1;
}

static int socketevent_udp_on(lua_State *L) {
    // sock struct
    LSocketEventUDP *sock = (LSocketEventUDP *)luaL_checkudata(L, 1, LUA_SOCKETEVENT_UDP_HANDLE);

    // get params
    const char *name = luaL_checkstring(L, 2);
    int handler = luaL_ref(L, LUA_REGISTRYINDEX);

    // save event handle
    if (strcmp("connect", name) == 0) {
        sock->event_connect = handler;
    } else if (strcmp("close", name) == 0) {
        sock->event_close = handler;
    } else if (strcmp("error", name) == 0) {
        sock->event_error = handler;
    } else if (strcmp("message", name) == 0) {
        sock->event_message = handler;
    } else {
        luaL_error(L, "event %s not support!", name);
    }

    return 1;
}

static int socketevent_udp_send(lua_State *L) {

    // sock struct
    LSocketEventUDP *sock = (LSocketEventUDP *)luaL_checkudata(L, 1, LUA_SOCKETEVENT_UDP_HANDLE);

    // check connect state
    if (sock->state != 1) {
        return 0;
    }

#ifdef _WIN32
        WaitForSingleObject(sock->mutex,INFINITE);
#else
        ::pthread_mutex_lock(&sock->mutex);
#endif
    // get params
    size_t data_size = 0;
    const char *data = luaL_checklstring(L, 2, &data_size); 

    if (sock->send_buffer_size - sock->send_buffer_use <data_size + PH_LEN) 
    {
        while (sock->send_buffer_size - sock->send_buffer_use < data_size + PH_LEN) {
            sock->send_buffer_size <<= 1;
        }

        /*if (
            sock->send_buffer_size > LUA_SOCKETEVENT_UDP_MESSAGE_MAX_SIZE) {
            sock->send_buffer_size = LUA_SOCKETEVENT_UDP_MESSAGE_MAX_SIZE;
        }*/ //缓冲不限制大小

        sock->send_data_buffer = (char *)realloc((void *)sock->send_data_buffer, sock->send_buffer_size + 1);
        memset(sock->send_data_buffer + sock->send_buffer_use,0,sock->send_buffer_size-sock->send_buffer_use);
    }

    data_size = pack_data(data,data_size,sock->send_data_buffer + sock->send_buffer_use);
    sock->send_buffer_use += (int)data_size;
    

#ifdef _WIN32
        ReleaseMutex(sock->mutex);
#else
        ::pthread_mutex_unlock(&sock->mutex);
#endif
    return (int) data_size;
}

static int socketevent_udp_sync_message(lua_State *L) {
    // sock struct
    LSocketEventUDP *sock = (LSocketEventUDP *)luaL_checkudata(L, 1, LUA_SOCKETEVENT_UDP_HANDLE);
    #ifdef _WIN32
        WaitForSingleObject(sock->mutex,INFINITE);
    #else
        ::pthread_mutex_lock(&sock->mutex);
    #endif
    // check connect state
    if (sock->state != 1) {
        return 0;
    }


    int message_raw_len = 0;
    while (1) {
        // check message head
        if (sock->message_buffer_use < PH_LEN) {
            break;
        }

        message_raw_len = packet_size(sock->message_buffer);
        // message len
        sock->message_len = packet_size(sock->message_buffer) - PH_LEN;

        // check message len
        if (sock->message_len > LUA_SOCKETEVENT_UDP_MESSAGE_MAX_SIZE) {
            break;
        }
    
        // check message len
        if (sock->message_buffer_use < message_raw_len) {
            
            break;
        }

        // trigger message event
        socketevent_udp_trigger_message(sock, L);

        // move data
        if (sock->message_buffer_use - message_raw_len > 0) {
            memmove(sock->message_buffer, sock->message_buffer + message_raw_len, sock->message_buffer_use - message_raw_len);
        }

        sock->message_buffer_use -= message_raw_len;
    }


    #ifdef _WIN32
        ReleaseMutex(sock->mutex);
    #else
        ::pthread_mutex_unlock(&sock->mutex);
    #endif

    return 1;
}

static int socketevent_udp_close(lua_State *L) {
    // sock struct
    LSocketEventUDP *sock = (LSocketEventUDP *)luaL_checkudata(L, 1, LUA_SOCKETEVENT_UDP_HANDLE);

    // check connect state
    if (sock->state != 1) {
        socketevent_udp_trigger_error(sock, sock->L, __LINE__, 5, "socket not connect!");
        lua_pushinteger(L, 0);
        return 0;
    }

    // close socket
    sock->isTerminate = 1;
#ifdef _WIN32

        closesocket(sock->socket);  
#else
        close(sock->socket);
        
#endif
    return 1;
}

static int socketevent_udp_wait(lua_State *L) {
    // sock struct
    LSocketEventUDP *sock = (LSocketEventUDP *)luaL_checkudata(L, 1, LUA_SOCKETEVENT_UDP_HANDLE);

    // wait thread exit
#ifdef _WIN32
    while (1) {
        // check close state
        if (sock->state == 2) {
            return 1;
        }
        Sleep(1000);
    }
#else
    void *retval = NULL;
    pthread_join(sock->thread, &retval);
#endif

    return 1;
}

static int socketevent_udp_gc(lua_State *L) {
    // sock struct
    LSocketEventUDP *sock = (LSocketEventUDP *)luaL_checkudata(L, 1, LUA_SOCKETEVENT_UDP_HANDLE);

    sock->isTerminate = 1;
    // close socket
#ifdef _WIN32
    closesocket(sock->socket);
#else
    close(sock->socket);
    // shutdown(sock->socket, SHUT_RD);
#endif

    // exit thread
#ifndef _WIN32
    pthread_cancel(sock->thread);
#endif


    // free buffer
    free((void *)sock->data_buffer);
    free((void *)sock->message_buffer);

    free((void *) sock->udp_recv_check_stat);
    free((void *) sock->udp_send_check_stat);

    for(unsigned int i = 0 ; i < sock->wrBufCount; ++i )
    {
        free((void * )sock->udp_send_buff[i] );
    }
    
    for(unsigned int i = 0 ; i < sock->rdBufCount; ++i )
    {
        free((void *) sock->udp_recved_buff[i]);
    }
    
    for(unsigned int i = 0 ; i < sock->fecBufCount; ++i )
    {
        free((void *) sock->udp_fec_buff[i]);
    }


    return 0;
}

static int socketevent_udp_tostring(lua_State *L) {
    // sock struct
    LSocketEventUDP *sock = (LSocketEventUDP *)luaL_checkudata(L, 1, LUA_SOCKETEVENT_UDP_HANDLE);

    // return string
    lua_pushfstring(L, "tcp sock %s:%d", sock->ip, sock->port);

    return 1;
}

static const luaL_Reg socketeventlib[] = {
    { "udp", socketevent_udp },
    { NULL, NULL }
};

static const luaL_Reg udplib[] = {
    { "connect", socketevent_udp_connect },
    { "on", socketevent_udp_on },
    { "send", socketevent_udp_send },
    { "sync",socketevent_udp_sync_message},
    { "close", socketevent_udp_close },
    { "wait", socketevent_udp_wait },
    { "__gc", socketevent_udp_gc },
    { "__tostring", socketevent_udp_tostring },
    { NULL, NULL }
};

LUAMOD_API int luaopen_udpsocketevent(lua_State *L) {

#if LUA_VERSION_NUM == 501
    // create tcp metatable
    luaL_newmetatable(L, LUA_SOCKETEVENT_UDP_HANDLE);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, udplib);

    // create lib
    luaL_register(L, "socketevent", socketeventlib);
#else
    // create lib
    luaL_newlib(L, socketeventlib);

    // create tcp metatable
    luaL_newmetatable(L, LUA_SOCKETEVENT_UDP_HANDLE);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, udplib, 0);
    lua_pop(L, 1);
#endif

    return 1;
}