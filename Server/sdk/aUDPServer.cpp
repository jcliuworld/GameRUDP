#include "aUDPServer.h"
#include "Age.h"
#include "sdkL.h"
#include "aUDPSocket.h"
/**
 * @file aUDPServic.cpp
 * @brief UDP 服务框架类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2017-11-1
 */


/**
 * @brief 构造函数
 *
 * @param name 服务器名字
 */

aUDPServic::aUDPServic(const std::string &name) : name(name),sock(-1)
{
	kdpfd = epoll_create(1);
	assert(-1 != kdpfd);
}
/**
 * @brief 析构函数
 */
aUDPServic::~aUDPServic()
{
	TEMP_FAILURE_RETRY(::close(kdpfd));
	if (-1 == sock)
	{
		::shutdown(sock,SHUT_RD);
		TEMP_FAILURE_RETRY(::close(sock));
		sock = -1;
	}
}


/**
 * @brief 绑定一个端口
 *
 * @param name 名字
 * @param port 端口号
 *
 * @return
 */


bool aUDPServic::bind(const std::string &name, const unsigned short port)
{
	struct sockaddr_in addr;

	if (-1 != sock)
	{
		Age::logger->error("the serivice has been init");
		return false;
	}
	sock = ::socket(AF_INET,SOCK_DGRAM,0);

	if (-1 == sock)
	{
		Age::logger->error("create the socket fail");
		return false;
	}

	socklen_t window_size = 128 * 1024;

	if ( -1 == ::setsockopt(sock,SOL_SOCKET,SO_RCVBUF,&window_size,sizeof(window_size)))
	{
		Age::logger->error("can not set the socket receive buffer size ");
		TEMP_FAILURE_RETRY(::close(sock));
		sock = -1;
		return false;
	}

	if ( -1 == setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&window_size,sizeof(window_size)))
	{
		Age::logger->error("can not set the socket send buffer size");
		TEMP_FAILURE_RETRY(::close(sock));
		sock = -1;
		return false;
	}

	bzero(&addr,sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(port);

	int retcode = ::bind(sock,(struct sockaddr *)&addr,sizeof(addr));

	if ( -1 == retcode)
	{
		Age::logger->error("can not bind the prot %d",port);
		TEMP_FAILURE_RETRY(::close(sock));
		sock = -1;
		return false;
	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = NULL;
	assert( 0 == epoll_ctl(kdpfd,EPOLL_CTL_ADD,sock,&ev));

	Age::logger->info(SYS_SDK_ID_000032,name.c_str(),port);
	return true;

}

/**
 * @brief 接收一个套接口的连接
 *
 * @param addr 地址
 *
 * @return
 */


int aUDPServic::accept(struct sockaddr_in * addr,DWORD connectID)
{

	struct epoll_event ev;
	int rc = epoll_wait(kdpfd,&ev,1,T_MSEC);
	if( 1 == rc && (ev.events & EPOLLIN))
	{
		socklen_t len = sizeof(sockaddr_in);

		char buf[UDP_BUFSIZE] = {};
		int ret = recvfrom(sock, buf, UDP_BUFSIZE, 0, (struct sockaddr *)addr, &len);

		if ( ret == -1 )
		{
			Age::logger->error("udp recv error");
			return -1;
		}

		stUDPFrame * frame = (stUDPFrame * )buf;

		if(frame->head.flag != F_CONN)
		{

			Age::logger->error("socket flags error %s",__FUNCTION__);
			return -1;
		}

		if(strncmp((const char *)frame->data,(const char *)CONN_KEY,sizeof(CONN_KEY)) != 0)
		{
			Age::logger->error("socket connkey error %s",__FUNCTION__);
			return -1;
		}

		int newsock = -1;

		newsock = ::socket(AF_INET,SOCK_DGRAM,0);

		if (-1 == newsock)
		{
			Age::logger->error("create the socket fail %s",__FUNCTION__);
			return false;
		}

		socklen_t window_size = 128 * 1024;

		if ( -1 == ::setsockopt(newsock,SOL_SOCKET,SO_RCVBUF,&window_size,sizeof(window_size)))
		{
			Age::logger->error("can not set the socket receive buffer size ");
			TEMP_FAILURE_RETRY(::close(newsock));
			newsock = -1;
			return false;
		}

		if ( -1 == setsockopt(newsock,SOL_SOCKET,SO_SNDBUF,&window_size,sizeof(window_size)))
		{
			Age::logger->error("can not set the socket send buffer size");
			TEMP_FAILURE_RETRY(::close(newsock));
			newsock = -1;
			return false;
		}

		bzero(buf,UDP_BUFSIZE);

		
		frame->head.flag = F_CONN;
		frame->head.connID = connectID;	
		if ( ::sendto(newsock,buf,aUDPSocket::UDP_HEADSIZE,0,(struct sockaddr*)addr,len) == -1)
		{
			Age::logger->error("can not send msg to %s",inet_ntoa(addr->sin_addr));
			return -1;
		}

		Age::logger->info("new udp sock %s",inet_ntoa(addr->sin_addr));


		return newsock;
	}

	return -1;
}
