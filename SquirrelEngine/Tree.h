#pragma once
#include "Nut.h"
#include "CameraNut.h"
#include "SQMath.h"
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
		/// <para>Gets the active camera. </para>
		/// <para>Starts searching from the nut provided, if no nut is provided then the root nut is used</para>
		/// </summary>
		/// <param name="nutToLookFrom">Nut to search below</param>
		/// <returns>Pointer to the active camera, nullptr if no active camera found</returns>
		CameraNut* GetActiveCamera(Nut* nutToLookFrom = nullptr);

		/// <summary>
		/// Quits the run loop, closes the game.
		/// </summary>
		void Quit();

	private:
		/// <summary>
		/// <para>Calls Update() on nut provided and all descendants.</para>
		/// <para>Recursive function. </para>
		/// </summary>
		/// <param name="nut">Nut to call on</param>
		void Update(Nut* nut);

		/// <summary>
		/// <para>Renders nut provided and all descendants.</para>
		/// <para>Render calls only happen for descendant classes of MeshNut.</para>
		/// <para>Recursive function. </para>
		/// </summary>
		/// <param name="nut">Nut to call on</param>
		/// <param name="worldMatrixSoFar">The world matrix so far.</param>
		void Render(Nut* nut, Mat4 worldMatrixSoFar);

		/// <summary>
		/// The root nut
		/// </summary>
		Nut rootNut;

		/// <summary>
		/// Stores if the game will close at the end of the frame
		/// </summary>
		bool toQuit;
	};
}