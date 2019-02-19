#ifndef _aEntryM_H__
#define _aEntryM_H__

/** 
* @file aEntryM.h
* @brief Entry管理器
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/
 #include <string.h>
#include <ext/hash_map>
#include "aEntry.h"
#include "aFunc.h"
/** 
* @brief key值是否相等比较 
*/

template <class keyT>
struct my_key_queal : public std::binary_function<keyT,keyT,bool>
{
	inline bool operator()(const keyT s1,const keyT s2)const;
};
/** 
* @brief 模板片特化 char *
* 
* @return 相等 true 不等 false
*/

template <>
inline bool my_key_queal<const char *>::operator ()(const char *  s1,const char * s2)const
{
	return strcmp(s1,s2) == 0;
}
/** 
* @brief 模板偏特化 DWORD
* 
* @return 相等 true 不等 false
*/

template<>
inline bool my_key_queal<DWORD>::operator ()(const DWORD  s1,const DWORD s2)const
{
	return s1 == s2;
}
/** 
* @brief 模板偏特化 QWORD
* 
* @return 相等 true 不等 false
*/

template<>
inline bool my_key_queal<QWORD>::operator ()(const QWORD  s1,const QWORD s2)const
{
	return s1 == s2;
}
/** 
* @brief 有限桶HASH模板，非线程安全 
* 
* @param keyT key类型支持DWORD char * 
* @param valueT value类型不做限制但是需要 有拷贝构造函数
* 
* @return 
*/
template <class keyT,class valueT>
class LimitHash:private aNoneCopyable
{
	protected:
		/** 
		 * @brief hash_map 容器封装
		 */

		typedef __gnu_cxx::hash_map<keyT,valueT,__gnu_cxx::hash<keyT>,my_key_queal<keyT> > hashmap;
		/** 
		 * @brief hash_map 迭代器封装
		 */

		typedef typename hashmap::iterator iter;
		/** 
		 * @brief hash_map 常量迭代器封装
		 */
		typedef typename hashmap::const_iterator const_iter;
		/** 
		 * @brief Entry容器
		 */

		hashmap ets;
		/** 
		 * @brief 插入一个Entry 
		 * 
		 * @param key  key
		 * @param value value
		 * 
		 * @return 成功 ture
		 */

		inline bool insert(const keyT &key,valueT &value)
		{
			ets[key] = value;
			return true;
		}
		/** 
		 * @brief 查找一个Entry 
		 * 
		 * @param key 需要查找的Entry的key 
		 * @param value 返回Entryvalue
		 * 
		 * @return 成功 true 失败 false
		 */

		inline bool find(const keyT &key,valueT &value)const
		{
			const_iter it = ets.find(key);
			if ( it != ets.end())
			{
				value = it->second;
				return true;
			}
			else
				return false;
		}
		/** 
		 * @brief 获取第一个Entry
		 * 
		 * @param value 返回第一个Entry的value
		 * 
		 * @return 成功 true 失败 false
		 */


		inline bool findOne(const valueT &value)const
		{
			if (!ets.end())
			{
				value = ets.begin()->second;
				return true;
			}
			return false;
		}
		/** 
		 * @brief 构造函数
		 */

		LimitHash(){}
		/** 
		 * @brief 析构函数
		 */

		~LimitHash(){clear();}
		/** 
		 * @brief 根据key 移除一个Entry
		 * 
		 * @param key EntryID
		 */

		inline void remove(const keyT &key)
		{
			ets.erase(key);
		}
		/** 
		 * @brief 清除所有Entry
		 */

		inline void clear()
		{
			ets.clear();
		}
		/** 
		 * @brief 获取Entry数量
		 * 
		 * @return Entry数量
		 */

		inline unsigned int size()
		{
			return ets.size();
		}
		/** 
		 * @brief Entry管理器是否为空
		 * 
		 * @return 是 true 否 false
		 */

		inline bool empty()const
		{
			return ets.empty();
		}

};
/** 
* @brief 有限桶MULTI HASH模板，非线程安全 
* 
* @param keyT key类型支持DWORD char * 
* @param valueT value类型不做限制但是需要 有拷贝构造函数
* 
* @return 
*/

template <class keyT,class valueT>
class MultiHash:private aNoneCopyable
{
	protected:

		/** 
		 * @brief hash_multimap 容器封装
		 */
		typedef __gnu_cxx::hash_multimap<keyT,valueT,__gnu_cxx::hash<keyT>,my_key_queal<keyT> > hashmap;
		/** 
		 * @brief hash_multimap 迭代器封装
		 */

		typedef typename hashmap::iterator iter;
		/** 
		 * @brief hash_multimap 常量迭代器封装
		 */

		typedef typename hashmap::const_iterator const_iter;
		/** 
		 * @brief Entry容器 
		 */


		hashmap ets;
		/** 
		 * @brief 插入一个Entry
		 * 
		 * @param key EntryID 
		 * @param value Entry值
		 * 
		 * @return 成功 true 
		 */

		inline bool insert(const keyT &key,valueT &value)
		{
			ets.insert(std::pair<keyT,valueT>(key,value));
			return true;
		}
		/** 
		 * @brief 构造函数
		 */

		MultiHash(){}
		/** 
		 * @brief 析构函数
		 */

		~MultiHash(){clear();}
		/** 
		 * @brief 移除一个Entry
		 * 
		 * @param key EntryID
		 */

		inline void remove(const keyT &key)
		{
			ets.erase(key);
		}
		/** 
		 * @brief 清除Entry容器
		 */

		inline void clear()
		{
			ets.clear();
		}
		/** 
		 * @brief 获取Entry容器大小
		 * 
		 * @return Entry容器大小
		 */

		inline unsigned int size()
		{
			return ets.size();
		}
		/** 
		 * @brief Entry容器是否为空
		 * 
		 * @return 是 true 否 false
		 */

		inline bool empty()const
		{
			return ets.empty();
		}

		/** 
		 * @brief 查找一个Entry 
		 * 
		 * @param key 需要查找的Entry的key 
		 * @param value 返回Entryvalue
		 * 
		 * @return 成功 true 失败 false
		 */

		inline bool find(const keyT &key,valueT &value)const
		{
			const_iter it = ets.find(key);
			if ( it != ets.end())
			{
				value = it->second;
				return true;
			}
			else
				return false;
		}

};
/** 
 * @brief 以临时ID为key的Entry 指针容器 ，需要继承使用
 */

class aEntryTempID:public LimitHash<DWORD,aEntry *>
{

	protected:
		/** 
		 * @brief 构造函数
		 */

		aEntryTempID(){}
		/** 
		 * @brief 析构函数
		 */

		virtual ~aEntryTempID(){}
		/** 
		 * @brief 将Entry加入到容器中 临时ID重复失败
		 * 
		 * @param e 要被加入的Entry
		 * 
		 * @return 成功 true 失败 false
		 */

		inline bool push(aEntry * e)
		{
			if (e != NULL && getUniqueID(e->tempid))
			{
				aEntry * temp;
				if (!find(e->tempid,temp))
				{
					if (insert(e->tempid,e))
					{
						return true;
					}

					putUniqueID(e->tempid);
				}
				return false;
			}
			return false;
		}
		/** 
		 * @brief 移除一个 Entry
		 * 
		 * @param e 要被移除的Entry
		 */

		inline void remove(aEntry * e)
		{
			if (e != NULL)
			{
				putUniqueID(e->tempid);
				LimitHash<DWORD,aEntry*>::remove(e->tempid);
			}
		}
		/** 
		 * @brief 通过临时ID获取Entry
		 * 
		 * @param tempid 要获取的临时ID
		 * 
		 * @return Entry 指针 未找到返回NULL
		 */

		inline aEntry * getEntryByTempID(const  DWORD tempid)const
		{
			aEntry * ret = NULL;
			LimitHash<DWORD,aEntry * >::find(tempid,ret);

			return ret;
		}
		/** 
		 * @brief 得到一个临时ID
		 * 
		 * @param tempID 得到的ID
		 * 
		 * @return 成功 true 失败 false
		 */

		virtual bool getUniqueID(DWORD &tempID) = 0 ;
		/** 
		 * @brief 释放一个临时ID
		 * 
		 * @param tempID 要释放的ID
		 */

		virtual void putUniqueID(DWORD &tempID) = 0 ;
};
/** 
 * @brief 以EntryID为key的Entry 指针容器 ，需要继承使用
 */

class aEntryID:public LimitHash<DWORD,aEntry *>
{

	protected:
		/** 
		 * @brief 将Entry放入容器
		 * 
		 * @param e 要放入的Entry
		 * 
		 * @return 成功 true 失败false
		 */

		inline bool push(aEntry *& e)
		{
			if (e != NULL )
			{
				aEntry * temp;
				if (!find(e->id,temp))
					return insert(e->id,e);
				else
					return false;
			}
			return false;
		}
		/** 
		 * @brief 移除一个 Entry
		 * 
		 * @param e 要被移除的Entry
		 */

		inline void remove(aEntry * e)
		{
			if (e != NULL)
			{
				LimitHash<DWORD,aEntry *>::remove(e->id);
			}
		}
		/** 
		 * @brief 通过ID获取一个Entry
		 * 
		 * @param id EntryID
		 * 
		 * @return EntryEntry 指针  未查找到返回 NULL
		 */

		inline aEntry * getEntryByID(const DWORD id)const
		{
			aEntry * ret = NULL;
			LimitHash<DWORD,aEntry *>::find(id,ret);

			return ret;
		}
};
/** 
 * @brief 以Entry名字为key的Entry 指针容器 ，需要继承使用
 */

class aEntryName:public LimitHash<const char *,aEntry *>
{
	protected:

		/** 
		 * @brief 将Entry放入容器
		 * 
		 * @param e 要放入的Entry
		 * 
		 * @return 成功 true 失败false
		 */

		inline bool push(aEntry *&e)
		{
			aEntry * temp = NULL;
			if (!find(e->name,temp))
				return insert(e->name,e);
			else
				return false;
		}
		/** 
		 * @brief 移除一个 Entry
		 * 
		 * @param e 要被移除的Entry
		 */

		inline void remove(aEntry * e)
		{
			if ( e != NULL )
			{
				LimitHash<const char * ,aEntry *>::remove(e->name);
			}
		}
		/** 
		 * @brief 通过Entry名字获取一个Entry
		 * 
		 * @param name Entry 名字
		 * 
		 * @return 
		 */

		inline aEntry * getEntryByName(const char * name) const
		{
			aEntry * ret =NULL;
			LimitHash<const char * ,aEntry *>::find(name,ret);
			return ret;
		}
		/** 
		 * @brief 通过Entry名字获取一个Entry
		 * 
		 * @param name Entry 名字
		 * 
		 * @return 
		 */


		inline aEntry * getEntryByName(const std::string &name) const
		{
			return getEntryByName(name.c_str());
		}

};

/** 
* @brief 以Entry 名字为key的multihashmap 需要继承使用
*/

class aMultiEntryName:public MultiHash<const char *,aEntry *>
{
	protected:
		/** 
		 * @brief 加入一个Entry
		 * 
		 * @param e 要加入的Entry
		 * 
		 * @return 成功 true 失败 false
		 */

		inline bool push(aEntry *e)
		{
			return insert(e->name,e);
		}
		/** 
		 * @brief 移除一个Entry
		 * 
		 * @param e 要移除的Entry
		 */

		inline void remove(aEntry * e)
		{
			__gnu_cxx::pair<iter,iter> its = ets.equal_range(e->name);

			for (iter it = its.first; it != its.second; ++it)
			{
				if (it->second == e)
				{
					ets.erase(it);
					return;
				}
			}
		}
		/** 
		 * @brief 查找一个Entry
		 * 
		 * @param name Entry 名字
		 * @param e 返回的 Entry
		 * @param r 在有多个同名Entry的时候 是否随机抽取一个 否在返回第一个
		 * 
		 * @return 成功 true 失败 false 
		 */

		inline bool find(const char * & name,aEntry * &e, const bool r = false)const
		{
			int rd = ets.count(name);
			if ( rd > 0)
			{
				int mrd = 0,j=0;
				if(r)
					mrd = Age::aMisc::randBetween(0,rd-1);
				__gnu_cxx::pair<const_iter,const_iter> its = ets.equal_range(name);
				for (const_iter it = its.first; it != its.second && j < rd; ++it,++j)
				{
					if (mrd == j)
					{
						e = it->second;
						return true;
					}
				}


			}
			return false;
		}
};

template <int i>
class aEntryNone
{
protected:
	inline bool push(aEntry * & e) {return true;}
	inline void remove(aEntry *&e){}
	inline void clear(){}

};
/**
 * @brief Entry回调遍历
 */
template <class YourEntry>
struct execEntry
{
	virtual bool exec(YourEntry * entry) = 0;
	virtual ~execEntry(){}
};
/**
 * @brief Entry删除遍历
 */

template <class YourEntry>
struct removeEntry_Pred
{
	std::vector<YourEntry *> removed;

	virtual bool isIt(YourEntry * entry) = 0;

	virtual ~removeEntry_Pred(){}
};

/** 
* @brief Entry管理器接口 应根据不同的情况继承使用 
*/

template <typename e1,typename e2=aEntryNone<1>,typename e3=aEntryNone<2> >
class aEntryM:protected e1,protected e2,protected e3
{
	protected:
		/** 
		 * @brief 添加Entry 重复会导致失败
		 * 
		 * @param e 要添加的Entry
		 * 
		 * @return 成功 true 失败 false
		 */

		inline bool addEntry(aEntry * e)
		{
			if (e1::push(e))
			{
				if (e2::push(e))
				{
					if (e3::push(e))
						return true;
					else
					{
						e2::remove(e);
						e1::remove(e);
					}
				}else
					e1::remove(e);
			}
			return false;
		}
		/** 
		 * @brief 移除一个Entry
		 * 
		 * @param e 要移除的Entry
		 */

		inline void removeEntry(aEntry * e)
		{
			e1::remove(e);
			e2::remove(e);
			e3::remove(e);
		}
		/** 
		 * @brief 析构函数
		 */

		~aEntryM(){};
		/** 
		 * @brief 管理器大小
		 * 
		 * @return 管理器大小
		 */

		inline unsigned int size()
		{
			return e1::size();
		}
		/** 
		 * @brief 是否为空
		 * 
		 * @return 是 true 否 false
		 */

		inline bool empty(void)const
		{
			return e1::empty();
		}
		/** 
		 * @brief 清除管理器中的所有Entry
		 */

		inline void clear()
		{
			e1::clear();
			e2::clear();
			e3::clear();
		}
		/** 
		 * @brief Entry 遍历回调
		 * 
		 * @param eee 处理接口
		 * 
		 * @return 全部成功 true 否则 false
		 */
		
		template <class YourEntry>
			inline bool execEveryEntry(execEntry<YourEntry> &eee)
			{
				typedef typename e1::iter my_iter;

				for (my_iter it=e1::ets.begin(); it != e1::ets.end(); ++ it)
				{
					if ( !eee.exec((YourEntry * )it->second)) 
						return false;
				}
				return true;
			}
		/** 
		 * @brief 条件移除Entry
		 * 
		 * @param pred 条件
		 * 
		 */

		template <class YourEntry>
			inline void removeEntry_if(removeEntry_Pred<YourEntry> & pred)
			{
				typedef typename e1::iter my_iter;
				my_iter it = e1::ets.begin();
				while(it != e1::ets.end())
				{
					if (pred.isIt((YourEntry * )it->second))
					{
						pred.removed.push_pack((YourEntry *)it->second);
					}
					++it;
				}

				for (unsigned int i = 0; i< pred.removed.size();++i)
				{
					removeEntry(pred.removed[i]);
				}
			}
};

#endif


