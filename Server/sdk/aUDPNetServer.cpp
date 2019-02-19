#include "aUDPNetServer.h"
/** 
 * @file aUDPNetService.cpp
 * @brief  UDP网络服务器框架类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2017-11-01
 */
aUDPNetService * aUDPNetService::_netinstance = NULL;

aUDPNetService::aUDPNetService(const std::string   &name,DWORD serverType) :aService(name)
{

	_wdServerID = 0;
	bzero(_pstrIP,sizeof(_pstrIP));
	_port = 0;
	bzero(_pstrExtIP,sizeof(_pstrExtIP));
	_extPort = 0;

	_wdServerType = serverType;
	serviceName = name;
	_netinstance = this;
	udpService = NULL;
	_netType= 0;

	_uniqueID = 1;

}
aUDPNetService::~aUDPNetService()
{
	//_netinstance = NULL;
}
/** 
 * @brief 初始化
 * 
 * @param port 端口
 * 
 * @return 成功 true 失败 false 
 */

bool aUDPNetService::init(unsigned short port)
{
	if (!aService::init())
	{
		return false;
	}

	udpService = new aUDPServic(serviceName);

	if (NULL == udpService)
	{
		return false;
	}

	if (!udpService->bind(serviceName,port))
	{
		return false;
	}
	return true;
}
/** 
 * @brief 服务器回调
 * 
 * @return 成功 true 失败 false
 */

bool aUDPNetService::serviceCallback()
{
	struct sockaddr_in addr;

	DWORD connectID =  _uniqueID;
	
	int retcode = udpService->accept(&addr,connectID);
	if ( retcode >= 0)
	{
		_uniqueID ++;
		newUDPTask(retcode,&addr,connectID);
	}

	return true;
}
/** 
 * @brief 终止
 */


void aUDPNetService::final()
{
	Age::logger->debug("%s",__PRETTY_FUNCTION__);
	SAFE_DELETE(udpService);
}

/** 
 * @brief 设置 ip
 * 
 * @param ip 地址
 */

void  aUDPNetService::setIP(const char * ip){

	strncpy(_pstrIP,ip,MAX_IPLENGTH);
};

/** 
 * @brief 获取 内网ip
 * 
 * @return ip 地址
 */
const char *  aUDPNetService::getIP(){

	return _pstrIP;

};
/** 
 * @brief 设置对外IP
 * 
 * @param ip ip地址
 */

void  aUDPNetService::setExtIP(const char *ip){

	strncpy(_pstrExtIP,ip,MAX_IPLENGTH);

};

/** 
 * @brief 获取对外IP
 * 
 * @return ip ip地址
 */

const char *  aUDPNetService::getExtIP(){

	return _pstrExtIP;

};
/** 
 * @brief 设置端口
 * 
 * @param port 端口
 */

void  aUDPNetService::setPort(DWORD port){
	_port = port;
};

/** 
 * @brief 获取端口
 * 
 * @return port 端口
 */

DWORD  aUDPNetService::getPort(){

	return _port;

};

/** 
 * @brief 设置对外端口
 * 
 * @param port 端口
 */

void  aUDPNetService::setExtPort(DWORD port){

	_extPort = port;

};

/** 
 * @brief 获取对外端口
 * 
 * @return port 端口
 */

DWORD aUDPNetService::getExtPort(){

	return _extPort;

};

/** 
 * @brief 设置服务框架类型
 * 
 * @param type 类型 
 */

void  aUDPNetService::setType(DWORD type){

	_wdServerType = type;

};

/** 
 * @brief 获取服务框架类型
 * 
 * @param type 类型 
 */

DWORD  aUDPNetService::getType(){

	return _wdServerType;
};

/** 
 * @brief 设置服务器ID
 * 
 * @param id 服务器id
 */

void  aUDPNetService::setServerID(DWORD id){

	_wdServerID = id;

};

/** 
 * @brief 获取服务器ID
 * 
 * @return id 服务器id
 */

DWORD  aUDPNetService::getServerID(){

	return _wdServerID ;
};

/** 
 * @brief 设置服务器网络类型
 * 
 * @pram netType 网络类型
 */

void aUDPNetService::setNetType(DWORD netType)
{
	_netType = netType;
}

/** 
 * @brief 获取服务器网络类型
 * 
 * @return 服务器网络类型
 */

const DWORD aUDPNetService::getNetType()
{
	return _netType;
}


