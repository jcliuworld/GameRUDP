#ifndef _aGlobal_H__
#define _aGlobal_H__

#include "aFunc.h"
#include <ext/hash_map>
/** 
* @file aGlobal.h
* @brief 全局变量管理器 
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/

class aGlobal
{
	public:
		/** 
		 * @brief 获取一个全局变量的值
		 * 
		 * @param key key
		 * 
		 * @return value
		 */

		const std::string &getProperty(const std::string &key);
		/** 
		 * @brief 设置一个全局变量的值
		 * 
		 * @param key key
		 * @param value value
		 */

		void setProperty(const std::string &key,const std::string &value);
		/** 
		 * @brief 重载[] 获取一个全局变量的值
		 * 
		 * @param key key
		 * 
		 * @return value
		 */

		std::string & operator[](const std::string &key);
		/** 
		 * @brief 输出所有的全局变量 以=为分割符号
		 * 
		 * @param out 输出全局值的字符串
		 */

		void dump(std::ostream &out);
		/** 
		 * @brief 解析以=分割的全局变量
		 * 
		 * @param cmdLine 变量字符串
		 * 
		 * @return 
		 */

		unsigned int parseCmdLine(const std::string & cmdLine);
		/** 
		 * @brief 解析以=分割的全局变量
		 * 
		 * @param cmdLine 变量字符串
		 * 
		 * @return 
		 */

		unsigned int parseCmdLine(const char * cmdLine);

	private:
		/** 
		 * @brief 重载比较方法
		 */

		struct key_hash:public std::unary_function<const std::string ,size_t>
		{
			size_t operator()(const std::string &x)const
			{
				std::string s = x;

				__gnu_cxx::hash<const char *> H;
				using namespace Age;
				toLower(s);
				return H(s.c_str());
			}
		};
		/** 
		 * @brief 重载比较方法
		 */

		struct key_equal :public std::binary_function<const std::string , const std::string ,bool>
		{
			bool operator()(const std::string &s1,const std::string &s2)const
			{
				return strcasecmp(s1.c_str(),s2.c_str()) == 0;
			}
		};
		/** 
		 * @brief hash_map  容器定义
		 */

		typedef __gnu_cxx::hash_map<std::string,std::string,key_hash,key_equal> GLOBALVAL;
		/** 
		 * @brief 全局变量容器
		 */

		GLOBALVAL _values;

};


#endif
