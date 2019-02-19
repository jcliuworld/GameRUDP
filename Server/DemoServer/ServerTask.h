#ifndef _serverTask_H__
#define _serverTask_H__
/**
 * @ServerTask.h
 * @brief 任务对象
 * @author ljc jc.liuworld@hotmail.com
 * @date   2011-3-11
 */
#include "UDPTaskProtoBuf.h"
#include "aMsgCallBack.h"
#include "DemoUser.h"
class ServerTask : public UDPTaskProtoBuf
{

public :
	/**
	 * @brief 构造函数
	 *
	 * @param sock 套接口句柄
	 * @param addr 套接口地址
	 * @return none
	 */

	ServerTask(const int sock,const struct sockaddr_in *addr ,DWORD connectID):UDPTaskProtoBuf(sock,addr,connectID)
	{
		_versionCheck = false;

		_pUser = NULL;


	}

	/**
	 * @brief 析构函数
	 */
	virtual	~ServerTask(){};
	/**
	 * @brief 消息解析处理
	 *
	 * @param msg 消息指针
	 * @return true 处理成功 false 处理失败
	 */
	virtual bool msgParse(DWORD msgID,const MSG * msg);

	virtual bool msgQueueParse(DWORD msgID,const MSG * msg){return true;};

	/**
	 * @brief 获取任务ID
	 *
	 * @param
	 * @return 任务ID
	 */
	DWORD getID();
	
	/**
	 * @brief 连接验证
	 *
	 * @param
	 * @return 1 成功 0 继续 -1 失败
	 */
	int verifyConn();
	/**
	 * @brief 同步验证
	 *
	 * @param
	 * @return 1 成功 0 继续 -1 失败
	 */
	int waitSync();
	/**
	 * @brief 回收连接
	 *
	 * @param
	 * @return 1成功 0失败
	 */
	int recycleConn();
	/**
	 * @brief 加入管理容器
	 *
	 * @param
	 * @return void
	 */
	void addToContainer();
	/**
	 * @brief 从管理器重移除
	 *
	 * @param
	 * @return void
	 */
	void removeFromContainer();
	/**
	 * @brief 添加到唯一性容器
	 *
	 * @param
	 * @return true 成功 false 失败
	 */
	bool uniqueAdd();
	/**
	 * @brief 从唯一性容器中移除
	 *
	 * @param
	 * @return true 成功 false 失败
	 */
	bool uniqueRemove();

private:

	/**
	 * @brief  任务名称
	 */
	char strName[MAX_NAMESIZE];
	/**
	 * @brief 任务内网地址
	 */
	char strIP[MAX_IPLENGTH];
	/**
	 * @brief 任务内网端口
	 */
	DWORD dwPort;
	/**
	 * @brief  任务外网地址
	 */
	char strExtIP[MAX_IPLENGTH];
	/**
	 * @brief 任务外网端口
	 */
	DWORD dwExtPort;

	/**
	 * @brief 版本验证是否ok
	 */
	bool _versionCheck;
	/**
	 * @brief user
	 */
	DemoUser * _pUser;

};

#endif


