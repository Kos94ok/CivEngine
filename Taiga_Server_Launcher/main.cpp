
#include <Windows.h>

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ShellExecute(0, 0, L"taiga.exe", L"-server", 0, 0);
	return 0;
}