#pragma once
#include "MeshNut.h"
#include "CameraNut.h"

namespace SQ {
	class Graphics
	{
	public:
		virtual int Init(std::string title, int width, int height) = 0;

		virtual void BeginRender() = 0;

		virtual void UpdateProjectionMatrix(CameraNut* camera) = 0;

		virtual void SetupCameraForFrame(CameraNut* camera) = 0;

		virtual void Render(MeshNut* toRender, Mat4 worldMatrix) = 0;

		virtual void EndRender() = 0;

		virtual Vec2 GetRenderWindowSize() = 0;
		virtual Vec2 GetWindowLocation() = 0;
	};
}