#include "pch.h"
#include "Vector3.h"

using namespace BlahEngine;

const float  PI = 3.14159265358979f;

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

float Vector3::Length() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

Vector3 Vector3::GetNorm() const
{
	float length = Length();
	return { X / length, Y / length, Z / length };
}

Vector3 Vector3::GetRotX(float angle) const
{
	float rad = angle * PI  / 180.0f;
	float cosA = cosf(rad);
	float sinA = sinf(rad);
	return { X, Y * cosA - Z * sinA, Y * sinA + Z * cosA };
}

Vector3 Vector3::GetRotY(float angle) const
{
	float rad = angle * PI / 180.0f;
	float cosA = cosf(rad);
	float sinA = sinf(rad);
	return { X * cosA + Z * sinA, Y, -X * sinA + Z * cosA };
}

Vector3 Vector3::GetRotZ(float angle) const
{
	float rad = angle * PI / 180.0f;
	float cosA = cosf(rad);
	float sinA = sinf(rad);
	return { X * cosA - Y * sinA, X * sinA + Y * cosA, Z };
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
