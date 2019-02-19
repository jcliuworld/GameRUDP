#include "aVec3.h"
 aVector3::aVector3()
{
	x = 0;
	y = 0;
	z = 0;
}
 aVector3::aVector3(const float &x, const float &y, const float &z): x( x ), y( y ), z( z ){}

// Math
 aVector3 aVector3::operator+(const aVector3& vector) const
{
	return aVector3(x + vector.x, y + vector.y, z + vector.z);
}

 aVector3 aVector3::operator-(const aVector3& vector) const
{
	return aVector3(x - vector.x, y - vector.y, z - vector.z);
}

 aVector3 aVector3::operator*(const aVector3& vector) const
{
	return aVector3(x * vector.x, y * vector.y, z * vector.z);
}

 aVector3 aVector3::operator/(const aVector3& vector) const
{
	return aVector3(x / vector.x, y / vector.y, z / vector.z);
}

 aVector3& aVector3::operator+=(const aVector3& vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
	return *this;
}

 aVector3& aVector3::operator-=(const aVector3& vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	return *this;
}

 aVector3& aVector3::operator*=(const aVector3& vector)
{
	x *= vector.x;
	y *= vector.y;
	z *= vector.z;
	return *this;
}

 aVector3& aVector3::operator/=(const aVector3& vector)
{
	x /= vector.x;
	y /= vector.y;
	z /= vector.z;
	return *this;
}

 aVector3 aVector3::operator-() const
{
	return aVector3(-x,-y,-z);
}

 aVector3 aVector3::operator-(float const&num) const
{
	return aVector3(x - num, y - num, z - num);
}

 aVector3 aVector3::operator+(float const&num) const
{
	return aVector3(x + num, y + num, z + num);
}

 aVector3 aVector3::operator*(float const&num) const
{
	return aVector3(x * num, y * num, z * num);
}

 aVector3 aVector3::operator/(float const&num) const
{
	return aVector3(x / num, y / num, z / num);
}

 aVector3& aVector3::operator+=(const float& num)
{
	x += num;
	y += num;
	z += num;
	return *this;
}

 aVector3& aVector3::operator-=(const float& num)
{
	x -= num;
	y -= num;
	z -= num;
	return *this;
}
 aVector3& aVector3::operator*=(const float& num)
{
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

 aVector3& aVector3::operator/=(const float& num)
{
	x /= num;
	y /= num;
	z /= num;
	return *this;
}

 bool aVector3::operator==(const aVector3& vector) const
{
	return x == vector.x && y == vector.y && z == vector.z;
}

 bool aVector3::operator!=(const aVector3& vector) const
{
	return x != vector.x || y != vector.y || z != vector.z;
}

 bool aVector3::isFloatEqual(float f1,float f2,float fEpsilon)const
{
	if(fEpsilon < 0)
		return false;

	if(fEpsilon == 0)
		return f1==f2;	

	return (f1 >= f2-fEpsilon) &&  (f1 <= f2+fEpsilon);
}

 bool aVector3::isIntEqual(float f1,float f2)const
{
	return int(f1) == int(f2);	
}

 bool aVector3::isEqula(const aVector3& vector)const
{
	return isFloatEqual(x,vector.x,EPSILON_FLT) && isFloatEqual(y,vector.y,EPSILON_FLT) && isFloatEqual(z,vector.z,EPSILON_FLT);
}

const aVector3 aVector3::Zero = aVector3(0,0,0);
const aVector3 aVector3::Left(-1.f, 0.f, 0.f);
const aVector3 aVector3::Right(1.f, 0.f, 0.f);
const aVector3 aVector3::Up(0.f, 1.f, 0.f);
const aVector3 aVector3::Down(0.f, -1.f, 0.f);
const aVector3 aVector3::Forward(0.f, 0.f, 1.f);
const aVector3 aVector3::Backward(0.f, 0.f, -1.f);

//-----------------------------------------------------------------------------------
//Fast non-member functions
//-----------------------------------------------------------------------------

 float dot(aVector3 const&vec1,aVector3 const&vec2){
	return vec2.x*vec1.x+vec2.y*vec1.y+vec2.z*vec1.z;
}

 float lengthSquared(aVector3 const&vector){
	return dot(vector,vector);
}

 float magnitude(aVector3 const &vector){
	return sqrt(dot(vector, vector));
}

 float distanceSquared(aVector3 const&vec1,aVector3 const&vec2){
	double ySeparation = vec1.y - vec2.y;
	double xSeparation = vec1.x - vec2.x;
	double zSeparation = vec1.z - vec2.z;
	return (float)ySeparation*ySeparation + xSeparation*xSeparation+ zSeparation*zSeparation;
}

 float distance(aVector3 const&vec1,aVector3 const&vec2){
	double ySeparation = vec1.y - vec2.y;
	double xSeparation = vec1.x - vec2.x;
	double zSeparation = vec1.z - vec2.z;
	return sqrtf(ySeparation*ySeparation + xSeparation*xSeparation+ zSeparation*zSeparation);
}

//Voids
 void setMagnitude(aVector3& vec2, float const&num)
{
	vec2 *= num / magnitude(vec2);
}

//Sets
 void setLengthSquared(aVector3& vector, float const&num)
{
	float vecLength = lengthSquared(vector);

	if(vecLength == 0)
	{
		return;
	}

	vector *= num / sqrt(vecLength);
}

//Returns a normalized aVector3 of the aVector3 object
 aVector3 normalize(aVector3 const &vector){
	return vector * (1.f /(magnitude(vector) + FLT_MIN));
}

 void toNormalized(aVector3 &vector){
	vector *= (1.f /(magnitude(vector) + FLT_MIN));
}

 void toPolar(aVector3 &vector,float const&x, float const&y, float const&z)
{
	vector=aVector3(
			atan2(y,x),
			sqrt(x * x + y * y),
			z);
}

 void toCartesian(aVector3 &vector,float const&radius, float const&angle, float const&z)
{
	vector= aVector3(
			radius * cos(angle),
			radius * sin(angle),
			z);
}

 void toEuler(aVector3 &vector,aVector3 axis, float const& angle)
{
	aVector3 out = aVector3();

	float s = sin(angle);
	float c = cos(angle);
	float t = static_cast<float>(1.0) - c;

	if ((axis.x * axis.y * t + axis.z * s) > static_cast<float>(0.998))// north pole singularity detected
	{
		out.y = static_cast<float>(2.0) * atan2(axis.x * sin(angle / static_cast<float>(2.0)), cos(angle / static_cast<float>(2.0)));
		out.z = static_cast<float>(1.57079633);//PI/2
		out.x = 0;
		vector= out;
		return;
	}
	if ((axis.x * axis.y * t + axis.z * s) < static_cast<float>(-0.998))// south pole singularity detected
	{
		out.y = static_cast<float>(-2.0) * atan2(axis.x * sin(angle / static_cast<float>(2.0)), cos(angle / static_cast<float>(2.0)));
		out.z = -static_cast<float>(1.57079633);//PI/2
		out.x = 0;
		vector=out;
		return;
	}
	out.y = atan2(axis.y * s - axis.x * axis.z * t , 1 - (axis.y * axis.y + axis.z * axis.z ) * t);
	out.z = asin(axis.x * axis.y * t + axis.z * s) ;
	out.x = atan2(axis.x * s - axis.y * axis.z * t , 1 - (axis.x * axis.x + axis.z * axis.z) * t);
	vector= out;
}

 aVector3 cross(aVector3 const &vec2, aVector3 const &vec1){
	return aVector3(
			vec2.y * vec1.z-vec1.y * vec2.z,
			-(vec2.x * vec1.z - vec1.x * vec2.z),
			vec2.x * vec1.y -vec1.x * vec2.y);
}

 const aVector3 perpendicular(aVector3 const &vector){
	return aVector3(-vector.z,vector.x,-vector.y);//right
}

 void truncate(aVector3 &vector,double const& max){
	if (magnitude(vector) > max)
	{
		toNormalized(vector);
		vector *= max;
	}
}
 void toZero(aVector3 &vec2){
	vec2.x=0.0f;
	vec2.y=0.0f;
	vec2.z=0.0f;
}

 const bool isZero(aVector3 const&vector){
	return (vector.x*vector.x+vector.y*vector.y+vector.z*vector.z)<(float)0.000001;//EPSILON
}

 const int sign(const aVector3& vec1,const aVector3& vec2){
	if (vec2.y*vec1.x > vec2.x*vec1.y)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

 aVector3 negate(aVector3 const &vec1){
	return aVector3(-vec1.x, -vec1.y,-vec1.z);
}


 aVector3 reflect(aVector3 const& vec1, const aVector3& vec2 )
{
	aVector3 out;
	const float dotProductTimesTwo = dot(vec2,vec1) * 2.0f;
	out.x=vec2.x -(dotProductTimesTwo * vec1.x);
	out.y=vec2.y - (dotProductTimesTwo * vec1.y);
	out.z=vec2.z -(dotProductTimesTwo * vec1.z);
	return out;
}

 aVector3 lerp(aVector3 const&vec1, aVector3 const&vec2, float const& num)
{
	return (vec1 + ((vec2 - vec1)*num));
}

 aVector3 slerp(aVector3 const& vec1, aVector3 const&vec2, float const& num)
{
	float doot = dot(vec1, vec2);
	//math.clamp(doot, -1,1);
	float theta = acos(doot)*num;
	aVector3 RelativeVec = vec2 - vec1*doot;
	toNormalized(RelativeVec);
	return ((vec1*cos(theta)) + (RelativeVec*sin(theta)));
}

 aVector3 nlerp(aVector3 const& vec1, aVector3 const&vec2, float const& num)
{
	return normalize(lerp(vec1,vec2,num));
}

 std::ostream& operator<< (std::ostream& os, const aVector3& vector)
{
	std::stringstream stream;
	stream << "x: " << vector.x << ", y: " << vector.y << ", z: " << vector.z;
	os.write(const_cast<char*>(stream.str().c_str()), static_cast<std::streamsize>(stream.str().size() * sizeof(char)) );
	return os;
}


