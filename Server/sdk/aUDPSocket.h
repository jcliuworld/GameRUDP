#ifndef _aUDPSocket_H__
#define _aUDPSocket_H__

#pragma once

/**
 * @file aUDPSocket.h
 * @brief  套接口封装
 * @author ljc jc.liuworld@hotmail.com
 * @date 2018-11-22
 */

#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <iostream>
#include <queue>
#include <errno.h>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <zlib.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <ext/pool_allocator.h>
#include <ext/mt_allocator.h>
#include <sys/poll.h>
#include <sys/epoll.h>


#include "aDef.h"
#include "aMemStream.h"
#include "aNoneCopyable.h"
#include "aTime.h"
#include "aFunc.h"

enum FrameFlag
{
    F_NONE,//空包
    F_NORMAL,//普通数据包
    F_ACK,//确认包
    F_FEC,//校验
    F_CONN,//连接
};
#pragma pack(1)
typedef struct  tUPDHeader
{
    WORD flag;// 0 普通数据包 标记位 1 重传请求包  2 FEC校验包    data = dataID xor (dataID + 1) 3 ack 确认包
    DWORD orderID; //udp 顺序编号 用于计算RTT
    DWORD connID;//连接ID
    DWORD dataID;//数据编号
    QWORD timestamp;//时戳
} UDPHeader;


typedef struct stUDPFrame
{
    UDPHeader head;
    WORD dataLen;
    unsigned char data[0];
}UDPFRAME;

#pragma pack()

#define UDP_BUFSIZE 548

#define MIN_RTO 100

#define CONN_KEY "cA8ZKLoo"

class aUDPSocket : private aNoneCopyable
{
    public:
        /**
         * @brief head size
         */
        static const int UDP_HEADSIZE;

        /**
         * @brief data size
         */
        static const int UDP_DATASIZE;
        /**
         * @brief read time out
         */

        static const int T_RD_MSEC;//   =   2100;
        /**
         * @brief write time out
         */

        static const int T_WR_MSEC;//   =   5100;
        /**
         * @brief packet len
         */

        static const unsigned int PH_LEN;// =   sizeof(unsigned int);
        /**
         * @brief check len
         */
        static const unsigned int CHECK_LEN;//  =   sizeof(unsigned int);

        /**
         * @brief  udp packet head
         */
        static const unsigned int UDP_LEN;

        /**
         * @brief min packet len
         */
        static const unsigned int PACKET_ZIP_MIN;// =   32;
        /**
         * @brief compress bits
         */

        static const unsigned int PACKET_ZIP;// =   0x40000000;
        /**
         * @brief compress read flag
         */

        static const unsigned int INCOMPLETE_READ;//    =   0x00000001;
        /**
         * @brief compress write flag
         */
        static const unsigned int INCOMPLETE_WRITE;//   =   0x00000002;
        /**
         * @brief compress mask
         */
        static const unsigned int PACKET_MASK;//=   _BLOCKSIZE - 1;
        /**
         * @brief max data buffer size
         */

        static const unsigned int MAX_DATABUFFERSIZE;// =   PACKET_MASK;
        /**
         * @brief max data size
         */

        static const unsigned int MAX_DATASIZE;//= (MAX_DATABUFFERSIZE - PH_LEN);
        /**
         * @brief max user data size
         */

        static const unsigned int MAX_USERDATASIZE;//= (MAX_DATASIZE -128);
        /**
         * @brief get ip addr by eth name
         *
         * @param ifName eth name
         *
         * @return ip addr
         */

        static const char * getIPByIfName(const char * ifName);
        /**
         * @brief  the default constructor
         *
         * @param sock socket
         * @param addr address
         * @param compress whether compress
         */

        aUDPSocket(const int sock, const struct sockaddr_in * addr, DWORD connectID ,int rdSize = 17024 ,int wrSize = 68096);
        /**
         * @brief the default extract.
         */
        ~aUDPSocket();

         /**
         * @brief send data
         *
         * @param pstrCmd data ptr
         * @param iCmd data len
         * @param buffer whether buffer
         *
         * @return successful true or  false
         */

        bool sendCmd(const void * pstrCmd, const int iCmd);
        bool sendCmd(const std::string & send);

        bool sync();
        /**
         * @brief sock sync
         */

        void force_sync();
        /**
         * @brief check io for read
         *
         * @return
         */

        int checkIOForRead();
        /**
         * @brief check ip for write
         *
         * @return
         */

        int checkIOForWrite();
        /**
         * @brief receive the data to buff with no pool
         *
         * @return
         */

        int recvToBufNoPoll();
        /**
         * @brief receive the data with no pool
         *
         * @param pstrCmd data ptr
         * @param iCmdLen data len
         *
         * @return
         */

        int recvToCmdNoPoll(void * pstrCmd, const int iCmdLen);
        /**
         * @brief
         *
         * @param pstrCmd
         * @param iCmdLen
         *
         * @return
         */
        int recvToNoPackNoPoll(void * pstrCmd, const unsigned int iCmdLen);

        /**
         * @brief get the socket ip
         *
         * @return ip addr
         */

        inline const char * getIP()const { return inet_ntoa(addr.sin_addr); }
        /**
         * @brief get the socket addr
         *
         * @return socket addr
         */

        inline const DWORD getAddr()const { return addr.sin_addr.s_addr; }
        /**
         * @brief get the port
         *
         * @return port
         */

        inline const unsigned short getPort()const { return ntohs(addr.sin_port); }
        /**
         * @brief get the local ip
         *
         * @return local ip
         */

        inline const char * getLocalIP() const { return inet_ntoa((localaddr.sin_addr)); }
        /**
         * @brief get the local port
         *
         * @return port
         */

        inline const unsigned short getLocalPort()const { return ntohs(localaddr.sin_port); }
        /**
         * @brief set the out time of read
         *
         * @param msec read timeout
         */

        inline void setReadTimeOut(const int msec) { rd_msec = msec; }
        /**
         * @brief set the out time of write
         *
         * @param mesc write timeout
         */

        inline void setWriteTimeOit(const int mesc) { wr_msec = mesc; }
        /**
         * @brief add the sock to epoll
         *
         * @param kdpfd sock handle
         * @param events events
         * @param ptr ptr
         */

        inline void addEpoll(int kdpfd, __uint32_t events, void * ptr)
        {
            struct epoll_event ev;

            ev.events = events;

            ev.data.ptr = ptr;

            if (-1 == epoll_ctl(kdpfd, EPOLL_CTL_ADD, sock, &ev))
            {

                Age::logger->fatal("%s:%d", __FUNCTION__, errno);
            }
        }
        /**
         * @brief remove the sock form epoll
         *
         * @param kdpfd sock handle
         * @param events events
         */

        inline void delEpoll(int kdpfd, __uint32_t events)
        {
            struct epoll_event ev;

            ev.events = events;

            ev.data.ptr = NULL;

            if (-1 == epoll_ctl(kdpfd, EPOLL_CTL_DEL, sock, &ev))
            {

                Age::logger->fatal("%s:%d", __FUNCTION__, errno);
            }
        }
        /**
         * @brief fill pollfd struct
         *
         * @param pfd pollfd struct
         * @param events events
         */

        inline void fillPollFD(struct pollfd & pfd, short events)
        {
            pfd.fd = sock;

            pfd.events = events;

            pfd.revents = 0;
        }
        /**
         * @brief set the encode method
         *
         * @param m method
         */


        inline unsigned int snd_queue_size() { return _snd_queue.rdSize() ; }
        /**
         * @brief set the recv buffer szie
         *
         * @return recv buffer size
         */

        inline unsigned int getBufferSize()const { return _rcv_queue.maxSize() + _snd_queue.maxSize(); }
        /**
         * @brief comoress the data
         *
         * @param pData data ptr
         * @param iLen data len
         * @param cmdQueue data of out
         * @param _compresss compress falg
         *
         * @return
         */

        unsigned int debug_currPtr;
        unsigned int debug_offPtr;
        unsigned int debug_max_size;
        unsigned int debug_wait_size;

        inline int cal_udp_index(DWORD index)
        {
            int  recv_index = (int)udp_recv_index;
            int  cal_index = (int)index;

            return  cal_index - recv_index;
        }

        inline bool CheckRecvState(DWORD index)
        {
            int offset = cal_udp_index(index);
            if(offset < 0)
                return true;
            if (offset > (int)rdBufCount)
            {
                return false;
            }

            return udp_recv_check_stat[offset / CHECK_LEN] & (1 << (offset % CHECK_LEN));
        }

        inline void SetCheckState(DWORD index)
        {
            int offset = cal_udp_index(index);
            if(offset < 0)
                return ;

            if (offset > (int)rdBufCount)
            {
                return ;
            }

            udp_recv_check_stat[ offset / CHECK_LEN ] |= (1 << (offset % CHECK_LEN));
        }

        inline void ShiftCheckState()
        {
            for ( DWORD i = 0; i < checkCount; ++i)
                udp_recv_check_stat[i] = (udp_recv_check_stat[i] >> 1);
        }

        inline bool CheckSendState(DWORD index)
        {
            int offset = (int) index - (int) udp_send_check_index;

            if(offset < 0)
                return true;
            if (offset > (int)wrBufCount)
            {
                return true;
            }

            return udp_send_check_stat[offset / CHECK_LEN] & (1 << (offset % CHECK_LEN));
        }

        inline void calcRTTRTO(DWORD mrtt);

	inline DWORD getConnectID(){return connectID;}

    private:

        DWORD connectID;
        DWORD rdBufCount;
        DWORD wrBufCount;
        DWORD checkCount;
        DWORD fecBufCount;

        DWORD winSize;

        DWORD srtt;
        DWORD mdev;
        DWORD mdev_max;
        DWORD rttvar;
        DWORD rtovar;

        int sock;
        struct sockaddr_in addr;
        struct sockaddr_in localaddr;
        socklen_t socklen;
        int rd_msec;
        int wr_msec;

        DWORD udp_recv_index; //udp 数据 接收标记
        DWORD udp_send_index; //udp 数据 发送标记

        DWORD udp_sendok_order;//最近一个发送确认的包
        DWORD udp_sended_order;//最近一个已经发送的包的index

        DWORD udp_recv_max; //udp 接收顺序标记

        DWORD udp_send_order; //udp 发送顺寻标记

        DWORD last_fec_order;

        DWORD * udp_recv_check_stat;// 接受到的数据包的check状态

        DWORD  udp_send_check_index;
        DWORD *udp_send_check_stat; //接收到的ack数据包的状态


        unsigned char  (*udp_send_buff)[UDP_BUFSIZE];//重发缓存
        unsigned char  (* udp_recved_buff)[UDP_BUFSIZE];//接收缓存
        unsigned char (*udp_fec_buff)[UDP_BUFSIZE];//FEC缓存

        unsigned char _udp_recv_tmp_queue[UDP_BUFSIZE];

        stBufferCmdQueue _rcv_queue;

        unsigned int _rcv_raw_size;
        stBufferCmdQueue _snd_queue;

        aMutex mutex;

        unsigned int  bitmask;

        aRealTime _timestamp;

        inline void setFlag(unsigned int _f) { bitmask |= _f; }

        inline bool issetFlag(unsigned int _f) const { return bitmask & _f; }

        inline void clearFlag(unsigned int _f) { bitmask &= ~_f; }

        inline unsigned int packetMinSize() const { return PH_LEN; }

        inline unsigned int packetSize(const unsigned char * in) const {
            return PH_LEN + ((*((unsigned int *)in))&PACKET_MASK);
        }

        inline int sendRawData(const void * pBuffer, const int iSize);
        inline int sendRawDataNoPoll(const void * pBuffer, const int iSize);
        inline bool setNonblock();
        inline int waitForRead();
        inline int waitForWrite();
        inline int udp_pack(DWORD dataID,const void * in,int iSize,UDPFRAME * frame);
        inline int udp_unpack(unsigned char * in, int iSize, unsigned char * out);

        inline void checkResend();
        inline int refreshRecvBuf(unsigned char * out);
        inline bool fecRecover();
        void sendAck(DWORD orderID,QWORD dwTime);
        UDPFRAME * getFrame(DWORD index);
        void  calFEC(UDPFRAME * frameFec,UDPFRAME * frame1,UDPFRAME * frame2);
        int sendFrame(UDPFRAME * frame);

        inline unsigned int packetUnPack(unsigned char * in, const unsigned int iPacketLen, unsigned char * out);
        template <typename bufferType>
            inline unsigned int packetPack(const void  * pData,const unsigned int iLen, bufferType & cmdQueue);

};

template<typename bufferType>
inline unsigned int aUDPSocket::packetPack(const void * pData,const unsigned int iLen, bufferType & cmdQueue)
{
    unsigned int iSize = iLen > MAX_DATASIZE ? MAX_DATASIZE : iLen;
    unsigned int iMask = 0;

    cmdQueue.wrReserve(iSize+PH_LEN);
    bcopy(pData,&(cmdQueue.wrBuf()[PH_LEN]),iSize);

    (*(unsigned int *)cmdQueue.wrBuf()) = (iSize | iMask);

    cmdQueue.wrFlip(iSize+PH_LEN);

    return iSize + PH_LEN;
}


#endif
