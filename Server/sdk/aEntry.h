#ifndef _aEntry_H__
#define _aEntry_H__
#include "aEntryBase.h"

#pragma pack(1)
/** 
* @file aEntry.h
* @brief      Entry管理基础结构
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/



/** 
* @brief Entry结构 禁用拷贝构造函数
*/

struct aEntry : public aEntryBase,private aNoneCopyable
{
	virtual ~aEntry(){};
	aEntry():aEntryBase()
	{

	};
};
#pragma pack()

#endif


