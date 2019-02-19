#include "aUserM.h"

/**
* @brief  根据用户ID获取用户对象
*
* @param id 用户ID
* @return 用户指针
*/
aUser *aUserM::getUserByID(const DWORD id)
{
	_rwlock.rdlock();
	aUser *ret = (aUser *)getEntryByID(id);
	_rwlock.unlock();
	return ret;
}

/**
* @brief 根据用户临时ID获取用户对象
*
* @param tempid 用户临时对象
* @return 用户指针
*/

aUser * aUserM::getUserByTempID(const DWORD tempid)
{
	_rwlock.rdlock();
	aUser * ret = (aUser *)getEntryByTempID(tempid);
	_rwlock.unlock();
	return ret;
}
/**
* @brief 根据用户名获取用户对象 
*
* @param name 用户名
* @return 用户指针
*/
aUser * aUserM::getUserByName(const char *name)
{
	_rwlock.rdlock();
	aUser * ret = (aUser *)getEntryByName(name);
	_rwlock.unlock();
	return ret;
}
/**
* @brief 添加一个用户 
*
* @param user 用户指针
* @return true 成功 false 失败
*/
bool aUserM::addUser(aUser *user)
{
	_rwlock.wrlock();
	bool ret = addEntry((aEntry *)user);
	_rwlock.unlock();
	return ret;
}
/**
* @brief 移除一个用户 
*
* @param user 用户指针
* @return void
*/
void aUserM::removeUser(aUser *user)
{
	_rwlock.wrlock();
	removeEntry((aEntry *)user);
	_rwlock.unlock();
}
/**
 * @brief 获取用户数量 
 * 
 * @return 用户数量
 *
 */

unsigned int aUserM::getSize() 
{
	_rwlock.wrlock();
	unsigned int ret = size();	
	_rwlock.unlock();
	return ret;
}
