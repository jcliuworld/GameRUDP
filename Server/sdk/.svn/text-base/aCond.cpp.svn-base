
/** 
* @file aCond.cpp
* @brief 系统条件变量封装
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/

#include "aCond.h"

/** 
* @brief 系统条件对象封装 禁用拷贝构造函数
*/


aCond::aCond()
{
	::pthread_cond_init(&cond,NULL);
}
/** 
 * @brief 析构函数
 */

aCond::~aCond()
{
	::pthread_cond_destroy(&cond);
}
/** 
 * @brief 对所有等待这个条件变量的线程广播发送信号
 */

void aCond::broadcast()
{
	::pthread_cond_broadcast(&cond);
}
/** 
 * @brief 对所有等待这个条件变量的线程发送信号
 */

void aCond::signal()
{
	::pthread_cond_signal(&cond);
}
/** 
 * @brief 等待特定的条件变量满足
 * 
 * @param mutex 需要等待的互斥体
 */

void aCond::wait(aMutex &mutex)
{
	::pthread_cond_wait(&cond,&mutex.mutex);
}


