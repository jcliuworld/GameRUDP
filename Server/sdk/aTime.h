#ifndef _aTime_H__
#define _aTime_H__

/** 
 * @file aTime.h
 * @brief 时间系统处理类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-18
 */

#include "aMutex.h"
#include <time.h>
#include <sys/time.h>
#include <sstream>
#include <string.h>

/**
 * @brief 当前真实时间
 */
class aRealTime
{
	private:
		unsigned long long _msecs; /*<时间 毫秒值>*/

		unsigned long long _now()
		{
			unsigned long long retval = 0LL;

			struct timespec tv;

			clock_gettime(CLOCK_REALTIME,&tv);

			retval = tv.tv_sec;

			retval *= 1000LL;

			retval += ( tv.tv_nsec / 1000000L );

			return retval;
		}

		void nowByDelay( int delay )
		{
			_msecs = _now();

			addDelay(delay);
		}



	public:

		/**
		 * @brief 构造函数
		 *
		 * \parapm delay 与当前时间的延迟值
		 */

		aRealTime(const int delay = 0 )
		{
			nowByDelay(delay);
		}

		/**
		 * @brief 构造函数
		 *
		 * \parapm rt 根据另一个aRealTime 时间结构初始化
		 */

		aRealTime(const aRealTime &rt)
		{
			_msecs = rt._msecs;
		}


		/**
		 * @brief 获取当前时间
		 */

		void now()
		{
			_msecs = _now();
		}

		/**
		 * @brief 得到当前时间的秒数 
		 * 
		 * @return 当前时间的秒数
		 */

		unsigned long sec() const
		{
			return _msecs/1000L;
		}

		/**
		 * @brief 得到当前时间的毫秒数 
		 * 
		 * @return 当前时间的毫秒数
		 */

		unsigned long msec() const
		{
			return _msecs;
		}

		/**
		 * @brief 设置当前时间的秒数 
		 * 
		 * @param data 设置时间的秒数
		 */

		void setmsecs( unsigned long long data)
		{
			_msecs = data;
		}

		/**
		 * @brief 在当前的时间基础上延迟时间 
		 * 
		 * @brief delay 单位毫秒
		 */

		void addDelay(int delay)
		{
			_msecs += delay;
		}

		aRealTime & operator = (const aRealTime & rt)
		{
			_msecs = rt._msecs;

			return *this;
		}

		const aRealTime & operator + (const aRealTime & rt)
		{
			_msecs += rt._msecs;

			return *this;
		}
		const aRealTime & operator - (const aRealTime & rt)
		{
			_msecs -= rt._msecs;

			return *this;
		}

		bool operator > (const aRealTime & rt) const
		{
			return _msecs > rt._msecs;
		}

		bool operator < (const aRealTime & rt) const
		{
			return _msecs < rt._msecs;
		}

		bool operator >= (const aRealTime & rt) const
		{
			return _msecs >= rt._msecs;
		}

		bool operator <= (const aRealTime & rt) const
		{
			return _msecs <= rt._msecs;
		}

		bool operator == (const aRealTime & rt) const
		{
			return _msecs == rt._msecs;
		}

		/**
		 * @brief 比较与另一个时间的差值
		 * 
		 * @brief rt 要比较的时间
		 *
		 * @return 时间的差值 单位毫秒 如果输入的时间小于自己 则返回0
		 */

		unsigned long long elapse( const aRealTime & rt) const
		{
			if (rt._msecs > _msecs)
			{
				return (rt._msecs - _msecs);
			} 
			else
			{
				return 0LL;
			}

		}

		static long _timezone; /*<时区>*/

		/**
		 * @brief 获得本地时区
		 *
		 * @return 返回时区
		 */

		static const std::string & getLocalTimeZone();

		/**
		 * @brief 设置时区
		 * 
		 * @brief strtz 时区值
		 */

		static void saveTimeZone(std::string & strtz);

		/**
		 * @brief 重新设置时区
		 * 
		 * @brief strtz 时区值
		 */

		static void restoreTimeZone(const std::string & strtz);

		/**
		 * @brief 本地时间格式转换
		 * 
		 * @brief timevalue 时区值
		 * @brief tv [out] 输出时间值
		 */

		static void GetLocalTime(struct tm & tv,time_t timevalue)
		{
			timevalue -= _timezone;
			gmtime_r(&timevalue,&tv);
		}


		/**
		 * @brief 判断今天是周几
		 * 
		 * @brief day 周几 1～7
		 * 
		 * @return true 是 false 否
		 */

		static bool isInWeekDay(int day)
		{
			time_t now = time(NULL);

			struct tm tv;

			aRealTime::GetLocalTime(tv,now);

			return (1<<(tv.tm_wday%7)& day);
		}
};

/**
 * @brief 对tm结构的时间进行封装
 */

class atmTime
{
	public:

		atmTime()
		{
			time(&secs);
			aRealTime::GetLocalTime(tv,secs);
		}

		atmTime(const atmTime &ct)
		{
			secs = ct.secs;
			aRealTime::GetLocalTime(tv,secs);
		}

		/**
		* @param time 指定时间
		*/
		atmTime(const time_t &inTime)
		{
			tm *pTmpTm = NULL;
			pTmpTm = localtime(&inTime);	
			memcpy(&tv, pTmpTm, sizeof(tm));
			secs = inTime;
		}

		/**
		* @param realTime  指定时间
		*/
		atmTime(const aRealTime &realTime)
		{
			secs = realTime.sec();
			aRealTime::GetLocalTime(tv,secs);
		}

		void now()
		{
			time(&secs);

			aRealTime::GetLocalTime(tv,secs);
		}

		time_t sec () const{

			return secs;
		}

		void setsec(const time_t &value)
		{
			secs = value;
			aRealTime::GetLocalTime(tv,secs);

		}

		atmTime operator = (const atmTime & rt)
		{
			secs = rt.secs;
			aRealTime::GetLocalTime(tv,secs);
			return *this;
		}

		const atmTime & operator + (const atmTime &rt)
		{
			secs += rt.secs;
			return *this;
		}
		const atmTime & operator - (const atmTime &rt)
		{
			secs -= rt.secs;
			return *this;
		}

		const atmTime & operator -= (const time_t s)
		{
			secs -= s;
			return *this;
		}

		bool operator > (const atmTime & rt) const
		{
			return secs > rt.secs;
		}

		bool operator >= (const atmTime & rt) const
		{
			return secs >= rt.secs;
		}

		bool operator < (const atmTime & rt) const
		{
			return secs < rt.secs;
		}

		bool operator <= (const atmTime & rt) const
		{
			return secs <= rt.secs;
		}

		bool operator == (const atmTime & rt) const
		{
			return secs == rt.secs;
		}

		/**
		 * @brief 比较与另一个时间的差值
		 * 
		 * @brief rt 要比较的时间
		 *
		 * @return 时间的差值  如果输入的时间小于自己 则返回0
		 */


		time_t elapse(const atmTime & rt) const
		{
			if(rt.secs > secs)
				return (rt.secs - secs);
			else
				return 0;
		}

		/**
		 * @brief 比较与当前时间的差值
		 *
		 * @return 时间的差值 单位毫秒 如果输入的时间小于自己 则返回0
		 */


		time_t elapse()const
		{
			atmTime rt;
			return (rt.secs - secs );
		}

		/**
		 * @brief  获取时间的秒数
		 *
		 * @return 秒数 
		 */	

		int getSec()
		{
			return tv.tm_sec;
		}

		/**
		 * @brief  获取时间的分数
		 *
		 * @return 分数 
		 */	

		int getMin()
		{
			return tv.tm_min;
		}

		/**
		 * @brief  获取时间的小时数
		 *
		 * @return 小时数 
		 */	

		int getHour()
		{
			return tv.tm_hour;
		}

		/**
		 * @brief  获取时间的日期数
		 *
		 * @return 日期数 
		 */	

		int getMDay()
		{
			return tv.tm_mday;
		}

		/**
		 * @brief  获取时间的星期数, 星期日为0
		 *
		 * @return 星期数 (0-6)
		 */	

		int getWDay()
		{
			return tv.tm_wday;
		}

		/**
		* @brief 返回星期数(1-7)
		*
		* @return 
		*/
		int getWeekDay()
		{
			if (tv.tm_wday == 0)
				return 7;

			return tv.tm_wday;
		}

		/**
		 * @brief  获取时间的天数
		 *
		 * @return 天数 
		 */	

		int getYDay()
		{
			return tv.tm_yday;
		}

		/**
		 * @brief  获取时间的月份数
		 *
		 * @return 月份数 
		 */	

		int getMonth()
		{
			return tv.tm_mon+1;
		}

		/**
		 * @brief  获取时间的年数
		 *
		 * @return 年数 
		 */	

		int getYear()
		{
			return tv.tm_year +1900;
		}


	private:
		time_t secs;

		struct tm tv;
};

/**
 * @brief 定时器 
 */
class aTimer
{
	public:
		aTimer(const float how_long,const int delay = 0) : _long ((int)(how_long*1000)),_timer(delay * 1000)
		{

		}
		void reset(const unsigned long how_long,const aRealTime & cur)
		{
			_long = how_long;
			_timer = cur;
			_timer.addDelay(_long);
		}

		aTimer(const float how_long,const aRealTime &cur):_long((int)(how_long * 1000)),_timer(cur)
		{
			_timer.addDelay(_long);
		}

		void next(const aRealTime &cur)
		{
			_timer = cur;
			_timer.addDelay(_long);
		}

		bool operator () (const aRealTime &cur)
		{
			if (_timer <= cur)
			{
				_timer = cur;
				_timer.addDelay(_long);
				return true;
			}
			return false;
		}
	private:
		int _long;
		aRealTime _timer;
};
#endif
