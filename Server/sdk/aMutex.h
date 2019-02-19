#ifndef  _aMutex_H__
#define  _aMutex_H__

#pragma once


/** 
* @file aMutex.h
* @brief 互斥体封装 
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-14
*/

#include <pthread.h>
#include "aNoneCopyable.h"

/** 
* @brief 互斥体封装 禁用拷贝构造函数
*/

class aMutex : private aNoneCopyable
{
	friend class aCond;

	public:
	/** 
	 * @brief 构造函数
	 * 
	 * @param kind mutex type include PTHREAD_MUTEX_NORMAL PTHREAD_MUTEX_ERRORCHECK PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_DEFAULT 
	 */

	aMutex(int kind = PTHREAD_MUTEX_FAST_NP)
	{
		pthread_mutexattr_t attr;
		::pthread_mutexattr_init(&attr);
		::pthread_mutexattr_settype(&attr,kind);
		::pthread_mutex_init(&mutex,&attr);
	}
	/** 
	 * @brief  析构函数
	 */

	~aMutex()
	{
		::pthread_mutex_destroy(&mutex);
	}

	/** 
	 * @brief lock the thread

	 */

	inline void lock()
	{
		::pthread_mutex_lock(&mutex);
	}

	/** 
	 * @brief unlock the thread

	 */

	inline void unlock()
	{
		::pthread_mutex_unlock(&mutex);
	}

	private:

	/** 
	 * @brief 互斥量
	 */

	pthread_mutex_t mutex;
};
/** 
* @brief the auto thread mutex class
*/

class aMutexAutoScop:private aNoneCopyable
{
	public:
		/** 
		 * @brief 构造函数
		 */

		aMutexAutoScop()
		{
			mutex.lock();
		}
		/** 
		 * @brief 析构函数
		 */

		~aMutexAutoScop()
		{
			mutex.unlock();
		}
	private:
		/** 
		 * @brief 互斥量封装
		 */

		aMutex mutex;
};
#endif
