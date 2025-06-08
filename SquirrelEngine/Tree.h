#pragma once
#include "Nut.h"
#include "CameraNut.h"
#include "SQMath.h"
#include "RootNut.h"
#include "Resource.h"
namespace SQ {
	class Tree
	{
	public:

		Tree();

		void Init(std::string initialNutPath);

		/// <summary>
		/// <para>Runs the game loop</para>
		/// <para>Engine call only!</para>
		/// </summary>
		void RunLoop();

		/// <summary>
		/// Runs a special game loop for the editor.
		/// </summary>
		void RunEditorLoop();
		
		/// <summary>
		/// <para>Gets the active camera. </para>
		/// <para>Starts searching from the nut provided, if no nut is provided then the root nut is used</para>
		/// </summary>
		/// <param name="nutToLookFrom">Nut to search below</param>
		/// <returns>Pointer to the active camera, nullptr if no active camera found</returns>
		CameraNut* GetActiveCamera(Nut* nutToLookFrom = nullptr);

		/// <summary>
		/// Returns the root nut
		/// </summary>
		/// <returns>Root Nut</returns>
		Nut* GetRootNut();

		/// <summary>
		/// Quits the run loop, closes the game.
		/// </summary>
		void Quit();


		void SetResourceInspector(std::shared_ptr<Resource> resource);

		/// <summary>
		/// Returns true if we are quitting
		/// </summary>
		/// <returns>If the game will close this frame</returns>
		bool IsGameClosingThisFrame() { return toQuit; }


	private:
		/// <summary>
		/// <para>Calls Update() on nut provided and all descendants.</para>
		/// <para>Recursive function. </para>
		/// </summary>
		/// <param name="nut">Nut to call on</param>
		void Update(Nut* nut);

		/// <summary>
		/// <para>Calls LateUpdate() on nut provided and all descendants.</para>
		/// <para>Recursive function. </para>
		/// </summary>
		/// <param name="nut">Nut to call on</param>
		void LateUpdate(Nut* nut);

		/// <summary>
		/// <para>Registers all lights with the graphics API</para>
		/// <para>Recursive function. </para>
		/// </summary>
		/// <param name="nut">Nut to call on.</param>
		void RegisterLights(Nut* nut);

		/// <summary>
		/// <para>Renders nut provided and all descendants.</para>
		/// <para>Render calls only happen for descendant classes of MeshNut.</para>
		/// <para>Recursive function. </para>
		/// </summary>
		/// <param name="nut">Nut to call on</param>
		void Render(Nut* nut);

		/// <summary>
		/// Destroy all queued for destruction nuts. 
		/// <para>Recursive function. </para>
		/// </summary>
		/// <param name="nut">Nut to call on</param>
		/// <returns>True if destroyed, needed for child list traversal</returns>
		bool DestroyQueued(Nut* nut);

		/// <summary>
		/// Destroy all nuts. Except from root. 
		/// <para>Recursive function. </para>
		/// </summary>
		/// <param name="nut">Nut to call on</param>
		/// <returns>True if destroyed, needed for child list traversal</returns>
		bool FreeAllNuts(Nut* nut);

		/// <summary>
		/// The root nut
		/// Special root nut type which calls ready when objects nuts the tree. 
		/// </summary>
		RootNut rootNut;

		/// <summary>
		/// Stores if the game will close at the end of the frame
		/// </summary>
		bool toQuit;

		void ImGuiRenderDebugInfo();
		void ImGuiRenderTreeNut(Nut* nut);
		Nut* currentInspectorTarget = &rootNut;
		std::shared_ptr<Resource> currentResourceInspectorTarget;
	};
}