#pragma once
#ifndef _H_QUATERNION_
#define _H_QUATERNION_

#include <DirectXMath.h>
#include "vector.h"

class Quaternion 
{
public:
	float x, y, z, w;

public:
	Quaternion();
	Quaternion(float W, float X, float Y, float Z);
	Quaternion(const Quaternion& q);
	~Quaternion();
	Quaternion& operator=(const Quaternion&);

	bool operator==(Quaternion& rhs);
	bool operator!=(Quaternion& rhs);

	Quaternion& operator*=(Quaternion& rhs);
	Quaternion& operator*=(const Quaternion& rhs);
	Quaternion operator*(Quaternion& rhs);
	const Quaternion operator*(const Quaternion& rhs);
	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);
	Vector3 operator*(Vector3& rhs);
	const Vector3 operator*(const Vector3& rhs);

	Quaternion Normalized();
	void Normalize();
	Quaternion Conjugate(); // Same as inverse

	void ToAngleAxis(float* angle, Vector3* axis);
	void SetEuler(float X, float Y, float Z);
	float Dot(Quaternion& b);
	static Quaternion Slerp(Quaternion& from, Quaternion& to, float t);
	static Quaternion Lerp(Quaternion& from, Quaternion& to, float t);
	static float Angle(Quaternion& a, Quaternion& b);
	static float Dot(Quaternion& a, Quaternion& b);
	static Quaternion AngleAxis(float angle, Vector3& axis);
	static Quaternion Inverse(Quaternion& rotation);
	static Quaternion Euler(float X, float Y, float Z);
	static Quaternion Euler(Vector3 vec);
	static DirectX::XMMATRIX ToMatrix(Quaternion& q);

	static const Quaternion identity;
};

const Vector3 operator*(Vector3& v, const Quaternion& m);
Vector3 operator*(Vector3& v, Quaternion& m);
Quaternion operator*(float f, Quaternion& m);
const Quaternion operator*(float f, const Quaternion& m);
#endif