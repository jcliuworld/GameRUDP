#include "DemoUserM.h"
#include "DemoServer.h"
DemoUserM * DemoUserM::_instance = NULL;

/**
 * @brief 获取网关用户对象管理器实例 单件模式
 *
 * @param
 * @return 管理器实例
 */
DemoUserM & DemoUserM::getInstance()
{
	if (_instance == NULL)
	{
		_instance = new DemoUserM();
	}
	return *_instance;
}
/**
* @brief 初始化函数
*
* @param
* @return true 成功 false 失败
*/
bool DemoUserM::init()
{
	if (!_inited)
	{
		DWORD firstID = 1000 + (DemoServer::getInstance().getServerID()%100)*5000;
		_uniqueID = new aUNIQUEIDDWORD(firstID,firstID+4999);
		_inited = true;
	}
	return _inited;
}
/**
* @brief 获取唯一性ID
*
* @param id 返回的唯一性ID
* @return true 成功 false 失败
*/
bool DemoUserM::getUniqueID(DWORD & id)
{
	if (_uniqueID)
	{
		id = _uniqueID->get();

		return (id != _uniqueID->invalid());
	}

	return false;
}
/**
* @brief 回收一个唯一性ID
*
* @param id 要回收的ID
* @return void
*/
void DemoUserM::putUniqueID(DWORD &tempID)
{
	if (_uniqueID)
	{
		_uniqueID->put(tempID);
	}
}
/**
* @brief 根据用户accid 获取网关用户对象
*
* @param accid 用户accid
* @return 网关用户指针
*/
DemoUser * DemoUserM::getUserByAccid(DWORD accid)
{
	_rwlock.rdlock();
	DemoUser * ret = NULL;
	DemoUserAccid::find(accid,ret);
	_rwlock.unlock();
	return ret;
}
/**
* @brief 向容器中添加一个网关用户
*
* @param e 网关用户
* @return true 成功 false 失败
*/
bool DemoUserM::addUserOnlyByAccid(DemoUser * user)
{
	_rwlock.wrlock();
	bool ret = DemoUserAccid::push(user);
	_rwlock.unlock();
	return ret;
}
/**
* @brief 从容器中移除一个网关用户
*
* @param accid 要移除用户的accid
* @return void
*/
void DemoUserM::removeUserOnlyByAccid(DWORD accid)
{
	_rwlock.wrlock();
	DemoUserAccid::remove(accid);
	_rwlock.unlock();
}
/**
* @brief 移除所有用户
*/
void DemoUserM::removeAllUser()
{
	struct stRemvoe : execEntry<DemoUser>
	{
		std::vector<DWORD> _removeVec;

		stRemvoe()
		{

		}
		bool exec(DemoUser * entry)
		{
				_removeVec.push_back(entry->id);
				return true;
		}
	};

	stRemvoe exec;
	execEveryEntry(exec);

	for (std::vector<DWORD>::iterator it = exec._removeVec.begin(); it != exec._removeVec.end();++it)
	{
		DemoUser * pUser = (DemoUser *)getUserByID(*it);
		if (pUser)
		{
			pUser->terminateWait();
		}
	}
}


