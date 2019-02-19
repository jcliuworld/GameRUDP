#include "aService.h"
#include "Age.h"
#include <signal.h>
/** 
* @file aService.cpp
* @brief  服务器回调结构封装
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-14
*/

/***
 * @brief  the default constructor
 * 
 * @param name server name
 */

aService::aService(const std::string  & name)
{
	_instance = this;
	this->name = name.c_str();
	_terminate =false;
}

/** 
 * @brief to hanlde the CTRL + C signal
 * 
 * @param signum 
 */

static void ctrlcHandler(int signum)
{
	aService * instance = aService::getInstance();
	instance->Terminate();
}

/** 
 * @brief to hanlde the HUP signal
 * 
 * @param signum 
 */

static void hupHandler(int signum)
{
	aService *instance = aService::getInstance();
	instance->reloadConfig();
}

static void userHandler_1(int signum)
{
	aService *instance = aService::getInstance();
	instance->userKill1();	
} 

static void userHandler_2(int sinum)
{
	aService *instance = aService::getInstance();
	instance->userKill2(); 
}
/** 
 * @brief 单件模式实例
 */

aService * aService::_instance = NULL;
/** 
 * @brief the initialization function 
 * 
 * @return true or false 
 */

bool aService::init()
{
	//int i = 0;
	//while(environ[i])
	//{
	//	std::string s(environ[i++]);
	//	std::vector<std::string> v;

	//}

	struct sigaction sig;

	sig.sa_handler = ctrlcHandler;

	sigemptyset( &sig.sa_mask );

	sig.sa_flags = 0;

	sigaction( SIGINT , &sig , NULL );
	sigaction( SIGQUIT , &sig , NULL );
	sigaction( SIGABRT , &sig , NULL );
	sigaction( SIGTERM , &sig , NULL );

	sig.sa_handler = hupHandler;
	sigaction( SIGHUP , &sig , NULL );

	sig.sa_handler = userHandler_1;

	sigaction( SIGUSR1,&sig,NULL);

	sig.sa_handler = userHandler_2;

	sigaction( SIGUSR2,&sig,NULL);

	sig.sa_handler = SIG_IGN;
	sigaction( SIGPIPE , &sig , NULL );

	Age::seedp = time(NULL);

	return true;
}

/** 
 * @brief the service main loop
 */

void aService::main()
{
	if ( init() && validate() )
	{
		while( !isTerminate() )
		{
			if ( !serviceCallback() )
			{
				break;
			}
		}
	}
	final();
}

/** 
 * @brief whether the service is terminate.
 * 
 * @return value  if return true the serive is terminate.
 */

bool aService::isTerminate()const
{
	return _terminate;
}

/** 
 * @brief to make the service stop
 */

void aService::Terminate()
{
	_terminate = true;
}


/** 
 * @brief return the servie name;
 */
const char * aService::getServerName()
{
	return "";
}

