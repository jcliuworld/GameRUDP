#include "aConfig.h"
#include "Age.h"
#include "sdkL.h"
#include "aBase64.h"
/** 
* @file aConfig.cpp
* @brief xml配置文件解析
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/

/** 
 * @brief 构造函数
 * 
 * @param configfile 配置文件文件名
 */

aConfig::aConfig(const char * configfile /* = */ ):filename(configfile)
{
}
/** 
 * @brief 析构函数
 */

aConfig::~aConfig()
{
	parser.final();
}
/** 
 * @brief 全局配置变量解析
 * 
 * @param node 全局配置解析节点
 * 
 * @return 成功true 失败false
 */

bool aConfig::parseGlobal(const xmlNodePtr node)
{
	xmlNodePtr child = parser.getChildNode(node,NULL);
	while(child)
	{
		if(strcasecmp((char *)child->name,"globalserver") ==0 )
			parseManager(child);
		else
			parseNormal(child);
		child = parser.getNextNode(child,NULL);
	}

	return true;
}
/** 
 * @brief 普通配置变量解析
 * 
 * @param node普通配置变量节点 
 * 
 * @return 成功 true 失败 false
 */

bool aConfig::parseNormal(const xmlNodePtr node)
{
	char buf[128];
	if (parser.getNodeContentStr(node,buf,128))
	{
		Age::global[(char *)node->name] = buf;
		if (/*(0 == strcmp((char *)node->name, "mysql"))
				&&*/ parser.getNodePropStr(node,"encode",buf,128)
				&& 0 == strcasecmp(buf, "yes"))
		{
			std::string tmpS;
			Age::base64_decrypt(Age::global[(char *)node->name], tmpS);
			Age::global[(char *)node->name]=tmpS;
		}
		return true;
	}
	else
		return false;
}
/** 
 * @brief 解析全局管理服务器数据
 * 
 * @param node 全局服务器节点
 * 
 * @return 成功true 失败 false
 */

bool aConfig::parseManager(const xmlNodePtr node)
{
	char buf[64];
	if(parser.getNodeContentStr(node,buf,64))
	{
		Age::global["server"] = buf;
		if(parser.getNodePropStr(node,"port",buf,64))
			Age::global["port"] = buf;
		else
			Age::global["port"] = "10000";
		return true;
	}

	return false;
}
/** 
 * @brief 解析配置文件
 * 
 * @param name 配置文件文件名
 * 
 * @return 
 */

bool aConfig::parse(const char * name)
{
	if (parser.initFile(filename))
	{
		xmlNodePtr root = parser.getRootNode("Age");

		if (root)
		{
			xmlNodePtr globalNode = parser.getChildNode(root,"global");
			if (globalNode)
			{
				if(!parseGlobal(globalNode))
					return false;
			}
			else
				Age::logger->warn(SYS_SDK_ID_000000);

			xmlNodePtr otherNode = parser.getChildNode(root,name);

			if (otherNode)
			{
				if (!parseYour(otherNode))
					return false;
			}
			else
				Age::logger->warn(SYS_SDK_ID_000001,name);
			return true;
		}
	}

	return false;
}
