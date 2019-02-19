#ifndef _DemoServer_H__
#define _DemoServer_H__

#pragma once

/**
*
* @brief the for manager the all server releation.
*
* \author ljc jc.liuworld@hotmail.com
*
* \date 2009-08-16
*
*/

#include "aUDPNetServer.h"
#include "aDef.h"

#include "UDPTaskProtoBuf.h"
#include "protocol.h"
class aUDPTaskPool;
class GlobalProto;

class DemoServer :public aUDPNetService
{
public:
	/**
	 * @brief 初始化函数
	 *
	 * @param
	 * @return
	 */
	bool init();
	/**
	 * @brief 结束函数
	 *
	 * @param
	 * @return
	 */
	void final();

	/**
	 * @brief 析构函数
	 *
	 * @param
	 * @return
	 */
	~DemoServer(){};
	/**
	 * @brief  获取实例 本类为单件模式
	 *
	 * @param
	 * @return 本类实例
	 */
	static DemoServer & getInstance()
	{
		if (instance == NULL)
		{
			instance = new DemoServer();
		}
		return *instance;
	}

	/**
	 * @brief 删除实例
	 *
	 * @param
	 * @return
	 */
	static void delInstance()
	{
		SAFE_DELETE(instance);
	}

    void reloadConfig();

	/**
	 * @brief 获取任务连接池大小
	 *
	 * @param
	 * @return 连接池大小
	 */
	int getTaskPoolSize();

    /**
	 * @brief 是否启动完成
	 *
	 * @param
	 * @return
	 */
	bool  validate();

	/**
	 * @brief 获取当前用户数量
	 *
	 * @param
	 * @return 用户数量
	 */
	DWORD getNum();

	/**
	 * @brief 服务器版本号
	 */
	DWORD _version_check;

	/**
	 * @brief 重新连接全局服务器
	 */
	void reConnectGlobal();

    bool dealDepend(){ return 0; }

private:

	/**
	 * @brief 新建连接任务
	 *
	 * @param sock 套接口句柄
	 * @param addr 地址
	 * @return
	 */
	void newUDPTask(const int sock,const struct sockaddr_in *addr,DWORD connectID);

	/**
	 * @brief 构造函数
	 *
	 * @param
	 * @return
	 */
	DemoServer():aUDPNetService("DemoServer", UNKNOWSERVER )
	{
		_taskpool = NULL;
		_netType = 0;
	}


	/**
	 * @brief 网关服务实例
	 */
	static DemoServer * instance;

	/**
	 * @brief 任务连接池
	 */
	aUDPTaskPool * _taskpool;

	/**
	 * @brief 网络类型 0 电信 1网通
	 */
	WORD _netType;

	/**
	 * @brief 服务器版本号
	 */
	DWORD _dwVersion;

	/**
	 * @brief 大区ID
	 */
	DWORD _zoneID;
};


#endif


