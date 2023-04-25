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
		Rotation(const DirectX::XMVECTOR& quaternion);

		Rotation(const Rotation& rot) = default;
		Rotation(Rotation&& rot) = default;

		Rotation& operator=(const Rotation& rot) = default;
		Rotation& operator=(Rotation&& rot) = default;

		bool operator==(const Rotation& other) const;
		bool operator!=(const Rotation& other) const;


		DirectX::XMVECTOR GetQuaternion() const;
		const DirectX::XMVECTOR& GetQuaternionRef() const;
		DirectX::XMFLOAT4 GetQuaternionFloat4() const;
		Vector3 GetEuler() const;
		float GetRoll() const;
		float GetPitch() const;
		float GetYaw() const;


		void Set(float roll, float pitch, float yaw);

		void AddAroundLocal(float roll, float pitch, float yaw);
		void AddAroundLocal(const Rotation& rot);
		void AddAroundWorld(float roll, float pitch, float yaw);
		void AddAroundWorld(const Rotation& rot);


		friend std::ostream& operator<<(std::ostream& os, const Rotation& r);

		friend Rotation operator-(const Rotation& rot);

	private:
		DirectX::XMVECTOR _q = DirectX::XMQuaternionIdentity();

		void AddAroundLocal(DirectX::XMVECTOR q);
		void AddAroundWorld(DirectX::XMVECTOR q);
	};
}


template <>
struct std::hash<BlahEngine::Rotation>
{
	std::size_t operator()(const BlahEngine::Rotation& rot) const noexcept
	{
		std::size_t h = 0;
		auto& q = rot.GetQuaternionRef();
		BlahEngine::HashCombine(h, q.m128_f32[0], q.m128_f32[1], q.m128_f32[2], q.m128_f32[3]);
		return h;
	}
};