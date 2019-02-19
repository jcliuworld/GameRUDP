#ifndef _aFUNC_H__
#define _aFUNC_H__

/** 
* @file aFunc.h
* @brief 公共包含文件 用于包含公共头文件信息 及 全局的变量和函数
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/
#include <stdio.h>
#include <stdarg.h>
#include <strings.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <ext/hash_map>
#include <functional>

#include "aDef.h"
/** 
* @brief Age 名称空间
*/

namespace Age
{
	/** 
	 * @brief 随机种子
	 */

	extern __thread unsigned int seedp;
	/** 
	 * @brief 随机函数封装
	 */

	class aMisc
	{
		public:
			/**
			 *
			 * @brief 随机数生成器
			 *
			 * \pararm min 最小值 max最大值
			 *
			 * \return 随机结果 
			 */
			static int randBetween(int min,int max)
			{
				if(min == max)
					return min;
				else if( min > max)
					return max + (int)(((double)min - (double)max + 1.0)*rand_r(&seedp)/(RAND_MAX + 1.0));
				else
					return min + (int)(((double)max - (double)min + 1.0)*rand_r(&seedp)/(RAND_MAX + 1.0));
			}

			//获取几分之的几率
			static bool selectByOdds(const DWORD upNum,const DWORD downNum)
			{
				DWORD m_rand;
				if (downNum < 1) return false;
				if (upNum < 1) return false;
				if (upNum > downNum - 1) return true;
				m_rand = 1 + (DWORD) ((double)downNum * rand() / (RAND_MAX + 1.0));
				return (m_rand <= upNum);
			}

			struct odds_t
			{
				DWORD upNum;
				DWORD downNum;
			};

			//获取几分之几的几率
			static bool selectByt_Odds(const odds_t &odds)
			{
				return selectByOdds(odds.upNum,odds.downNum);
			}

			//获取百分之的几率
			static bool selectByPercent(const DWORD percent)
			{
				return selectByOdds(percent,100);
			}

			//获取万分之的几率
			static bool selectByTenTh(const DWORD tenth)
			{
				return selectByOdds(tenth,10000);
			}

			//获取十万分之的几率
			static bool selectByLakh(const DWORD lakh)
			{
				return selectByOdds(lakh,100000);
			}

			//获取亿分之之的几率
			static bool selectByOneHM(const DWORD lakh)
			{
				return selectByOdds(lakh,100000000);
			}
	};

	/** 
	 * @brief 大小写转换
	 */

	struct ToLower 
	{
		char operator()(char c)const
		{
			return std::tolower(c);
		}
	};

	/** 
	 * @brief 将输入字符全部转成小写
	 * 
	 * @param s [in][out]
	 */

	inline void toLower(std::string &s)
	{
		std::transform(s.begin(),s.end(),s.begin(),ToLower());
	}

	/** 
	 * @brief 转换成大写
	 */
	struct ToUpper 
	{
		char operator()(char c)const
		{
			return std::toupper(c);
		}
	};

	/**
	 * @brief 将输入字符全部转成大写
	 *
	 * @prarm s [in][out]
	 */

	inline void toUpper(std::string &s)
	{
		std::transform(s.begin(),s.end(),s.begin(),ToUpper());
	}

	/** 
	 * @brief 分割字符串
	 * 
	 * @param container [out] 输出结果 应为容器且支持push_back()方法
	 * @param in 输入
	 * @param dellimiters 分隔符
	 * @param deep  结果数量， 0 为未限制
	 * 
	 * @return 
	 */

	template <typename Container>
		inline void stringbreak(Container &container,std::string const &in,const char * const dellimiters = "\t\n",const int deep = 0)
		{
			const std::string::size_type len = in.length();
			std::string::size_type i = 0;

			int count = 0;
			while(i<len)
			{
				i = in.find_first_not_of(dellimiters,i);
				if (i == std::string::npos)
				{
					return;
				}

				std::string::size_type j = in.find_first_of(dellimiters,i);

				count ++;
				if (j == std::string::npos || (deep > 0 && count > deep))
				{
					container.push_back(in.substr(i));
					return;
				}
				else
					container.push_back(in.substr(i,j-i));

				i = j + 1;

			}
		}

	/**
	 * @brief 字符串hash函数 
	 */

	inline unsigned int strHash(const char *str)
	{
		register unsigned int   h;
		register unsigned char * p;
		for (h = 0, p = (unsigned char *)str ; *p ; p++)
			h = (31 * h) + *p;
		return h;
	}
	inline unsigned int hashAccount(const char* __s)
	{
		unsigned int __h = 0;
		for ( ; *__s; ++__s)
			__h = 5*__h + *__s;
		return __h;
	}
	struct key_hash : public std::unary_function<const std::string , size_t>
	{
		size_t operator()(const std::string & x)const
		{
			std::string s = x;
			__gnu_cxx::hash<const char * >H;
			toLower(s);
			return H(s.c_str());	
		}
	};
	struct key_equal : public std::binary_function<const std::string , const std::string , bool>
	{
		bool operator()(const std::string & s1,const std::string &s2)const
		{
			return strcasecmp(s1.c_str(),s2.c_str()) == 0;
		}
	};


	static int isInitCRC = 0;
		
	static  int CRCTable[256];

	inline void initCRCTable()
	{
		for (unsigned int i = 0; i < 256; i++) {
			unsigned  int crc = i;
			for (unsigned int j = 0; j < 8; j++) {

				crc=(crc & 1)?(crc >> 1) ^ 0xEDB88320:(crc >> 1);
			}
			CRCTable[i]=crc;
		}

	}

	inline unsigned int getCRC(const char * buff,size_t len)
	{
		if(isInitCRC == 0)
		{	
			isInitCRC = 1;
			initCRCTable();
		}
		unsigned int iRet = 0;
		unsigned int crc = ~iRet;
		for (unsigned int  i= 0 ; i < len; i++) {
			crc = CRCTable[(crc ^ buff[i]) & 0xFF] ^ (crc >> 8);
		}
		iRet = ~crc;

		return  iRet & 0xFFFFFFFF;

	}	
	inline void delSpace(std::string &in)
	{
		int begin = 0;

		begin = in.find(" ",begin); 

		while(begin != -1)
		{
			in.replace(begin, 1, "");
			begin = in.find(" ",begin);
		}		
	}

	inline std::string strFormat(const char * pattern,...)
	{
		char msg[4096];
			
		int msglen = 4096;

		do 
		{ 
			va_list ap; 
			bzero(msg,msglen);
			va_start(ap,pattern);
			vsnprintf(msg , msglen - 1, pattern ,ap);
			va_end(ap);
		}while(false);

		return std::string(msg);

	}


	template<typename R = bool, typename... Args>
		class aFuncion {
			public:
				aFuncion(std::function<R(Args...)> fun) : _fun(fun) {
				}

				R operator()(Args... args) {
					return _fun(args...);
				}
			private:
				std::function<R(Args...) > _fun;
		};
};

#endif


