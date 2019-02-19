#ifndef _ASINGTON_H_
#define _ASINGTON_H_

#include <aDef.h>

/**
* @brief 单件类
*/

/**
* @brief 在继承类加上此声明
*/
#define DECLARE_SINGLETON_CLASS(cls) \
	friend class aSingleton<cls>;

template<class T>
class aSingleton
{
public:
	static T& getInstance()
	{
		if (_instance == NULL)
		{
			_instance = new T;
		}

		return *_instance;
	}

	static void releaseInstance()
	{
		delete _instance;
		_instance = NULL;
	}

protected:
	aSingleton() {}
	virtual ~aSingleton() {}

private:
	aSingleton(const aSingleton &);
	aSingleton& operator=(const aSingleton &);

private:
	/**
	* @brief 实例
	*/
	static T * _instance;
};
template<class T>
T* aSingleton<T>::_instance = NULL;

#endif
