#pragma once
// =====================================================================
//							Squirrel Engine
//						   Cormac Somerville
// 
//					  Platform Independency Header
//			Should be included in any platform specific classes
//			And where any knowledge of platforms is needed
// =====================================================================

#ifdef WINDOWS
	#include <Windows.h>
	// ImGui for win32
	#include "imgui_impl_win32.h"

#endif // WINDOWS

#ifdef DX11
	// DirectX includes
	#include <d3d11.h>
	#include <DirectXMath.h>
	#include <wrl.h>
	
	// Link library dependencies
	#pragma comment(lib, "d3d11.lib")
	//#pragma comment(lib, "dxgi.lib") I've commented these out only going to comment them in when I think I need them
	//#pragma comment(lib, "d3dcompiler.lib")
	//#pragma comment(lib, "winmm.lib")

	// Define ComPtr as Microsoft::WRL::ComPtr for ease of use.
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// Imgui for DX11
	#include "imgui_impl_dx11.h"
#endif // DX11

	// Jolt Physics
	// (Rouwe, no date a)

#ifdef JOLT
	#ifdef _DEBUG
		#define JPH_DEBUG_RENDERER 
		#define JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
		#define JPH_PROFILE_ENABLED
		#define JPH_OBJECT_STREAM
		#pragma comment(lib, "Jolt.lib")
	#else
		#define JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
		#define JPH_OBJECT_STREAM
		#define JPH_DEBUG_RENDERER 
		#define JPH_PROFILE_ENABLED
		#pragma comment(lib, "Jolt_Release.lib")
	#endif

	#include <Jolt/Jolt.h>
	#include <Jolt/Core/Core.h>

	#include <Jolt/RegisterTypes.h>
	#include <Jolt/Core/Factory.h>
	#include <Jolt/Core/TempAllocator.h>
	#include <Jolt/Core/JobSystemThreadPool.h>
	#include <Jolt/Physics/PhysicsSettings.h>
	#include <Jolt/Physics/PhysicsSystem.h>
	#include <Jolt/Physics/Collision/Shape/BoxShape.h>
	#include <Jolt/Physics/Collision/Shape/SphereShape.h>
	#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
	#include <Jolt/Physics/Body/BodyCreationSettings.h>
	#include <Jolt/Physics/Body/BodyActivationListener.h>
	#include <cstdarg>
	#include <thread>



#endif // JOLT
