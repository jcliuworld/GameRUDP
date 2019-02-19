#include "DemoUser.h"
#include "DemoUserM.h"
#include "ServerTask.h"
/**
* @brief 用户注册 这个时候用户会拥有id tempid 及name
*
* @param
* @return true 成功 false 失败
*/
bool DemoUser::reg()
{
	return true;
}

/**
* @brief 用户卸载 不等同于结束  如用户退回到选择角色上
*
* @param
* @return true 成功 false 失败
*/
bool DemoUser::unreg()
{
		return true;
}

/**
* @brief 用户等待结束 这次用户是真的结束了 task 会调用 final函数
*
* @param
* @return true 成功 false 失败
*/
bool DemoUser::terminateWait()
{
	if (_thetask != NULL)
	{
		_thetask->TerminateWait();
		return true;
	}
	return false;
}
/**
* @brief 结束函数
*
* @param
* @return true 成功 false 失败
*/
bool DemoUser::final()
{
	lock();

	if (tempid != 0)
	{
		DemoUserM::getInstance().removeUser(this);
	}


	DemoUserM::getInstance().removeUserOnlyByAccid(this->_dwAccID);
	unlock();
	return true;
}

/**
 * @brief 发送消息
 *
 * @param msg
 *
 * @return
 */
bool DemoUser::sendCmd(const MSG * msg)
{
	if(_thetask)
		return	_thetask->sendCmd(msg);
	return false;
}
/**
 * @brief 发送消息
 *
 * @param msg
 *
 * @return
 */
bool DemoUser::sendCmd(std::string & msg)
{
	if(_thetask)
		return	_thetask->aUDPTask::sendCmd(msg);
	return false;

}

