#pragma once
#include "MeshNut.h"
#include "CameraNut.h"
#include "LightNut.h"

namespace SQ {
	class Graphics
	{
	public:
		/// <summary>
		/// Initialise the graphics service
		/// </summary>
		/// <param name="title">Window Title</param>
		/// <param name="width">Width</param>
		/// <param name="height">Height</param>
		/// <param name="clearColor">Background clear color</param>
		/// <returns></returns>
		virtual int Init(std::string title, int width, int height, Vec4 clearColor) = 0;

		/// <summary>
		/// Function to setup the beginning of a render
		/// </summary>
		virtual void BeginRender() = 0;

		/// <summary>
		/// Update the projection matrix with the camera's parameters
		/// </summary>
		/// <param name="camera">Camera to use</param>
		virtual void UpdateProjectionMatrix(CameraNut* camera) = 0;

		/// <summary>
		/// Setup the camera for frame. 
		/// </summary>
		/// <param name="camera">Camera to use</param>
		virtual void SetupCameraForFrame(CameraNut* camera) = 0;

		/// <summary>
		/// Render the given mesh. Using its mesha and material data. 
		/// </summary>
		/// <param name="toRender">Mesh to render</param>
		virtual void Render(MeshNut* toRender) = 0;

		/// <summary>
		/// Register a light in the lights list for this frame. 
		/// </summary>
		/// <param name="light">Light to register</param>
		virtual void RegisterLightForFrame(LightNut* light) = 0;
		/// <summary>
		/// Clear the lights for this frame
		/// </summary>
		virtual void ClearFrameLights() = 0;

		/// <summary>
		/// End render and display output to screen
		/// </summary>
		virtual void EndRender() = 0;

		/// <summary>
		/// Get render window size. 
		/// </summary>
		/// <returns>Size of render window</returns>
		virtual Vec2 GetRenderWindowSize() = 0;

		/// <summary>
		/// Get window location on screen. 
		/// </summary>
		/// <returns>Gets the window location on the users screen. </returns>
		virtual Vec2 GetWindowLocation() = 0;

		/// <summary>
		/// <para>For internal use only</para>
		/// <para>Informs the graphics service the swap chain size needs to change. </para>
		/// </summary>
		/// <param name="newSize">New size of the window render area.</param>
		virtual void RegisterWindowSizeChange(Vec2 newSize) = 0;
	};
}