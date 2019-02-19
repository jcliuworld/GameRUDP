#ifndef __aRWLock_H__
#define __aRWLock_H__

#pragma  once

/** 
* @file aRWLock.h
* @brief 系统锁 
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-14
*/

#include "aNoneCopyable.h"
#include "Age.h"
/** 
 * @brief 系统读写锁
 */

class aRWLock : private aNoneCopyable
{
	public:
		/** 
		 * @brief 读锁计数
		 */

		unsigned int rd_count;
		/** 
		 * @brief 写锁计数
		 */

		unsigned int wr_count;
		/** 
		 * @brief 构造函数 
		 */

		aRWLock():rd_count(0),wr_count(0)
		{
			int iRet = ::pthread_rwlock_init(&rwlock,NULL);
			if ( 0 != iRet)
			{
				Age::logger->error("pthread_rwlock_init fail %s:%s",__FUNCTION__,__LINE__);

			}
		}
		/** 
		 * @brief 析构函数
		 */

		~aRWLock()
		{
			int iRet = ::pthread_rwlock_destroy(&rwlock);

			if ( 0 != iRet)
			{
				Age::logger->error("pthread_rwlock_destroy fail %s:%d",__FUNCTION__,__LINE__);
			}
		}
		/** 
		 * @brief 读锁
		 */

		inline void rdlock()
		{
			int iRet = ::pthread_rwlock_rdlock(&rwlock);

			if ( 0 != iRet)
			{
				Age::logger->error("pthread_rwlock_rdlock fail %s:%d",__FUNCTION__,__LINE__);
			}

			++rd_count;
		}
		/** 
		 * @brief 写锁
		 */

		inline void wrlock()
		{
			int iRet = ::pthread_rwlock_wrlock(&rwlock);

			if ( 0 != iRet)
			{
				Age::logger->error("pthread_rwlock_wrlock fail %s:%d",__FUNCTION__,__LINE__);
			}
			++rd_count;
			++wr_count;
		}
		/** 
		 * @brief 解锁
		 */

		inline void unlock()
		{
			int iRet = ::pthread_rwlock_unlock(&rwlock);

			if ( 0 != iRet)
			{
				Age::logger->error("pthread_rwlock_unlock fail %s:%s",__FUNCTION__,__LINE__);
			}

			--rd_count;
		}

		/** 
		 * @brief 线程读写结构 
		 */


		pthread_rwlock_t rwlock;
};
/** 
 * @brief 自动读锁 禁用拷贝构造函数
 */

class aRWLockAutoScopeRD :private aNoneCopyable
{
	private:
		aRWLock &rwlock;
	public:
		/** 
		 * @brief 构造函数
		 * 
		 */

		aRWLockAutoScopeRD(aRWLock & lock) : rwlock(lock)
		{
			rwlock.rdlock();
		}
		/** 
		 * @brief 析构函数
		 */

		~aRWLockAutoScopeRD()
		{
			rwlock.unlock();
		}
};
/** 
 * @brief 自动写锁 禁用拷贝构造函数
 */

class aRWLockAutoScopeRW : private aNoneCopyable
{
	private:

		aRWLock & rwlock;

	public:
		/** 
		 * @brief 构造函数
		 * 
		 */

		aRWLockAutoScopeRW(aRWLock & lock) : rwlock(lock)
		{
			rwlock.wrlock();
		}
		/** 
		 * @brief 析构函数
		 */

		~aRWLockAutoScopeRW()
		{
			rwlock.unlock();
		}
};

#endif
