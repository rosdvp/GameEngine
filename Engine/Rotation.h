#pragma once
#include "Vector3.h"

namespace BlahEngine
{
	struct Rotation
	{
#define CLIP_360(x) if ((x) > 0) while((x) > 360) (x) -= 360; else while((x) < -360) (x) += 360;
#define DEGREE_TO_RADIAN(x) (x) * DirectX::XM_PI / 180.0f
#define RADIAN_TO_DEGREE(x) (x) * 180.0f / DirectX::XM_PI


	public:
		Rotation() = default;
		Rotation(float roll, float pitch, float yaw);
		Rotation(DirectX::XMVECTOR quaternion);

		Rotation(const Rotation& rot) = default;
		Rotation(Rotation&& rot) = default;

		Rotation& operator=(const Rotation& rot) = default;
		Rotation& operator=(Rotation&& rot) = default;

		bool operator==(const Rotation& other) const;
		bool operator!=(const Rotation& other) const;


		DirectX::XMVECTOR GetQuaternion() const;
		DirectX::XMVECTOR GetQuaternionInv() const;
		Vector3 GetEuler() const;
		float GetRoll() const;
		float GetPitch() const;
		float GetYaw() const;


		void Set(float roll, float pitch, float yaw);

		void AddAroundLocal(float roll, float pitch, float yaw);
		void AddAroundWorld(float roll, float pitch, float yaw);


		friend std::ostream& operator<<(std::ostream& os, const Rotation& r);

	private:
		float _roll = 0;
		float _pitch = 0;
		float _yaw = 0;
		DirectX::XMVECTOR _q = DirectX::XMQuaternionIdentity();
	};
}
