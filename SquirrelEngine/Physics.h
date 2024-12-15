#pragma once

#include "PhysicsNut.h"

namespace SQ {
	class Physics
	{
	public:
		virtual void Init() = 0;

		virtual void RegisterBody(PhysicsNut* nut) = 0;

		virtual void Update() = 0;

		virtual void RemoveBody(PhysicsNut* nut) = 0;

		virtual void BodyShapeUpdated(PhysicsNut* nut) = 0;

		virtual void BodyElasticityUpdated(PhysicsNut* nut) = 0;

		virtual void AddForce(PhysicsNut* nut, Vec3 force) = 0;
		virtual void AddImpulse(PhysicsNut* nut, Vec3 impulse) = 0;
		virtual void AddAngularImpulse(PhysicsNut* nut, Vec3 impulse) = 0;
		virtual void AddTorque(PhysicsNut* nut, Vec3 torque) = 0;
		virtual void SetLinearVelocity(PhysicsNut* nut, Vec3 velocity) = 0;
		virtual void SetAngularVelociry(PhysicsNut* nut, Vec3 velocity) = 0;

	};
}