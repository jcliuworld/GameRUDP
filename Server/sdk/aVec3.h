#ifndef _aVec3_H__ 
#define _aVec3_H__

#include "aDef.h"

class aVector3
{
	public:
		float x;
		float y;
		float z;

		//Constructors
		aVector3();
		aVector3(const float &x, const float &y, const float &z);

		//Math
		aVector3		operator+(const aVector3& vector) const;
		aVector3		operator-(const aVector3& vector) const;
		aVector3		operator*(const aVector3& vector) const;
		aVector3		operator/(const aVector3& vector) const;
		aVector3&	operator+=(const aVector3& vector);
		aVector3&	operator-=(const aVector3& vector);
		aVector3&	operator*=(const aVector3& vector);
		aVector3&	operator/=(const aVector3& vector);

		//aVector3 by Float Math
		aVector3		operator-() const;
		aVector3		operator+(float const&num) const;
		aVector3		operator-(float const&num) const;
		aVector3		operator*(float const&num) const;
		aVector3		operator/(float const&num) const;
		aVector3&	operator+=(float const&num);
		aVector3&	operator-=(float const&num);
		aVector3&	operator*=(float const&num);
		aVector3&	operator/=(float const&num);

		//Boolean Equals Operators
		bool		operator==(const aVector3& vector) const;
		bool		operator!=(const aVector3& vector) const;

		bool		isFloatEqual(float f1,float f2,float fEpsilon)const;
		bool		isIntEqual(float f1,float f2)const;
		bool		isEqula(const aVector3& vector)const;

		//Static Predefines
		static const aVector3 Zero;
		static const aVector3 Left;
		static const aVector3 Right;
		static const aVector3 Up;
		static const aVector3 Down;
		static const aVector3 Forward;
		static const aVector3 Backward;

		//Print out
		friend std::ostream& operator<< (std::ostream& ofs, const aVector3& vector);
};
#endif 

