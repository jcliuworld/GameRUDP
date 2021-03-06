#ifndef _aUDPNetService_H__
#define _aUDPNetService_H__

/** 
* @file aUDPNetService.h
* @brief  UDP网络服务器框架类
* @author ljc jc.liuworld@hotmail.com
* @date 2017-11-01
*/

#include "aNoneCopyable.h"
#include "aUDPServer.h"
#include "aService.h"
#include "aUDPTask.h"
#include "Age.h"
/** 
* @brief 网络服务器框架类
*/

class aUDPNetService : public aService
{
	public:
		/** 
		 * @brief  析构函数
		 */

		virtual ~aUDPNetService();
		/** 
		 * @brief 新建一个TCP 任务框架
		 * 
		 * @param sock 
		 * @param addr 
		 */

		virtual void newUDPTask(const int sock,const struct sockaddr_in * addr,DWORD connectID){};
		/** 
		 * @brief 获连接池取池大小
		 * 
		 * @return 连接池大小
		 */

		virtual const int getPoolSize()const
		{
			return 0;
		}
		/** 
		 * @brief 获取连接池状态
		 * 
		 * @return 连接池状态  
		 */

		virtual const int getPollState()const
		{
			return 0;
		}
		/** 
		 * @brief 设置 ip
		 * 
		 * @param ip 地址
		 */

		 void  setIP(const char * ip);

		 /** 
		 * @brief 获取 内网ip
		 * 
		 * @return ip 地址
		 */
		 const char *  getIP();
		/** 
		 * @brief 设置对外IP
		 * 
		 * @param ip ip地址
		 */

		 void  setExtIP(const char *ip);
		/** 
		 * @brief 获取对外IP
		 * 
		 * @return ip ip地址
		 */

		 const char *  getExtIP();
		 /** 
		 * @brief 设置端口
		 * 
		 * @param port 端口
		 */

		 void  setPort(DWORD port);

		 /** 
		 * @brief 获取端口
		 * 
		 * @return port 端口
		 */

		 DWORD  getPort();
		/** 
		 * @brief 设置对外端口
		 * 
		 * @param port 端口
		 */

		 void  setExtPort(DWORD port);

		 /** 
		 * @brief 获取对外端口
		 * 
		 * @return port 端口
		 */

		 DWORD getExtPort();

		/** 
		 * @brief 设置服务框架类型
		 * 
		 * @param type 类型 
		 */

		 void  setType(DWORD type);

		 /** 
		 * @brief 获取服务框架类型
		 * 
		 * @param type 类型 
		 */

		 DWORD  getType();

		/** 
		 * @brief 设置服务器ID
		 * 
		 * @param id 服务器id
		 */

		void  setServerID(DWORD id);

		/** 
		* @brief 获取服务器ID
		* 
		* @return id 服务器id
		*/

		DWORD  getServerID();

		/** 
		* @brief 设置服务器网络类型
		* 
		* @pram netType 网络类型
		*/

		void setNetType(DWORD netType);

		/** 
		* @brief 获取服务器网络类型
		* 
		* @return 服务器网络类型
		*/

		const DWORD getNetType();


		/**
		 * @brief 解析来自管理器的信息 
		 *
		 * @param cmd cmd
		 * @param iLen cmd的长度
		 * @return true 处理成功 false 处理失败
		 */
		virtual bool msgParseGlobal(const char *msg , const int iLen) 
		{
			return false;
		};
		/** 
		 * @brief 初始化
		 * 
		 * @param port 端口
		 * 
		 * @return 成功 true 失败 false 
		 */

		bool init(unsigned short port);
		/** 
		 * @brief 初始化
		 * 
		 * @param port 端口
		 * 
		 * @return 成功 true 失败 false 
		 */
		virtual bool dealDepend() = 0;

	protected:
		/** 
		 * @brief 构造函数
		 * 
		 * @param name 服务器名 
		 */

		aUDPNetService(const std::string & name,DWORD serverType);
			/** 
		 * @brief 服务器回调
		 * 
		 * @return 成功 true 失败 false
		 */

		bool serviceCallback();
		/** 
		 * @brief 终止
		 */

		void final();
	private:
		/** 
		 * @brief 对象实例 单价模式
		 */

		static aUDPNetService * _netinstance;
		/** 
		 * @brief tcp服务器封装
		 */

		aUDPServic * udpService;

		/** 
		* @brief 服务器类型
		*/

		DWORD _wdServerType;
		/** 
		* @brief 服务器ID
		*/

		DWORD _wdServerID;
		
		/** 
		* @brief 服务器内网地址
		*/
		char _pstrIP[MAX_IPLENGTH+1]
		/** 
		* @brief 服务器内网端口
		*/;
		DWORD _port;
		/** 
		* @brief 服务器外网地址
		*/
		char _pstrExtIP[MAX_IPLENGTH+1];
		/** 
		* @brief 服务器外网端口
		*/
		DWORD _extPort;

		/**
		* @brief 网络类型 0电信 1网通
		*
		*/
		DWORD _netType;
		
		std::string serviceName;

		DWORD _uniqueID;
			
};
#endif


