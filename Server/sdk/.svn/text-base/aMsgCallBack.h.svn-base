#ifndef _MsgCallBack_H__
#define _MsgCallBack_H__
/**
 * @file aMsgCallBack.h
 * @brief 消息回调处理 本类的使用方式 1 、 在每个Task里申请一个aMsgtarget类型对象 如 aMsgtarget msgCall 在本类中实现虚函数 initMsg(). 3 用宏定义绑定接口 注意如果是网关 最好处理成静态的 防止占用过多的代码空间 4、BEGIN_MSGMAP END_MSGMAP 需要成对的出现在同一个对象的定义里，中间不能夹杂其他内容 一个task 只能绑定一个msgobj 如果需要对多个task处理类表 只需要在其他的地方绑定同样的 msgobj
 * @author ljc jc.liyuworld@hotmail.com
 * @version 
 * @date 2011-10-08
 */

#include <map>

#include "sdkL.h"
#include "aDef.h"
#include "aFunc.h"
class aMsgtarget;

typedef bool (aMsgtarget::*AGE_PMSG)(const void * msg);

typedef __gnu_cxx::hash_map<std::string,AGE_PMSG,Age::key_hash,Age::key_equal> AGE_MSGMAP_ENTRY;
//#pragma pack(1)
typedef struct stAgeMsgMap
{      
	const stAgeMsgMap* (* pfnGetBaseMap)(); 
	const AGE_MSGMAP_ENTRY* lpEntries;
} AGE_MSGMAP;
//#pragma pack()
#define DECLARE_MESSAGE_MAP()\
 protected: \
      static const AGE_MSGMAP*  getThisMessageMap(); \
      virtual const AGE_MSGMAP* getMessageMap() const; \


#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
const AGE_MSGMAP* theClass::getMessageMap() const \
{ return getThisMessageMap(); } \
const AGE_MSGMAP*  theClass::getThisMessageMap() \
{ \
	typedef theClass ThisClass;                        \
	typedef baseClass TheBaseClass;                    \
	static AGE_MSGMAP_ENTRY _messageEntries;  \
	std::string pack; 

 
#define END_MESSAGE_MAP() \
\
	static const AGE_MSGMAP messageMap = \
{ &TheBaseClass::getThisMessageMap, &_messageEntries }; \
return &messageMap; \
}


#define BEGIN_PACK(packname) \
	pack = packname ;\
	pack += "."                                

#define END_PACK() \
        pack=""; 

#define ON_COMMAND(msgdesc, memberFxn) \
	_messageEntries.insert(AGE_MSGMAP_ENTRY::value_type(msgdesc,static_cast<AGE_PMSG> (&ThisClass::memberFxn)));


class aMsgtarget
{  
  	protected:
	    static const AGE_MSGMAP*  getThisMessageMap();
};
#endif
