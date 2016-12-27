#include "stdneb.h"

#include "simplegameapplication.h"
#include "system/appentry.h"


//ImplementNebulaApplication()

void NebulaMain(const Util::CommandLineArgs& args, bool mono_debug = false); 

//LRESULT CALLBACK
//WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch( uMsg )
//	{
//	case WM_DESTROY:
//		PostQuitMessage( 0 );
//		return 0;
//	default:
//		return DefWindowProc(hWnd, uMsg, wParam, lParam);
//	}
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}

#if __WIN32__
//------------------------------------------------------------------------
//Èë¿Úµã
//int main(int argc, char *argv[])
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd) 
{ 
	bool mono_debug = false;

	char* begin = strstr(lpCmdLine,"--mono_debug");
	
	if (begin)
	{
		mono_debug = true;
		*begin = 0;
	}	

	Util::CommandLineArgs args(lpCmdLine); //argv[1]
	NebulaMain(args, mono_debug); 
	return 0; 

	//HWND mWnd;
	//WNDCLASS wc = { 0, WinProc, 0, 0, GetModuleHandle(0),
	//	LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
	//	(HBRUSH)GetStockObject(BLACK_BRUSH), 0, "Test" };
	//RegisterClass(&wc);
	//mWnd = CreateWindowEx(0, "Test", "WebJet", WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
	//	0, 0, 800, 600, 0, 0, GetModuleHandle(0), 0);

	//ShowWindow(mWnd, nShowCmd);
	//UpdateWindow(mWnd);

	//MSG msg;
	//while (1)
	//{
	//	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	//	{
	//		//int msgHandled = TranslateAccelerator(mWin32Device->hPresentWnd, mWin32Device->hAccel, &msg);
	//		//if (0 == msgHandled)
	//		//{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//		//}
	//	}

	//}

	//return 0; 

}

#else
ImplementNebulaApplication()

#endif

using namespace Sample;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const Util::CommandLineArgs& args, bool mono_debug)
{

    SimpleGameApplication app;
	app.SetMonoDebug(mono_debug);
    app.SetCompanyName("Radon Labs GmbH");
    app.SetAppTitle("UI Game");
    app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}