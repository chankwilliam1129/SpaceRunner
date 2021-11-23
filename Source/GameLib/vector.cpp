#include "vector.h"

Vector2& Vector2::operator=(const Vector2& v)
{
    x = v.x; 
    y = v.y; 
    return *this;
}
Vector2& Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
    return *this;
}
Vector2& Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}
Vector2& Vector2::operator*=(float f)
{
    x *= f;
    y *= f;
    return *this;
}
Vector2& Vector2::operator/=(float f)
{
    x /= f;
    y /= f;
    return *this;
}
Vector2 Vector2::operator+() const
{
    return Vector2(x, y);
}
Vector2 Vector2::operator-() const
{
    return Vector2(-x, -y);
}
Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}
Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}
Vector2 Vector2::operator*(float f) const
{
    return Vector2(x * f, y * f);
}
Vector2 operator*(float f, const Vector2& v)
{
    return Vector2(v.x * f, v.y * f);
}
Vector2 Vector2::operator/(float f) const
{
    return Vector2(x / f, y / f);
}
bool Vector2::operator == (const Vector2& v) const
{
    return (x == v.x) && (y == v.y);
}
bool Vector2::operator != (const Vector2& v) const
{
    return (x != v.x) || (y != v.y);
}
//--------------------------------------------------------------
//  í∑Ç≥ÇÃ2èÊÇéÊìæ
//--------------------------------------------------------------
float vec2LengthSq(const Vector2& v)
{
    return v.x * v.x + v.y * v.y;
}
//--------------------------------------------------------------
//  í∑Ç≥ÇéÊìæ
//--------------------------------------------------------------
float vec2Length(const Vector2& v)
{
    return sqrtf(vec2LengthSq(v));
}
//--------------------------------------------------------------
//  í∑Ç≥Ç1Ç…Ç∑ÇÈ
//--------------------------------------------------------------
Vector2* vec2Normalize(const Vector2& v, Vector2* out)
{
    *out = v;
    float d = vec2Length(v);
    if (d < FLT_EPSILON) return out;
    *out /= d;
    return out;
}

Vector3& Vector3::operator=(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}
Vector3& Vector3::operator+=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
Vector3& Vector3::operator-=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
Vector3& Vector3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}
Vector3& Vector3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    return *this;
}
Vector3 Vector3::operator+() const
{
    return Vector3(x, y, z);
}
Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}
Vector3 Vector3::operator+(const Vector3& v) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}
Vector3 Vector3::operator-(const Vector3& v) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}
Vector3 Vector3::operator*(float f) const
{
    return Vector3(x * f, y * f, z * f);
}
Vector3 operator*(float f, const Vector3& v)
{
    return Vector3(v.x * f, v.y * f, v.z * f);
}
Vector3 Vector3::operator/(float f) const
{
    return Vector3(x / f, y / f, z / f);
}
bool Vector3::operator == (const Vector3& v) const
{
    return (x == v.x) && (y == v.y) && (z == v.z);
}
bool Vector3::operator != (const Vector3& v) const
{
    return (x != v.x) || (y != v.y) || (z != v.z);
}
Vector3::Vector3(const Vector2& v)
{
    x = v.x;
    y = v.y;
    z = 0;
}

Vector3 Vector3::Normalized()
{
    DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(this);
    DirectX::XMFLOAT3 result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVector3Normalize(vec));

    return { result.x,result.y,result.z };
}

Vector3 Vector3::Cross(Vector3& vector)
{
    DirectX::XMVECTOR vec1, vec2;
    vec1 = DirectX::XMLoadFloat3(this);
    vec2 = DirectX::XMLoadFloat3(&vector);
    DirectX::XMVECTOR vec = DirectX::XMVector3Cross(vec1, vec2);
    DirectX::XMFLOAT3 result;
    DirectX::XMStoreFloat3(&result, DirectX::XMVector3Normalize(vec));
    return { result.x,result.y,result.z };
}

DirectX::XMVECTOR Vector3::ToXMVECTOR()
{
    return DirectX::XMLoadFloat3(new DirectX::XMFLOAT3(x, y, z));
}

//--------------------------------------------------------------
//  Vector2ÇVector3Ç…ë„ì¸
//--------------------------------------------------------------
Vector3& Vector3::operator=(const Vector2& v)
{
    x = v.x;
    y = v.y;
    z = 0;
    return *this;
}