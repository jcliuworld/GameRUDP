#ifndef _TCPProtoBufClient_H__
#define _TCPProtoBufClient_H__
#include "sdk/aTCPClient.h"
#include "ProtoFun.h"
#include "aRWLock.h"
#include "aXMLReader.h"

class TCPProtoBufClient : public aTCPClient
{
	public:
		TCPProtoBufClient(const std::string &lip,const DWORD sport,const bool bcompress = false) : aTCPClient(lip,sport,bcompress)
		{
			init();
		}
		/** 
		 * @brief 消息处理回调
		 * 
		 * @param cmd cmd
		 * @param iLen cmd len
		 * 
		 * @return 已处理 true 未处理 false
		 */
		virtual bool msgParse(const char * msg ,const int iLen ){ 
			std::string buf;
			buf.append(msg,iLen);
	
			DWORD msgid = getmsgid(msg,iLen);
			std::string msgname = id2name(msgid);

			MSG * protoMsg  = decode(buf,msgname);
			if(NULL == protoMsg)
				return true;

			msgParse(msgid,protoMsg);

			SAFE_DELETE(protoMsg);
			return true;
		}

		virtual bool msgQueueParse(const char * msg,const int iLen)
		{
			std::string buf;
			buf.append(msg,iLen);

			DWORD msgid = getmsgid(msg,iLen);
			std::string msgname = id2name(msgid);

			MSG * protoMsg  = decode(buf,msgname);
			if(NULL == protoMsg)
				return true;

			msgQueueParse(msgid,protoMsg);

			SAFE_DELETE(protoMsg);

			return true;
		};

		virtual bool sendCmd(const MSG * msg )
		{
			std::string send;
			encode(msg,send);
			return aTCPClient::sendCmd(send);
		}

		virtual bool msgParse(DWORD msgID,const MSG * msg) = 0;

		virtual bool msgQueueParse(DWORD msgID,const MSG * msg)  = 0;		

		   /**
                 * @brief 是否是服务器心跳信号
                 *
                 * @param msg
                 * @param iLen
                 *
                 * @return 
                 */
                virtual bool isTickServer( const char * msg,const int iLen ){

                        if( getmsgid( msg,iLen )== _id_svrbeat)
                                return true;

                        else
                                return false;


                };

                /**
                 * @brief 是否是客户端心跳信号
                 *
                 * @param msg
                 * @param iLen
                 *
                 * @return 
                 */
                virtual bool isTickClient(const char * msg , const int iLen ) {

                        if( getmsgid( msg,iLen) == _id_clibeat )
                                return true;

                        else
                                return false;


                };

		/**
		 * @brief 获取服务器心跳协议
		 *
		 * @param out
		 */
		virtual void getServerTickMsg(std::string &out) {
			Base::msgHeartbeartServer send;
			encode(&send,out);
		};

		/**
		 * @brief 获取客户端心跳协议
		 *
		 * @param out
		 */
		virtual void getClientTickMsg(std::string &out) {
			Base::msgHeartbeartClient send;
			encode(&send,out);
		};

		void init()
		{
			_msgLock.wrlock();

			if ( !_msgnames.empty() )
			{
				_msgLock.unlock();
				return;
			}


			aXMLReader xml;

			if(xml.initFile((Age::global["cfg"]+"/Xml/MsgList.xml").c_str()))
			{
				xmlNodePtr root = xml.getRootNode("Age");
				if(root)
				{
					xmlNodePtr msgNode = xml.getChildNode(root,"msg");
					while(msgNode)
					{
						std::string msg;
						xml.getNodePropStr(msgNode,"name",msg);

						_msgnames.insert(std::make_pair(Age::strHash(msg.c_str()),msg));
						msgNode = xml.getNextNode(msgNode,NULL);
					}
				}
			}

			_msgLock.unlock();
		}




		static std::string  id2name(DWORD id)
		{
			_msgLock.rdlock();
			MSG_NAME_MAP::iterator it = _msgnames.find(id);
			if ( it == _msgnames.end())
			{
				_msgLock.unlock();
				return "";
			}
			else
			{
				_msgLock.unlock();
				return it->second;	
			}
		}

		static DWORD name2id(const char * name)
		{
			return Age::strHash(name);
		}


	private:	
		typedef  std::map<DWORD,std::string> MSG_NAME_MAP;//msg 名称 容器 key msg id string msgname  

		static MSG_NAME_MAP _msgnames;//msgname

		static DWORD _id_svrbeat;
		static DWORD _id_clibeat;

		static aRWLock _msgLock;


};

#endif
