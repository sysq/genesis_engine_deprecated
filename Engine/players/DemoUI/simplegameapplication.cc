//------------------------------------------------------------------------------
//  testgameapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "simplegameapplication.h"
#include "simplegamestatehandler.h"

#include "input/inputhardwaresource.h"
#include "input/inputwindowsource.h"

#include "app/appframework/statehandler.h"
#include "inputprocss.h"

#include "graphicobjectmanager.h"
#include "particlefeature/particlefeature.h"
#include "scriptfeature/inc/script_utility.h"

#include "spritebacth/debugboard.h"

#include "app/guifeature/GuiInputHandler.h"


namespace Sample
{

__ImplementThreadSingleton(SimpleGameApplication);

/// the WinProc
static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//------------------------------------------------------------------------------
/**
*/
SimpleGameApplication::SimpleGameApplication()
: mWnd(0)
, mMonoDebug(false)
{
    __ConstructThreadSingleton;
	mGuiServer = App::GUIServer::Create();
}

//------------------------------------------------------------------------------
/**
*/
SimpleGameApplication::~SimpleGameApplication()
{
	
    if (this->IsOpen())
    {
        this->Close();
    }
	mGuiServer = NULL;
	__DestructThreadSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void 
SimpleGameApplication::SetupStateHandlers()
{


	// - setup ScriptSystem here
	Util::String binPath = IO::AssignRegistry::Instance()->ResolveAssignsInString( App::s_csScriptBinaryAssign );
	Util::String emptyStr;
	Util::String monoDllPath = IO::AssignRegistry::Instance()->ResolveAssignsInString( App::s_csMonoDllAssign );
	App::StartupSciptSystem( binPath, emptyStr, monoDllPath, mMonoDebug );

	//App::DebugBoard::Open();
	App::GUIServer* gui_server = App::GUIServer::Instance();
	if (!gui_server->IsOpen())
	{
		//gui_server->SetConfigPath("ui:");
		//gui_server->SetResourePath("ui:");
		gui_server->SetCoreFile("MyGUI_Core.xml");
		gui_server->Open();
	}
    // create our standard gamestatehandler
	Ptr<SimpleStateHandler> gameState = SimpleStateHandler::Create();
	gameState->SetName("GameState");
	gameState->SetSceneName("scene:IntersectScene.scene");

	this->AddStateHandler(gameState.get());

	this->SetState("GameState");
}


//------------------------------------------------------------------------------
/**
    The Nebula3 WinProc.
*/
LRESULT CALLBACK
WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg )
	{
	case WM_DESTROY:
		App::GameServer::Instance()->SetQuitRequested();
		PostQuitMessage( 0 );
		return 0;
	}

	SimpleGameApplication* pApp = SimpleGameApplication::Instance();
	Ptr<Input::InputWindowSource> pInputSource = pApp->GetInputWindowsSource();

	if ( pInputSource.isvalid() )
	{
		return pInputSource->OnWinProc(hWnd, uMsg, wParam, lParam );
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	
}

void SimpleGameApplication::Run(void)
{

	//MSG msg;
	//while (1)
	//{
	//	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}

	//}

	//return; 



	while ( this->GetCurrentState() != "Exit" )
	{
		if ( mWnd )
		{
			MSG msg;
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))//mWnd
			{
				//int msgHandled = TranslateAccelerator(mWin32Device->hPresentWnd, mWin32Device->hAccel, &msg);
				//if (0 == msgHandled)
				//{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				//}
			}
		}

		//_start_timer(mGameApplicationFrameTimeAll);

#if __NEBULA3_HTTP__
		this->mHttpServerProxy->HandlePendingRequests(); 
#endif

		this->mCoreServer->Trigger();

		//trigger our game server, which triggers all game features
		this->mGameServer->OnFrame();

		Util::String curState = this->GetCurrentState();
		Ptr<App::StateHandler> curStateHandler;
		Util::String newState;
		if (curState.IsValid())
		{                                               
			// call the current state handler
			curStateHandler = this->FindStateHandlerByName(curState);
			n_assert(curStateHandler);
			newState = curStateHandler->OnFrame(); 
		}

		// call the app's Run() method
		Application::Run();

		// a requested state always overrides the returned state
		if (this->mRequestedState.IsValid())
		{
			this->SetState(this->mRequestedState);
		}
		else if (newState.IsValid() && newState != curStateHandler->GetName())
		{
			// a normal state transition
			this->SetState(newState);
		}
		//_stop_timer(mGameApplicationFrameTimeAll);
	}
}
//------------------------------------------------------------------------------
/**
*/
void
SimpleGameApplication::SetupGameFeatures()
{
    GameApplication::SetupGameFeatures();

    // create and attach default features
	this->mBaseGameFeature = App::BaseGameFeature::Create();
    this->mBaseGameFeature->SetCmdLineArgs(this->GetCmdLineArgs());
    this->mBaseGameFeature->SetRenderDebug(true);

    this->mGameServer->AttachGameFeature(this->mBaseGameFeature.cast<App::Feature>());

    // game features
    this->mGraphicsFeature = App::GraphicsFeature::Create();
	mGraphicObjectManager = App::GraphicObjectManager::Create();
	mGraphicObjectManager->OnActivate();
	
	// input features
	this->mInputFeature = App::InputFeature::Create();
	Ptr<App::GyrostProcessInput> pInputProcess = App::GyrostProcessInput::Create();
	this->mInputFeature->AttachManager( pInputProcess.upcast<App::Manager>() );

	// register window class
	WNDCLASSEX wndClass;
	Memory::Clear(&wndClass, sizeof(wndClass));
	wndClass.cbSize        = sizeof(wndClass);
	wndClass.style         = CS_DBLCLKS;
	wndClass.lpfnWndProc   = WinProc;
	wndClass.cbClsExtra    = 0;
	wndClass.cbWndExtra    = sizeof(void*);   // used to hold 'this' pointer
	wndClass.hInstance     = GetModuleHandle(0);
	wndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
	wndClass.lpszMenuName  = NULL;
	wndClass.lpszClassName = L"Test";
	wndClass.hIconSm       = NULL;
	RegisterClassEx(&wndClass);

	int wndWidth,wndHeight;
	wndWidth = 1024;
	wndHeight = 768;
	mWnd = ::CreateWindow(L"Test",                 // lpClassName
		L"WebJet",        // lpWindowName
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,                          // dwStyle
		//WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		0,                    // x
		0,                    // y
		wndWidth,                   // nWidth
		wndHeight,                  // nHeight
		NULL,                           // hWndParent
		NULL,                                 // hMenu
		GetModuleHandle(0),                          // hInstance
		NULL);                                // lParam


	//WNDCLASS wc = { 0, WinProc, 0, 0, GetModuleHandle(0),
	//	LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
	//	(HBRUSH)GetStockObject(BLACK_BRUSH), 0, "Test" };
	//RegisterClass(&wc);
	//mWnd = CreateWindowEx(0, "Test", "WebJet", WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
	//	0, 0, wndWidth, wndHeight, 0, 0, GetModuleHandle(0), this);


	ShowWindow(mWnd, SW_SHOWDEFAULT);
	UpdateWindow(mWnd);


	this->mGraphicsFeature->SetWebVersion(false);
	this->mGraphicsFeature->SetWindow(mWnd);

	RECT clientRect;
	GetClientRect(mWnd,&clientRect);
	wndHeight = clientRect.bottom - clientRect.top  ;
	wndWidth = clientRect.right - clientRect.left;

	this->mGraphicsFeature->SetWindowHeight(wndHeight);
	this->mGraphicsFeature->SetWindowWidth(wndWidth);

	App::InputSourceList inputsourceList;

	mInputWindowsSource = Input::InputWindowSource::Create();
	mInputWindowsSource->SetWnd(mWnd);
	inputsourceList.Append( mInputWindowsSource.upcast<Input::InputSource>() );

	// DInput 的IO输入
	Ptr< Input::InputHardwareSource > hardwareSource = Input::InputHardwareSource::Create();
	inputsourceList.Append( hardwareSource.upcast<Input::InputSource>() );


	
	mInputFeature->InitSource( inputsourceList );

    this->mGameServer->AttachGameFeature(this->mGraphicsFeature.upcast<App::Feature>());

	mGameServer->AttachGameFeature(this->mInputFeature.upcast<App::Feature>());

	Ptr<Input::GuiInputHandler> pUIHandler = Input::GuiInputHandler::Create();
	n_assert(pUIHandler);
	mInputFeature->GetInputServer()->AttachInputHandler(Input::InputPriority::Gui,pUIHandler.upcast<Input::InputHandler>());

	this->mParticleFeature = App::ParticleFeature::Create();
	mGameServer->AttachGameFeature(this->mParticleFeature.upcast<App::Feature>());

	
	
}

//------------------------------------------------------------------------------
/**
    Cleanup all added game features
*/
void
SimpleGameApplication::CleanupGameFeatures()
{   
	this->mGameServer->RemoveGameFeature(this->mInputFeature.upcast<App::Feature>());
	this->mInputFeature = 0;
    this->mGameServer->RemoveGameFeature(this->mBaseGameFeature.upcast<App::Feature>());  
    this->mBaseGameFeature = 0;
    this->mGameServer->RemoveGameFeature(this->mGraphicsFeature.upcast<App::Feature>());
    this->mGraphicsFeature = 0;
	this->mGraphicObjectManager->OnDeactivate();
	this->mGraphicObjectManager = 0;
	this->mGameServer->RemoveGameFeature(this->mParticleFeature.upcast<App::Feature>());
	this->mParticleFeature = 0;


    GameApplication::CleanupGameFeatures();
}

//------------------------------------------------------------------------------
/**
*/
bool 
SimpleGameApplication::Open()
{
	if (NULL == mGuiServer)
	{
		mGuiServer = App::GUIServer::Create();
	}	
    bool ret = App::GameApplication::Open();
    return ret;
}
void SimpleGameApplication::Close()
{
	mInputWindowsSource = NULL;
	App::GUIServer::Instance()->Close();
	mGuiServer = NULL;
	App::GameApplication::Close();
}
//------------------------------------------------------------------------
#define USE_HTTP_SERVER 0
void SimpleGameApplication::SetupProjectAssigns()
{
#if USE_HTTP_SERVER
	IO::AssignRegistry* pAssignRegistry = IO::AssignRegistry::Instance();

	// 网络测试 temp code

	pAssignRegistry->SetAssign( IO::Assign("Net","http://10.7.82.13:8080") );
	pAssignRegistry->SetAssign( IO::Assign("project", "Net:test_project"));

	pAssignRegistry->SetAssign(IO::Assign("asset", "project:Asset") );

	// setup defalut content assigns
	pAssignRegistry->SetAssign(IO::Assign("atp", "asset:ActorTemplate"));
	pAssignRegistry->SetAssign(IO::Assign("ani", "asset:Animation"));
  	pAssignRegistry->SetAssign(IO::Assign("msh", "asset:Mesh"));
	pAssignRegistry->SetAssign(IO::Assign("phy", "asset:physics"));
	pAssignRegistry->SetAssign(IO::Assign("scene", "asset:Scene"));
	pAssignRegistry->SetAssign(IO::Assign("script", "asset:Script"));
	pAssignRegistry->SetAssign(IO::Assign("sound", "asset:Sound"));
	pAssignRegistry->SetAssign(IO::Assign("tex", "asset:Texture"));

	// setup engine default assigns
	pAssignRegistry->SetAssign(IO::Assign("sys", "asset:System"));

	// 现在shader需要编译。不能放在服务器上进行处理。必须下载到本地
	pAssignRegistry->SetAssign(IO::Assign("shd", "home:test_project/asset/Shader"));

#else
	IO::AssignRegistry* pAssignRegistry = IO::AssignRegistry::Instance();

	const Util::String &projectPath = args.GetString("ProjectPath");
	if (!projectPath.IsValid())
	{
		n_error("Setup project path error, project path is empty.");
	}
	if (!IO::IoServer::Instance()->DirectoryExists(projectPath))
	{
		n_error("Setup project path error, project path is not a full path, or the project folder is not exist.");
	}

	pAssignRegistry->SetAssign( IO::Assign("project", projectPath));

	pAssignRegistry->SetAssign(IO::Assign("asset", "project:Asset") );

	// setup defalut content assigns
	pAssignRegistry->SetAssign(IO::Assign("atp", "asset:ActorTemplate"));
	pAssignRegistry->SetAssign(IO::Assign("ani", "asset:Animation"));
	pAssignRegistry->SetAssign(IO::Assign("msh", "asset:Mesh"));
	pAssignRegistry->SetAssign(IO::Assign("phy", "asset:physics"));
	pAssignRegistry->SetAssign(IO::Assign("scene", "asset:Scene"));
	pAssignRegistry->SetAssign(IO::Assign("script", "asset:Script"));
	pAssignRegistry->SetAssign(IO::Assign("shd", "asset:Shader"));
	pAssignRegistry->SetAssign(IO::Assign("sound", "asset:Sound"));
	pAssignRegistry->SetAssign(IO::Assign("tex", "asset:Texture"));
	pAssignRegistry->SetAssign(IO::Assign("ui", "asset:UIMedia"));
	// setup engine default assigns
	pAssignRegistry->SetAssign(IO::Assign("sys", "asset:System"));
	


#endif


}


} // namespace Tools
