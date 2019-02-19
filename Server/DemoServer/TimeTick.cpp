#include "TimeTick.h"
#include "Age.h"
#include "DemoServer.h"
TimeTick * TimeTick::_instance = NULL;

/**
* @brief 线程回调
*/
void TimeTick::run()
{
	while(!isFinal())
	{
		current.now();
		if(_one_sec(current))
		{

			Age::logger->debug("time tick test");
		}

		aThread::msleep(50);

	}
}
