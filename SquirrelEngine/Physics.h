#pragma once

#include "PhysicsNut.h"

namespace SQ {
	class Physics
	{
	public:
		/// <summary>
		/// Setup physics system
		/// </summary>
		virtual void Init(int targetFPS) = 0;

		/// <summary>
		/// Register a nut as a body in the physics system
		/// </summary>
		/// <param name="nut">Nut to register</param>
		virtual void RegisterBody(PhysicsNut* nut) = 0;

		/// <summary>
		/// Update all physics bodies
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// Remove a nut from the physics system
		/// </summary>
		/// <param name="nut">Nut to remove</param>
		virtual void RemoveBody(PhysicsNut* nut) = 0;

		/// <summary>
		/// Update the collision shape in the physics system. Take it from the nut passed in. 
		/// </summary>
		/// <param name="nut">Nut with shape update</param>
		virtual void BodyShapeUpdated(PhysicsNut* nut) = 0;

		/// <summary>
		/// Update the nuts elasticity in the system. 
		/// </summary>
		/// <param name="nut">Nut to update</param>
		virtual void BodyElasticityUpdated(PhysicsNut* nut) = 0;

		// All physical interations, see PhysicsNut for details. 
		
		virtual void AddForce(PhysicsNut* nut, Vec3 force) = 0;
		virtual void AddImpulse(PhysicsNut* nut, Vec3 impulse) = 0;
		virtual void AddAngularImpulse(PhysicsNut* nut, Vec3 impulse) = 0;
		virtual void AddTorque(PhysicsNut* nut, Vec3 torque) = 0;
		virtual void SetLinearVelocity(PhysicsNut* nut, Vec3 velocity) = 0;
		virtual void SetAngularVelociry(PhysicsNut* nut, Vec3 velocity) = 0;

		// Get velocities, see PhysicsNut for details.

		virtual Vec3 GetLinearVelocity(PhysicsNut*) = 0;
		virtual Vec3 GetAngularVelocity(PhysicsNut*) = 0;

	};
}