#ifndef _aUserM_H__
#define _aUserM_H__
#include "aUser.h"
#include "aEntryM.h"
#include "aRWLock.h"
/**
 * @aUserM.h
 * @brief 场景进入对象管理器
 * @author ljc jc.liuworld@hotmail.com
 * @date   2011-3-10
 */

class aUserM:public aEntryM<aEntryID,aEntryTempID,aEntryName>
{
protected:

	/**
	 * @brief 内置读写锁
	 */
	aRWLock _rwlock; 

public:
	/**
	 * @brief 构造函数 
	 */
	aUserM()
	{

	}
	/**
	 * @brief 析构函数
	 */
	virtual ~aUserM()
	{
		clear();
	}
	
	/**
	 * @brief  根据用户ID获取用户对象
	 *
	 * @param id 用户ID
	 * @return 用户指针
	 */
	aUser * getUserByID( const DWORD id);
	
	/**
	 * @brief 根据用户临时ID获取用户对象
	 *
	 * @param tempid 用户临时对象
	 * @return 用户指针
	 */
	aUser * getUserByTempID(const DWORD tempid);

	/**
	 * @brief 根据用户名获取用户对象 
	 *
	 * @param name 用户名
	 * @return 用户指针
	 */
	aUser * getUserByName(const char * name);

	/**
	 * @brief 添加一个用户 
	 *
	 * @param user 用户指针
	 * @return true 成功 false 失败
	 */
	bool addUser(aUser *user);

	/**
	 * @brief 移除一个用户 
	 *
	 * @param user 用户指针
	 * @return void
	 */
	void removeUser(aUser *user);

	/**
	 * @brief 获取用户数量 
	 *
	 * @return 用户数量
	 */

	unsigned int getSize();
	
	/**
	 * @brief 根据条件删除用户 
	 *
	 * @param pred 移除条件回调
	 * @return void
	 */
	template<class YOURUSERENTRY>
	void removeUser_if(removeEntry_Pred<YOURUSERENTRY> &pred)
	{
		_rwlock.wrlock();
		removeUser_if<>(pred);
		_rwlock.unlock();
	}
	/**
	 * @brief 用户回调 
	 *
	 * @param 回调结构
	 * @return void
	 */
	template <class YOURUSERENTRY>
	void exeEveryUser(execEntry<YOURUSERENTRY> & exec)
	{
		_rwlock.rdlock();
		execEveryEntry<>(exec);
		_rwlock.unlock();
	}
};


#endif

