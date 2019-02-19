#include "aUDPSocket.h"
#include "aMemStream.h"
/**
 * @file aUDPSocket.cpp
 * @brief  套接口封装
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-14
 */

const int aUDPSocket::UDP_HEADSIZE = sizeof(UDPHeader) + sizeof(WORD);
const int aUDPSocket::UDP_DATASIZE = UDP_BUFSIZE - UDP_HEADSIZE;

const int aUDPSocket::T_RD_MSEC = 2100;
const int aUDPSocket::T_WR_MSEC = 5100;
const unsigned int aUDPSocket::PH_LEN = sizeof(unsigned int);
const unsigned int aUDPSocket::CHECK_LEN = sizeof(unsigned int);
const unsigned int aUDPSocket::INCOMPLETE_READ = 0x00000001;
const unsigned int aUDPSocket::INCOMPLETE_WRITE = 0x00000002;
const unsigned int aUDPSocket::PACKET_MASK = _BLOCKSIZE - 1;
const unsigned int aUDPSocket::MAX_DATABUFFERSIZE = PACKET_MASK;
const unsigned int aUDPSocket::MAX_DATASIZE = (MAX_DATABUFFERSIZE - PH_LEN);
const unsigned int aUDPSocket::MAX_USERDATASIZE = (MAX_DATASIZE - 128);

aUDPSocket::aUDPSocket(const int sock, const struct sockaddr_in * addr , DWORD connectID,int rdSize /* = 17024*/, int wrSize /* = 68096*/)
{
    assert(-1 != sock);

    this->sock = sock;
    this->connectID = connectID;

    bzero(&this->addr, sizeof(struct sockaddr_in));

    if (NULL == addr)
    {
        socklen_t len = sizeof(struct sockaddr);
        getpeername(this->sock, (struct sockaddr *)&this->addr, &len);
    }
    else
    {
        bcopy(addr, &this->addr, sizeof(struct sockaddr_in));
    }

    bzero(&this->localaddr, sizeof(struct sockaddr_in));

    {
        socklen_t len = sizeof(struct sockaddr);
        getpeername(this->sock, (struct sockaddr *)&this->localaddr, &len);
    }


    socklen = sizeof(struct sockaddr_in);

    fcntl(this->sock, F_SETFD, fcntl(this->sock, F_GETFD, 0) | FD_CLOEXEC);


    rd_msec = T_RD_MSEC;

    wr_msec = T_WR_MSEC;

    _rcv_raw_size = 0;


    setFlag(INCOMPLETE_READ | INCOMPLETE_WRITE);

    bitmask = 0;

    udp_recv_index = 0 ;
    udp_send_index = 0;

    udp_sendok_order = 0;
    udp_sended_order = 0;

    udp_recv_max = 0;
    udp_send_order = 0;

    srtt = 0;
    mdev = 0;
    mdev_max = 0;
    rttvar = 0;
    rtovar = 0;

    wrBufCount = wrSize / UDP_DATASIZE;
    rdBufCount = rdSize / UDP_DATASIZE;
    fecBufCount = wrBufCount / 2 + 1;
    checkCount = rdBufCount / CHECK_LEN + 1;
    winSize = rdBufCount;

    udp_send_check_index = 0;

    udp_recv_check_stat = new DWORD[checkCount];
    bzero(udp_recv_check_stat, sizeof(DWORD) * checkCount);

    udp_send_check_stat = new DWORD[checkCount];
    bzero(udp_send_check_stat, sizeof(DWORD) * checkCount);


    udp_send_buff = new unsigned char[wrBufCount][UDP_BUFSIZE];
    bzero(udp_send_buff, UDP_BUFSIZE * wrBufCount);

    udp_recved_buff = new unsigned char[rdBufCount][UDP_BUFSIZE];
    bzero(udp_recved_buff, UDP_BUFSIZE * rdBufCount);

    udp_fec_buff = new unsigned char[fecBufCount][UDP_BUFSIZE];
    bzero(udp_fec_buff, UDP_BUFSIZE * fecBufCount);

    debug_offPtr = debug_currPtr = debug_max_size = debug_wait_size = 0;

}

aUDPSocket::~aUDPSocket()
{
    delete[] udp_recv_check_stat;
    delete[] udp_send_check_stat;

    delete[] udp_send_buff;

    delete[] udp_recved_buff;

    delete [] udp_fec_buff;

    Age::logger->debug(" the socket is closed");
    ::shutdown(sock, SHUT_RDWR);
    TEMP_FAILURE_RETRY(::close(sock));

    sock = -1;
}

#define success_unpack() \
    do { \
        if( _rcv_raw_size >= packetMinSize())\
        {\
            unsigned int iRecordLen = packetSize(_rcv_queue.rdBuf());\
            if(_rcv_raw_size >= iRecordLen )\
            {\
                int retval = packetUnPack(_rcv_queue.rdBuf(),iRecordLen,(unsigned char *)pstrCmd );\
                _rcv_queue.rdFlip(iRecordLen);\
                _rcv_raw_size -= iRecordLen;\
                return retval;\
            }\
        }\
    }while(0)

int aUDPSocket::sendRawData(const void * pBuffer, const int iSize)
{

    int buf_count = wrBufCount - ((int)udp_send_order - (int) udp_sendok_order);

    if(buf_count <= 0)
        return 0;

    int bufSize  = buf_count * UDP_DATASIZE - PH_LEN;

    int maxSize = iSize > bufSize ? bufSize : iSize;
    int size = 0;
    int len = 0;

    while (true)
    {

        UDPFRAME * frame = (UDPFRAME *)udp_send_buff[udp_send_order];

        if (maxSize > UDP_DATASIZE)
            size = UDP_DATASIZE;
        else
            size = maxSize;

        udp_pack(udp_send_index,&(((char *)pBuffer)[len]), size, frame);

        frame->head.orderID = udp_send_order;
        len += size;
        maxSize -= size;

        if (udp_send_index % 2 == 1)
        {//Ð£ÑéÖ¡
            ++udp_send_order;

            UDPFRAME * frameFec = (UDPFRAME *)udp_send_buff[udp_send_order];
            UDPFRAME * frame1 = (UDPFRAME *)udp_send_buff[last_fec_order];
            UDPFRAME * frame2 = frame;

            calFEC(frameFec, frame1, frame2);
        }
        else
        {
            last_fec_order = udp_send_order;
        }

        ++udp_send_order;
        ++udp_send_index;

        if (maxSize == 0)
            break;

    }

    return len;

}

void aUDPSocket::calFEC( UDPFRAME * frameFec, UDPFRAME * frame1, UDPFRAME *frame2)
{
    bzero(frameFec, UDP_BUFSIZE);
    frameFec->head.flag = F_FEC;
    frameFec->head.connID = connectID;
    frameFec->head.orderID = udp_send_order;
    frameFec->head.dataID = (frame1->head.dataID / 2);

    int max = std::max(frame1->dataLen, frame2->dataLen);

    for (int i = 0; i < max; ++i)
    {
        frameFec->data[i] = frame1->data[i] ^ frame2->data[i];
    }

    frameFec->dataLen = frame1->dataLen ^ frame2->dataLen;

}

int aUDPSocket::sendFrame(UDPFRAME  * frame)
{
    int retcode = 0;
    int iSize  = 0;
    if (issetFlag(INCOMPLETE_WRITE))
    {
        clearFlag(INCOMPLETE_WRITE);
        goto do_udp_select;
    }

    iSize = frame->dataLen + UDP_HEADSIZE;
    _timestamp.now();
    if(frame->head.flag != F_ACK)
        frame->head.timestamp = _timestamp.msec();

    retcode = ::sendto(sock, (const void *)frame, iSize, 0, (struct sockaddr*)&addr, socklen);

    if (retcode == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
    {
do_udp_select:
        retcode = waitForWrite();
        if (1 == retcode)
        {
            retcode = ::sendto(sock, (const void *)frame, iSize, 0, (struct sockaddr*)&addr, socklen);
        }
        else
        {
            return retcode;
        }

    }

    if (retcode > 0 && retcode < frame->dataLen + UDP_HEADSIZE )
        setFlag(INCOMPLETE_WRITE);
    return retcode;
}

int aUDPSocket::udp_pack(DWORD dataID,const void * in, int iSize, UDPFRAME * frame)
{
    frame->head.flag = F_NORMAL;
    frame->head.connID = connectID;
    frame->head.orderID = 0;
    frame->head.dataID = dataID;
    frame->dataLen = iSize;
    frame->head.timestamp = 0;

    bcopy(in, frame->data, iSize);

    return UDP_HEADSIZE + iSize;
}


int aUDPSocket::udp_unpack(unsigned char * in, int iSize, unsigned char * out)
{

    UDPFRAME * frame = (UDPFRAME *)in;

    if(frame->head.connID != this->connectID)
	return 0;
    switch (frame->head.flag)
    {
        case F_NORMAL:
            {
                if(CheckRecvState(frame->head.dataID))//重复的不需要处理
                    break;

                int buf_index = frame->head.dataID % rdBufCount;
                bcopy(in, &udp_recved_buff[buf_index], iSize);
                SetCheckState(frame->head.dataID);
                int ret = refreshRecvBuf(out);
                sendAck( frame->head.orderID,frame->head.timestamp );

                int  diff = (int)frame->head.dataID - (int)udp_recv_max ;
                if(diff >= 0)
                {
                    udp_recv_max = frame->head.dataID;
                    winSize = rdBufCount -  ((int) udp_recv_max - (int)udp_recv_index ) - 1;//更新窗口大小
                }


                return ret;

            }break;
        case F_FEC:
            {
                int index = frame->head.dataID % fecBufCount;
                bcopy(in, udp_fec_buff[index], iSize);
                if(fecRecover())
                {
                    int  diff = (int(frame->head.dataID * 2 + 1)) - (int)udp_recv_max ;
                    int ret = refreshRecvBuf(out);
                    sendAck( frame->head.orderID,frame->head.timestamp );
                    if(diff >= 0)
                    {
                        udp_recv_max = frame->head.dataID;
                        winSize = rdBufCount -  ((int) udp_recv_max - (int)udp_recv_index ) - 1;//更新窗口大小
                    }
                    return ret;
                }
            }break;
        case F_ACK:
            {
                if(frame->head.dataID >= udp_send_check_index)
                {

                    udp_send_check_index = frame->head.dataID;
                    bcopy(frame->data,udp_send_check_stat , CHECK_LEN *  checkCount  );
                    _timestamp.now();
                    DWORD mrtt =_timestamp.msec()-frame->head.timestamp;
                    calcRTTRTO(mrtt);
                }

                return 0;
            }break;

        default:
            break;
    }

    return 0;

}

int aUDPSocket::refreshRecvBuf(unsigned char *out)
{
    int ret = 0;

    for (DWORD i = 0; i < rdBufCount; ++i)////½²Êý¾ÝË¢ÐÂµ½buffer
    {
        DWORD  buf_index = udp_recv_index % rdBufCount;

        UDPFRAME * frame = (UDPFRAME * )udp_recved_buff[buf_index];
        if (frame->head.flag != F_NONE)
        {
            if (ret + frame->dataLen > (int)MAX_DATASIZE)
                break;

            bcopy(frame->data, &out[ret], frame->dataLen);

            ret += frame->dataLen;

            udp_recv_index++;
            ShiftCheckState();
            bzero(frame, frame->dataLen + UDP_HEADSIZE);
        }

    }

    return ret;

}

bool aUDPSocket::fecRecover()
{
    bool recover = false;
    for (DWORD  i = 0; i < fecBufCount; ++i)
    {
        UDPFRAME * frame = (UDPFRAME *)udp_fec_buff[i];

        if (frame->head.flag !=  F_NONE)
        {
            int index = frame->head.dataID * 2;

            bool hasFirst  = CheckRecvState(index);
            bool hasSecond = CheckRecvState(index + 1);
            if ( (hasFirst == false && hasSecond == true ) || ( hasFirst == true && hasSecond == false  ) )
            {
                recover = true;
                unsigned int index_miss =  hasFirst ? index+1 : index;
                unsigned int index_exist = hasFirst ?  index : index + 1;

                index = hasFirst ? index + 1 : index ;

                UDPFRAME * frameiss = (UDPFRAME *)udp_recved_buff[index_miss % rdBufCount];
                UDPFRAME * frameExist = (UDPFRAME *)udp_recved_buff[index_exist % rdBufCount];
                frameiss->head.flag = F_NORMAL;
                frameiss->head.connID = connectID;
                frameiss->head.orderID = 0;
                frameiss->head.dataID = index;
                frameiss->dataLen = frameExist->dataLen ^ frame->dataLen;

                for (int i =  0; i < frameiss->dataLen; ++i)
                {
                    frameiss->data[i] = frame->data[i] ^ frameExist->data[i];
                }
                SetCheckState(index_miss);
                bzero(udp_fec_buff, UDP_BUFSIZE);
            }

        }
    }

    return recover;
}

void aUDPSocket::sendAck(DWORD orderID,QWORD dwTime)
{

    int buf_count = wrBufCount - ((int)udp_send_order - (int) udp_sendok_order );

    if(buf_count <= 0)
        return;//如果没有足够的空间 就不再发送ack

    UDPFRAME * frame = (UDPFRAME *)udp_send_buff[udp_send_order];

    frame->head.flag = F_ACK;
    frame->head.connID = connectID;
    frame->head.orderID = orderID;
    frame->head.dataID = udp_recv_index;
    frame->head.timestamp = dwTime;
    frame->dataLen = checkCount * CHECK_LEN;
    bcopy(udp_recv_check_stat,frame->data,frame->dataLen);

    udp_send_order++;

}

void aUDPSocket::calcRTTRTO( DWORD mrtt )
{
    long m = mrtt;
    if(m == 0)
        m = 1;

    if(srtt != 0)
    {
        m -= srtt >> 3;
        srtt += m;

        if(m < 0)
        {
            m = -m;
            m -= (mdev>>2);

            if(m > 0)
                m >>= 3;

        }else
        {
            m -= (mdev>> 2);

        }

        mdev += m;

        if(mdev > mdev_max)
        {
            mdev_max = mdev;
            if(mdev_max > rttvar)
                rttvar = mdev_max;
        }

    }
    else
    {

        srtt = m << 3;
        mdev = m << 1;
        mdev_max = rttvar = mdev >  MIN_RTO ?  mdev : MIN_RTO;

    }

    rtovar  =  srtt +  (mdev <<  2);
}

void aUDPSocket::checkResend()
{
    _timestamp.now();

    int sept = (int)udp_sended_order - (int)udp_sendok_order  ;

    if(sept <= 0 )
        return;

    bool  increase = true;

    for(int i = 0 ; i <= sept; ++i)
    {
        int frameID = (udp_sendok_order + i) % wrBufCount;

        UDPFRAME *  frame_buf = (UDPFRAME *)udp_send_buff[frameID];

        if(frame_buf->head.flag == F_NORMAL)
        {
            if(CheckSendState(frame_buf->head.dataID ))
            {
                bzero(frame_buf, UDP_BUFSIZE);
            }
            else
            {
                //rto 重传
                int buf_count = wrBufCount - ((int)udp_send_order - (int) udp_sendok_order);
                if(buf_count <= 0)
                    return;//如果没有足够的空间 就不在重传


                DWORD  mrtt =_timestamp.msec()-frame_buf->head.timestamp;
                if(mrtt > rtovar && rtovar != 0)
                {
                    UDPFRAME * newFrame = (UDPFRAME *)(udp_send_buff[udp_send_order% wrBufCount]);
                    bcopy(frame_buf, newFrame, UDP_BUFSIZE);
                    newFrame->head.orderID = udp_send_order;
                    bzero(frame_buf, UDP_BUFSIZE);
                    udp_send_order ++;
                }
                else
                {
                    increase = false;
                }

            }

        }
        else if(frame_buf->head.flag != F_NONE)
        {
            bzero(frame_buf, UDP_BUFSIZE);
        }

        if(increase)
            udp_sendok_order ++;
    }

}

UDPFRAME * aUDPSocket::getFrame( DWORD index)
{
    for (DWORD i = 0; i < wrBufCount; ++i)
    {
        DWORD dwIndex = (udp_recv_index + i) %  wrBufCount;
        UDPFRAME * frame = (UDPFRAME *)udp_recved_buff[dwIndex];
        if (frame->head.dataID ==index)
        {
            return frame;
        }
    }

    return NULL;
}


bool aUDPSocket::sendCmd(const void * pstrCmd, const int iCmd)
{
    if (NULL == pstrCmd || iCmd <= 0)
        return false;

    mutex.lock();
    stStackCmdQueue _raw_queue;
    packetPack(pstrCmd,iCmd,_raw_queue);
    _snd_queue.wrReserve(MAX_DATABUFFERSIZE);
    _snd_queue.put(_raw_queue.rdBuf(),_raw_queue.rdSize());
    mutex.unlock();

    return true;
}

bool aUDPSocket::sendCmd(const std::string & send)
{
    if (send.empty())
        return false;
    return sendCmd(send.c_str(), send.size());
}

bool aUDPSocket::sync()
{
    mutex.lock();

    int index = udp_sended_order;
    checkResend();

    if(_snd_queue.rdReady())
    {
        int retcode = sendRawData(_snd_queue.rdBuf(),_snd_queue.rdSize());
        _snd_queue.rdFlip(retcode);

    }

    for (DWORD i = index; i < udp_send_order;++i)
    {
        DWORD dwIndex = i % wrBufCount;
        UDPFRAME * frame = ( UDPFRAME * )udp_send_buff[dwIndex];
        sendFrame(frame);

        udp_sended_order = i + 1;
    }
    mutex.unlock();
    return true;
}

int aUDPSocket::checkIOForRead()
{
    struct pollfd pfd;

    pfd.fd = sock;
    pfd.events = POLLIN | POLLERR | POLLPRI;
    pfd.revents = 0;

    int recode = TEMP_FAILURE_RETRY(::poll(&pfd, 1, 0));
    if (recode > 0 && 0 == (pfd.revents & POLLIN))
        recode = -1;
    return recode;
}
int aUDPSocket::checkIOForWrite()
{
    struct pollfd pfd;

    pfd.fd = sock;
    pfd.events = POLLOUT | POLLERR | POLLPRI;
    pfd.revents = 0;

    int recode = TEMP_FAILURE_RETRY(::poll(&pfd, 1, 0));
    if (recode > 0 && 0 == (pfd.revents & POLLOUT))
        recode = -1;
    return recode;
}


const char * aUDPSocket::getIPByIfName(const char * ifName)
{
    int s;

    struct ifreq ifr;

    static const char * none_ip = "0.0.0.0";

    if (NULL == ifName)
        return none_ip;
    s = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == s)
        return none_ip;
    bzero(ifr.ifr_name, sizeof(ifr.ifr_name));
    strncpy(ifr.ifr_name, ifName, sizeof(ifr.ifr_name) - 1);

    if (-1 == ioctl(s, SIOCGIFADDR, &ifr))
    {
        TEMP_FAILURE_RETRY(::close(s));
        return none_ip;
    }
    TEMP_FAILURE_RETRY(::close(s));
    return inet_ntoa(((struct sockaddr_in *) & ifr.ifr_addr)->sin_addr);
}


#define success_recv()\
    do { \
        if ((unsigned int) retcode < _rcv_queue.wrSize())\
        {\
            setFlag(INCOMPLETE_READ);\
        }\
        _rcv_queue.wrFlip(retcode);\
        _rcv_raw_size += retcode;\
    }while(0)



int aUDPSocket::recvToBufNoPoll()
{
    int retcode = 0;

    _rcv_queue.wrReserve(MAX_DATABUFFERSIZE);

    bzero(_udp_recv_tmp_queue, UDP_BUFSIZE);
    retcode = ::recvfrom(sock, _udp_recv_tmp_queue, UDP_BUFSIZE, 0, (struct sockaddr*)&addr, &socklen);

    if (retcode == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        return 0;

    if (0 == retcode)
        return -1;


    retcode = udp_unpack(_udp_recv_tmp_queue, retcode, _rcv_queue.wrBuf());

    if (retcode > 0)
    {
        success_recv();

    }
    else
    {
        return 1;//ÕâÀï·µ»Ø1 ÊÇÎªÁË²»³öÏÖ¶ÁÈ¡´íÎó ²»ÊÇ·µ»Ø1×Ö½Ú len = 0 ÓÐ¿ÉÄÜÔÚµÈ´ýÕýÈ·µÄÐ­ÒéÐòºÅ³öÏÖ
    }

    return retcode;


}

int aUDPSocket::recvToCmdNoPoll(void * pstrCmd, const int iCmdLen)
{
    success_unpack();
    return 0;
}
/**
 * @brief
 *
 * @param pstrCmd
 * @param iCmdLen
 *
 * @return
 */
int aUDPSocket::recvToNoPackNoPoll(void * pstrCmd, const unsigned int iCmdLen)
{
    DWORD iRecvCmdLen = _rcv_queue.rdSize();
    if (iCmdLen < iRecvCmdLen)
        return -1;
    bcopy(_rcv_queue.rdBuf(), pstrCmd, iRecvCmdLen);
    _rcv_queue.rdFlip(iRecvCmdLen);
    _rcv_raw_size -= iRecvCmdLen;

    return iRecvCmdLen;
}
int aUDPSocket::sendRawDataNoPoll(const void * pBuffer, const int iSize)
{
    int retcode = TEMP_FAILURE_RETRY(::send(sock, pBuffer, iSize, MSG_NOSIGNAL));
    if (retcode == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        return 0;
    if (retcode > 0 && retcode < iSize)
        setFlag(INCOMPLETE_WRITE);
    return retcode;
}


int aUDPSocket::waitForRead()
{
    struct pollfd pfd;

    pfd.fd = sock;
    pfd.events = POLLIN | POLLERR | POLLPRI;
    pfd.revents = 0;

    int recode = TEMP_FAILURE_RETRY(::poll(&pfd, 1, rd_msec));
    if (recode > 0 && 0 == (pfd.revents & POLLIN))
        recode = -1;
    return recode;

}
int aUDPSocket::waitForWrite()
{
    struct pollfd pfd;

    pfd.fd = sock;
    pfd.events = POLLOUT | POLLERR | POLLPRI;
    pfd.revents = 0;

    int recode = TEMP_FAILURE_RETRY(::poll(&pfd, 1, wr_msec));
    if (recode > 0 && 0 == (pfd.revents & POLLOUT))
        recode = -1;
    return recode;
}

unsigned int aUDPSocket::packetUnPack(unsigned char * in, const unsigned int iPacketLen, unsigned char * out)
{
    unsigned int iRecvCmdLen = iPacketLen - PH_LEN;
    bcopy(&(in[PH_LEN]), out, iRecvCmdLen);
    return iRecvCmdLen;
}
