#include "aThread.h"
/** 
 * @file aThread.cpp
 * @brief  线程类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-18
 */

/**
 * @brief 线程函数
 *
 * @param arg 运行对象
 */


void * aThread::threadFunc(void * arg)
{

	aThread * pthread = (aThread *) arg;

	Age::seedp = time(NULL);

	pthread->_mutex.lock();

	pthread->_alive = true;

	pthread->_cond.broadcast();

	pthread->_mutex.unlock();

	sigset_t sig_mask;

	sigfillset(&sig_mask);

	pthread_sigmask(SIG_SETMASK,&sig_mask,NULL);

	pthread->run();

	pthread->_mutex.lock();
	pthread->_alive = false;

	pthread->_cond.broadcast();
	pthread->_mutex.unlock();

	if (!pthread->isJoinable())
	{
		pthread->_mutex.lock();
		while (pthread->_alive)
		{
			pthread->_cond.wait(pthread->_mutex);
		}
		pthread->_mutex.unlock();
		SAFE_DELETE(pthread);
	}

	pthread_exit(NULL);
}


/**
 * @brief 开始线程
 *
 * @breturn 
 */


bool aThread::start()
{
	if (_alive)
	{
		Age::logger->warn("the thread %s has been run , some one try to run again",getThreadName().c_str());
		return true;
	}

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	if (!_joinable)
	{
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	}

	if (0 != ::pthread_create( &_thread, NULL,aThread::threadFunc,this))
	{
		Age::logger->error(" create the thread fail %s" ,getThreadName().c_str());
		return false;
	}



	pthread_attr_destroy(&attr);
	_mutex.lock();
	while(!_alive)
		_cond.wait(_mutex);
	_mutex.unlock();
	return true;
}


/**
 * @brief 挂起主线程 
 *
 * @breturn 
 */


void aThread::join()
{
	if (0 != _thread && _joinable)
	{
		::pthread_join(_thread,NULL);
		_thread = 0;
		_mutex.lock();
		while(_alive)
			_cond.wait(_mutex);
		_mutex.unlock();
	}
}

aThreadGroup::aThreadGroup():vts(),rwlock()
{

}
aThreadGroup::~aThreadGroup()
{
	joinAll();
}


/**
 * @brief 添加一个线程对象
 *
 * @param thread 线程对象
 * @param
 * @param
 *
 * @return 
 */

void aThreadGroup::add(aThread * _thread)
{
	aRWLockAutoScopeRW scop(rwlock);

	Container::iterator it = std::find(vts.begin(),vts.end(),_thread);

	if ( it == vts.end())
		vts.push_back(_thread);
}

/**
 * @brief 根据索引获取线程对象
 *
 * @param index 索引ID
 *
 * @breturn 线程对象
 */


aThread * aThreadGroup::getByIndex(const Container::size_type index)
{
	aRWLockAutoScopeRD scop(rwlock);

	if (index >= vts.size())
		return NULL;
	else
		return vts[index];

}

aThread * aThreadGroup::operator [](const Container::size_type index)
{
	aRWLockAutoScopeRD scop(rwlock);

	if (index >= vts.size())
		return NULL;
	else
		return vts[index];
}



/**
 * @brief 挂起所有线程
 *
 * @breturn 
 */

void aThreadGroup::joinAll()
{
	aRWLockAutoScopeRW scop(rwlock);

	while( !vts.empty())
	{
		aThread * pthread = vts.back();

		vts.pop_back();

		if (pthread)
		{
			pthread->final();
			pthread->join();
			SAFE_DELETE(pthread);
		}

	}
}

/**
 * @brief 执行所有线程
 *
 * @breturn 
 */


void aThreadGroup::execAll(Callback & cb)
{
	aRWLockAutoScopeRD scop(rwlock);

	for (Container::iterator it = vts.begin(); it != vts.end();++it)
	{
		cb.exec(*it);
	}

}
