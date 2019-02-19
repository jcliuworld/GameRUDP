#ifndef _aDef_H__
#define _aDef_H__

/** 
 * @file aDef.h
 * @brief 全局宏及类型定义
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-12
 */
//#include <google/protobuf/descriptor.h>

#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <ext/hash_map>
#include <cmath>
#include <cfloat>
#include <ostream>
#include <sstream>
#include <tuple>

#define  SAFE_DELETE(x){if (x) {delete (x); (x) = NULL; }}
#define SAFE_DELETE_VEC(x){if (x) {delete [] (x); (x) = NULL; }}

/**
 * @brief  版本号定义
 */

#ifndef MAJOR_VERSION
#define MAJOR_VERSION 0
#endif

#ifndef MINOR_VSERSION
#define MINOR_VSERSION 0
#endif

#ifndef MICRO_VERSION
#define MICRO_VSRSION 0
#endif

#ifndef VS
#define VERSION_STRING _S(MAJOR_VERSION)"."_S(MINOR_VSERSION)"."_S(MICRO_VERSION)
#else
#define VERSION_STRING _S(VS)
#endif

#ifndef AStr
#define AStr std::string
#endif

#ifndef AMap
#define AMap    std::map
#endif

#ifndef AMMap
#define AMMap   std::multimap
#endif

#ifndef AVec
#define AVec std::vector
#endif

#ifndef AList
#define AList  std::list
#endif

#ifndef ASet
#define ASet std::set
#endif

#ifndef AMSet
#define AMSet std::multiset
#endif

#ifndef ATuple
#define ATuple std::tuple
#endif

#ifndef AHMap
#define AHMap __gnu_cxx::hash_map
#endif

#ifndef AHMMap
#define AHMMap __gnu_cxx::hash_multimap
#endif


const float EPSILON_FLT	= 0.000001;

const double EPSILON_DBL = 0.0000000001;

const float PIPow2			=	9.8696044;

const float PIDiv2			=	1.57079633;

const float PI				=	3.14159265;

#define check_ptr(ptr ,ret,info) \
	if( ptr == NULL ) \
	{\
		if( info != NULL) \
			Age::logger->error("%s",info);\
		return ret; \
	}


/**
* @brief 消息结构定义封装
*/

//#define MSG google::protobuf::Message


/**
 * @brief 描述结构定义封装
 */

//#define DESCPROTO google::protobuf::Descriptor 

/**
 * @brief short
 */
typedef short SHORT;

/**
 * @brief long long
 */
typedef long long LONGLONG;

/**
 * @brief LONG
 */
typedef long LONG;

/**
 * @brief FLOAT
 */

typedef float FLOAT;

/**
 * @brief char
 */

typedef char CHAR;
/** 
 * @brief unsingend char
 */

typedef unsigned char BYTE;
/** 
 * @brief unsigned short
 */

typedef unsigned short WORD;
/** 
 * @brief signed short
 */

typedef signed short SWORD;
/** 
 * @brief unsigned int
 */

typedef unsigned int DWORD;
/** 
 * @brief signed int
 */

typedef signed int SDWORD;
/** 
 * @brief unsigned long
 */

typedef unsigned long QWORD;
/** 
 * @brief signed long
 */

typedef signed long SQWORD;
/** 
 * @brief INT64
 */
typedef long INT64;

#define  MAX_NAMESIZE 32

#define  MAX_IPLENGTH 16

#define  MAX_ACCOUNTSIZE 48

#define  MAX_PASSWDSIZE 32

#define  MAX_ROLESIZE 3

#define MAX_CHATSIZE	256

#define INVALID64 (QWORD) - 1

#define INVALID32 (DWORD) - 1


/** 
 * @brief 游戏区结构
 */
#pragma pack(1)
struct stZONE
{
	union{
		DWORD id; /**<游戏区ID*/

		struct{
			WORD zoneid; /**<区ID*/
			WORD type;/***<游戏类型号*/
		};
	};

	stZONE()
	{
		this->zoneid = 0;
		this->type = 0;
	}

	stZONE(const stZONE & zone)
	{
		this->id = zone.id;
	}

	stZONE & operator = (const stZONE & zone)
	{
		this->id = zone.id;
		return *this;
	}

	bool operator== (const stZONE & zone)const
	{
		return this->id == zone.id;
	}
};
#pragma pack()
/*template <class T>
inline void constructPlace(T * p)
{

	new (static_cast<void*>(p))T();
}
#define BUF_CMD( cmd ,var,len) char buffer##name[len] ={0};\
	cmd * var = (cmd *) buffer##name; constructPlace(var);
*/
/** 
 * @brief 服务器类型定义
 */

enum 	
{
	UNKNOWSERVER	= 1, /**<未知服务器类型*/
	GLOBALSERVER	= 10,/**<全局管理服务器*/
	GATEWAYSERVER	= 20,/**<网关服务器*/
	GAMESERVER		= 30,/**<用户逻辑服务器*/
	SESSIONSERVER	= 40,/**<会话服务器>*/
	RECORDSERVER	= 50,/**<档案服务器>*/
	LOGINSERVER		= 60,/**<登录服务器>*/
	BILLSERVER		= 70, /**<计费服务器>*/
	LICENSESERVER	= 80,/**<用户验证服务器*/
	DEBUGSERVER	= 100,/**<debug服务器>*/
	MAX_SERVER,
};

#endif

