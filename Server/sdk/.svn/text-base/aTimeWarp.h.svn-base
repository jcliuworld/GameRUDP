
#ifndef _aTimeWarp_H__
#define _aTimeWarp_H__

/**
 * @file aTimeWarp.h
 * @brief 性能时间统计
 * @author ljc jc.liuworld@hotmail.com
 * @version 
 * @date 2013-07-11
 */

#include "Age.h"
#include "aTime.h"
class FunctionTime
{
	private:
		struct stFunctionTime
		{
			stFunctionTime():_times(0),_total_time(0){}
			unsigned long _times;
			unsigned long _total_time;
		};
		typedef  __gnu_cxx::hash_map<std::string,stFunctionTime,Age::key_hash,Age::key_equal> FUNTIME_HASH;
		typedef FUNTIME_HASH::iterator FUNTIME_HASH_IT;
		FUNTIME_HASH _times;
		aTimer timer;
	public:
		FunctionTime(const int  how_long = 600):timer(how_long){}
		~FunctionTime (){}
		void inc(const std::string & func,const unsigned long total)
		{
			stFunctionTime & mt = _times[func];
			if(mt._times)
			{
				++mt._times;
				mt._total_time += total;
			}
			else
			{
				++mt._times;
				mt._total_time = total;
			}
		}
		void reset(const bool force_print,aRealTime * cur = NULL)
		{
			if(force_print || (cur != NULL && timer(*cur)))
			{
				Age::logger->trace("[分时统计] %s,%u",force_print?"force":"timer",size());
				for(FUNTIME_HASH_IT it = _times.begin();it != _times.end(); it++)
				{
					if(it->second._times)
					{
						Age::logger->trace("[分时统计]: %s,%lu毫秒,%lu次,%lu 毫秒/次",it->first.c_str(),it->second._total_time/1000000L,it->second._times,(it->second._total_time/100000L)/it->second._times);
					}
				}
				_times.clear(); 
			}
		}
		int size()
		{
			return _times.size();
		}
};

class FunctionTime_Wrapper
{
	private:
		const std::string _func;
		struct timespec _tv_1;
	public:
		static FunctionTime func;
		FunctionTime_Wrapper(const std::string & func):_func(func)
		{
			clock_gettime(CLOCK_REALTIME,&_tv_1);
		}
		~FunctionTime_Wrapper()
		{
			struct timespec _tv_2;
			clock_gettime(CLOCK_REALTIME,&_tv_2);
			unsigned long end = (unsigned long) _tv_2.tv_sec*1000000000L + _tv_2.tv_nsec;
			unsigned long begin = (unsigned long) _tv_1.tv_sec*1000000000L + _tv_1.tv_nsec;
			func.inc(_func,end-begin);
		}
};

#define FUNCTION_TIME_WARPPER(func) FunctionTime_Wrapper func_time(func)

#endif
