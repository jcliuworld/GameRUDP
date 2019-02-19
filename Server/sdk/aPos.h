#ifndef _aPos_H__
#define _aPos_H__
#pragma once

/** 
 * @file aPos.h
 * @brief 场景坐标封装
 * @author ljc jc.liuworld@hotmail.com
 * @date 2010-05-19
 */
#include <stdlib.h>
#include <algorithm>
#include "aDef.h"
#include "aVec3.h"
/** 
* @brief 两个坐标的相对方向
*/

enum
{
	DIR_UP		=	0,/**<上*/
	DIR_UPRIGHT	=	1,/**<右上*/
	DIR_RIGHTUP	=	1,/**<右上*/
	DIR_RIGHT	=	2,/**<右*/
	DIR_RIGHTDOWN	=	3,/**<右下*/
	DIR_DOWNRIGHT	=	3,/**<右下*/
	DIR_DOWN	=	4,/**<下*/
	DIR_DOWNLEFT	=	5,/**左下*/
	DIR_LEFTDOWN	=	5,/**<左下*/
	DIR_LEFT	=	6,/**<左*/
	DIR_LEFTUP	=	7,/**<左上*/
	DIR_UPLEFT	=	7,/**<左上*/
	DIR_ERROR	=	8 /**<错误方向*/

};

struct aPos : public aVector3
{
	/** 
	 * @brief 构造函数
	 */

	aPos(); 
	/** 
	 * @brief 构造函数
	 */

	aPos(const float & x, const float &y,const float &z); 
	
	bool isSame(const aPos& pos)const;

	const int get2DDir(const aPos & pos);

};

#endif
