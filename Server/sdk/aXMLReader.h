#ifndef _aXMLReader_H__
#define _aXMLReader_H__

#pragma once
/** 
 * @file aXMLReader.h
 * @brief XML解析器封装 需要xml2支持
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-18
 */

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string>

class aXMLReader
{
	public:

		aXMLReader();

		~aXMLReader();

		/**
		 * @brief 根据文件名初始化
		 *
		 * @param xmlFiel XML文件名
		 *
		 * @return true 成功 false 失败
		 */

		bool initFile(const std::string &xmlFile);

		/**
		 * @brief 根据文件名初始化
		 *
		 * @param xmlFiel XML文件名
		 *
		 * @return true 成功 false 失败
		 */

		bool initFile(const char * xmlFile);

		/**
		 * @brief 根据字符串初始化
		 *
		 * @param xmlStr 包含xml格式的字符串
		 *
		 * @return true 成功 false 失败
		 */

		bool initStr(const std::string &xmlStr);

		/**
		 * @brief 根据字符串初始化
		 *
		 * @param xmlStr 包含xml格式的字符串
		 *
		 * @return true 成功 false 失败
		 */

		bool initStr(const char * xmlStr);

		/**
		 * @brief 初始化函数 
		 *
		 * @return true 成功 false 失败
		 */

		bool init();

		/**
		 * @brief 结束函数
		 */

		void final();

		/**
		 * @brief dump 出XML文档
		 *
		 * @param s[out] 文档位置
		 * @param format 是否格式化
		 *
		 * @return 返回 s
		 */

		std::string & dump(std::string &s,bool foramt = false);

		/**
		 * @brief dump 出一段XML文档
		 *
		 * @param dumpNode 要dump的节点
		 * @param s[out] 文档位置
		 * @param head 是否添加xml头
		 *
		 * @return 返回 s
		 */

		std::string & dump(xmlNodePtr dumpNode,std::string &s,bool head = true);

		/**
		 * @brief 得到XML根节点
		 *
		 * @param 根节点的名字
		 *
		 * @return 根节点的指针
		 */

		xmlNodePtr getRootNode(const char * nodeName);

		/**
		 * @brief 得到子节点
		 *
		 * @param parent 父节点指针
		 * @param nodeName 子节点的名字
		 * 
		 * @return 子节点指针
		 */

		xmlNodePtr getChildNode(const xmlNodePtr parent,const char * childName);

		/**
		 * @brief 得到下一个节点
		 *
		 * @param node 当前节点指针
		 * @param nodeName 下一个节点的名字 NULL则为相邻的下一个节点
		 * 
		 * @return 下一个节点的指针
		 */

		xmlNodePtr getNextNode(const xmlNodePtr node,const char * nodeName);

		/**
		 * @brief 统计子节点的数量
		 *
		 * @param node 被统计的父节点
		 * @param nodeName 子节点的名字 NULL则为所有子节点
		 * 
		 * @return 子节点的数量
		 */

		unsigned int getChildNodeNum(const xmlNodePtr parent,const char * nodeName);

		/**
		 * @brief 添加一个根节点
		 *
		 * @param nodeName 根节点的名字
		 * 
		 * @return 根节点的指针
		 */

		xmlNodePtr newRootNode(const char * nodeName);

		/**
		 * @brief 添加一个子节点
		 *
		 * @param parent 父节点的名字
		 * @param childName 子节点的名字
		 * @param content 子节点的内容
		 *
		 * @return 子节点的指针
		 */

		xmlNodePtr newChildNode(const xmlNodePtr parent,const char * childName,const char * content);

		/**
		 * @brief 添加一个节点属性
		 *
		 * @param node 节点的名字
		 * @param propName 节点属性的名字
		 * @param prop 属性的内容
		 *
		 * @return true 成功 false 失败
		 */

		bool newNodeProp(const xmlNodePtr node,const char * propName,const char * prop);

		/**
		 * @brief 添加一个节点属性 返回数值
		 *
		 * @param node 节点的指针
		 * @param propName 节点属性的名字
		 * @param prop [out] 输出的数字 DWORD &value
		 * @param propSize 数字的长度 sizeof(DWORD)
		 *
		 * @return true 成功 false 失败
		 */

		bool getNodePropNum(const xmlNodePtr node,const char * propName,void * prop, int propSize);

		/**
		 * @brief 获得一个节点属性 返回时间
		 *
		 * @param node 节点的指针
		 * @param propName 节点属性的名字
		 * @param prop [out] 输出的时间
		 *
		 * @return true 成功 false 失败
		 */

		bool getNodePropTime(const xmlNodePtr node,const char * propName,time_t &prop);

		/**
		 * @brief 获得一个节点属性 返回字符串
		 *
		 * @param node 节点的指针
		 * @param propName 节点属性的名字
		 * @param prop [out] 输出的字符指针
		 * @param propSize 字符串的长度
		 *
		 * @return true 成功 false 失败
		 */

		bool getNodePropStr(const xmlNodePtr node,const char * propName,void *prop,int propSize);

		/**
		 * @brief 获得一个节点属性 返回string 对象
		 *
		 * @param node 节点的指针
		 * @param propName 节点属性的名字
		 * @param prop [out] 输出的string对象
		 *
		 * @return true 成功 false 失败
		 */

		bool getNodePropStr(const xmlNodePtr node,const char * propName,std::string &prop);

		/**
		 * @brief 获得一个节点内容 返回数值
		 *
		 * @param node 节点的指针
		 * @param content [out] 输出的数值
		 * @param contentSize 数值的长度
		 *
		 * @return true 成功 false 失败
		 */

		bool getNodeContentNum(const xmlNodePtr node,void * content,int contentSize);

		/**
		 * @brief 获得一个节点内容 返回时间
		 *
		 * @param node 节点的指针
		 * @param prop [out] 输出的时间
		 *
		 * @return true 成功 false 失败
		 */

		bool getNodeContentTime(const xmlNodePtr node,time_t &prop);

		/**
		 * @brief 获得一个节点内容 返回一个字符串
		 *
		 * @param node 节点的指针
		 * @param content [out] 输出的字符串指针
		 * \prarm contentSize 字符串指针的长度
		 *
		 * @return true 成功 false 失败
		 */

		bool getNodeContentStr(const xmlNodePtr node,void *content , int contentSize);

		/**
		 * @brief 获得一个节点内容 返回一个string对象
		 *
		 * @param node 节点的指针
		 * @param content [out] 输出的string 对象
		 *
		 * @return true 成功 false 失败
		 */

		bool getNodeContentStr(const xmlNodePtr node,std::string &content);

	private:

		/**
		 * @brief 字符编码转换
		 *
		 * @param in 输入的字符串
		 * @param fromEncording 输入字符串的编码格式
		 * @param toEncoding 输出编码的编码格式
		 * 
		 * @return 转换后字符串的指针
		 */

		unsigned char * charConv(unsigned char *in,const char * fromEncording,const char *toEncoding);


		xmlDocPtr _doc; /**< XML2 对象结构指针 */
};


#endif
