#pragma once
#include "MeshNut.h"

namespace SQ {
	class Graphics
	{
	public:
		virtual int Init(int width, int height) = 0;

		virtual void BeginRender() = 0;

		//virtual void SwitchCamera() = 0;

		virtual void Render(MeshNut* toRender) = 0;

		virtual void EndRender() = 0;
	};
}