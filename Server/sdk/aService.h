#ifndef _aService_H__
#define _aService_H__

#pragma once

/** 
* @file aService.h
* @brief  服务器回调结构封装
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-14
*/
#include "Age.h"
#include "aNoneCopyable.h"
#include <string>
class aService : private aNoneCopyable
{
	public:

		/** 
		 * @brief the default extract.
		 */

		virtual ~aService(){ 
			//_instance=NULL;
		}

		/** 
		 * @brief whether the service is terminate.
		 * 
		 * @return value  if return true the serive is terminate.
		 */

		bool isTerminate()const;

		/** 
		 * @brief to make the service stop
		 */

		void Terminate();

		/** 
		 * @brief the service main loop
		 */

		void main();

		/** 
		 * @brief get the class instance.
		 * 
		 * @return aService * 
		 */

		static aService * getInstance()
		{
			return _instance;
		}

		/** 
		 * @brief reload the config the file.
		 */

		virtual void reloadConfig(){};
		/** 
		 * @brief user define kill sig
		 */

		virtual void userKill1()
		{
			Age::logger->info("user kill sig SIGUSR1");
		}
		/** 
		 * @brief user define kill sig
		 */

		virtual void userKill2()
		{
			Age::logger->info("user kill sig SIGUSR2");
		}

		/** 
		* @brief return the servie name;
		*/
		const char * getServerName();

		/**
		 * @brief 解析来自管理器的信息 
		 *
		 * @param cmd cmd
		 * @param iLen cmd的长度
		 * @return true 处理成功 false 处理失败
		 * 
		 **/
		virtual bool msgParseGlobal(const char *msg , const int iLen)
		{
			return false;
		};

		virtual void  setIP(const char * ip) {};

		/** 
		 * @brief 获取 内网ip
		 * 
		 * @return ip 地址
		 */
		virtual const char *  getIP() { return NULL;};
		/** 
		 * @brief 设置对外IP
		 * 
		 * @param ip ip地址
		 */

		virtual void  setExtIP(const char *ip){};
		/** 
		 * @brief 获取对外IP
		 * 
		 * @return ip ip地址
		 */

		virtual const char *  getExtIP(){ return NULL;};
		/** 
		 * @brief 设置端口
		 * 
		 * @param port 端口
		 */

		virtual void  setPort(DWORD port){};

		/** 
		 * @brief 获取端口
		 * 
		 * @return port 端口
		 */

		virtual DWORD  getPort(){ return 0;};
		/** 
		 * @brief 设置对外端口
		 * 
		 * @param port 端口
		 */

		virtual void  setExtPort(DWORD port) {};

		/** 
		 * @brief 获取对外端口
		 * 
		 * @return port 端口
		 */

		virtual DWORD getExtPort() {return 0;};
		/** 
		 * @brief 设置服务框架类型
		 * 
		 * @param type 类型 
		 */

		virtual void  setType(DWORD type) {};

		/** 
		 * @brief 获取服务框架类型
		 * 
		 * @param type 类型 
		 */

		virtual DWORD  getType() { return 0;};

		/** 
		 * @brief 设置服务器ID
		 * 
		 * @param id 服务器id
		 */
 
		virtual void  setServerID(DWORD id) { };

		/** 
		 * @brief 获取服务器ID
		 * 
		 * @return id 服务器id
		 */

		virtual DWORD  getServerID() { return 0;};

		/** 
		 * @brief 设置服务器网络类型
		 * 
		 * @pram netType 网络类型
		 */

		virtual void setNetType(DWORD netType) {};

		/** 
		 * @brief 获取服务器网络类型
		 * 
		 * @return 服务器网络类型
		 */

		virtual const DWORD getNetType(){return 0;};


		/** 
		 * @brief 初始化
		 * 
		 * @param port 端口
		 * 
		 * @return 成功 true 失败 false 
		 */
		virtual bool dealDepend() { return false;};



	protected:

		/**
		 *
		 * @brief the default constructor 
		 *
		 */
		/** 
		 * @brief  the default constructor 
		 * 
		 * @param name server name 
		 */

		aService(const std::string & name);

		/** 
		 * @brief the initialization function 
		 * 
		 * @return true or false 
		 */

		virtual bool init();

		/** 
		 * @brief whether the serice is start sucessful.
		 * 
		 * @return true or false 
		 */

		virtual bool validate()
		{
			return true;
		}

		/** 
		 * @brief the callback function of service.
		 * 
		 * @return true or false 
		 */

		virtual bool serviceCallback() = 0;

		/** 
		 * @brief  the final function of service.
		 */

		virtual void final() = 0;

	private:
		/** 
		 * @brief the instace of the service
		 */

		static aService *_instance;
		/** 
		 * @brief the stat of the service
		 */

		bool _terminate; 
		/** 
		 * @brief  the service name 
		 */
		const char *  name;
};

#endif

