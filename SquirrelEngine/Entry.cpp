#include "PCH.h"
#ifdef WINDOWS
#include <Windows.h>
#endif // WINDOWS



int main() {

}

#ifdef WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	main();
}
#endif // WINDOWS