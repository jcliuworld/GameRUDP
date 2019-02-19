#ifndef _ProtoFun_H__
#define _ProtoFun_H__
#include "protocol.h"
#include "aFunc.h"
#include "Age.h"
/**
* @brief 消息结构定义封装
*/

#define MSG google::protobuf::Message


/**
 * @brief 描述结构定义封装
 */

#define DESCPROTO google::protobuf::Descriptor 

////////////////协议格式/////////////////////


/////////////////////////////////////////////
const unsigned int PH_LEN     =       sizeof(unsigned int);

inline DWORD getmsgid(const char * msg ,const int iLen)
{
	if (iLen < 4)
		return 0;
	DWORD msgid= (*((DWORD *)msg));
	return msgid;
}
/*
 * @brief covertstring to int
 *              
 * @param type_name type name
 *              
 * @return Msg * 
 */
inline int asInt32(const char* buf)
{
	int be32 = 0;
	memcpy(&be32, buf, sizeof(be32));
	return be32;
}
/**     
 * @brief encode covert th message to a stream which struct as ProtobufTransportFormat
 *              
 * @param message user data msg
 *              
 * @return string user data 
 */  

inline void encode(const MSG * message,std::string & result)
{


	const std::string& typeName = message->GetTypeName();
	DWORD dwMsgID = Age::strHash(typeName.c_str());
	result.append(reinterpret_cast<char*>(&dwMsgID), sizeof(dwMsgID));
	bool succeed = message->AppendToString(&result);

	if (succeed)
	{
		const char* begin = result.c_str() ;//+ PH_LEN/2;
		unsigned int checkSum = Age::getCRC(begin,result.size());
		result.append(reinterpret_cast<char*>(&checkSum), sizeof(checkSum));

	}
	else
	{
		result.clear();
	}

	//Age::logger->debug("[sock] nameLen %d name %s msgsize %d",nameLen ,typeName.c_str(),result.size());
}
inline void encode(const std::string & msgname,const std::string &msg,std::string & result)
{
	DWORD msgID = Age::strHash(msgname.c_str());

	result.append(reinterpret_cast<char*>(&msgID), sizeof(msgID));
	result.append(msg);

	const char* begin = result.c_str() ;//+ PH_LEN/2;
	unsigned int checkSum = Age::getCRC(begin,result.size());
	result.append(reinterpret_cast<char*>(&checkSum), sizeof(checkSum));

	//Age::logger->debug("[sock] nameLen %d name %s",nameLen ,msgname.c_str(),result.size());
}

/**     
 * @brief create a null msassage throw a name
 *              
 * @param type_name type name
 *              
 * @return Msg * 
 */  

inline MSG * createMessage(const std::string& type_name)
{
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	if (descriptor)
	{
		const google::protobuf::Message* prototype =
			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			message = prototype->New();
		}
	}
	return message;
}



/**
 * Decode protobuf Message from transport format defined above.
 * returns a Message*
 *
 * returns NULL if fails.
 */
inline MSG * decode(const std::string& buf,const std::string & typeName)
{
	MSG * result = NULL;

	int len = static_cast<int>(buf.size());
	if (len >= 8)
	{
		int expectedCheckSum = asInt32(buf.c_str() + buf.size() - PH_LEN);
		const char* begin = buf.c_str() ;//+ PH_LEN/2;
		int checkSum = Age::getCRC(begin,len - PH_LEN);
		if (checkSum == expectedCheckSum)
		{

			MSG * message = createMessage(typeName);
			if (message)
			{
				const char* data = buf.c_str() + PH_LEN;
				int dataLen = len - 2 * PH_LEN;
				if (message->ParseFromArray(data, dataLen))
				{
					result = message;
				}
				else
				{
					// parse error
					SAFE_DELETE(message);
				}
			}
			else
			{
				// unknown message type
				Age::logger->debug("未知消息类型%s",typeName.c_str());
			}
		}
		else
		{
			// check sum error
			Age::logger->debug("CRC校验失败");
		}
	}

	return result;
}

#endif
