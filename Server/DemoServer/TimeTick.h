#ifndef _TimeTick_H__
#define _TimeTick_H__

/**
 * @TimeTick.h
 * @brief 定时器 
 * @author ljc jc.liuworld@hotmail.com
 * @date   2011-3-11
 */
#include "aTime.h"
#include "aThread.h"

class TimeTick :public aThread
{
	public:
	
		/**
		 * @brief 线程回调
		 */
		void run();
		/**
		 * @brief 获取定时器实例 
		 *
		 * @param
		 * @return 定时器实例
		 */
		static TimeTick & getInstance()
		{
			if(_instance == NULL)
				_instance = new TimeTick();
			return *_instance;
		}
	private:
		/**
		 * @brief 构造函数 
		 */
		TimeTick():aThread("TimeTick"),_one_sec(1),_five_sec(5)
		{}
		/**
		 * @brief 定时器实例 
		 */
		static TimeTick * _instance;
		/**
		 * @brief 当前时间 
		 */
		aRealTime current;
		/**
		 * @brief 一秒钟定时器 
		 */
		aTimer _one_sec;
		/**
		 * @brief 五秒钟定时器 
		 */
		aTimer _five_sec;

};

#endif 
