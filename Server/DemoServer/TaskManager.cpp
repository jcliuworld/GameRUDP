#include "TaskManager.h"
#include "ServerTask.h"

TaskManager * TaskManager::_instance = NULL;
/**
* @brief 向管理器中加入一个任务对象 
*
* @param task 任务对象
* @return void
*/
void TaskManager::addTask(ServerTask * task)
{
	if(task)
	{
		mutex.lock();
			
		container.push_front(task);		

		mutex.unlock();
	}
}
/**
* @brief 移除一个任务对象 
*
* @param task 任务对象
* @return void
*/
void TaskManager::removeTask(ServerTask * task)
{
	if(task)
	{
		mutex.lock();
		container.remove(task);
		mutex.unlock();
	}
}
/**
* @brief 根据任务ID 获取一个任务对象 
*
* @param dwTaskID 任务对象ID
* @return 任务对象指针
*/
ServerTask *TaskManager::getTaskByID(DWORD dwTaskID)
{
	CONST_TASKLISTITER it;
	
	ServerTask * retval = NULL;

	mutex.lock();
	for(it = container.begin();it != container.end(); it++)
	{
		if((*it)->getID() == dwTaskID)
		{
			retval = *it;
			break;
		}
	}
	mutex.unlock();
	
	return retval;
}

/**
* @brief 将一个任务对象加入到唯一性容器中
*
* @param task 任务对象指针
* @return 是否添加成功
*/
bool TaskManager::uniqueAdd(ServerTask * task)
{
	CONST_TASKMAPITER it;

	mutex.lock();
	
	it = taskUniqueContainer.find(task->getID());
	
	if(it != taskUniqueContainer.end())
	{
		Age::logger->error("%s",__PRETTY_FUNCTION__);
		mutex.unlock();
		return false;
	}	

	taskUniqueContainer.insert(TASKMAPPAIR(task->getID(),task));


	mutex.unlock();
	return true;
}
/**
* @brief 唯一性验证 
*
* @param wdTaskID 要验证的任务对象的ID
* @return true 成功 false 失败
*/
bool  TaskManager::uniqueVerify(const DWORD dwTaskID)
{
	
	CONST_TASKMAPITER it;
	mutex.lock();
	it = taskUniqueContainer.find(dwTaskID);
	
	if(it != taskUniqueContainer.end())
	{
		mutex.unlock();
		return false;
	}
	mutex.unlock();
	
	return true;
}
/**
* @brief 从唯一性容器中移除一个任务对象
*
* @param task 任务对象指针
* @return 是否移除成功
*/
bool TaskManager::uniqueRemove(ServerTask * task)
{
	TASKMAPITER it;		
	
	mutex.lock();
	it = taskUniqueContainer.find(task->getID());

	if(it != taskUniqueContainer.end())
	{
		taskUniqueContainer.erase(it);
	}
	else
		Age::logger->warn("%s",__PRETTY_FUNCTION__);
	mutex.unlock();
	return true;
}
/**
* @brief 向所有任务对象广播 
*
* @param msg 广播数据指针
* @return true 成功 false 失败
*/
bool TaskManager::broadcast(const MSG * msg)
{
	bool retval = true;
	mutex.lock();
	for( CONST_TASKLISTITER it = container.begin(); it != container.end();++it)
	{
		if(!(*it)->sendCmd(msg));
			retval = false;
	}
	mutex.unlock();
	return retval;	
}
/**
* @brief 向指定任务ID的任务对象广播 
*
* @param dwTaskID 任务ID
* @param msg 数据指针
* @return true 成功 false 失败
*/
bool TaskManager::broadcastByID(DWORD dwTaskID, const MSG * msg)
{
	bool retval = true;
        mutex.lock();
        for( CONST_TASKLISTITER it = container.begin(); it != container.end();++it)
        {
		if((*it)->getID() == dwTaskID)
		{
                	retval=(*it)->sendCmd(msg);
                        break;
		}
        }
        mutex.unlock();
        return retval;
	
}

/**
* @brief 统计任务对象数量 
*
* @param
* @return 任务对象数量
*/
const DWORD TaskManager::caculateTaskNum()
{
	return container.size();	
}




