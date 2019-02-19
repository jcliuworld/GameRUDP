#ifndef _aConfig_H__
#define _aConfig_H__

#pragma once

/** 
* @file aConfig.h
* @brief xml配置文件解析
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/

#include "aXMLReader.h"
/** 
* @brief xml 配置文件解析类
*/

class aConfig
{
protected:
	/** 
	 * @brief xml 解析对象
	 */

	aXMLReader parser;
	/** 
	 * @brief 需要解析的配置文件文件名
	 */

	std::string filename;
	/** 
	 * @brief 全局配置变量解析
	 * 
	 * @param node 全局配置解析节点
	 * 
	 * @return 成功true 失败false
	 */

	bool parseGlobal(const xmlNodePtr node);
	/** 
	 * @brief 普通配置变量解析
	 * 
	 * @param node普通配置变量节点 
	 * 
	 * @return 成功 true 失败 false
	 */

	bool parseNormal(const xmlNodePtr node);
	/** 
	 * @brief 解析全局管理服务器数据
	 * 
	 * @param node 全局服务器节点
	 * 
	 * @return 成功true 失败 false
	 */

	bool parseManager(const xmlNodePtr node);
	/** 
	 * @brief 各自特殊解析数据 派生对象必须实现
	 * 
	 * @param node 各自需要解析的节点
	 * 
	 * @return  成功 true 失败false
	 */

	virtual bool parseYour(const xmlNodePtr node) = 0;
public:
	/** 
	 * @brief 构造函数
	 * 
	 * @param configfile 配置文件文件名
	 */

	aConfig(const char * configfile = "config.xml");
	/** 
	 * @brief 析构函数
	 */

	virtual ~aConfig();
	/** 
	 * @brief 解析配置文件
	 * 
	 * @param name 配置文件文件名
	 * 
	 * @return 
	 */

	bool parse(const char * name);
};

#endif

