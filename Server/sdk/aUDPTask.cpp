#include "aUDPTask.h"
#include "aTime.h"
/**
 * @file aUDPTask.cpp
 * @brief  tcp 任务类
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-18
 */

/**
 * @brief 构造函数
 *
 * @param sock 套接口
 * @param *addr 地址
 * @param compress 是否有压缩
 * @param checkSignal 是否检查
 */
aUDPTask::aUDPTask(const int sock, const struct sockaddr_in *addr,DWORD connectID,const bool checkSignal): _socket(sock,addr,connectID),lifeTime(),_checkSignal(checkSignal),_tick_timer(30),tick(false)
{
	lifeTime.now();
	terminate = terminate_no;
	terminate_wait = false;
	fdsradd = false;
	buffered = false;
	state = notuse;
	check_count = 0;
}


/**
 * @brief 发送消息
 *
 * @param
 * @param
 *
 * @return
 */

bool aUDPTask::sendCmd(std::string & msg)
{
	return _socket.sendCmd(msg);
}


/**
 * @brief 监听接收消息
 *
 * @param bool
 *
 * @return
 */

bool aUDPTask::listeningRecv(bool needRecv)
{
	int retcode = 0;

	if (needRecv)
	{
		retcode = _socket.recvToBufNoPoll();
	}
	if (-1 == retcode)
	{
		return false;
	}
	else
	{
		do
		{
			char pstrCmd[aUDPSocket::MAX_DATASIZE];
			int iLen = _socket.recvToCmdNoPoll(pstrCmd,sizeof(pstrCmd));

			if (iLen <= 0)
			{
				break;
			}
			else
			{

				if(isTickServer(pstrCmd,iLen))
				{
					check_count = 0;
					clearTick();

				}
				else if ( isTickClient(pstrCmd,iLen))
				{
					std::string out;
					getClientTickMsg(out);
					sendCmd(out);
				}
				else
				{
					msgParse(pstrCmd,iLen);

				}

			}
		} while (true);
	}

	return true;
}

/**
 * @brief 监听发送
 *
 * @return
 */

bool aUDPTask::listeningSend()
{
	return _socket.sync();
}

/**
 * @brief 获取下一个连接状态
 */

void aUDPTask::getNextState()
{
	aUDPTask_State oldState = getState();

	switch(oldState)
	{
		case notuse:
			setState(verify);
			break;
		case verify:
			setState(sync);
			break;
		case sync:
			buffered = true;
			addToContainer();
			setState(okay);
			break;
		case okay:
			removeFromContainer();
			setState(recyle);
			break;
		case recyle:
			setState(notuse);
			break;
	}

	Age::logger->debug("%s( %s, %s->%s)",__FUNCTION__,getIP(),getStateString(oldState),getStateString(getState()));
}

/**
 * @brief 重置连接状态
 */


void aUDPTask::resetState()
{
	aUDPTask_State oldState = getState();

	switch(oldState)
	{
		case notuse:
		case recyle:
			Age::logger->fatal("%s impossible recycle->recycle",__FUNCTION__);
			break;
		case verify:
		case sync:
		case okay:
			break;
	}
	setState(recyle);

	Age::logger->debug("%s( %s, %s->%s)",__FUNCTION__,getIP(),getStateString(oldState),getStateString(getState()));
}
/**
 * @brief 定时检测函数
 *
 * @param ct
 */


void aUDPTask::checkSignal(const aRealTime &ct)
{
	if (ifCheckSinal() && checkInterval(ct))
	{
		if (checkTick())
		{
			if ( check_count > 3)
			{
				Age::logger->error("the socket check fail %u:(%s:%u),the send buf size",check_count,_socket.getIP(),_socket.getPort(),_socket.snd_queue_size());
				Terminate(aUDPTask::terminate_active);
			}
			else
			{
				check_count ++;

				std::string send;
				getServerTickMsg(send);
				//if(sendCmd(send))
				//	setTick();

			}
		}
		else
		{
			std::string send;
			getServerTickMsg(send);
			//if(sendCmd(send))
			//	setTick();
		}
	}
}



/**
 * @brief 加入epoll
 *
 * @param kdpfd epoll 文件句柄
 * @param events 事件结构
 * @param ptr 事件指针
 */

void aUDPTask::addEpoll(int kdpfd,__uint32_t events,void *ptr)
{
	_socket.addEpoll(kdpfd,events,ptr);
}

/**
 * @brief 移除epoll
 *
 * @param kpdfd epoll句柄
 * @param events 事件结构
 *
 */

void aUDPTask::delEpoll(int kpdfd,__uint32_t events)
{
	_socket.delEpoll(kpdfd,events);
}

bool aUDPTask::checkVerifyTimeOut(const aRealTime &rt,const unsigned long long interval)const
{
	unsigned long long time = lifeTime.elapse(rt);
	return (time > interval );
}

bool aUDPTask::isFdsrAdd()
{
	return fdsradd;
}

bool aUDPTask::fdsrAdd()
{
	fdsradd = true;
	return fdsradd;
}

/**
 * @brief  设置从唯一性验证通过
 */

void aUDPTask::setUnique()
{
	uniqueVerified = true;
}


/**
 * @brief 是否通过唯一性验证
 *
 * @return
 */
bool aUDPTask::isUnique() const
{
	return uniqueVerified;
}

/**
 * @brief 是否被其他线程设置为等待断开连接状体
 *
 * @return
 */

bool aUDPTask::isTerminateWait()
{
	return terminate_wait;
}

/**
 * @brief 设置为断开连接状态
 */

void aUDPTask::TerminateWait()
{
	terminate_wait = true;
}

/**
 * @brief 是否为结束状态
 *
 * @return
 */

bool aUDPTask::isTerminate()const
{
	return terminate_no != terminate;
}

/**
 * @brief 设置为结束状态
 *
 * @param method
 */

void aUDPTask::Terminate(const TerminateMethod method)
{
	terminate = method;
	clearTick();
}


/**
 * @brief 获取连接状态
 *
 * @return 连接状态
 */

const aUDPTask::aUDPTask_State aUDPTask::getState()const
{
	return state;
}

/**
 * @brief 设置连接状态
 *
 * @param state
 */

void aUDPTask::setState(const aUDPTask_State state)
{
	this->state = state;
}

/**
 * @brief 获取下一个连接状态
 */

void aUDPTask::getNextState();

/**
 * @brief 重置连接状态
 */

void aUDPTask::resetState();


/**
 * @brief  根据状态获取相应的状态名称
 *
 * @param state
 *
 * @return 状态名称
 */
const char * aUDPTask::getStateString(const aUDPTask_State state) const
{
	const char * retval = NULL;
	switch(state)
	{
		case notuse:
			retval = "notues";
			break;
		case verify:
			retval = "verify";
			break;
		case sync:
			retval = "sync";
			break;
		case okay:
			retval = "okay";
			break;
		case recyle:
			retval = "recyle";
			break;
		default:
			retval = "none";
			break;
	}
	return retval;
}

/**
 * @brief  获取连接的IP
 *
 * @return  IP
 */

const char * aUDPTask::getIP() const
{
	return _socket.getIP();
}

/**
 * @brief  获得连接地址
 *
 * @return  连接地址
 */

const DWORD aUDPTask::getAddr()const
{
	return _socket.getAddr();
}

/**
 * @brief 是否发送测试信号
 *
 * @return
 */

const bool aUDPTask::ifCheckSinal()const
{
	return _checkSignal;
}

/**
 * @brief 检测测试信号发送间隔
 *
 * @param ct
 *
 * @return
 */

bool aUDPTask::checkInterval(const aRealTime &ct)
{
	return _tick_timer(ct);
}

/**
 * @brief 是否在规定时间内检测成功
 *
 * @return
 */

bool aUDPTask::checkTick()const
{
	return tick;
}

/**
 * @brief 清楚检测状态
 */

void aUDPTask::clearTick()
{
	tick =false;
}

/**
 * @brief 设置检测状态
 */

void aUDPTask::setTick()
{
	tick = true;
}

