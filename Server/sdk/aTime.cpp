#include "aTime.h"
/** 
 * @file aTime.cpp
 * @brief 时间系统处理类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-18
 */
#include <stdlib.h>
#include <string.h>
long aRealTime::_timezone = 0L;

/**
* @brief 获得本地时区
*
* @return 返回时区
*/

const std::string & aRealTime::getLocalTimeZone()
{
	static std::string s;

	static bool init =false;

	if(!init)
	{
		std::ostringstream so;
		tzset();
		if ( 0L == _timezone )
		{
			_timezone = timezone;
		}
		 so << tzname[0]<<timezone/3600;
		 s = so.str();
		 init = true;
	}

	return s;
}

static aMutex tzLock;

/**
* @brief 设置时区
* 
* @brief strtz 时区值
*/

void aRealTime::saveTimeZone(std::string & strtz)
{
	tzLock.lock();
	std::string ss = aRealTime::getLocalTimeZone();
	std::ostringstream so;
	so<<"TimeZone = "<<ss;
	strtz = so.str();
}

/**
* @brief 重新设置时区
* 
* @brief strtz 时区值
*/


void aRealTime::restoreTimeZone(const std::string & strtz)
{
	static char tmptzstr[256+1];
	bzero(tmptzstr,sizeof(tmptzstr));
	strncpy(tmptzstr,strtz.c_str(),sizeof(tmptzstr)-1);
	putenv(tmptzstr);
	tzLock.unlock();
}

