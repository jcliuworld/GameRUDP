#ifndef _aUniqueID_H__
#define _aUniqueID_H__
/**
 * @aUniqueID.h
 * @brief 唯一ID管理器 
 * @author ljc jc.liuworld@hotmail.com
 * @date   2011-3-10
 */
#include <set>

#include "aNoneCopyable.h"
#include "aMutex.h"
#include "aFunc.h"
template <class T>
class aUniqueID : public aNoneCopyable
{
	
public:

	/**
	 * @brief 构造函数 
	 *
	 * @param
	 * @return
	 */
	aUniqueID()
	{
		init(1,(T)-1);
	}

	/**
	 * @brief 构造函数 
	 *
	 * @param miniD 起始ID
	 * @return
	 */
	aUniqueID(T minID)
	{
		init(minID,(T)-1);
	}
	/**
	 * @brief 构造函数 
	 *
	 * @param minID 起始ID
	 * @param maxID 最大ID
	 * @return
	 */
	aUniqueID(T minID, T maxID)
	{
		init(minID,maxID);
	}
	/**
	 * @brief 析构函数 
	 *
	 * @param
	 * @return
	 */
	~aUniqueID()
	{
		_mutex.lock();
		_ids.clear();
		_mutex.unlock();
	}

	/**
	 * @brief 获取最大有效值 
	 *
	 * @param
	 * @return 最大有效值
	 */
	T invalid()
	{
		return _maxID;
	}

	/**
	 * @brief 判断一个ID是否有效 
	 *
	 * @param tvalue 要判断的值
	 * @return true 成功 false 失败
	 */
	bool hasAssigned(T tvalue)
	{
		_mutex.lock();
		if ((tvalue < _curMax) &&( tvalue > _minID))
		{
			typename std::set<T>::iterator it = _ids.find(tvalue);
			if (it != _ids.end())
			{
				_mutex.unlock();
				return false;
			}
			_mutex.unlock();
			return true;
		}
		_mutex.unlock();
		return false;
	}

	/**
	 * @brief 获取唯一ID
	 *
	 * @param 
	 * @return ID
	 */
	T get()
	{
		T ret;
		_mutex.lock();
		if (_maxID > _curMax)
		{
			ret = _curMax;
			_curMax ++;
		}
		else
			ret = _maxID;

		if (ret == _maxID && !_ids.empty())
		{
			DWORD index = Age::aMisc::randBetween(0,_ids.size() - 1);
			typename std::set<T>::iterator it = _ids.begin();
			std::advance(it,index);
			ret = *it;
			_ids.erase(it);
		}
		_mutex.unlock();
		return ret;
	}

	/**
	 * @brief 批量获取唯一ID 
	 *
	 * @param size 想要获取的ID数量
	 * @param count 实际获取的ID数量
	 * @return 最小的ID
	 */
	T get(int size ,int &count)
	{
		T ret;
		_mutex.lock();
		if (_maxID > _curMax)
		{
			count = (_maxID - _curMax)>size?0:size(_maxID-_curMax);
			ret = _curMax;
			_curMax+=count;

		}
		else
		{
			count = 0;
			ret = _maxID;
		}
		_mutex.unlock();
		return ret;
	}

	/**
	 * @brief 释放ID 
	 *
	 * @param id 要释放的ID
	 * @return
	 */
	void put( T id)
	{
		_mutex.lock();
		if (id<_maxID && id >= _minID)
		{
			typename std::set<T>::iterator it = _ids.find(id);
			if (it == _ids.end())
			{
				_ids.insert(id);
			}
		}
		_mutex.unlock();
	}

private:

	/**
	 * @互斥变量
	 */
	aMutex _mutex;
	/**
	 * @brief 最大ID
	 */
	T _maxID;

	/**
	 * @brief 最小ID
	 */
	T _minID;
	/**
	 * @brief 当前使用的最大ID
	 */
	T _curMax;

	/**
	 * @brief 回收后的ID
	 */
	std::set<T> _ids;

	/**
	 * @brief 初始化函数 
	 *
	 * @param min 最小ID值
	 * @param max 最大ID值
	 * @return
	 */
	void init( T min, T max)
	{
		_minID = min;
		_maxID = max;
		_curMax = min;
	}
};

/**
 * @brief 实现一个DWORD的 的ID容器
 */
typedef aUniqueID<DWORD> aUNIQUEIDDWORD;

#endif

