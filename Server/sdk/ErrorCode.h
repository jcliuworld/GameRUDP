#ifndef _ERRORCODE_H__
#define _ERRORCODE_H__
/** 
 * @file ErrorCode.h
 * @brief  错误代码返回定义
 * @author ljc jc.liuworld@hotmail.com
 * @date 2012-10-22
 */


#define AGEERR_UNKNOW 100
#define AGEERR_SHM  200


#define AGEERR_SHM_BADALLOC 		AGEERR_SHM - 1	//内存申请失败
#define AGEERR_SHM_NOT_EXISTS		AGEERR_SHM - 2  //SHM ID 失效



#endif
