#ifndef _aEntryBase_H__
#define _aEntryBase_H__

#include <strings.h>
#include "aDef.h"
#include "aNoneCopyable.h"

#pragma pack(1)

struct aEntryBase
{
	/** 
	 * @brief Entry唯一ID
	 */
	DWORD id;
	/** 
	 * @brief Entry临时ID（本服务器唯一ID）
	 */

	DWORD tempid;
	/** 
	 * @brief Entry名
	 */

	char name[MAX_NAMESIZE+1];
	/** 
	 * @brief 构造函数
	 */

	aEntryBase()
	{
		id = 0;

		tempid = 0;

		bzero(name,sizeof(name));
	};

};
/**
 * @brief Entry回调函数
 */
template <typename T,typename RTValue = bool>
struct aEntryCallBack
{
	virtual RTValue exec( T * e) = 0;
	virtual ~aEntryCallBack(){};
};

#endif
