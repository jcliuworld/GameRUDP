#ifndef _aEvent_H__
#define _aEvent_H__


/** 
* @file aEvent.h
* @brief 脚本系统事件封装
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-28
*/

#include "aEntry.h"
#include "aDef.h"
namespace EVENT
{
	const BYTE EVNET_CMD_QUEST	= 	1;
	const BYTE EVNET_CMD_PK		=	2;

	struct stEVENT
	{
		BYTE cmd;
		BYTE para;
		aEntry * source;
		aEntry * dest;
		DWORD dwTimestamp;
		DWORD dateSize;
		void * date;
	};	
};

#endif
