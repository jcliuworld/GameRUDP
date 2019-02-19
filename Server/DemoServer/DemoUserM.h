#ifndef _DemoUserM_H__
#define _DemoUserM_H__
/**
 * @DemoUserM.h
 * @brief 网关用户管理器
 * @author ljc jc.liuworld@hotmail.com
 * @date   2011-3-11
 */
#include "aUserM.h"
#include "aUniqueID.h"

#include "DemoUser.h"
/**
 * @brief 网关用户accid 容器  注意：网关用户在没有登录到逻辑场景上的时候只有accid
 * 因此只能用 如getUserByAccid() 等进行操作。
 */
class DemoUserAccid : protected LimitHash<DWORD,DemoUser *>
{
protected:
	/**
	 * @brief 加入一个网关用户
	 *
	 * @param e 网关用户
	 * @return true 加入成功 false 加入失败
	 */
	bool push(DemoUser * &e)
	{
		DemoUser * account;
		return (!find(e->_dwAccID,account)&&insert(e->_dwAccID,e));
	}
public:
	/**
	 * @brief 构造函数
	 */
	DemoUserAccid(){};
	/**
	 * @brief 析构函数
	 */
	virtual ~DemoUserAccid(){};
	/**
	 * @brief 根据用户accid 获取网关用户对象 子类必须实现
	 *
	 * @param accid 用户accid
	 * @return 网关用户指针
	 */
	virtual DemoUser * getUserByAccid(DWORD accid) = 0;
	/**
	 * @brief 从容器中移除一个网关用户 子类必须实现
	 *
	 * @param accid 要移除用户的accid
	 * @return void
	 */
	virtual void removeUserOnlyByAccid(DWORD accid) = 0;
	/**
	 * @brief 向容器中添加一个网关用户 子类必须实现
	 *
	 * @param e 网关用户
	 * @return true 成功 false 失败
	 */
	virtual bool addUserOnlyByAccid(DemoUser * user) = 0;
};


class DemoUserM : public aUserM,protected DemoUserAccid
{
public:

	/**
	 * @brief 获取网关用户对象管理器实例 单件模式
	 *
	 * @param
	 * @return 管理器实例
	 */
	static DemoUserM & getInstance();

	/**
	 * @brief 删除实例
	 */
	static void delInstance()
	{
			SAFE_DELETE(_instance);
	}

	/**
	 * @brief 获取唯一性ID
	 *
	 * @param id 返回的唯一性ID
	 * @return true 成功 false 失败
	 */
	bool getUniqueID(DWORD & id);

	/**
	 * @brief 初始化函数
	 *
	 * @param
	 * @return true 成功 false 失败
	 */
	bool init();
	/**
	 * @brief 移除所有用户
	 */
	void removeAllUser();
	/**
	* @brief 根据用户accid 获取网关用户对象
	*
	* @param accid 用户accid
	* @return 网关用户指针
	*/
	DemoUser * getUserByAccid(DWORD accid);
	/**
	* @brief 从容器中移除一个网关用户
	*
	* @param accid 要移除用户的accid
	* @return void
	*/
	void removeUserOnlyByAccid(DWORD accid);
	/**
	* @brief 向容器中添加一个网关用户
	*
	* @param e 网关用户
	* @return true 成功 false 失败
	*/
	bool addUserOnlyByAccid(DemoUser * user);


private:

	/**
         * @brief 回收一个唯一性ID
         *
         * @param id 要回收的ID
         * @return void
         */
        void putUniqueID(DWORD &tempID);


	/**
	 * @brief 管理器实例
	 */
	static	DemoUserM * _instance;

	/**
	 * @brief 构造函数
	 */
	DemoUserM(){
		_inited = false;
		_uniqueID = NULL;
		init();
	}
	/**
	 * @brief 析构函数
	 */
	~DemoUserM()
	{
		SAFE_DELETE(_uniqueID);
	}

	/**
	 * @brief 用于标记唯一性对象是否初始化过
	 */
	bool _inited;

	/**
	 * @brief  唯一性对象
	 */
	aUNIQUEIDDWORD *_uniqueID;

	/**
	 * @brief 读写锁
	 */
	aRWLock _rwlock;
};

#endif

