#include "Age.h"
#include "protocol.h"
#include "DemoServer.h"
#include "ServerTask.h"
#include "TaskManager.h"
#include "DemoUserM.h"
#include "DemoUser.h"


/**
* @brief 消息解析处理
*
* @param msg 消息指针
* @return true 处理成功 false 处理失败
*/
bool ServerTask::msgParse(DWORD msgID,const MSG * msg)
{
	//截获消息 可以在这里处理
	//const AGE_MSGMAP * msgMap = getMessageMap();

	std::string type_name = msg->GetDescriptor()->full_name();
	Age::logger->debug("DemoServer/ServerTask::msgParse 收到客户端消息[%s]", type_name.c_str());

		return true;
}
/**
* @brief 获取任务ID
*
* @param
* @return 任务ID
*/
DWORD ServerTask::getID()
{
	return _socket.getConnectID() ;
}
/**
* @brief 连接验证
*
* @param
* @return 1 成功 0 继续 -1 失败
*/
int ServerTask::verifyConn()
{
    return 1;
}
/**
* @brief 同步验证
*
* @param
* @return 1 成功 0 继续 -1 失败
*/
int ServerTask::waitSync()
{
	return 1;
}
/**
* @brief 回收连接
*
* @param
* @return 1成功 0失败
*/
int ServerTask::recycleConn()
{
	return 1;
}
/**
* @brief 加入管理容器
*
* @param
* @return void
*/
void ServerTask::addToContainer()
{
	DemoUser * pUser = new DemoUser(this);
	if (pUser)
	{
		pUser->_dwAccID = _socket.getConnectID();

		DemoUser *pTempUser = DemoUserM::getInstance().getUserByAccid(pUser->_dwAccID);
		if(pTempUser != NULL)
		{
			Demo::msgRetcodeRsp send;
			send.set_dwret(0);
			pTempUser->sendCmd(&send);
			//这里只能先下线一个玩家
			pTempUser->terminateWait();
			Age::logger->trace("[下线]同网关重复用户登陆 %u",pTempUser->_dwAccID);

			Demo::msgRetcodeRsp sendto;
			send.set_dwret(0);
			//sendCmd(&sendto);
			this->TerminateWait();
			SAFE_DELETE(pUser);

			Age::logger->trace("重复用户下线 %d",_socket.getConnectID());
			return ;
		}

		bool ret = DemoUserM::getInstance().addUserOnlyByAccid(pUser);

		if(!ret)
		{
			this->TerminateWait();
			SAFE_DELETE(pUser);
			Age::logger->trace("发现重复用户 %d",_socket.getConnectID());
			return ;
		}
		_pUser = pUser;

		_pUser->reg();

		Age::logger->trace("用户%d 正在登陆",_socket.getConnectID());
	}
	TaskManager::getInstance().addTask(this);
}
/**
* @brief 从管理器重移除
*
* @param
* @return void
*/
void ServerTask::removeFromContainer()
{
	if(_pUser)
	{
		_pUser->unreg();
		_pUser->final();
		SAFE_DELETE(_pUser);
	}
	TaskManager::getInstance().removeTask(this);
}
/**
* @brief 添加到唯一性容器
*
* @param
* @return true 成功 false 失败
*/
bool ServerTask::uniqueAdd()
{
	return TaskManager::getInstance().uniqueAdd(this);
}
/**
* @brief 从唯一性容器中移除
*
* @param
* @return true 成功 false 失败
*/
bool ServerTask::uniqueRemove()
{
	return TaskManager::getInstance().uniqueRemove(this);
}

