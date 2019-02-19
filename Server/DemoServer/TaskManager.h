#ifndef __TaskManager_H__
#define __TaskManager_H__
/**
 * @TaskManager.h
 * @brief  任务管理器
 * @author ljc jc.liuworld@hotmail.com
 * @date   2011-3-11
 */


#include <ext/hash_map>
#include <list>

#include "aNoneCopyable.h"
#include "aMutex.h"
#include "aDef.h"
#include "protocol.h"
#include "ServerTask.h"
class TaskManager : private aNoneCopyable
{
	public:
		/**
		 * @brief 获取管理器实例 单件模式 
		 *
		 * @param
		 * @return 管理器实例
		 */
		static	TaskManager & getInstance()
		{
			if( NULL == _instance)
				_instance = new TaskManager();
		
			return *_instance;
		}
		/**
		 * @brief 析构函数 
		 *
		 * @param
		 * @return
		 */

		~TaskManager(){};
		/**
		 * @brief 向管理器中加入一个任务对象 
		 *
		 * @param task 任务对象
		 * @return void
		 */
		void addTask(ServerTask * task);
		/**
		 * @brief 移除一个任务对象 
		 *
		 * @param task 任务对象
		 * @return void
		 */
		void removeTask(ServerTask * task);
		/**
		 * @brief 根据任务ID 获取一个任务对象 
		 *
		 * @param dwTaskID 任务对象ID
		 * @return 任务对象指针
		 */
		ServerTask * getTaskByID(DWORD dwTaskID);
		
		/**
		 * @brief 将一个任务对象加入到唯一性容器中
		 *
		 * @param task 任务对象指针
		 * @return 是否添加成功
		 */
		bool uniqueAdd(ServerTask * task);
		/**
		 * @brief 唯一性验证 
		 *
		 * @param wdTaskID 要验证的任务对象的ID
		 * @return true 成功 false 失败
		 */
		bool uniqueVerify(const DWORD dwTaskID);
		/**
		 * @brief 从唯一性容器中移除一个任务对象
		 *
		 * @param task 任务对象指针
		 * @return 是否移除成功
		 */
		bool uniqueRemove(ServerTask * task);
		/**
		 * @brief 向所有任务对象广播 
		 *
		 * @param msg 广播数据指针
		 * @return true 成功 false 失败
		 */
		bool broadcast(const MSG * msg);
		/**
		 * @brief 向指定任务ID的任务对象广播 
		 *
		 * @param dwTaskID 任务ID
		 * @param pstrCmd 数据指针
		 * @return true 成功 false 失败
		 */
		bool broadcastByID(DWORD dwTaskID, const MSG * msg);

		/**
		 * @brief 统计任务对象数量 
		 *
		 * @param
		 * @return 任务对象数量
		 */
		const DWORD caculateTaskNum();

	private:
	
		/**
		 * @brief 构造函数 
		 *
		 */
		TaskManager(){};

		typedef std::list<ServerTask * > TASKLIST;
	
		typedef TASKLIST::iterator TASKLISTITER;

		typedef TASKLIST::const_iterator CONST_TASKLISTITER;

		typedef __gnu_cxx::hash_map<WORD,ServerTask *> TASKMAP;
		
		typedef TASKMAP::iterator TASKMAPITER;
		
		typedef TASKMAP::const_iterator CONST_TASKMAPITER;

		typedef TASKMAP::value_type TASKMAPPAIR;

		/**
		 * @brief 互斥变量
		 */
		aMutex mutex;

		/**
		 * @brief 任务对象容器 
		 */
		TASKLIST container;
		/**
		 * @brief 任务对象唯一性容器 
		 */
		TASKMAP taskUniqueContainer;		
		
		/**
		 * @brief 任务管理器对象实例指针 
		 */
		static TaskManager * _instance;
};



#endif
