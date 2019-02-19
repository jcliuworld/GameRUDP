#ifndef _aUser_H__
#define _aUser_H__

/**
 * @aUser.h
 * @brief 用户场景进入对象 带有互斥锁 本对象特指‘人’
 * @author ljc jc.liuworld@hotmail.com
 * @date   2011-3-10
 */

#include "aSceneEntry.h"
#include "aMutex.h"
struct aUser : aSceneEntry
{
	/**
	 * @brief 构造函数 
	 *
	 * @param
	 * @return
	 */
	aUser(SceneEntryType type = Entry_Player):aSceneEntry(type)
	{

	}
	/**
	 * @brief 互斥加锁 
	 *
	 * @param
	 * @return
	 */
	void lock()
	{
		_mutex.lock();
	}

	/**
	 * @brief 互斥解锁 
	 *
	 * @param
	 * @return
	 */
	void unlock()
	{
		_mutex.unlock();
	}
private:
	/**
	 * @brief 互斥对象
	 */
	aMutex _mutex;
};
#endif

