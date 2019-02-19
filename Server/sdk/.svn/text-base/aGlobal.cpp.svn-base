#include "aGlobal.h"
/** 
 * @brief 解析以=分割的全局变量
 * 
 * @param cmdLine 变量字符串
 * 
 * @return 
 */

/**
 * @brief 获取一个全局变量的值
 * 
 * @param key key
 * 
 * @return value
 */

const std::string & aGlobal::getProperty(const std::string &key)
{
	return _values[key];
}
/**
 * @brief 设置一个全局变量的值
 * 
 * @param key key 
 * @param value value
 */

void aGlobal::setProperty(const std::string &key,const std::string &value)
{       
	_values[key] = value;
}               
/**
 * @brief 重载[] 获取一个全局变量的值 
 *              
 * @param key key
 *              
 * @return value
 */

std::string & aGlobal::operator[](const std::string &key)
{
	return _values[key];
}
/**
 * @brief 输出所有的全局变量 以=为分割符号
 *      
 * @param out 输出全局值的字符串
 */     

void aGlobal::dump(std::ostream &out)
{
	GLOBALVAL::const_iterator it;
	for (it = _values.begin(); it != _values.end(); ++it)
		out<<it->first<<"="<<it->second<<std::endl;
}
unsigned int aGlobal::parseCmdLine(const char * cmdLine)
{
	std::vector<std::string> sv;
	Age::stringbreak(sv,cmdLine);
	for (std::vector<std::string>::const_iterator it = sv.begin(); it != sv.end(); ++ it)
	{
		std::vector<std::string> ssv;
		Age::stringbreak(ssv,*it,"=",1);
		if( ssv.size() == 2)
			_values[ssv[0]] = ssv[1];
	}

	return _values.size();

}
/** 
 * @brief 解析以=分割的全局变量
 * 
 * @param cmdLine 变量字符串
 * 
 * @return 
 */


unsigned int aGlobal::parseCmdLine(const std::string & cmdLine)
{
	std::vector<std::string> sv;
	Age::stringbreak(sv,cmdLine);
	for (std::vector<std::string>::const_iterator it = sv.begin(); it != sv.end(); ++ it)
	{
		std::vector<std::string> ssv;
		Age::stringbreak(ssv,*it,"=",1);
		if( ssv.size() == 2)
			_values[ssv[0]] = ssv[1];
	}

	return _values.size();

}
