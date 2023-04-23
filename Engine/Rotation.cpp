#include "pch.h"
#include "Rotation.h"

using namespace BlahEngine;
using namespace DirectX;

Rotation::Rotation(float roll, float pitch, float yaw)
{
	Set(roll, pitch, yaw);
}

Rotation::Rotation(XMVECTOR quaternion)
{
	_q = quaternion;
}

bool Rotation::operator==(const Rotation& other) const
{
	return XMQuaternionEqual(_q, other._q);
}

bool Rotation::operator!=(const Rotation& other) const
{
	return !XMQuaternionEqual(_q, other._q);
}

XMVECTOR Rotation::GetQuaternion() const
{
	return _q;
}

XMVECTOR Rotation::GetQuaternionInv() const
{
	return XMQuaternionInverse(_q);
}

Vector3 Rotation::GetEuler() const
{
	float x = _q.m128_f32[0];
	float y = _q.m128_f32[1];
	float z = _q.m128_f32[2];
	float w = _q.m128_f32[3];

	float t0 = 2.0f * (w * x + y * z);
	float t1 = 1.0f - 2.0f * (x * x + y * y);
	float pitch = std::atan2(t0, t1);
	
	float t2 = std::clamp(2.0f * (w * y - z * x), -1.0f, 1.0f);
	float yaw = std::asin(t2);

	float t3 = 2.0f * (w * z + x * y);
	float t4 = 1.0f - 2.0f * (y * y + z * z);
	float roll = std::atan2(t3, t4);

	return { RADIAN_TO_DEGREE(roll), RADIAN_TO_DEGREE(pitch), RADIAN_TO_DEGREE(yaw) };
}

float Rotation::GetRoll() const
{
	float x = _q.m128_f32[0];
	float y = _q.m128_f32[1];
	float z = _q.m128_f32[2];
	float w = _q.m128_f32[3];

	float t3 = 2.0f * (w * z + x * y);
	float t4 = 1.0f - 2.0f * (y * y + z * z);
	float roll = std::atan2(t3, t4);

	return RADIAN_TO_DEGREE(roll);
}

float Rotation::GetPitch() const
{
	float x = _q.m128_f32[0];
	float y = _q.m128_f32[1];
	float z = _q.m128_f32[2];
	float w = _q.m128_f32[3];

	float t0 = 2.0f * (w * x + y * z);
	float t1 = 1.0f - 2.0f * (x * x + y * y);
	float pitch = std::atan2(t0, t1);

	return RADIAN_TO_DEGREE(pitch);
}

float Rotation::GetYaw() const
{
	float x = _q.m128_f32[0];
	float y = _q.m128_f32[1];
	float z = _q.m128_f32[2];
	float w = _q.m128_f32[3];

	float t2 = std::clamp(2.0f * (w * y - z * x), -1.0f, 1.0f);
	float yaw = std::asin(t2);

	return RADIAN_TO_DEGREE(yaw);
}

void Rotation::Set(float roll, float pitch, float yaw)
{
	_q = XMQuaternionNormalize(XMQuaternionRotationRollPitchYaw(
		DEGREE_TO_RADIAN(pitch),
		DEGREE_TO_RADIAN(yaw),
		DEGREE_TO_RADIAN(roll)
	));
}

void Rotation::AddAroundLocal(float roll, float pitch, float yaw)
{
	auto q = XMQuaternionRotationRollPitchYaw(
		DEGREE_TO_RADIAN(pitch),
		DEGREE_TO_RADIAN(yaw),
		DEGREE_TO_RADIAN(roll)
	);
	q = XMQuaternionNormalize(q);
	_q = XMQuaternionMultiply(q, _q);
	_q = XMQuaternionNormalize(_q);
}

void Rotation::AddAroundWorld(float roll, float pitch, float yaw)
{
	auto q = XMQuaternionRotationRollPitchYaw(
		DEGREE_TO_RADIAN(pitch),
		DEGREE_TO_RADIAN(yaw),
		DEGREE_TO_RADIAN(roll)
	);
	q = XMQuaternionNormalize(q);
	_q = XMQuaternionMultiply(_q, q);
	_q = XMQuaternionNormalize(_q);
}


std::ostream& BlahEngine::operator<<(std::ostream& os, const Rotation& r)
{
	return os << "("
			<< r._q.m128_f32[0] << ", "
			<< r._q.m128_f32[1] << ", "
			<< r._q.m128_f32[2] << ", "
			<< r._q.m128_f32[3] << ", "
			<< ")";
}