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

float Rotation::GetRoll() const
{
	return _roll;
}

float Rotation::GetPitch() const
{
	return _pitch;
}

float Rotation::GetYaw() const
{
	return _yaw;
}

void Rotation::Set(float roll, float pitch, float yaw)
{
	_roll = roll;
	_pitch = pitch;
	_yaw = yaw;

	CONVERT_360(_roll);
	CONVERT_360(_pitch);
	CONVERT_360(_yaw);

	_q = XMQuaternionRotationRollPitchYaw(
		DEGREE_TO_RADIAN(pitch), 
		DEGREE_TO_RADIAN(yaw), 
		DEGREE_TO_RADIAN(roll));
}

void Rotation::AddLocal(float roll, float pitch, float yaw)
{
	_roll += roll;
	_pitch += pitch;
	_yaw += yaw;

	CONVERT_360(roll);
	CONVERT_360(pitch);
	CONVERT_360(yaw);
	
	_q = XMQuaternionMultiply(
		_q,
		XMQuaternionRotationRollPitchYaw(
			DEGREE_TO_RADIAN(pitch),
			DEGREE_TO_RADIAN(yaw),
			DEGREE_TO_RADIAN(roll)));
	_q = XMQuaternionNormalize(_q);
}

void Rotation::AddGlobal(float roll, float pitch, float yaw)
{
	_roll += roll;
	_pitch += pitch;
	_yaw += yaw;

	CONVERT_360(roll);
	CONVERT_360(pitch);
	CONVERT_360(yaw);

	_q = XMQuaternionMultiply(
		XMQuaternionRotationRollPitchYaw(
			DEGREE_TO_RADIAN(pitch),
			DEGREE_TO_RADIAN(yaw),
			DEGREE_TO_RADIAN(roll)),
		_q);
	_q = XMQuaternionNormalize(_q);
}
