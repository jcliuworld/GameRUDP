#ifndef _aTable_H__
#define _aTable_H__

/** 
 * @file aTable.h
 * @brief 数据库表格 读写处理类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-17
 */

#include <string.h>
#include <map>
#include <vector>
#include <sstream>
#include <stdlib.h>
//#include <strings.h>

#include "aDef.h"
class aMysqlDB;
/** 
 * @brief 数据库变量封装 此对象可存储任意数据类型
 */
class DBVARTYPE
{

	friend class aMysqlDB;
	public:
		DBVARTYPE():_bData(1,'\0')
		{
		}

		DBVARTYPE(const DBVARTYPE & bData): _bData(bData._bData)
		{
		}
	
		~DBVARTYPE(){}
		template <typename X> 
			explicit DBVARTYPE(const X & bdata)
			{
				this->put<X>(bdata);
			}

		template <typename X>
			void put( const X & bdata)
			{
				std::ostringstream oss;
				oss << bdata;
				_bData.resize(oss.str().length()+1);
				::strncpy((char *)(&(this->_bData[0])),oss.str().c_str(),oss.str().length());	
			}

		void put( const void *  data, const unsigned int len)
		{
			_bData.resize(len+1);
			if(len)
			{
				bcopy(data,&_bData[0],len);
			}
			_bData[len]='\0';
		}
		template <const void *>
			inline void put(const void * bdata);


		inline	operator unsigned short()const
		{
			return atoi((const char *)(*this));
		}
		inline  operator short()const
		{
			return atoi((const char *)(*this));
		}
		inline	operator unsigned int () const
		{
			return atoi((const char *)(*this));
		}
		inline	operator int () const
		{
			return atoi((const char *)(*this));
		}
		inline	operator unsigned long()const
		{
			return strtol((const char *)(*this),NULL,10);
		}
		inline	operator long()const
		{
			return strtol((const char *)(*this),NULL,10);
		}
		inline	operator unsigned long long ()const
		{
			return strtoll((const char *)(*this),NULL,10);
		}
		inline	operator long long()const
		{
			return strtoll((const char *)(*this),NULL,10);
		}
		inline	operator float()const
		{
			return atof((const char *)(*this));
		}
		inline	operator double()const
		{
			return atof((const char *)(*this));
		}
		inline	operator unsigned char ()const
		{
			return atoi((const char *)(*this));
		}
		inline	operator char ()const
		{
			return atoi((const char *)(*this));
		}
		inline	operator const char *()const
		{
			return (const char *)(&_bData[0]);
		}

		inline	operator const void *()const
		{
			return (const void *)(&_bData[0]);
		}

		inline unsigned int size()const
		{
			if(_bData.empty())
				return 0 ;
			else
				return _bData.size()-1;
		}
		inline bool empty()const
		{
			return _bData.empty()|| (1== _bData.size());
		}	
		const DBVARTYPE & operator = (const DBVARTYPE & bdata)
			{
				_bData = bdata._bData;
			return *this;
		}
		template <typename X>
		const DBVARTYPE & operator = (const X & bdata)
		{
			put(bdata);
			return *this;
		}
	private:
		std::vector<unsigned char > _bData;

};
/** 
 * @brief 数据库的一行记录
 */

class aRecord
{
	public:
		/** 
		 * @brief 构造函数
		 */

		aRecord(){};
		/** 
		 * @brief 析构函数
		 */

		~aRecord(){clear();}

		/** 
		 * @brief 插入一列记录 
		 * 
		 * @param colName 列名  
		 * @param value 列值
		 */
		 
        	friend class aMysqlDB;

		template <typename T>
		inline void put(std::string colName,const T& value)
		{
			if(!colName.size())
				return;
			std::ostringstream oss;
			oss << value;
			_record.insert(std::make_pair(colName,DBVARTYPE(oss.str())));
		}
		/** 
		 * @brief 插入一列记录 
		 * 
		 * @param colName 列名  
		 * @param value 列值
		 */

		inline void put(std::string colName,const unsigned char & value)
		{
			if(!colName.size())
				return;
			std::ostringstream oss;
			oss << (unsigned int)value;
			_record.insert(std::make_pair(colName,DBVARTYPE(oss.str())));
		}
		/** 
		 * @brief 插入一列记录 
		 * 
		 * @param colName 列名  
		 * @param value 列值
		 */

		inline void put(std::string colName,const  char & value)
		{
			if(!colName.size())
				return;
			std::ostringstream oss;
			oss << (int)value;
			_record.insert(std::make_pair(colName,DBVARTYPE(oss.str())));
		}
		/** 
		 * @brief 插入一列记录 
		 * 
		 * @param colName 列名  
		 * @param value 列值
		 */

		inline void put(std::string colName,const  void  *value,const unsigned int  size)
		{
			if(!colName.size())
				return;
			DBVARTYPE var;
			var.put(value,size);
			_record.insert(std::make_pair(colName,var));
		}
		/** 
		 * @brief 获取一个列的记录
		 * 
		 * @param colName 列名
		 * 
		 * @return 列值
		 */

		inline DBVARTYPE& get(const std::string &colName)
		{
			return _record[colName];
		}
		/** 
		 * @brief 获取列的数量
		 * 
		 * @return 列的数量
		 */

		inline size_t getSize() const
		{
			return _record.size();
		}
		/** 
		 * @brief 清除所有列
		 */

		inline void clear()
		{
			_record.clear();
		}
		inline DBVARTYPE & operator [](const std::string &name)
		{
			return this->get(name);
		}

		inline DBVARTYPE &operator[](const unsigned int index)
		{
			DBVARTYPE *ret;
			RECORD_IT it;
			unsigned int i = 0;
			for (it = _record.begin(); it != _record.end(); ++i , ++it)
			{
				if (index == i)
				{
					ret =&(it->second);
					break;
				}
			}

			return *ret;
		}
	
		typedef std::map<std::string,DBVARTYPE> RECORD;
                typedef std::map<std::string,DBVARTYPE>::iterator RECORD_IT;
	private:

		RECORD _record;

};
/** 
 * @brief 多行数据库记录
 */

class aRecordSet
{
	public:	
		/** 
		 * @brief 构造函数
		 */
		aRecordSet(){};
		/** 
		 * @brief 析构函数
		 */
		~aRecordSet(){
			for( std::vector<aRecord *>::iterator it = _recordSet.begin(); it != _recordSet.end(); ++ it)
			{
				SAFE_DELETE(*it)
			}
		};
		/** 
		 * @brief 插入一行
		 * 
		 * @param rec 一行
		 */

		inline void put(aRecord * rec)
		{
			if (rec)
			{
				_recordSet.push_back(rec);
			}
		}
		/** 
		 * @brief 获取一行
		 * 
		 * @param index 一行
		 * 
		 * @return 
		 */

		inline aRecord * get(const unsigned int index)
		{
			if (index < _recordSet.size())
			{
				return _recordSet[index];
			}
			return NULL;
		}

		inline aRecord * operator [](const unsigned int index)
		{
			return this->get(index);
		}

		inline void clear()
		{
			_recordSet.clear();
		}

		inline bool empty(){return _recordSet.empty();}
		/** 
		 * @brief 获取行数
		 * 
		 * @return 行数
		 */

		inline unsigned int size()
		{
			return _recordSet.size();
		}

	private:
		std::vector<aRecord *> _recordSet;
};
#endif

