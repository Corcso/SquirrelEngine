#pragma once
#include "Nut.h"
#include "CameraNut.h"
#include "SQMath.h"
namespace SQ {
	class Tree
	{
	public:

		Tree();

		void Init();

		void RunLoop();
		
		CameraNut* GetActiveCamera(Nut* nutToLookFrom = nullptr);

		void Quit();

	private:
		void Update(Nut* nut);
		void Render(Nut* nut, Mat4 worldMatrixSoFar);


		Nut rootNut;

		bool toQuit;
	};
}