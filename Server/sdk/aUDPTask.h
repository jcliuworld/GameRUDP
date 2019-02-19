
#ifndef _aUDPTask_H__
#define _aUDPTask_H__

/**
 * @file aUDPTask.h
 * @brief  udp 任务类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2017-11-01
 */

#include "aNoneCopyable.h"
#include "aDef.h"
#include "aUDPSocket.h"
#include "Age.h"
#include "aDef.h"
#include "aMsgParse.h"

class aRealTime;

/**
 * @brief 封装 tcp任务的各种操作
 */

class aUDPTask :private aNoneCopyable,public aMsgParse
{
	public:

		/**
		 * @brief 结束标记
		 */

		enum TerminateMethod
		{
			terminate_no,	/**<没结束任务*/
			terminate_active,/**<客户端主动断开连接，主要是服务器检测到套接口关闭或者异常*/
			terminate_passive,/**<服务器主动断开连接*/
		};
		/**
		 * @brief tcp stat
		 */

		enum aUDPTask_State
		{
			notuse = 0,
			verify = 1,
			sync = 2,
			okay = 3,
			recyle = 4
		};

		/**
		 * @brief 构造函数
		 *
		 * @param sock 套接口
		 * @param *addr 地址
		 * @param checkSignal 是否检查
		 */
		aUDPTask(const int sock, const struct sockaddr_in *addr ,DWORD connectID ,const bool checkSignal = true);
		/**
		 * @brief 析构函数
		 */

		virtual ~aUDPTask(){}
		/**
		 * @brief 消息解析和处理函数
		 *
                 * @param cmd 消息
                 * @param iLen 消息长度
                 *
                 * @return true 有处理 false 没有处理
                 */
                virtual bool msgParse(const char * msg ,const int iLen ){ return false;};

                virtual bool msgQueueParse(const char * msg,const int iLen){ return false;};

		 /**
                 * @brief 是否是服务器心跳信号
                 *
                 * @param msg
                 * @param iLen
                 *
                 * @return
                 */
                virtual bool isTickServer( const char * msg,const int iLen )=0;


                /**
                 * @brief 是否是客户端心跳信号
                 *
                 * @param msg
                 * @param iLen
                 *
                 * @return
                 */
                virtual bool isTickClient(const char * msg , const int iLen )=0;
		/**
		 * @brief 获取服务器心跳协议
		 *
		 * @param out
		 */
		virtual void getServerTickMsg(std::string &out) = 0;

		/**
		 * @brief 获取客户端心跳协议
		 *
		 * @param out
		 */
		virtual void getClientTickMsg(std::string &out)= 0;


		/**
		 * @brief 加入epoll
		 *
		 * @param kdpfd epoll 文件句柄
		 * @param events 事件结构
		 * @param ptr 事件指针
		 */

		void addEpoll(int kdpfd,__uint32_t events,void *ptr);

		/**
		 * @brief 移除epoll
		 *
		 * @param kpdfd epoll句柄
		 * @param events 事件结构
		 *
		 */

		void delEpoll(int kpdfd,__uint32_t events);

		bool checkVerifyTimeOut(const aRealTime &rt,const unsigned long long interval = 2000L)const;

		bool isFdsrAdd();

		bool fdsrAdd();
		/**
		 * @brief 连接验证函数
		 *
		 * @return 1成功
		 */
		virtual int verifyConn()
		{
			return 1;
		}


		/**
		 * @brief 同步验证函数
		 *
		 * @return
		 */
		virtual int waitSync()
		{
			return 1;
		}

		/**
		 * @brief  回收函数
		 *
		 * @return
		 */
		virtual int recycleConn()
		{
			return 1;
		}

		/**
		 * @brief  加入到管理容器
		 */
		virtual void addToContainer(){}

		/**
		 * @brief 从管理容器移除
		 */

		virtual void removeFromContainer(){}


		/**
		 * @brief  添加到唯一性验证容器
		 *
		 * @return
		 */
		virtual bool uniqueAdd()
		{
			return true;
		}

		/**
		 * @brief 从唯一性容器验证移除
		 *
		 * @return
		 */

		virtual bool uniqueRemove()
		{
			return true;
		}

		/**
		 * @brief  设置从唯一性验证通过
		 */

		void setUnique();


		/**
		 * @brief 是否通过唯一性验证
		 *
		 * @return
		 */
		bool isUnique() const;

		/**
		 * @brief 是否被其他线程设置为等待断开连接状体
		 *
		 * @return
		 */

		bool isTerminateWait();

		/**
		 * @brief 设置为断开连接状态
		 */

		void TerminateWait();

		/**
		 * @brief 是否为结束状态
		 *
		 * @return
		 */

		bool isTerminate()const;

		/**
		 * @brief 设置为结束状态
		 *
		 * @param method
		 */

		void Terminate(const TerminateMethod method  = terminate_no);

		/**
		 * @brief 发送消息
		 *
		 * @param
		 * @param
		 *
		 * @return
		 */

		virtual bool sendCmd(std::string & msg);

		/**
		 * @brief 监听接收消息
		 *
		 * @param bool
		 *
		 * @return
		 */

		virtual bool listeningRecv(bool);

		/**
		 * @brief 监听发送
		 *
		 * @return
		 */

		virtual bool listeningSend();

		/**
		 * @brief 获取连接状态
		 *
		 * @return 连接状态
		 */

		const aUDPTask_State getState()const;

		/**
		 * @brief 设置连接状态
		 *
		 * @param state
		 */

		void setState(const aUDPTask_State state);

		/**
		 * @brief 获取下一个连接状态
		 */

		void getNextState();

		/**
		 * @brief 重置连接状态
		 */

		void resetState();


		/**
		 * @brief  根据状态获取相应的状态名称
		 *
		 * @param state
		 *
		 * @return 状态名称
		 */
		const char * getStateString(const aUDPTask_State state) const;

		/**
		 * @brief  获取连接的IP
		 *
		 * @return  IP
		 */

		const char * getIP() const;

		/**
		 * @brief  获得连接地址
		 *
		 * @return  连接地址
		 */

		const DWORD getAddr()const;

		/**
		 * @brief 是否发送测试信号
		 *
		 * @return
		 */

		const bool ifCheckSinal()const;

		/**
		 * @brief 检测测试信号发送间隔
		 *
		 * @param ct
		 *
		 * @return
		 */

		bool checkInterval(const aRealTime &ct);

		/**
		 * @brief 是否在规定时间内检测成功
		 *
		 * @return
		 */

		bool checkTick()const;

		/**
		 * @brief 清楚检测状态
		 */

		void clearTick();

		/**
		 * @brief 设置检测状态
		 */

		void setTick();

		/**
		 * @brief 定时检测函数
		 *
		 * @param ct
		 */

		void checkSignal(const aRealTime &ct);

	protected:
		bool buffered; /**<发送端是否做发送缓冲*/
		aUDPSocket _socket;/*<套接口>**/
		aUDPTask_State state;/**<连接状态*/

	private:

		TerminateMethod terminate; /**<是否结束状态*/

		bool terminate_wait; /**<设置等待结束状态*/

		bool fdsradd;/**<读事件标志*/

		aRealTime lifeTime;/**<连接建立的时间*/

		bool uniqueVerified;/**<唯一性验证通过标志*/

		const bool _checkSignal;/**<是否发送链路检测标记*/

		aTimer _tick_timer;/**<定时器*/

		bool tick ;

		BYTE check_count; /**<信号检测次数*/


};
#endif
