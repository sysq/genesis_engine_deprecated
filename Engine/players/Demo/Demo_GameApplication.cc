/** \file: DemoGameApplication.cc
* \brief: implement the DemoGameApplication
* Copyright (c) 2011,畅游天下WebJet事业部
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/14   1.0    SunHao   
*/
#include "stdneb.h"
#include "appframework/app_fwd_decl.h"
#include "Demo_GameApplication.h"
#include "DemoGameStateHandler.h"
#include "InputSystem.h"
#include "input/inputhardwaresource.h"
#include "input/inputwindowsource.h"
#include "app/appframework/statehandler.h"
#include "app/inputfeature/defaultinputprocss.h"
#include "graphicsystem/graphicobjectmanager.h"
#include "graphicfeature/graphicsfeature.h"
#include "basegamefeature/basegamefeature.h"
#include "inputfeature/inputfeature.h"
#include "simplegamestatehandler.h"
#include "soundfeature/SoundFeature.h"
#include "vegetationfeature/vegetationfeature.h"
#include "particlefeature/particlefeature.h"
#include "physXfeature/PhysicsFeature.h"
#include "animationfeature/AnimationFeature.h"
#include "app/scriptfeature/inc/script_feature.h"


#include "graphicsystem/Renderable/SimpleShapeRenderable.h"
#include "graphicfeature/components/cameracomponent.h"
#include "io/memorystream.h"
namespace Demo
{

	__ImplementThreadSingleton(DemoGameApplication);

	/// the WinProc
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//------------------------------------------------------------------------------
	/**
	*/
	DemoGameApplication::DemoGameApplication()
		: m_Wnd(0)
		, m_TestWnd(0)
	{
		__ConstructThreadSingleton;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	DemoGameApplication::~DemoGameApplication()
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
	void DemoGameApplication::SetupStateHandlers()
	{
#if 0 
		// create our standard gamestatehandler
		Ptr<DemoGameStateHandler> gameState = DemoGameStateHandler::Create();
		gameState->SetName("GameState");
		gameState->SetSceneName("scene:DemoScene.scene");
		
		this->AddStateHandler(gameState.get());
#else
		Ptr<SimpleStateHandler> gameState = SimpleStateHandler::Create();
		gameState->SetName("GameState");

		const Util::String &SceneName = args.GetString("Scene");
		if ( SceneName.IsValid() )
		{
			gameState->SetSceneName(SceneName);
		}
		

		this->AddStateHandler(gameState.get());
#endif

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

		DemoGameApplication* pApp = DemoGameApplication::Instance();
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
	void DemoGameApplication::Run(void)
	{
		while ( this->GetCurrentState() != "Exit" )
		{
			if ( m_Wnd )
			{
				MSG msg;
				while (PeekMessage(&msg, m_Wnd, 0, 0, PM_REMOVE))
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
			App::TStateHandlerPtr curStateHandler;
			Util::String newState;
			if (curState.IsValid())
			{                                               
				// call the current state handler
				curStateHandler = this->FindStateHandlerByName(curState);
				n_assert(curStateHandler);
				newState = curStateHandler->OnFrame(); 
			}
#if 0 
			//测试多窗口的代码
			static bool Test_S = true;
			if( Test_S )
			{
				Test_S = false;
				Graphic::CameraSetting camera;
				Math::matrix44 test1;
				test1.identity();
				test1.set_position( Math::float4( 3000.0f, 50.0f, 1220.0f, 1.0f ) );
				camera.UpdateViewMatrix( test1 );
				camera.SetupPerspectiveFovRH( 1.1f, 1.1f, 0.1f, 1000.0f );
				this->mGraphicsFeature->AddViewPortWindow( m_TestWnd, camera );
			}
#endif
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
	void DemoGameApplication::SetupGameFeatures()
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
		//Ptr<DemoInputManager> pInputProcess = DemoInputManager::Create();
		//this->m_InputFeature->AttachManager( pInputProcess.upcast<App::Manager>() );

		int wndWidth,wndHeight;
		wndWidth = 1024;
		wndHeight = 768;
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
		wndClass.lpszClassName = "Demo";
		wndClass.hIconSm       = NULL;
		RegisterClassEx(&wndClass);



		m_Wnd = ::CreateWindow("Demo",	// lpClassName
			"WebJetDemo",				// lpWindowName
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,                          // dwStyle
			0,						// x
			0,						// y
			wndWidth,					// nWidth
			wndHeight,					// nHeight
			NULL,                   // hWndParent
			NULL,                   // hMenu
			GetModuleHandle(0),     // hInstance
			NULL);                  // lParam
		this->mGraphicsFeature->SetWebVersion(false);
		this->mGraphicsFeature->SetMainWindow(m_Wnd);

		RECT clientRect;
		GetClientRect(m_Wnd,&clientRect);
		wndHeight = clientRect.bottom - clientRect.top  ;
		wndWidth = clientRect.right - clientRect.left;

		this->mGraphicsFeature->SetWindowSize( wndWidth, wndHeight);


#if 0
		//测试多窗口的代码
		wndClass.lpszClassName = "DemoTest";
		RegisterClassEx(&wndClass);
		m_TestWnd = ::CreateWindow("DemoTest",	// lpClassName
			"WebJetDemo_Test",				// lpWindowName
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,                          // dwStyle
			0,						// x
			0,						// y
			wndWidth / 2,					// nWidth
			wndHeight / 2,					// nHeight
			NULL,                   // hWndParent
			NULL,                   // hMenu
			GetModuleHandle(0),     // hInstance
			NULL);                  // lParam
#endif

		App::InputSourceList inputsourceList;

		m_InputWindowsSource = Input::InputWindowSource::Create();
		m_InputWindowsSource->SetWnd(m_Wnd);
		inputsourceList.Append( m_InputWindowsSource.upcast<Input::InputSource>() );

		// DInput's IO
		Ptr< Input::InputHardwareSource > hardwareSource = Input::InputHardwareSource::Create();
		inputsourceList.Append( hardwareSource.upcast<Input::InputSource>() );

		mInputFeature->InitSource( inputsourceList );

		Ptr<App::DefaultProcessInput> pInputProcess = App::DefaultProcessInput::Create();
		this->mInputFeature->AttachManager( pInputProcess.upcast<App::Manager>() );

		// attach feature
		this->mGameServer->AttachGameFeature(this->mGraphicsFeature.upcast<App::Feature>());

		this->mGameServer->AttachGameFeature(this->mInputFeature.upcast<App::Feature>());

		this->mParticleFeature = App::ParticleFeature::Create();
		mGameServer->AttachGameFeature(this->mParticleFeature.upcast<App::Feature>());

		this->mVegeFeature = App::VegetationFeature::Create();
		mGameServer->AttachGameFeature(this->mVegeFeature.upcast<App::Feature>());

		// sound feature
		this->mSoundFeature = App::SoundFeature::Create();
		this->mGameServer->AttachGameFeature(this->mSoundFeature.cast<App::Feature>());

		this->mPhysicsFeature = App::PhysicsFeature::Create();
		this->mGameServer->AttachGameFeature( this->mPhysicsFeature.upcast<App::Feature>() );

		this->mAnimationFeature = App::AnimationFeature::Create();
		this->mGameServer->AttachGameFeature( this->mAnimationFeature.upcast<App::Feature>() );

		this->mScriptFeature = App::ScriptFeature::Create();
		this->mGameServer->AttachGameFeature( this->mScriptFeature.upcast<App::Feature>() );

		mBaseGameFeature->LoadActorTemplate();

	}

	//------------------------------------------------------------------------------
	/**
	Cleanup all added game features
	*/
	void DemoGameApplication::CleanupGameFeatures()
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
		this->mGameServer->RemoveGameFeature(this->mVegeFeature.upcast<App::Feature>());
		this->mVegeFeature = 0;
		this->mGameServer->RemoveGameFeature(this->mSoundFeature.upcast<App::Feature>());
		this->mSoundFeature = 0;
		this->mGameServer->RemoveGameFeature(this->mPhysicsFeature.upcast<App::Feature>());
		this->mPhysicsFeature = 0;
		this->mGameServer->RemoveGameFeature(this->mAnimationFeature.upcast<App::Feature>());
		this->mAnimationFeature = 0;
		this->mGameServer->RemoveGameFeature(this->mScriptFeature.upcast<App::Feature>());
		this->mScriptFeature = 0;



		GameApplication::CleanupGameFeatures();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool DemoGameApplication::Open()
	{
		bool ret = App::GameApplication::Open();
		return ret;
	}
	void DemoGameApplication::Close()
	{
		m_InputWindowsSource = NULL;
		App::GameApplication::Close();
	}
	//------------------------------------------------------------------------
	void DemoGameApplication::SetupProjectAssigns()
	{

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
		pAssignRegistry->SetAssign(IO::Assign("logic", "asset:Logic"));


		// setup engine default assigns
		pAssignRegistry->SetAssign(IO::Assign("sys", "asset:System"));
	}


} // namespace Tools
