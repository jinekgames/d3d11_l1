
#pragma once

#include "DrawableBase.h"

#include "PhysicsThread.h"



class Box : public DrawableBase<Box> {

public:
	Box(
		Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		void (*dr)(float&, float&, float&)
	);
	~Box() {
		PhysClass.breakThread = true;
	}

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float dr;
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	void (*radRule)(float& dt, float& dr, float& r);
	PhysicsThread PhysClass;

	// model transform
	DirectX::XMFLOAT3X3 mt;
	std::thread thr;

};
