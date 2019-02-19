#ifndef _aUDPService_H__
#define _aUDPService_H__

#pragma  once

/** 
 * @file aUDPServic.h
 * @brief UDP 服务框架类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2017-11-01
 */

#include "aDef.h"
#include "aNoneCopyable.h"
#include "aUDPSocket.h"

class aUDPServic : private aNoneCopyable
{
	public :
		/** 
		 * @brief 构造函数
		 * 
		 * @param name 服务器名字
		 */

		aUDPServic(const std::string &name);
		/** 
		 * @brief 析构函数
		 */

		~aUDPServic();

		/** 
		 * @brief 绑定一个端口
		 * 
		 * @param name 名字 
		 * @param port 端口号
		 * 
		 * @return 
		 */

		bool bind(const std::string &name, const unsigned short port);

		/** 
		 * @brief 接收一个套接口的连接
		 * 
		 * @param addr 地址
		 * 
		 * @return 
		 */

		int accept(struct sockaddr_in * addr,DWORD connectID);

	private:

		static const int T_MSEC = 2100;
		/** 
		 * @brief 等待最长队列数量
		 */

		static const int MAX_WAITQUEUE =2000;
		/** 
		 * @brief 名字
		 */

		std::string name;
		/** 
		 * @brief 套接口
		 */

		int sock;

		int kdpfd;
};

#endif


