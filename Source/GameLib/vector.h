#pragma once
#include <DirectXMath.h>

class Vector2 : public DirectX::XMFLOAT2
{
public:
    Vector2() : DirectX::XMFLOAT2(0, 0) {}
    Vector2(float x, float y) : DirectX::XMFLOAT2(x, y) {}
    Vector2(const Vector2& v) { x = v.x; y = v.y; }
    ~Vector2() {}

    Vector2& operator=(const Vector2&);
    Vector2& operator+=(const Vector2&);
    Vector2& operator-=(const Vector2&);
    Vector2& operator*=(float);
    Vector2& operator/=(float);

    Vector2 operator+() const;
    Vector2 operator-() const;

    Vector2 operator+(const Vector2&) const;
    Vector2 operator-(const Vector2&) const;
    Vector2 operator*(float) const;
    friend Vector2 operator*(float, const Vector2&);
    Vector2 operator/(float) const;

    bool operator == (const Vector2&) const;
    bool operator != (const Vector2&) const;
};

float vec2LengthSq(const Vector2&);
float vec2Length(const Vector2&);
Vector2* vec2Normalize(const Vector2&, Vector2*);

class Vector3 : public DirectX::XMFLOAT3
{
public:
    Vector3() : DirectX::XMFLOAT3(0, 0, 0) {}
    Vector3(float x, float y, float z) : DirectX::XMFLOAT3(x, y, z) {}
    Vector3(const Vector3& v) { x = v.x; y = v.y; z = v.z; }
    ~Vector3() {}

    Vector3& operator=(const Vector3&);
    Vector3& operator+=(const Vector3&);
    Vector3& operator-=(const Vector3&);
    Vector3& operator*=(float);
    Vector3& operator/=(float);

    Vector3 operator+() const;
    Vector3 operator-() const;

    Vector3 operator+(const Vector3&) const;
    Vector3 operator-(const Vector3&) const;
    Vector3 operator*(float) const;
    friend Vector3 operator*(float, const Vector3&);
    Vector3 operator/(float) const;

    bool operator == (const Vector3&) const;
    bool operator != (const Vector3&) const;

    Vector3(const Vector2& v);
    Vector3& operator=(const Vector2&);

    Vector3 Cross(Vector3& vector);
    Vector3 Normalized();

    DirectX::XMVECTOR ToXMVECTOR();
};

class Vector4 : public DirectX::XMFLOAT4
{
public:
    Vector4() : DirectX::XMFLOAT4(0, 0, 0, 0) {}
    Vector4(float x, float y, float z, float w) : DirectX::XMFLOAT4(x, y, z, w) {}
    ~Vector4() {}
};
