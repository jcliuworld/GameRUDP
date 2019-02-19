 #include <string.h>
#include "aXMLReader.h"
#include "aDef.h"
/** 
 * @file aXMLReader.cpp
 * @brief XML解析器封装 需要xml2支持
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-18
 */

static void finalLibXML2() __attribute__((destructor));


void finalLibXML2()
{
	xmlCleanupParser();
}


aXMLReader::aXMLReader()
{
	_doc = NULL;
}


aXMLReader::~aXMLReader()
{
	final();
}


/**
 * @brief 根据文件名初始化
 *
 * @param xmlFiel XML文件名
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::initFile(const std::string &xmlFile)
{
	return initFile(xmlFile.c_str());
}


/**
 * @brief 根据文件名初始化
 *
 * @param xmlFiel XML文件名
 *
 * @return true 成功 false 失败
 */

bool aXMLReader::initFile(const char * xmlFile)
{
	final();
	if (xmlFile == NULL)
		return false;
	_doc = xmlParseFile(xmlFile);
	return (_doc != NULL );
}


/**
 * @brief 根据字符串初始化
 *
 * @param xmlStr 包含xml格式的字符串
 *
 * @return true 成功 false 失败
 */

bool aXMLReader::initStr(const std::string &xmlFile)
{
	return initStr(xmlFile.c_str());
}
/**
 * @brief 根据字符串初始化
 *
 * @param xmlStr 包含xml格式的字符串
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::initStr(const char * xmlStr)
{
	final();
	if(xmlStr == NULL )return false;
	_doc = xmlParseDoc((xmlChar *)xmlStr);
	return (_doc != NULL);
}


/**
 * @brief 初始化函数 
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::init()
{
	final();
	_doc = xmlNewDoc((const xmlChar *) "1.0");
	return (_doc != NULL);
}


/**
 * @brief 结束函数
 */


void aXMLReader::final()
{
	if(_doc)
	{
		xmlFreeDoc(_doc);
		_doc = NULL;
	}
}


/**
 * @brief dump 出XML文档
 *
 * @param s[out] 文档位置
 * @param format 是否格式化
 *
 * @return 返回 s
 */


std::string & aXMLReader::dump(std::string &s,bool foramt /* = false */)
{
	if (_doc)
	{
		xmlChar * out = NULL;
		int size = 0;
		xmlDocDumpFormatMemory(_doc,&out,&size,foramt?1:0);
		if (out !=  NULL)
		{
			s = (char *)out;
			xmlFree(out);
		}
	}
	return s;
}


/**
 * @brief dump 出一段XML文档
 *
 * @param dumpNode 要dump的节点
 * @param s[out] 文档位置
 * @param head 是否添加xml头
 *
 * @return 返回 s
 */


std::string & aXMLReader::dump(xmlNodePtr dumpNode,std::string &s,bool head /* = true */)
{
	/*if (dumpNode == NULL) return s;

	xmlBufferPtr out = xmlBufferCreate();

	xmlOutputBufferPtr outbuf;
	outbuf = (xmlOutputBufferPtr)xmlMalloc(sizeof(xmlOutputBuffer));
	if (outbuf != NULL)
	{
		memset(outbuf,0,(size_t)sizeof(xmlOutputBuffer));
		outbuf->buffer = out;
		xmlNodeDumpOutput(outbuf,_doc,dumpNode,1,1,"UTF-8");
	}

	unsigned char * cout = charConv((unsigned char * )out->content,"UTF-8",(const char * )_doc->encoding);
	if (out)
	{
		if (head)
		{
			s="<?xml version=\"1.0\"encoding=\"";
			s += (char * )_doc->encoding;
			s += "\"?>";
		}
		else
			s = "";
		s += (char *)cout;
		SAFE_DELETE(cout);
	}

	xmlBufferFree(out);
	xmlFree(outbuf);*/
	return s;
}


/**
 * @brief 得到XML根节点
 *
 * @param 根节点的名字
 *
 * @return 根节点的指针
 */


xmlNodePtr aXMLReader::getRootNode(const char * nodeName)
{
	if(_doc == NULL)return NULL;
	xmlNodePtr cur = xmlDocGetRootElement(_doc);
	if (nodeName != NULL)
		while(cur != NULL && xmlStrcmp(cur->name,(const xmlChar *)nodeName))
			cur = cur->next;
	return cur;
}


/**
 * @brief 得到子节点
 *
 * @param parent 父节点指针
 * @param nodeName 子节点的名字
 * 
 * @return 子节点指针
 */


xmlNodePtr aXMLReader::getChildNode(const xmlNodePtr parent,const char * childName)
{
	if (parent == NULL) return NULL;
	xmlNodePtr retval = parent->children;
	if (childName)
		while(retval)
		{
			if (!xmlStrcmp(retval->name,(const xmlChar *)childName))break;
			retval = retval->next;

		}	
	else
		while(retval)
		{
			if(!xmlNodeIsText(retval))break;
			retval = retval->next;
		}
	return retval;

}


/**
 * @brief 得到下一个节点
 *
 * @param node 当前节点指针
 * @param nodeName 下一个节点的名字 NULL则为相邻的下一个节点
 * 
 * @return 下一个节点的指针
 */


xmlNodePtr aXMLReader::getNextNode(const xmlNodePtr node,const char * nodeName)
{
	if (node == NULL) return NULL;
	xmlNodePtr retval = node->next;
	if (nodeName)
		while(retval)
		{
			if (!xmlStrcmp(retval->name,(const xmlChar *)nodeName))break;
			retval = retval->next;

		}	
	else
		while(retval)
		{
			if(!xmlNodeIsText(retval))break;
			retval = retval->next;
		}
	return retval;
}


/**
 * @brief 统计子节点的数量
 *
 * @param node 被统计的父节点
 * @param nodeName 子节点的名字 NULL则为所有子节点
 * 
 * @return 子节点的数量
 */


unsigned int aXMLReader::getChildNodeNum(const xmlNodePtr parent,const char * nodeName)
{
	int retval = 0;
	if (parent == NULL) return retval;
	xmlNodePtr child = parent->children;
	if (nodeName)
		while(child)
		{
			if (!xmlStrcmp(child->name,(const xmlChar *)nodeName))retval++;
			child = child->next;

		}	
	else
		while(child)
		{
			if(!xmlNodeIsText(child))retval++;
			child = child->next;
		}
	return retval;
}


/**
 * @brief 添加一个根节点
 *
 * @param nodeName 根节点的名字
 * 
 * @return 根节点的指针
 */


xmlNodePtr aXMLReader::newRootNode(const char * nodeName)
{
	if (NULL == _doc)
		return NULL;
	xmlNodePtr root_node = xmlNewNode(NULL,(const xmlChar *)nodeName);
	xmlDocSetRootElement(_doc,root_node);
	return root_node;
}


/**
 * @brief 添加一个子节点
 *
 * @param parent 父节点的名字
 * @param childName 子节点的名字
 * @param content 子节点的内容
 *
 * @return 子节点的指针
 */


xmlNodePtr aXMLReader::newChildNode(const xmlNodePtr parent,const char * childName,const char * content)
{
	if(NULL == parent)
		return NULL;
	return xmlNewChild(parent,NULL,(const xmlChar *)childName,(const xmlChar *)content);
}


/**
 * @brief 添加一个节点属性
 *
 * @param node 节点的名字
 * @param propName 节点属性的名字
 * @param prop 属性的内容
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::newNodeProp(const xmlNodePtr node,const char * propName,const char * prop)
{
	if(NULL == node)
		return false;
	return (NULL != xmlNewProp(node,(const xmlChar *)propName,(const xmlChar *)prop));
}


/**
 * @brief 获得一个节点属性 返回数值
 *
 * @param node 节点的指针
 * @param propName 节点属性的名字
 * @param prop [out] 输出的数字 DWORD &value
 * @param propSize 数字的长度 sizeof(DWORD)
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::getNodePropNum(const xmlNodePtr node,const char * propName,void * prop, int propSize)
{
	char * temp = NULL;
	bool ret = true;
	if (node == NULL || prop==NULL || propName == NULL) return false;

	temp = (char *)xmlGetProp(node,(const xmlChar *)propName);
	if (temp == NULL) return false;

	switch(propSize)
	{
		case sizeof(BYTE):
			*(BYTE*)prop = (BYTE)atoi(temp);
			break;
		case sizeof(WORD):
			*(WORD*)prop = (WORD)atoi(temp);
			break;
		case sizeof(DWORD):
			*(DWORD*)prop = atoi(temp);
			break;
//		case sizeof(QWORD):
//			*(QWORD*)prop = atoll(temp);
//			break;
		default:
			ret = false;
	}
	if(temp) xmlFree(temp);

	return ret;
}


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




bool aXMLReader::getNodePropStr(const xmlNodePtr node,const char * propName,void *prop,int propSize)
{
	char * temp = NULL;
	bool ret = true;
	if (node == NULL || prop==NULL || propName == NULL) return false;

	temp = (char *)xmlGetProp(node,(const xmlChar *)propName);
	if (temp == NULL) return false;

	unsigned char * out = charConv((unsigned char *)temp,"UTF-8",(const char *)_doc->encoding);
	if (out)
	{
		bzero(prop,propSize);
		strncpy((char *)prop,(const char *)out,propSize - 1);
		SAFE_DELETE_VEC(out);
	}
	if(temp) xmlFree(temp);
	return ret;
}



/**
 * @brief 获得一个节点属性 返回string 对象
 *
 * @param node 节点的指针
 * @param propName 节点属性的名字
 * @param prop [out] 输出的string对象
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::getNodePropStr(const xmlNodePtr node,const char * propName,std::string &prop)
{
	char * temp = NULL;
	bool ret = true;
	if (node == NULL || propName == NULL) return false;

	temp = (char *)xmlGetProp(node,(const xmlChar *)propName);
	if (temp == NULL) return false;

	unsigned char * out = charConv((unsigned char *)temp,"UTF-8",(const char *)_doc->encoding);
	if (out)
	{
		prop = (char *)out;
		SAFE_DELETE_VEC(out);
	}
	if(temp) xmlFree(temp);
	return ret;
}


/**
 * @brief 获得一个节点属性 返回时间
 *
 * @param node 节点的指针
 * @param propName 节点属性的名字
 * @param prop [out] 输出的时间
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::getNodePropTime(const xmlNodePtr node,const char * propName,time_t &prop)
{
	std::string str;
	if (!getNodePropStr(node,propName,str))
		return false;
	tm tv;
	time_t theTime = 0;
	if(NULL == strptime(str.c_str(),"%Y%m%d:%H%M%S",&tv))
		return false;
	theTime = timegm(&tv);

	if ((time_t)-1 == theTime)
		return false;
	prop = theTime - 8*60*60;//UTC 时间
	return true;
}


/**
 * @brief 获得一个节点内容 返回时间
 *
 * @param node 节点的指针
 * @param prop [out] 输出的时间
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::getNodeContentTime(const xmlNodePtr node,time_t &prop)
{
	std::string str;
	if (!getNodeContentStr(node,str))
		return false;
	tm tv;
	time_t theTime = 0;
	if(NULL == strptime(str.c_str(),"%Y%m%d:%H%M%S",&tv))
		return false;
	theTime = timegm(&tv);

	if ((time_t)-1 == theTime)
		return false;
	prop = theTime - 8*60*60;//UTC 时间
	return true;
}


/**
 * @brief 获得一个节点内容 返回数值
 *
 * @param node 节点的指针
 * @param content [out] 输出的数值
 * @param contentSize 数值的长度
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::getNodeContentNum(const xmlNodePtr node,void * content,int contentSize)
{
	char * temp = NULL;
	bool ret = true;
	if (node == NULL || content == NULL ) return false;

	xmlNodePtr text = node->children;

	while(text != NULL)
	{
		if (!xmlStrcmp(text->name,(const xmlChar *)"text"))
		{
			temp = (char *)text->content;
			break;
		}
		text = text->next;
	}

	if (temp == NULL) return false;

	switch(contentSize)
	{
		case sizeof(BYTE):
			*(BYTE*)content = (BYTE)atoi(temp);
			break;
		case sizeof(WORD):
			*(WORD*)content = (WORD)atoi(temp);
			break;
		case sizeof(DWORD):
			*(DWORD*)content = atoi(temp);
			break;
//		case sizeof(QWORD):
//			*(QWORD*)content = atoll(temp);
//			break;
		default:
			ret = false;
	}
	return ret;
}


/**
 * @brief 获得一个节点内容 返回一个字符串
 *
 * @param node 节点的指针
 * @param content [out] 输出的字符串指针
 * @parrm contentSize 字符串指针的长度
 *
 * @return true 成功 false 失败
 */

bool aXMLReader::getNodeContentStr(const xmlNodePtr node,void *content , int contentSize)
{
	char * temp = NULL;
	bool ret = true;
	if (node == NULL || content == NULL) return false;

	xmlNodePtr text = node->children;

	while(text != NULL)
	{
		if (!xmlStrcmp(text->name,(const xmlChar *)"text"))
		{
			temp = (char *)text->content;
			break;
		}
		text = text->next;
	}

	if (temp == NULL) return false;

	unsigned char * out = charConv((unsigned char *)temp,"UTF-8",(const char *)_doc->encoding);
	if (out)
	{
		bzero(content,contentSize);
		strncpy((char *)content,(const char *)out,contentSize);
		SAFE_DELETE_VEC(out);
	}

	return ret;

}


/**
 * @brief 获得一个节点内容 返回一个string对象
 *
 * @param node 节点的指针
 * @param content [out] 输出的string 对象
 *
 * @return true 成功 false 失败
 */


bool aXMLReader::getNodeContentStr(const xmlNodePtr node,std::string &content)
{
	char * temp = NULL;
	bool ret = true;
	if (node == NULL ) return false;

	xmlNodePtr text = node->children;

	while(text != NULL)
	{
		if (!xmlStrcmp(text->name,(const xmlChar *)"text"))
		{
			temp = (char *)text->content;
			break;
		}
		text = text->next;
	}

	if (temp == NULL) return false;

	unsigned char * out = charConv((unsigned char *)temp,"UTF-8",(const char *)_doc->encoding);
	if (out)
	{
		content = (char *)out;
		SAFE_DELETE_VEC(out);
	}
	return ret;
}


/**
 * @brief 字符编码转换
 *
 * @param in 输入的字符串
 * @param fromEncording 输入字符串的编码格式
 * @param toEncoding 输出编码的编码格式
 * 
 * @return 转换后字符串的指针
 */


unsigned char * aXMLReader::charConv(unsigned char *in,const char * fromEncording,const char *toEncoding)
{
	unsigned char *out;
	size_t ret,size,out_size;
	size = strlen((char *)in);
	out_size = size*2 + 1;
	out = new unsigned char[out_size];
	bzero(out,out_size);
	if (out)
	{
		if (fromEncording != NULL && toEncoding != NULL)
		{
			iconv_t icv_in = iconv_open(toEncoding,fromEncording);
			if ( (iconv_t)-1 == icv_in)
			{
				SAFE_DELETE_VEC(out);
				out = NULL;
			}
			else
			{
				char * fromtemp = (char *)in;
				char * totemp = (char *)out;
				size_t tempout = out_size - 1;
				ret = iconv(icv_in,&fromtemp,&size,&totemp,&tempout);
				if ((size_t)-1 == ret)
				{
					SAFE_DELETE_VEC(out);
					out = NULL;
				}
				iconv_close(icv_in);
			}
		}else
			strncpy((char *)out,(char *)in,size);
	}
	return (out);
}

