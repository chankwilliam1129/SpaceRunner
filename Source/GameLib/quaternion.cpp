#include "Quaternion.h"
#include <cmath>

// http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
// http://www.flipcode.com/documents/matrfaq.html
// http://db-in.com/blog/2011/04/cameras-on-opengl-es-2-x/
// http://wiki.beyondunreal.com/Legacy:Quaternion
// http://clb.demon.fi/MathGeoLib/docs/float3x3.cpp_code.html#612
// http://clb.demon.fi/MathGeoLib/docs/Quat_summary.php
// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
// http://www.koders.com/cpp/fid6E231B19C37F95D54405AEADD2C7CE1E84C5CEF1.aspx
// http://forums.create.msdn.com/forums/t/4574.aspx

const Quaternion Quaternion::identity(1.0f, 0.0f, 0.0f, 0.0f);

Quaternion::Quaternion() : w(0.0f), x(0.0f), y(0.0f), z(0.0f) { }
Quaternion::Quaternion(float W, float X, float Y, float Z) : w(W), x(X), y(Y), z(Z) { }
Quaternion::~Quaternion() { }

Quaternion::Quaternion(const Quaternion& q) 
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
}

Quaternion& Quaternion::operator=(const Quaternion& q) 
{
	if (this == &q) return *this;
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}


DirectX::XMMATRIX Quaternion::ToMatrix(Quaternion& q) 
{
	float sqw = q.w * q.w;
	float sqx = q.x * q.x;
	float sqy = q.y * q.y;
	float sqz = q.z * q.z;
	float invs = 1.0f / (sqx + sqy + sqz + sqw);

	DirectX::XMFLOAT4X4* matrix = new DirectX::XMFLOAT4X4();

	matrix->_11 = (sqx - sqy - sqz + sqw) * invs;
	matrix->_22 = (-sqx + sqy - sqz + sqw) * invs;
	matrix->_33 = (-sqx - sqy + sqz + sqw) * invs;

	float tmp1 = q.x * q.y;
	float tmp2 = q.z * q.w;
	matrix->_21 = 2.0 * (tmp1 + tmp2) * invs;
	matrix->_12 = 2.0 * (tmp1 - tmp2) * invs;

	tmp1 = q.x * q.z;
	tmp2 = q.y * q.w;
	matrix->_31 = 2.0 * (tmp1 - tmp2) * invs;
	matrix->_13 = 2.0 * (tmp1 + tmp2) * invs;

	tmp1 = q.y * q.z;
	tmp2 = q.x * q.w;
	matrix->_32 = 2.0 * (tmp1 + tmp2) * invs;
	matrix->_23 = 2.0 * (tmp1 - tmp2) * invs;

	matrix->_44 = 1.0f;

	return DirectX::XMLoadFloat4x4(matrix);
}

bool Quaternion::operator==(Quaternion& rhs)
{
	return (w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z);
}

bool Quaternion::operator!=(Quaternion& rhs) 
{
	return !(w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z);
}

Quaternion& Quaternion::operator*=(Quaternion& rhs)
{
	Quaternion q;

	q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
	q.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
	q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
	q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

	*this = q;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
	Quaternion q;

	q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
	q.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
	q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
	q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

	*this = q;
	return *this;
}

Quaternion Quaternion::operator*(Quaternion& rhs) 
{
	Quaternion q;

	q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
	q.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
	q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
	q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

	return q;
}

const Quaternion Quaternion::operator*(const Quaternion& rhs) 
{
	Quaternion q;

	q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
	q.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
	q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
	q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

	return q;
}

Quaternion Quaternion::Normalized() 
{
	float mag = sqrtf(w * w + x * x + y * y + z * z);
	return Quaternion(w / mag, x / mag, y / mag, z / mag);
}

Quaternion Quaternion::Conjugate() 
{
	return Quaternion(w, -x, -y, -z);
}

void Quaternion::Normalize()
{
	float mag = sqrtf(w * w + x * x + y * y + z * z);

	w /= mag;
	x /= mag;
	y /= mag;
	z /= mag;
}

Vector3 Quaternion::operator*(Vector3& rhs)
{

	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&rhs);
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVector3Transform(vec, Quaternion::ToMatrix(*this)));
	return { result.x,result.y,result.z };
}

const Vector3 operator*(Vector3& v, const Quaternion& m)
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&v);
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVector3Transform(vec, Quaternion::ToMatrix(Quaternion(m))));
	return { result.x,result.y,result.z };
}

Vector3 operator*(Vector3& v, Quaternion& m) 
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&v);
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVector3Transform(vec, Quaternion::ToMatrix(Quaternion(m))));
	return { result.x,result.y,result.z };
}

const Vector3 Quaternion::operator*(const Vector3& rhs) 
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&rhs);
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVector3Transform(vec, Quaternion::ToMatrix(*this)));
	return { result.x,result.y,result.z };
}

Quaternion Quaternion::AngleAxis(float angle, Vector3& axis) 
{
	Vector3 vn = axis.Normalized();

	angle *= 0.0174532925f; // To radians!
	angle *= 0.5f;
	float sinAngle = sin(angle);

	return Quaternion(cos(angle), vn.x * sinAngle, vn.y * sinAngle, vn.z * sinAngle);
}

Quaternion Quaternion::Euler(float X, float Y, float Z) 
{
	X *= 0.0174532925f; // To radians!
	Y *= 0.0174532925f; // To radians!
	Z *= 0.0174532925f; // To radians!

	X *= 0.5f;
	Y *= 0.5f;
	Z *= 0.5f;

	float sinx = sinf(X);
	float siny = sinf(Y);
	float sinz = sinf(Z);
	float cosx = cosf(X);
	float cosy = cosf(Y);
	float cosz = cosf(Z);

	Quaternion q;

	q.w = cosx * cosy * cosz + sinx * siny * sinz;
	q.x = sinx * cosy * cosz + cosx * siny * sinz;
	q.y = cosx * siny * cosz - sinx * cosy * sinz;
	q.z = cosx * cosy * sinz - sinx * siny * cosz;

	return q;
}

Quaternion Quaternion::Euler(Vector3 vec)
{
	return Euler(vec.x, vec.y, vec.z);
}

void Quaternion::SetEuler(float X, float Y, float Z) 
{
	X *= 0.0174532925f; // To radians!
	Y *= 0.0174532925f; // To radians!
	Z *= 0.0174532925f; // To radians!

	X *= 0.5f;
	Y *= 0.5f;
	Z *= 0.5f;

	float sinx = sinf(X);
	float siny = sinf(Y);
	float sinz = sinf(Z);
	float cosx = cosf(X);
	float cosy = cosf(Y);
	float cosz = cosf(Z);

	w = cosx * cosy * cosz + sinx * siny * sinz;
	x = sinx * cosy * cosz + cosx * siny * sinz;
	y = cosx * siny * cosz - sinx * cosy * sinz;
	z = cosx * cosy * sinz - sinx * siny * cosz;
}

void Quaternion::ToAngleAxis(float* angle, Vector3* axis)
{
	*angle = acosf(w);
	float sinz = sinf(*angle);

	if (fabsf(sinz) > 1e-4f)
	{
		sinz = 1.0f / sinz;

		axis->x = x * sinz;
		axis->y = y * sinz;
		axis->z = z * sinz;

		*angle *= 2.0f * 57.2957795f;
		if (*angle > 180.0f)
		{
			*angle = 360.0f - *angle;
		}
	}
	else 
	{
		*angle = 0.0f;
		axis->x = 1.0f;
		axis->y = 0.0f;
		axis->z = 0.0f;
	}
}

Quaternion Quaternion::Inverse(Quaternion& rotation)
{
	return Quaternion(rotation.w, -1.0f * rotation.x, -1.0f * rotation.y, -1.0f * rotation.z);
}

float Quaternion::Dot(Quaternion& a, Quaternion& b)
{
	return (a.w * b.w + a.x * b.x * a.y * b.y + a.z * b.z);
}

float Quaternion::Dot(Quaternion& b)
{
	return (w * b.w + x * b.x * y * b.y + z * b.z);
}

float Quaternion::Angle(Quaternion& a, Quaternion& b)
{
	float degrees = acosf((b * Quaternion::Inverse(a)).w) * 2.0f * 57.2957795f;
	if (degrees > 180.0f)
	{
		return 360.0f - degrees;
	}
	return degrees;
}

Quaternion operator*(float f, Quaternion& m)
{
	return Quaternion(m.w * f, m.x * f, m.y * f, m.z * f);
}

const Quaternion operator*(float f, const Quaternion& m) 
{
	return Quaternion(m.w * f, m.x * f, m.y * f, m.z * f);
}

Quaternion Quaternion::operator*(float& f)
{
	return Quaternion(w * f, x * f, y * f, z * f);
}

const Quaternion Quaternion::operator*(const float& f)
{
	return Quaternion(w * f, x * f, y * f, z * f);
}

Quaternion Quaternion::operator+(Quaternion& rhs)
{
	return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}

const Quaternion Quaternion::operator+(const Quaternion& rhs)
{
	return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}

Quaternion Quaternion::Lerp(Quaternion& from, Quaternion& to, float t) 
{
	Quaternion src = from * (1.0f - t);
	Quaternion dst = to * t;

	Quaternion q = src + dst;
	return q;
}

Quaternion Quaternion::Slerp(Quaternion& from, Quaternion& to, float t) 
{
	float cosTheta = Quaternion::Dot(from, to);
	Quaternion temp(to);

	if (cosTheta < 0.0f)
	{
		cosTheta *= -1.0f;
		temp = temp * -1.0f;
	}

	float theta = acosf(cosTheta);
	float sinTheta = 1.0f / sinf(theta);

	return sinTheta * (
		((Quaternion)(from * sinf(theta * (1.0f - t)))) +
		((Quaternion)(temp * sinf(t * theta)))
		);
}
