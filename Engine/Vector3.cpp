#include "pch.h"
#include "Vector3.h"

using namespace BlahEngine;
using namespace DirectX;

Vector3::Vector3():
	X(0),
	Y(0),
	Z(0) {}

Vector3::Vector3(float x, float y, float z):
	X(x),
	Y(y),
	Z(z) { }

Vector3::Vector3(const Vector3& other):
	X(other.X),
	Y(other.Y),
	Z(other.Z) {}

Vector3::Vector3(Vector3&& other) noexcept:
	X(other.X),
	Y(other.Y),
	Z(other.Z) {}

Vector3::~Vector3() {}

XMVECTOR Vector3::ToXMVector() const
{
	return { X, Y, Z };
}

XMFLOAT4 Vector3::ToXMFloat4() const
{
	return { X, Y, Z, 0};
}

float Vector3::Length() const
{
	if (X == 0 && Y == 0 && Z == 0)
		return 0;
	return sqrt(X * X + Y * Y + Z * Z);
}

Vector3 Vector3::GetNorm() const
{
	float length = Length();
	if (length == 0)
		return { 0, 0, 0 };
	return { X / length, Y / length, Z / length };
}

Vector3 Vector3::Rotate(float roll, float pitch, float yaw) const
{
	auto q = XMQuaternionRotationRollPitchYaw(pitch * XM_PI / 180, yaw * XM_PI / 180, roll * XM_PI / 180);
	return Rotate(XMQuaternionNormalize(q));
}


Vector3 Vector3::Rotate(const XMVECTOR& quaternion) const
{
	auto v = ToXMVector();
	auto inv = XMQuaternionInverse(quaternion);
	v = XMQuaternionMultiply(quaternion, v);
	v = XMQuaternionMultiply(v, inv);
	return { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };

	//does not work!
	//auto v = XMVector3Rotate({X, Y, Z}, quaternion);
	//return { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
}

Vector3& Vector3::operator=(const Vector3& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

Vector3& Vector3::operator=(Vector3&& other) noexcept
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	return *this;
}

bool Vector3::operator==(const Vector3& other) const
{
	return abs(X - other.X) <= EQUALITY_TOLERANCE &&
		abs(Y - other.Y) <= EQUALITY_TOLERANCE &&
		abs(Z - other.Z) <= EQUALITY_TOLERANCE;
}

bool Vector3::operator!=(const Vector3& other) const
{
	return !(*this == other);
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	return *this;
}

Vector3& Vector3::operator*=(const Vector3& other)
{
	X *= other.X;
	Y *= other.Y;
	Z *= other.Z;
	return *this;
}

Vector3& Vector3::operator/=(const Vector3& other)
{
	X /= other.X;
	Y /= other.Y;
	Z /= other.Z;
	return *this;
}

Vector3& Vector3::operator*=(float k)
{
	X *= k;
	Y *= k;
	Z *= k;
	return *this;
}

Vector3& Vector3::operator/=(float k)
{
	X /= k;
	Y /= k;
	Z /= k;
	return *this;
}

namespace BlahEngine
{
Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return {a.X + b.X, a.Y + b.Y, a.Z + b.Z};
}

Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return {a.X - b.X, a.Y - b.Y, a.Z - b.Z};
}

Vector3 operator*(const Vector3& a, const Vector3& b)
{
	return {a.X * b.X, a.Y * b.Y, a.Z * b.Z};
}

Vector3 operator/(const Vector3& a, const Vector3& b)
{
	return {a.X / b.X, a.Y / b.Y, a.Z / b.Z};
}

Vector3 operator-(const Vector3& a)
{
	return { -a.X, -a.Y, -a.Z };
}

Vector3 operator*(const Vector3& a, float k)
{
	return { a.X * k, a.Y * k, a.Z * k };
}

Vector3 operator/(const Vector3& a, float k)
{
	return { a.X / k, a.Y / k, a.Z / k };
}
}
