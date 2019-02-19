#include <vector>
#include "aArg.h"
#include "Age.h"
#include "aUDPTaskPool.h"
#include "aGlobal.h"
#include "aConfig.h"
#include "aUDPSocket.h"
#include "protocol.h"

#include "DemoServer.h"
#include "ServerTask.h"
#include "TaskManager.h"
#include "TimeTick.h"
#include "aSerialization.h"


static struct argp_option server_options[]=
{
	{"daemon",'d',0,0,"run as a daemon",0},
	{"log",'l',"level",0,"log level",0},
	{"ifname",'i',"ifname",0,"local network name",0},
	{0,0,0,0,0,0}
};

DemoServer * DemoServer::instance = NULL;


/**
* @brief 结束函数
*
* @param
* @return
*/
bool DemoServer::init()
{

	_version_check = atoi(Age::global["checkversion"].c_str());

	setIP(aUDPSocket::getIPByIfName(Age::global["ifname"].c_str()));

	_dwVersion = atoi(Age::global["version"].c_str());

	_taskpool = new aUDPTaskPool(4000,state_maintain);

	if ( _taskpool == NULL || !_taskpool->init())
	{
		return false;
	}

	if (!aUDPNetService::init(8000))
	{
		return false;
	}


	TimeTick::getInstance().start();

	return true;
}

void DemoServer::reloadConfig ()
{
}

/**
* @brief 是否启动完成
*
* @param
* @return
*/
bool DemoServer::validate()
{

	 return true;
}

/**
 * @brief 获取任务连接池大小
 *
 * @param
 * @return 连接池大小
 */
int DemoServer::getTaskPoolSize()
{
	return _taskpool->getSize();
}
/**
 * @brief 配置文件读取类
 */
class DemoConfig : public aConfig
{
	public:
		DemoConfig(const char * configfile):aConfig(configfile)
		{}
	private:
		 bool parseYour(const xmlNodePtr node)
		{
			if(node)
			{
				xmlNodePtr child = parser.getChildNode(node,NULL);
				while(child)
				{
					parseNormal(child);
					child = parser.getNextNode(child,NULL);
				}
				return true;
			}else
				return false;
		}
};


/**
* @brief 获取当前用户数量
*
* @param
* @return 用户数量
*/
unsigned int DemoServer::getNum()
{
	if (_taskpool)
	{
		return _taskpool->getSize();
	}
	else
		return 0;
}
/**
* @brief 新建连接任务
*
* @param sock 套接口句柄
* @param addr 地址
* @return
*/
void DemoServer::newUDPTask(const int sock,const struct sockaddr_in *addr,DWORD connectID)
{
	ServerTask * udpTask = new ServerTask(sock,addr,connectID);
	if (!udpTask)
	{
		TEMP_FAILURE_RETRY(sock);
	}
	if (!_taskpool->addVerify(udpTask))
	{
		SAFE_DELETE(udpTask);
	}
}
/**
* @brief 结束函数
*
* @param
* @return
*/
void DemoServer::final()
{

	TimeTick::getInstance().final();
	aUDPNetService::final();

}

static error_t server_parse_opt(int key,char * arg,struct argp_state *state)
{
	switch(key)
	{
	case 'd':
		{
			Age::global["daemon"] = "true";
		}break;
	case 'l':
		{
			Age::global["logfilename"] = arg;
		}break;
	case 'i':
		{
			Age::global["ifname"] = arg;
		}break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static char server_doc[] = "\n Server\n " "\t Demo Server ";

int main(int  argc , char ** argv)
{
	Age::global["configdir"] = "config.xml";
	Age::global["logfilename"] = "/tmp/demoserver.log";

	Age::logger = new aLogger("DemoServer");

	DemoConfig  sc(Age::global["configdir"].c_str());

	if(!sc.parse("DemoServer"))
		return EXIT_FAILURE;


	aArg::getArg()->add(server_options,server_parse_opt,0,server_doc);
	aArg::getArg()->parse(argc,argv);

	Age::logger->setLevel(Age::global["log"]);
	Age::logger->addLocalFileLog(Age::global["logfilename"]);
	if (Age::global["daemon"] == "true")
	{
		Age::logger->info("the server will be run as a demo!");
		Age::logger->removeConsoleLog();
		daemon(1,1);
	}

	DemoServer::getInstance().main();
	DemoServer::delInstance();


	return 0;
}

