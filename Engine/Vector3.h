#pragma once

namespace BlahEngine
{
struct Vector3
{
public:
	float X;
	float Y;
	float Z;

	Vector3();

	Vector3(float x, float y, float z);

	Vector3(const Vector3& other);

	Vector3(Vector3&& other) noexcept;

	~Vector3();

	DirectX::XMVECTOR ToXMVector() const;

	float Length() const;

	Vector3 GetNorm() const;

	Vector3 Rotate(float roll, float pitch, float yaw) const;
	Vector3 Rotate(const DirectX::XMVECTOR& quaternion) const;

	Vector3& operator=(const Vector3& other);
	Vector3& operator=(Vector3&& other) noexcept;

	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;

	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3& operator*=(const Vector3& other);
	Vector3& operator/=(const Vector3& other);

	Vector3& operator*=(float k);
	Vector3& operator/=(float k);

	friend Vector3 operator+(const Vector3& a, const Vector3& b);
	friend Vector3 operator-(const Vector3& a, const Vector3& b);
	friend Vector3 operator*(const Vector3& a, const Vector3& b);
	friend Vector3 operator/(const Vector3& a, const Vector3& b);
	friend Vector3 operator-(const Vector3& a);

	friend Vector3 operator*(const Vector3& a, float k);
	friend Vector3 operator/(const Vector3& a, float k);

	friend std::ostream& operator<<(std::ostream& os, const Vector3 v)
	{
		return os << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
	}

private:
	static constexpr float EQUALITY_TOLERANCE = 0.0001f;
};
}

template <>
struct std::hash<BlahEngine::Vector3>
{
	std::size_t operator()(const BlahEngine::Vector3& v) const noexcept
	{
		std::size_t h1 = std::hash<float>{}(v.X);
		std::size_t h2 = std::hash<float>{}(v.Y);
		std::size_t h3 = std::hash<float>{}(v.Z);
		return (h1 ^ (h2 << 1) >> 1) ^ (h3 << 1);
	}
};