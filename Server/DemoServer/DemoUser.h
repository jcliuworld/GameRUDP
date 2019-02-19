#ifndef _DemoUser_H__
#define _DemoUser_H__
/**
 * @DemoUser.h
 * @brief 网关用户
 * @author ljc jc.liuworld@hotmail.com
 * @date   2011-3-11
 */
#include "aUser.h"
#include "protocol.h"
#include "ProtoFun.h"
class ServerTask;

class DemoUser:public aUser
{
	public:
	/**
	 * @brief  构造函数
	 *
	 * @param task 对应的任务对象指针
	 * @return none
	 */
	DemoUser(ServerTask * task)
	{
		_thetask = task;
		_dwAccID = 0;
		_dwGameID = 0;
		_dwPlatID = 0;
	}

	/**
	 * @brief 用户注册 这个时候用户会拥有id tempid 及name
	 *
	 * @param
	 * @return true 成功 false 失败
	 */
	bool reg();

	/**
	 * @brief 用户卸载 不等同于结束  如用户退回到选择角色上
	 *
	 * @param
	 * @return true 成功 false 失败
	 */
	bool unreg();

	/**
	 * @brief 用户等待结束 这次用户是真的结束了 task 会调用 final函数
	 *
	 * @param
	 * @return true 成功 false 失败
	 */
	bool terminateWait();
	/**
	 * @brief 结束函数
	 *
	 * @param
	 * @return true 成功 false 失败
	 */
	bool final();

	/**
	 * @brief  用户accid
	 */
	DWORD _dwAccID;
/**
	 * @brief 发送消息
	 *
	 * @param msg
	 *
	 * @return
	 */
	bool sendCmd(const MSG * msg);

	/**
	 * @brief 发送消息
	 *
	 * @param msg
	 *
	 * @return
	 */
	bool sendCmd(std::string & msg);

	/**
	 * @brief 所在逻辑ID
	 */
	DWORD _dwGameID;
	DWORD _dwPlatID;

private:

	/**
	 * @brief  任务对象实例
	 */

	ServerTask * _thetask;
};
#endif
