#ifndef _aThread_H__
#define _aThread_H__

#pragma once

/** 
 * @file aThread.h
 * @brief  线程类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-18
 */

#include "aCond.h"
#include "aNoneCopyable.h"
#include "aMutex.h"
#include "aCond.h"
#include "aRWLock.h"

#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <signal.h>
/** 
 * @brief 对系统线程API进行封装
 */

class aThread : private aNoneCopyable
{
	public:
		aThread(const std::string & name = std::string("aThread"),const bool joinable = true):_threadName(name),_alive(false),_complete(false)
		{
			_thread = 0;
			_joinable = joinable;
		};

		virtual ~aThread(){};

		/**
		 * @brief  获取线程ID
		 *
		 * @breturn 线程ID
		 */

		virtual pthread_t getCurrentThreadID()
		{
			return ::pthread_self();
		}

		/**
		 * @brief 挂起线程
		 *
		 * \param sec 挂起的秒数
		 */

		static void sleep( unsigned long sec)
		{
			::sleep(sec);

		}

		/**
		 * @brief 挂起线程
		 *
		 * \param mesc 挂起的毫秒数
		 */

		static void msleep(const long msec)
		{
			::usleep(1000*msec);
		}

		/**
		 * @brief 挂起线程
		 *
		 * \param 挂起的微妙数
		 */

		static void usleep (const unsigned long usec)
		{
			::usleep(usec);
		}

		/**
		 * @brief 是否挂起主进程
		 *
		 * @breturn true 是 false 否
		 */

		bool isJoinable()const
		{
			return _joinable;
		}

		/**
		 * @brief 是否激活
		 *
		 * @breturn true 是 false 否
		 */

		const bool isAlive()const
		{
			return _alive;
		}

		/**
		 * @brief 线程函数
		 *
		 * \param arg 运行对象
		 */

		static void * threadFunc(void * arg);

		/**
		 * @brief 开始线程
		 *
		 * @breturn 
		 */

		bool start();

		/**
		 * @brief 挂起主线程 
		 *
		 * @breturn 
		 */

		void join();

		/**
		 * @brief 结束线程
		 *
		 * @breturn 
		 */

		void final()
		{
			_complete = true;
		}

		/**
		 * @brief 线程是否结束 
		 *
		 * @breturn 
		 */

		const bool isFinal()const
		{
			return _complete;
		}

		/**
		 * @brief 线程执行的实体函数 派生类应实现此函数
		 *
		 * @breturn 
		 */

		virtual void run() = 0;

		bool operator == (const aThread & other) const
		{
			return ::pthread_equal(_thread,other._thread) != 0;
		}

		bool operator != (const aThread & othrer) const
		{
			return ! operator==(othrer);
		}

		const std::string & getThreadName()const
		{
			return _threadName;
		}


	private:

		std::string _threadName; /**<线程名*/

		volatile bool _alive; /**<激活标记*/

		volatile bool _complete; /**<结束标记*/

		bool _joinable; /**<挂起标记*/

		pthread_t _thread;/**<线程对象*/


		aCond _cond;/**<环境变量*/
		aMutex _mutex; /**<互斥量*/

};

/**
 * @brief 线程组
 */

class aThreadGroup : private aNoneCopyable
{
	public:

		/**
		 * @brief 线程回调结构
		 */
		struct Callback
		{
			virtual void exec(aThread * e) = 0;
			virtual  ~Callback(){};
		};

		typedef std::vector<aThread*> Container;

		aThreadGroup();
		~aThreadGroup();

		/**
		 * @brief 添加一个线程对象
		 *
		 * \param thread 线程对象
		 *
		 * @breturn 
		 */

		void add(aThread * thread);

		/**
		 * @brief 根据索引获取线程对象
		 *
		 * \param index 索引ID
		 *
		 * @breturn 线程对象
		 */


		aThread * getByIndex(const Container::size_type index);
		aThread * operator[](const Container::size_type index);

		/**
		 * @brief 挂起所有线程
		 *
		 * @breturn 
		 */
		void joinAll();

		/**
		 * @brief 执行所有线程
		 *
		 * @breturn 
		 */

		void execAll(Callback & cb);

		/**
		 * @brief 线程数量 
		 *
		 * @breturn 数量
		 */

		const Container::size_type size()
		{
			aRWLockAutoScopeRW scop(rwlock);
			return vts.size();
		}

	private:

		Container vts; /**<线程容器*/

		aRWLock rwlock;/**<读写锁*/

};

#endif
