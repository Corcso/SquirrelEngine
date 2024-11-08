#include "PCH.h"
#ifdef WINDOWS
#include <Windows.h>
#endif // WINDOWS

#include "Services.h"

#ifdef DX11
#include "GraphicsDX11.h"
#endif // DX11
#ifdef WINDOWS
#include "InputWindows.h"
#endif // WINDOWS


#include "Math.h"

// For testing
#include <iostream>
#include "Mesh.h"
#include "ResourceManager.h"
#include "MeshNut.h"

int main() {

#ifdef WINDOWS
	SQ::Services::RegisterInput(new SQ::InputWindows());
#endif // WINDOWS
#ifdef DX11
	SQ::Services::RegisterGraphics(new SQ::GraphicsDX11());
#endif // DX11

	/*SQ::Vec4 myVector = SQ::V4(1, 2, 0, 1);
	SQ::Mat4 matrix{ 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, 1, -2, 1};

	SQ::Vec4 myVectorOut = matrix * myVector;
	
	DirectX::XMFLOAT4 myV = myVector;
	std::cout << myV.x << myV.y << myV.z;*/
	SQ::Services::GetGraphics()->Init(1600, 900);
	SQ::ResourceManager rm;
	std::shared_ptr<SQ::Mesh> myMesh = rm.Retrieve<SQ::Mesh>("./Resources/handbag.obj");
	std::shared_ptr<SQ::Mesh> myMesh2 = rm.Retrieve<SQ::Mesh>("./Resources/TestQuad.obj");

	SQ::MeshNut mynut;
	mynut.SetMesh(myMesh);

	SQ::CameraNut myCam;
	myCam.SetPosition(SQ::V3(0, -0, -10));
	myCam.SetFov(70);

	std::cout << "Count of meshes " << myMesh.use_count();
	std::cout << GetLastError();
    MSG msg = { 0 };

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else
        {
			mynut.SetEulerAngles(SQ::V3(0, mynut.GetEulerAngles().Y + 0.001f, 0));
			if (SQ::Services::GetInput()->IsKeyDown(30)) myCam.SetPosition(SQ::V3(myCam.GetPosition().X - 0.0005f, myCam.GetPosition().Y, myCam.GetPosition().Z));
			if (SQ::Services::GetInput()->IsKeyDown(32)) myCam.SetPosition(SQ::V3(myCam.GetPosition().X + 0.0005f, myCam.GetPosition().Y, myCam.GetPosition().Z));
			if (SQ::Services::GetInput()->IsKeyDown(17)) myCam.SetPosition(SQ::V3(myCam.GetPosition().X, myCam.GetPosition().Y, myCam.GetPosition().Z + 0.0005f));
			if (SQ::Services::GetInput()->IsKeyDown(31)) myCam.SetPosition(SQ::V3(myCam.GetPosition().X, myCam.GetPosition().Y, myCam.GetPosition().Z - 0.0005f));

			SQ::Services::GetGraphics()->BeginRender();
			SQ::Services::GetGraphics()->UpdateProjectionMatrix(&myCam);
			SQ::Services::GetGraphics()->SetupCameraForFrame(&myCam);
			SQ::Services::GetGraphics()->Render(&mynut);

            SQ::Services::GetGraphics()->EndRender();
            
        }
    }
    return static_cast<int>(msg.wParam);


	return 0;
}

#ifdef WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	return main();
}
#endif // WINDOWS