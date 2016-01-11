#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

int __stdcall WinMain(HINSTANCE thisAppInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int nShowCmd) {
	std::string test = "Hello World";
	MessageBoxA(nullptr, test.c_str(), test.c_str(), MB_OK);

	return 0;
}