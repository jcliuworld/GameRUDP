#ifndef _aSerialization_H__
#define _aSerialization_H__
/** 
* @file aSerialization.h
* @brief 序列化对象管理器
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-14
*/
#include <string>
#include <fstream>
#include "Age.h"
#include "sdkL.h"
#include "aRWLock.h"
#include "aEntryM.h"
#include "aDef.h"
/** 
* @brief 序列化对象回调处理 
*/

#define DATA_BUFFER_SIZE 1024*64*60
template < class T >
class DataCallBack
{
	public:
		virtual bool exec(T * entry)=0;
		virtual ~DataCallBack(){};
		
};
typedef aEntryM<aEntryID,aMultiEntryName> aDataM;
/** 
* @brief 序列化对象管理 
*/

template <typename DATA>
class DataSerialization : public aDataM
{
	private:
		/** 
		 * @brief 单件模式 对象实例
		 */

		static DataSerialization<DATA> * _instance;
		/** 
		 * @brief 读写锁
		 */

		aRWLock rwlock;
		/** 
		 * @brief 构造函数
		 */

		DataSerialization(){};
		/** 
		 * @brief Entry 删除回调
		 */

		public:
		class DataEntry : public aEntry,public DATA
		{
			friend class DataSerialization;
		};	
		class delEvery:public DataCallBack<DataEntry>
		{
			public:
				bool exec( DataEntry * entry)
				{
					delete entry;
					return true;
				}
		};


		/** 
		 * @brief 析构函数
		 */

		~DataSerialization()
		{
			delEvery temp;
			execAll(temp);
			rwlock.wrlock();
			clear();
			rwlock.unlock();
		};
		/** 
		 * @brief 通过ID获取一个Entry
		 * 
		 * @param id Entry ID
		 * 
		 * @return Entry 指针
		 */


		aEntry * getEntryByID(DWORD id)const
		{
			aEntry * ret = NULL;
			aEntryID::find(id,ret);
			return ret;
		};
		/** 
		 * @brief 移除一个Entry
		 * 
		 * @param id Entry ID
		 */


		void removeEntryByID(DWORD id)
		{
			aEntry * ret = NULL;
			if(aEntryID::find(id,ret))
				removeEntry(ret);
		};
		/** 
		 * @brief 通过名字获取一个Entry
		 * 
		 * @param name Entry name
		 * 
		 * @return Entry指针
		 */

		aEntry * getEntryByName(const char * name)
		{
			aEntry * ret = NULL;
			aMultiEntryName::find(name,ret,true);
			return ret;
		}
		/** 
		 * @brief 通过name移除一个Entry
		 * 
		 * @param name Entry name
		 */

		void removeEntryByName(const char * name){
			aEntry * ret = NULL;
			if(aMultiEntryName::find(name,ret))
				removeEntry(ret);
		};
		/** 
		 * @brief 加载一个序列化对象
		 * 
		 * @return 成功 true 失败 false 
		 */

		bool readAll(const char * fileName )
		{
			std::ifstream file;
			file.open(fileName,std::ios::in | std::ios::binary );

			if(!file.is_open())
			{
				Age::logger->debug(SYS_SDK_ID_000054 , fileName);
				return false;
			}
			//注意支持640KB大小以下的资源文件 ， 如果超过请修改buf大小的宏定
			delEvery temp;
			execAll(temp);
			clear();
			char *buf = new char[DATA_BUFFER_SIZE];
			bzero(buf,DATA_BUFFER_SIZE);

			file.read(buf,DATA_BUFFER_SIZE);

			char * bengin = buf;
			char * current = buf;

			unsigned int allsize = *((int *)current);

			current += sizeof(int);

			unsigned int headsize =  *((int *)current);

			current += sizeof(int);

			char type_name[MAX_NAMESIZE];
			bzero(type_name,MAX_NAMESIZE);

			strncpy( type_name , current , headsize );

			current += headsize;
			while(true)
			{
				if( (current - bengin) >= allsize || (current - bengin) >= DATA_BUFFER_SIZE  )
					break;

				int dataLen = *((int *)current);
				current += sizeof(dataLen);
				DataEntry  * data = new DataEntry();
			
				if(!data->ParseFromArray(current,dataLen))
				{
					Age::logger->error("[protobuf]序列化解析错误 %s %d",data->strname().c_str(),data->dwid());
				}

				current += dataLen;
				data->id = data->dwid();
				strncpy(data->name,data->strname().c_str(),MAX_NAMESIZE);
				addEntry(data);
			}
			//listAll();
			Age::logger->debug(SYS_SDK_ID_000055,fileName);
			SAFE_DELETE_VEC(buf)
			return true;
		};
		/** 
		 * @brief 写序列化对象
		 * 
		 * @return 成功 true 失败 false
		 */

		bool writeAll(){
			 return true;
		};
		/** 
		 * @brief 单件模式 获取实例
		 * 
		 * @return DataSerialization 实例
		 */

		static DataSerialization & getInstance()
		{
			if(NULL == _instance)
				_instance = new DataSerialization();
			return *_instance;
		} 	
		/** 
		 * @brief 获取数据 通过id
		 * 
		 * @param id 
		 * 
		 * @return DATA *
		 */

		DATA * get(DWORD id){
			rwlock.rdlock();
			DATA * ret = (DATA * )getEntryByID(id);
			rwlock.unlock();
			return ret;
		};
		/** 
		 * @brief 获取数据 通过名字
		 * 
		 * @param name 
		 * 
		 * @return DATE *
		 */

		DATA * get(const char * name){
			rwlock.rdlock();
			DATA * ret = (DATA * )getEntryByName(name);
			rwlock.unlock();
			return ret;
		};
		/** 
		 * @brief 回调循环
		 * 
		 * @param base 回调方法 
		 */

		void execAll( DataCallBack<DataEntry> & base){
			rwlock.rdlock();
			for (aEntryID::hashmap::iterator it = aEntryID::ets.begin(); it != aEntryID::ets.end(); ++ it)
			{
				if(!base.exec((DataEntry *)it->second))
				{
					rwlock.unlock();
					return;
				}
			}
			rwlock.unlock();
		};
		/** 
		 * @brief 列印所有数据
		 */

		void listAll(){

			class ListEvery : public  DataCallBack<DataEntry>
			{
				public:
					int i;
					ListEvery()
					{
						i = 0;
					}
					bool exec(DataEntry * entry)
					{
						++ i;
						Age::logger->debug("%ld\t %s",entry->id,entry->name);
						return true;
					}
			};
			
			ListEvery one;
			execAll(one);
		};
};
#endif


