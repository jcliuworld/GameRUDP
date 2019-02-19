#include "aPos.h"
/** 
 * @brief 构造函数
 */

aPos::aPos() 
{
}
/** 
 * @brief 构造函数
 */

aPos::aPos(const float & x, const float &y,const float &z) : aVector3(x,y,z)
{

}

bool aPos::isSame(const aPos& pos)const
{
	return isIntEqual(x,pos.x) && isIntEqual(y,pos.y);	
}

const int aPos::get2DDir(const aPos & pos)
{
	if(isSame(pos))
		return DIR_ERROR;

	if( isIntEqual(x , pos.x ) && y > pos.y)
		return DIR_UP;	
	else if( x < pos.x && y > pos.y)
		return DIR_UPRIGHT;
	else if ( x < pos.x && isIntEqual (y , pos.y) )
		return DIR_RIGHT;
	else if ( x < pos.x && y < pos.y)
		return DIR_RIGHTDOWN;
	else if ( isIntEqual(x , pos.x ) && y < pos.y )
		return DIR_DOWN;
	else if ( x > pos.x && y < pos.y)
		return DIR_DOWNLEFT;
	else if ( x > pos.x && isIntEqual (y , pos.x ) )
		return DIR_LEFT;
	else if ( x >  pos.x && y > pos.y)
		return DIR_LEFTUP;
	return DIR_ERROR;
}

