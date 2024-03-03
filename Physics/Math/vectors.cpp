#include "vectors.h"
#include <cmath>
#include <cfloat>

#define ABSOLUTE(x,y) (fabsf((x)-(y)) <= FLT_EPSILON)
#define RELATIVE(x,y) \
		(fabsf((x)-(y)) <= FLT_EPSILON * Max(fabsf(x),fabsf(y)))

float CorrectDegrees(float degrees) {
	while (degrees > 360.0f) {
		degrees -= 360.0f;
	}
	while (degrees < -360.0f) {
		degrees += 360.0f;
	}
	return degrees;
}

#ifndef RAD2DEG
float RAD2DEG(float radians) {
	float degrees = radians * 57.295754f;
	degrees = CorrectDegrees(degrees);
	return degrees;
}
#endif
#ifndef DEG2RAD
float DEG2RAD(float degrees) {
	degrees = CorrectDegrees(degrees);
	float radians = degrees * 0.0174533f;
	return radians;
}
#endif

vec2 operator+(const vec2& l, const vec2& r) {
	return { l.x + r.x,l.y + r.y };
}

vec3 operator+(const vec3& l, const vec3& r) {
	return { l.x + r.x,l.y + r.y,l.z + r.z };
}

vec2 operator-(const vec2& l, const vec2& r) {
	return { l.x - r.x,l.y - r.y };
}

vec3 operator-(const vec3& l, const vec3& r) {
	return { l.x - r.x,l.y - r.y,l.z - r.z };
}

vec2 operator*(const vec2& l, const vec2& r) {
	return { l.x * r.x,l.y * r.y };
}

vec3 operator*(const vec3& l, const vec3& r) {
	return { l.x * r.x,l.y * r.y,l.z * r.z };
}

vec2 operator*(const vec2& l, float r) {
	return { l.x * r,l.y * r };
}

vec3 operator*(const vec3& l, float r) {
	return { l.x * r,l.y * r,l.z * r };
}

bool operator==(const vec2& l, const vec2& r) {
	return CMP(l.x, r.x) && CMP(l.y, r.y);
}

bool operator==(const vec3& l, const vec3& r) {
	return CMP(l.x, r.x) && CMP(l.y, r.y) && CMP(l.z, r.z);
}

bool operator!=(const vec2& l, const vec2& r) {
	return !(l==r);
}

bool operator!=(const vec3& l, const vec3& r) {
	return !(l == r);
}

float Dot(const vec2& l, const vec2& r) {
	return l.x * r.x + l.y * r.y;
}

float Dot(const vec3& l, const vec3& r) {
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

float Magnitude(const vec2& v) {
	return sqrtf(Dot(v, v));
}

float Magnitude(const vec3& v) {
	return sqrtf(Dot(v, v));
}

float MagnitudeSq(const vec2& v) {
	return Dot(v, v);
}

float MagnitudeSq(const vec3& v) {
	return Dot(v, v);
}
float Distance(const vec3& p1, const vec3& p2) {
	vec3 t = p1 - p2;
	return Magnitude(t);
}

void Normalize(vec2& v) {
	v = v * (1.0f / Magnitude(v));
}

void Normalize(vec3& v) {
	v = v * (1.0f / Magnitude(v));
}

vec2 Normalized(const vec2& v) {
	return v * (1.0f / Magnitude(v));
}

vec3 Normalized(const vec3& v) {
	return v * (1.0f / Magnitude(v));
}


/// <summary>
/// 结果就是求
/// (det是行列式，这里不是真正0的行列式，
/// 而是把i,j,k作为向量代入,A×B的结果等价于下列矩阵求行列式)
/// [这种方法被称作笛卡尔坐标式]
///     i   j   k
/// det Ax  Ay  Az
///     Bx  By  Bz
/// 至于上述方法为何成立，可以看一下下面的视频
/// https://www.bilibili.com/video/BV1Zd4y1n7eB/?vd_source=1140bfac569fe6b7e83c9f1282924d06
/// 
/// 叉乘的定义:
/// 找到一个和l以及r两个向量均垂直的向量p，且p的长度等于l和r围成的四边形面积
/// 然后推导出叉积公式
/// </summary>
/// <param name="l"></param>
/// <param name="r"></param>
/// <returns></returns>
vec3 Cross(const vec3& l, const vec3& r) {
	vec3 result;
	result.x = l.y * r.z - l.z * r.y;
	result.y = l.z * r.x - l.x * r.z;
	result.z = l.x * r.y - l.y * r.x;
	return result;
}

float Angle(const vec2& l, const vec2& r) {
	float m = sqrtf(MagnitudeSq(l) * MagnitudeSq(r));
	return acos(Dot(l, r) / m);
}

float Angle(const vec3& l, const vec3& r) {
	float m = sqrtf(MagnitudeSq(l) * MagnitudeSq(r));
	return acos(Dot(l, r) / m);
}

vec2 Project(const vec2& length, const vec2& direction) {
	float dot = Dot(length, direction);
	float magSq = Magnitude(direction);
	return direction * (dot / magSq);
}

vec3 Project(const vec3& length, const vec3& direction) {
	float dot = Dot(length, direction);
	float magSq = MagnitudeSq(direction);
	return direction * (dot / magSq);
}

//垂直于投影的
vec2 Perpendicular(const vec2& len, const vec2& dir) {
	return len - Project(len, dir);
}

vec3 Perpendicular(const vec3& len, const vec3& dir) {
	return len - Project(len, dir);
}

vec2 Reflection(const vec2& vec, const vec2& normal) {
	float d = Dot(vec, normal);
	return vec - normal * (d * 2.0f);
}

vec3 Reflection(const vec3& vec, const vec3& normal) {
	float d = Dot(vec, normal);
	return vec - normal * (d * 2.0f);
}