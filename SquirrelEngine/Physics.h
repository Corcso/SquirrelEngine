#pragma once

#include "PhysicsNut.h"

namespace SQ {
	class Physics
	{
	public:
		virtual void Init() = 0;

		virtual void RegisterBody(PhysicsNut* nut) = 0;

		virtual void Update() = 0;
	};
}