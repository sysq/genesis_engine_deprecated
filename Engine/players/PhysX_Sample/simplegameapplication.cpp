#include "stdneb.h"
#include "simplegameapplication.h"
#include "simplegamestatehandler.h"

#include "input/inputhardwaresource.h"
#include "input/inputwindowsource.h"

#include "app/appframework/statehandler.h"
#include "app/inputfeature/defaultinputprocss.h"

#include "graphicsystem/GraphicObjectManager.h"
#include "basegamefeature/basegamefeature.h"
#include "graphicfeature/graphicsfeature.h"o
#include "inputfeature/inputfeature.h"

#include "particlefeature/particlefeature.h"

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
	{
		__ConstructThreadSingleton;
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
		__DestructThreadSingleton;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void 
		SimpleGameApplication::SetupStateHandlers()
	{
		// create our standard gamestatehandler
		Ptr<SimpleStateHandler> gameState = SimpleStateHandler::Create();
		gameState->SetName("GameState");
		gameState->SetSceneName("scene:TestAnimationScene.scene");

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
		else
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	void SimpleGameApplication::Run(void)
	{
		while ( this->GetCurrentState() != "Exit" )
		{
			if ( mWnd )
			{
				MSG msg;
				while (PeekMessage(&msg, mWnd, 0, 0, PM_REMOVE))
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
		Ptr<App::DefaultProcessInput> pInputProcess = App::DefaultProcessInput::Create();
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
		wndClass.lpszClassName = "Test";
		wndClass.hIconSm       = NULL;
		RegisterClassEx(&wndClass);

		mWnd = ::CreateWindow("Test",                 // lpClassName
			"WebJet",        // lpWindowName
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,                          // dwStyle
			840-1024/2,                    // x
			525-768/2,                    // y
			1024,                   // nWidth
			768,                  // nHeight
			NULL,                           // hWndParent
			NULL,                                 // hMenu
			GetModuleHandle(0),                          // hInstance
			NULL);                                // lParam
		this->mGraphicsFeature->SetWebVersion(false);
		this->mGraphicsFeature->SetWindow(mWnd);

		RECT clientRect;
		GetClientRect(mWnd,&clientRect);
		int wndHeight = clientRect.bottom - clientRect.top  ;
		int wndWidth = clientRect.right - clientRect.left;

		this->mGraphicsFeature->SetWindowHeight(wndHeight);
		this->mGraphicsFeature->SetWindowWidth(wndWidth);

		App::InputSourceList inputsourceList;

		mInputWindowsSource = Input::InputWindowSource::Create();
		mInputWindowsSource->SetWnd(mWnd);
		inputsourceList.Append( mInputWindowsSource.upcast<Input::InputSource>() );

		// DInput 的IO输入
		Ptr< Input::InputHardwareSource > hardwareSource = Input::InputHardwareSource::Create();
		inputsourceList.Append( hardwareSource.upcast<Input::InputSource>() );

		this->mParticleFeature = App::ParticleFeature::Create();
		mGameServer->AttachGameFeature(this->mParticleFeature.upcast<App::Feature>());

		mInputFeature->InitSource( inputsourceList );

		this->mGameServer->AttachGameFeature(this->mGraphicsFeature.upcast<App::Feature>());

		mGameServer->AttachGameFeature(this->mInputFeature.upcast<App::Feature>());

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
		this->mGameServer->RemoveGameFeature(this->mParticleFeature.upcast<App::Feature>());
		this->mParticleFeature = 0;
		this->mGraphicObjectManager->OnDeactivate();
		this->mGraphicObjectManager = 0;

		GameApplication::CleanupGameFeatures();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool 
		SimpleGameApplication::Open()
	{
		bool ret = App::GameApplication::Open();
		return ret;
	}
	void SimpleGameApplication::Close()
	{
		mInputWindowsSource = NULL;
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
		pAssignRegistry->SetAssign(IO::Assign("editor", "project:Editor") );

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

		Util::String projectPath = args.GetString("ProjectPath", "home:test_project");

		pAssignRegistry->SetAssign( IO::Assign("project", projectPath));

		pAssignRegistry->SetAssign(IO::Assign("asset", "project:Asset") );
		pAssignRegistry->SetAssign(IO::Assign("editor", "project:Editor") );

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

		// setup engine default assigns
		pAssignRegistry->SetAssign(IO::Assign("sys", "asset:System"));


#endif


	}


} // namespace Tools