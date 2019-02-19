#ifndef _aMsgParse_H__
#define _aMsgParse_H__

/**
* @file aMsgParse.h
* @brief 消息回调接口
* @author ljc jc.liuworld@hotmail.com
* @version 
* @date 2012-12-25
*/
/**
 * @brief 消息处理接口 所有任务对象需要实现本类中的两个虚函数
 */
class aMsgParse
{
	public:
		/** 
		 * @brief 消息解析和处理函数
		 * 
		 * @param cmd 消息
		 * @param iLen 消息长度
		 * 
		 * @return true 有处理 false 没有处理
		 */
		virtual bool msgParse(const char * msg ,const int iLen ) = 0;

		virtual bool msgQueueParse(const char * msg,const int iLen) = 0;


		/**
		 * @brief 是否是服务器心跳信号
		 *
		 * @param msg
		 * @param iLen
		 *
		 * @return 
		 */
		virtual bool isTickServer( const char * msg,const int iLen ) = 0;


		/**
		 * @brief 是否是客户端心跳信号
		 *
		 * @param msg
		 * @param iLen
		 *
		 * @return 
		 */
		virtual bool isTickClient(const char * msg , const int iLen ) = 0;

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
		virtual void getClientTickMsg(std::string &out) = 0;

		/**
		 * @brief aMsgParse 构造函数
		 */
		aMsgParse()
		{
		}

		/**
		 * @brief ~aMsgParse 析构函数
		 */
		virtual ~aMsgParse(){}
};
#endif
