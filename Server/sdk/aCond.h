#ifndef _aConf_H__
#define _aConf_H__

#pragma once

/** 
* @file aCond.h
* @brief 系统条件变量封装
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/

#include "aNoneCopyable.h"
#include "aMutex.h"
/** 
* @brief 系统条件对象封装 禁用拷贝构造函数
*/

class aCond : private aNoneCopyable
{
	public:
		/** 
		 * @brief 构造函数
		 */

		aCond();
		/** 
		 * @brief 析构函数
		 */

		~aCond();
		/** 
		 * @brief 对所有等待这个条件变量的线程广播发送信号
		 */

		void broadcast();
		/** 
		 * @brief 对所有等待这个条件变量的线程发送信号
		 */

		void signal();
		/** 
		 * @brief 等待特定的条件变量满足
		 * 
		 * @param mutex 需要等待的互斥体
		 */

		void wait(aMutex &mutex);

	private:
		/** 
		 * @brief 系统条件变量
		 */

		pthread_cond_t cond;
};

#endif
