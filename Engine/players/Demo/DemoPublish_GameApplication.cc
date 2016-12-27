/** \file: DemoPublishGameApplication.cpp
* \brief: 
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/28   1.0    SunHao   
*/

#include "stdneb.h"
#include "input/inputhardwaresource.h"
#include "input/inputwindowsource.h"
#include "graphicsystem/graphicobjectmanager.h"
#include "app/inputfeature/defaultinputprocss.h"
#include "app/appframework/statehandler.h"
#include "DemoGameStateHandler.h"
#include "DemoPublish_GameApplication.h"
#include "InputSystem.h"

namespace DemoPublish
{


__ImplementThreadSingleton(DemoPublishGameApplication);

/// the WinProc
static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//------------------------------------------------------------------------------
/**
*/
DemoPublishGameApplication::DemoPublishGameApplication()
: mExternWnd(0)
{
    __ConstructThreadSingleton;
	mbWebVersion = false;
	mhWebWindow = NULL;
	gameState = NULL;
}



//------------------------------------------------------------------------------
/**
*/
DemoPublishGameApplication::~DemoPublishGameApplication()
{
	__DestructThreadSingleton;
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
DemoPublishGameApplication::SetupStateHandlers()
{
    // create our standard gamestatehandler
	gameState = Demo::DemoGameStateHandler::Create();
	gameState->SetName("GameState");
	Util::String fullScenePath = "scene:";
	fullScenePath += m_sSceneName;
	gameState->SetSceneName(fullScenePath);

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
	DemoPublishGameApplication* pApp = DemoPublishGameApplication::Instance();
	Ptr<Input::InputWindowSource> pInputSource = pApp->GetInputWindowsSource();

	if ( pInputSource.isvalid() )
	{
		return pInputSource->OnWinProc(hWnd, uMsg, wParam, lParam );

	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

void DemoPublishGameApplication::Run()
{
	if ( this->GetCurrentState() != "Exit" )
	{
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
void DemoPublishGameApplication::SetupGameFeatures()
{
    GameApplication::SetupGameFeatures();

    // create and attach default features
	this->mBaseGameFeature = App::BaseGameFeature::Create();
    this->mBaseGameFeature->SetCmdLineArgs(this->GetCmdLineArgs());
    this->mBaseGameFeature->SetRenderDebug(true);
    this->mGameServer->AttachGameFeature(this->mBaseGameFeature.cast<App::Feature>());

	// call this function when the baseGameFeature is attach
	this->mBaseGameFeature->LoadActorTemplate();

    // game features
    this->mGraphicsFeature = App::GraphicsFeature::Create();
	mGraphicObjectManager = App::GraphicObjectManager::Create();
	mGraphicObjectManager->OnActivate();
	//this->mGraphicsFeature->SetWebVersion(false,NULL);
	this->mGraphicsFeature->SetWebVersion(mbWebVersion);
	this->mGraphicsFeature->SetMainWindow(mhWebWindow);

	
	// input features
	//this->mInputFeature = App::InputFeature::Create();
	this->mInputFeature = App::InputFeature::Create();
	Ptr<Demo::DemoInputManager> pInputProcess = Demo::DemoInputManager::Create();
	this->mInputFeature->AttachManager( pInputProcess.upcast<App::Manager>() );

	if ( true )
	{

		//test
		// register window class
		//WNDCLASSEX wndClass;
		//Memory::Clear(&wndClass, sizeof(wndClass));
		//wndClass.cbSize        = sizeof(wndClass);
		//wndClass.style         = CS_DBLCLKS;
		//wndClass.lpfnWndProc   = WinProc;
		//wndClass.cbClsExtra    = 0;
		//wndClass.cbWndExtra    = sizeof(void*);   // used to hold 'this' pointer
		//wndClass.hInstance     = GetModuleHandle(0);
		//wndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		//wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		//wndClass.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
		//wndClass.lpszMenuName  = NULL;
		//wndClass.lpszClassName = "Test";
		//wndClass.hIconSm       = NULL;
		//RegisterClassEx(&wndClass);

		//mExternWnd = ::CreateWindow("Test",                 // lpClassName
		//	"Nebula3",        // lpWindowName
		//	WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,                          // dwStyle
		//	0,                    // x
		//	0,                    // y
		//	640,                   // nWidth
		//	480,                  // nHeight
		//	NULL,                           // hWndParent
		//	NULL,                                 // hMenu
		//	GetModuleHandle(0),                          // hInstance
		//	NULL);                                // lParam
		this->mGraphicsFeature->SetWebVersion(mbWebVersion);
		this->mGraphicsFeature->SetMainWindow(mhWebWindow);
		this->mGraphicsFeature->SetWindowSize( muGameWidth, muGameHeight);
		//this->mGraphicsFeature->SetWindowHeight(muGameHeight);
		

		//this->mInputFeature->SetWebVersion(mbWebVersion,mhWebWindow);

		App::InputSourceList inputsourceList;

		mInputWindowsSource = Input::InputWindowSource::Create();
		mInputWindowsSource->SetWnd((HWND)mhWebWindow);
		inputsourceList.Append( mInputWindowsSource.upcast<Input::InputSource>() );

		// DInput 的IO输入
		Ptr< Input::InputHardwareSource > hardwareSource = Input::InputHardwareSource::Create();
		inputsourceList.Append( hardwareSource.upcast<Input::InputSource>() );

		mInputFeature->InitSource( inputsourceList );
	}
	else
	{
		//this->mGraphicsFeature->SetWebVersion(false);
		//this->mGraphicsFeature->SetWindow(NULL);

		//App::InputSourceList inputsourceList;
		//// 内部渲染窗口的IO输入
		//
		//Ptr< Input::DisplayEventInputSource > displaySource = Input::DisplayEventInputSource::Create();
		//inputsourceList.Append( displaySource.upcast<Input::InputSource>() );

		//// DInput 的IO输入
		//Ptr< Input::InputHardwareSource > hardwareSource = Input::InputHardwareSource::Create();
		//inputsourceList.Append( hardwareSource.upcast<Input::InputSource>() );

		//mInputFeature->InitSource( inputsourceList );
	}


    this->mGameServer->AttachGameFeature(this->mGraphicsFeature.upcast<App::Feature>());


	// input features
	//this->mInputFeature = App::InputFeature::Create();	
	//this->mInputFeature->SetWebVersion(mbWebVersion,mhWebWindow);


	mGameServer->AttachGameFeature(this->mInputFeature.upcast<App::Feature>());
	

}

//------------------------------------------------------------------------------
/**
    Cleanup all added game features
*/
void
DemoPublishGameApplication::CleanupGameFeatures()
{   
	this->mGameServer->RemoveGameFeature(this->mInputFeature.upcast<App::Feature>());
	this->mInputFeature = 0;
    this->mGameServer->RemoveGameFeature(this->mBaseGameFeature.upcast<App::Feature>());  
    this->mBaseGameFeature = 0;
    this->mGameServer->RemoveGameFeature(this->mGraphicsFeature.upcast<App::Feature>());
    this->mGraphicsFeature = 0;
	this->mGraphicObjectManager->OnDeactivate();
	this->mGraphicObjectManager = 0;
    GameApplication::CleanupGameFeatures();
}

//------------------------------------------------------------------------------
/**
*/
bool 
DemoPublishGameApplication::Open()
{
    bool ret = App::GameApplication::Open();
    return ret;
}
void DemoPublishGameApplication::Close()
{
	mInputWindowsSource = NULL;
	App::GameApplication::Close();
}

void DemoPublishGameApplication::SetWebVersion( bool bWebVersion,void* hWebWindow )
{
	mbWebVersion = bWebVersion;
	mhWebWindow = hWebWindow;
}

void DemoPublishGameApplication::End()
{
	const Ptr<App::StateHandler> gameState = this->GetCurrentStateHandler();
	gameState->OnStateLeave( "Null" );

	App::GameServer::Instance()->SetQuitRequested();
	Close();
	//Exit();
}

//------------------------------------------------------------------------
void DemoPublishGameApplication::SetupProjectAssigns()
{

	// local solution
	IO::AssignRegistry* pAssignRegistry = IO::AssignRegistry::Instance();

	pAssignRegistry->SetAssign(IO::Assign("home",m_sResourceBaseDir ));
	pAssignRegistry->SetAssign(IO::Assign("netBase", m_sResDownloadUrl) );

	pAssignRegistry->SetAssign(IO::Assign("localAsset", "home:Asset") );	
	pAssignRegistry->SetAssign(IO::Assign("netAssert", "netBase:DownloadAsset/Demo") );

	//pAssignRegistry->SetAssign(IO::Assign("asset", "home:Asset") );
	pAssignRegistry->SetAssign(IO::Assign("atp", "netAssert:ActorTemplate"));
	pAssignRegistry->SetAssign(IO::Assign("ani", "netAssert:Animation"));
	pAssignRegistry->SetAssign(IO::Assign("msh", "netAssert:Mesh"));
	pAssignRegistry->SetAssign(IO::Assign("phy", "netAssert:physics"));
	pAssignRegistry->SetAssign(IO::Assign("scene", "netAssert:Scene"));
	pAssignRegistry->SetAssign(IO::Assign("script", "netAssert:Script"));
	pAssignRegistry->SetAssign(IO::Assign("sound", "netAssert:Sound"));
	pAssignRegistry->SetAssign(IO::Assign("tex", "netAssert:Texture"));

	pAssignRegistry->SetAssign(IO::Assign("logic", "localAsset:Logic"));
	pAssignRegistry->SetAssign(IO::Assign("shd", "localAsset:Shader"));
	pAssignRegistry->SetAssign(IO::Assign("sys", "localAsset:System"));
}


} // namespace Tools
