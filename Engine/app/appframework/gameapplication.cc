/****************************************************************************
Copyright (c) 2007,Radon Labs GmbH
Copyright (c) 2011-2013,WebJet Business Division,CYOU
 
http://www.genesis-3d.com.cn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "stdneb.h"
#include "profiletool.h"
#include "gameapplication.h"
#include "gameserver.h"

#include "io/logfileconsolehandler.h"
#include "io/assignregistry.h"

#include "inputfeature/inputfeature.h"
#include "basegamefeature/basegamefeature.h"
#include "graphicfeature/graphicsfeature.h"
#include "particlefeature/particlefeature.h"
#include "vegetationfeature/vegetationfeature.h"
#include "resourcefeature/ResourceFeature.h"
#include "physXfeature/PhysicsFeature.h"
#include "animationfeature/AnimationFeature.h"
#include "scriptfeature/inc/script_feature.h"
#include "soundfeature/SoundFeature.h"
#include "guifeature/FontFeature.h"
#include "guifeature/guiserver.h"
#include "guifeature/guiinputhandler.h"
#include "appframework/scene.h"
#include "graphicsystem/Projector/CollisionModelServer.h"
#include "ALSystemFeature/ALSystemFeature.h"

#include "app/scriptfeature/script_root_instance.h"
#include "input/inputsource.h"


#ifdef __WIN32__
#include <windowsx.h>
#endif

#if __NEBULA3_HTTP__
#include "core/debug/corepagehandler.h"
#include "threading/debug/threadpagehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "io/debug/consolepagehandler.h"
#include "io/debug/iopagehandler.h"
#endif

#include "shadercompiler/ShaderFactory.h"

namespace App
{

__ImplementThreadSingleton(App::GameApplication);

using namespace Util;
using namespace Core;
using namespace IO;
using namespace Debug;

static void pack_system_waning(const char* msg)
{
	n_warning(msg);
}

//------------------------------------------------------------------------------
/**
*/
GameApplication::GameApplication()
	: mGameWidth( 0 )
	, mGameHeight( 0 )
	, mResourceBaseDir()
	//, mAssetName()
	, mResDownloadUrl()
	, mPipeName()
	//, mUseLocalRes( false )
	, mDebugScript( false )
	, mInitGui( true )
	, mbUseFileService( false )
	, mUseWeb( false )
	, mWinHandle( NULL )
	, mAssignRegistry(NULL)
	, mQuit(false)
	, mDeviceState(DS_Normal)
	, m_pShaderFactory(NULL)
	, mOpenSceneDirty(false)
	, mCloseSceneDirty(false)
	, mCurSceneState(Unknown)
{
    __ConstructThreadSingleton;
	_RegisterDynamicClass();
}

//------------------------------------------------------------------------------
/**
*/
GameApplication::~GameApplication()
{
    n_assert(!this->IsOpen());
    __DestructThreadSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
GameApplication::Open()
{
	Super::Open();
	mQuit = false;
	n_assert(NULL == this->mAssignRegistry);
	this->mAssignRegistry = n_new(IO::AssignRegistry);
	this->mAssignRegistry->Setup();
	setupProjectAssigns();

	// setup from cmd line args
	this->setupAppFromCmdLineArgs();

	// setup basic Nebula3 runtime system
	this->mCoreServer = CoreServer::Create();
	this->mCoreServer->SetCompanyName(Application::Instance()->GetCompanyName());
	this->mCoreServer->SetAppName(Application::Instance()->GetAppTitle());
	this->mCoreServer->Open();

	// setup the job system
#if __WIN32__
	this->mJobSystem = Jobs::JobSystem::Create();
	this->mJobSystem->Setup();
#endif
	

	// setup io subsystem
	this->mIoServer = IoServer::Create();//主线程io。
	this->mIoServer->MountStandardArchives();

	this->mIoInterface = IoInterface::Create();
	this->mIoInterface->Open();
	GPtr<UseFileService> msg = UseFileService::Create();
	

	if (this->mbUseFileService)
	{
		msg->SetUseFileService(true);
		msg->SetFileServiceDir(mFileServicePath);
		msg->SetServerAddress(mResDownloadUrl);
		msg->SetCacheDir(mResourceBaseDir);
	}
	else
	{
		msg->SetUseFileService(false);
	}

	IO::IoInterface::Instance()->SendWait( msg.upcast<Messaging::Message>() );
	n_assert(msg->GetResult() == true);
	Pack::PackageSystem::SetWarningCallBack(pack_system_waning);
	//this->mIoServer->MountArchive("D:\\TLRun.zip");
#if __ANDROID__
	if(mbNeedReadApk)
	{
		//Pack::PackageSystem::OpenPackageInAPK( mApkPath.AsCharPtr() );
		this->mIoServer->MountArchive(mApkPath);
	}
	else
	{
		Pack::PackageSystem::OpenPackage();
	}
#else
	Pack::PackageSystem::OpenPackage();
#endif

	displayLOGO();
	// attach a log file console handler
#if __WIN32__
	GPtr<LogFileConsoleHandler> logFileHandler = LogFileConsoleHandler::Create();
	Console::Instance()->AttachHandler(logFileHandler.upcast<ConsoleHandler>());
#endif

#if __NEBULA3_HTTP__
	// setup http subsystem
	this->mHttpInterface = Http::HttpInterface::Create();
	this->mHttpInterface->Open();
	this->mHttpServerProxy = Http::HttpServerProxy::Create();
	this->mHttpServerProxy->Open();
	this->mHttpServerProxy->AttachRequestHandler(Debug::CorePageHandler::Create());
	this->mHttpServerProxy->AttachRequestHandler(Debug::ThreadPageHandler::Create());
	this->mHttpServerProxy->AttachRequestHandler(Debug::MemoryPageHandler::Create());
	this->mHttpServerProxy->AttachRequestHandler(Debug::ConsolePageHandler::Create());
	this->mHttpServerProxy->AttachRequestHandler(Debug::IoPageHandler::Create());

	// setup debug subsystem
	this->mDebugInterface = DebugInterface::Create();
	this->mDebugInterface->Open();

#endif

	m_pShaderFactory = ShaderProgramCompiler::ShaderFactory::Create();

	// create our game server and open it
	this->mGameServer = App::GameServer::Create();
	this->mGameServer->Open();

	// create and add new game features
	this->setupGameFeatures();
	this->mGraphicObjectManager = App::GraphicObjectManager::Create();
	this->mGraphicObjectManager->OnActivate();

	// create collision server
	mCollisonServer = Graphic::CollisionModelServer::Create();

	mGuiServer = App::GUIServer::Create();
	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool GameApplication::Start()
{
	Super::Start();
	// setup the game
	// start game world
	this->mGameServer->Start();
	if ( mInitGui )
	{
		initGui();
	}
	// - execute root's load method
	scriptRootLoad();
	if ( mInitGui )
	{
		initGuiScript();
	}

	return true;
}

//------------------------------------------------------------------------------
/**
*/
void
GameApplication::Close()
{
	Super::Close();
	mQuit = true;

	this->mGameServer->Stop();

	//[2012/11/15 zhongdaohuan] 
	//先退出脚本系统,
	//再退出gui系统，
	//然后再关掉 GameServer
	if (mInitGui)
	{
		exitGuiScript();
	}

#ifndef __SCRIPT_COMMIT__
	App::Utility_ScriptRootExit();
	App::ShutDownScriptSystem();
#endif

    exitGui();
	mGuiServer = NULL;

	this->cleanupGameFeatures();
	this->mGameServer->Close();
    
    this->mGameServer = 0;
    
	this->mGuiInputHandler = NULL;
	this->mInputSource = NULL;
	this->mGraphicObjectManager->OnDeactivate();
	this->mGraphicObjectManager = 0;
    
    this->mIoInterface->Close();
    this->mIoInterface = 0;
    
    this->mIoServer = 0;
    
	this->mCollisonServer = NULL;

	//this->mPackageServer = NULL;

#if __WIN32__
	this->mJobSystem->Discard();
	this->mJobSystem = 0;
#endif
   

#if __NEBULA3_HTTP__
	this->mDebugInterface->Close();
	this->mDebugInterface = 0;

	this->mHttpServerProxy->Close();
	this->mHttpServerProxy = 0;
	this->mHttpInterface->Close();
	this->mHttpInterface = 0;

#endif

    this->mCoreServer->Close();
    this->mCoreServer = 0;
	n_delete(this->mAssignRegistry);
	this->mAssignRegistry = NULL;
	Pack::PackageSystem::Close();

	n_delete(m_pShaderFactory);
	
}
//------------------------------------------------------------------------------
void
GameApplication::Exit()
{
	CloseAllScenes();
	if (IsOpen())
	{
		Close();
	}

	eventExit(*this);
	Super::Exit();

}

void GameApplication::Quit()
{
	mQuit = true;
}

bool GameApplication::IsQuit() const
{
	return mQuit;
}

//------------------------------------------------------------------------------
/**
    Run the application. This method will return when the application wishes 
    to exist.
*/
void
GameApplication::Run()
{
#if __NEBULA3_HTTP__
		this->mHttpServerProxy->HandlePendingRequests(); 
#endif
	 
		if (mOpenSceneDirty)
		{
			openScene(mOpenSceneName);
			ActorManager::Instance()->ForceGC();
			mOpenSceneDirty = false;
		}
		switch (mDeviceState)
		{
		case DS_Lost:
			{
				mGraphicsFeature->OnDeviceLost();
				mDeviceState = DS_Losted;
			}
			break;
		case DS_Losted:
			{
				if (mGraphicsFeature->CheckReset())
				{
					mDeviceState = DS_Reset;  
				}
				this->mCoreServer->Trigger();
				mGameServer->OnFrameWithoutGraphics(); 
				if (mGuiServer)
				{
					mGuiServer->OnFrame();
				}
			}
			break;
		case DS_Reset:
			{
				mGraphicsFeature->OnDeviceReset();
				mDeviceState = DS_Normal;
			}
			break;
		case DS_Normal:
			{
				this->mCoreServer->Trigger();
				mGameServer->OnFrame();   
				if (mGuiServer)
				{
					mGuiServer->OnFrame();
				}
			}
			break;
		default:
			break;
		}
		if (mCloseSceneDirty)
		{
			closeScene(mCloseSceneName);
			ActorManager::Instance()->ForceGC();
			mCloseSceneDirty = false;
		}
	PROFILE_PRESENT();
}

//------------------------------------------------------------------------------
bool GameApplication::OpenScene(const Util::String& sceneName, bool force)
{
	n_assert(IsOpen());
	mOpenSceneName = sceneName;
	if (force)
	{
		mCurSceneState = Loaded;
		return openScene(mOpenSceneName);
	}
	else
	{
		mCurSceneState = Loading;
		mOpenSceneDirty = true;
		return true;
	}
}

bool GameApplication::openScene(const Util::String& sceneName)
{
	mCurSceneState = Loaded;
	App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
	return pSceneSchedule->OpenScene(sceneName, false);
}
//------------------------------------------------------------------------------
void GameApplication::OnStopped()
{
	if(this->mScriptFeature)
	{
		this->mScriptFeature->OnStopped();
	}
#if __USE_AUDIO__
#if __ANDROID__
	// on mobile device, when press home button, stop music
	if ( mSoundFeature )
	{
		mSoundFeature->OnStopped();
	}
#endif
#endif
}
//------------------------------------------------------------------------------
void GameApplication::OnResumed()
{
	if(this->mScriptFeature)
	{
		this->mScriptFeature->OnResumed();
	}
#if __USE_AUDIO__
#if __ANDROID__
	// on mobile device, when press home button, resume music
	if ( mSoundFeature )
	{
		mSoundFeature->OnResumed();
	}
#endif
#endif
}

//-----------------------------------------------------------------------------
void GameApplication::OnExited()
{
	App::ScriptRootInstance::Instance()->OnExited();
}


//------------------------------------------------------------------------------
const GPtr<Scene>& GameApplication::GetCurrentScene() const
{
	n_assert(IsOpen());
	App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
	return pSceneSchedule->GetMainScene();
}

int GameApplication::GetCurrentSceneState()
{
	return mCurSceneState;
}

int GameApplication::GetSceneState(const Util::String& sceneName)
{
	if (sceneName == mOpenSceneName)
	{
		return mCurSceneState;
	}
	App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
	GPtr<Scene> scene = pSceneSchedule->GetScene(sceneName);
	if (scene.isvalid())
	{
		return Loaded;
	}
	return Unknown;
}

//------------------------------------------------------------------------------
bool GameApplication::closeScene(const Util::String& sceneName)
{
	mCurSceneState = Closed;
	App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
	return	pSceneSchedule->CloseScene(sceneName);
}

bool GameApplication::CloseScene( const Util::String& sceneName, bool force )
{
	n_assert(IsOpen());
	if (force)
	{
		mCurSceneState = Closed;
		return closeScene(sceneName);
	}
	else
	{
		mCurSceneState = Closing;
		mCloseSceneName = sceneName;
		mCloseSceneDirty = true;
		return true;
	}
}

//------------------------------------------------------------------------
void GameApplication::CloseAllScenes()
{
	n_assert(IsOpen());
	mCurSceneState = Closed;
	App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
	pSceneSchedule->CloseAllScenes();
}
//------------------------------------------------------------------------
void GameApplication::SetInput(const GPtr<Input::InputSource>& input)
{
	mInputSource = input;
}

//------------------------------------------------------------------------------
/**
    Setup new game features which should be used by this application.
    Overwrite for all features which have to be used.
*/
void
GameApplication::setupGameFeatures()
{
	this->mBaseGameFeature = App::BaseGameFeature::Create();
#if __WIN32__
	this->mBaseGameFeature->SetCmdLineArgs(this->GetCmdLineArgs());
#endif

	this->mGraphicsFeature = App::GraphicsFeature::Create();
	this->mGraphicsFeature->SetWebVersion(mUseWeb);
	this->mGraphicsFeature->SetMainWindow(mWinHandle);
	this->mGraphicsFeature->SetWindowSize(mGameWidth, mGameHeight);

	this->mGameServer->AttachGameFeature(this->mGraphicsFeature.upcast<App::Feature>());
	this->mGraphicsFeature->SetDeviceLostCallBack(&_renderDeviceLost);

	this->mGameServer->AttachGameFeature(this->mBaseGameFeature.cast<App::Feature>());

	// input features
	this->mInputFeature = App::InputFeature::Create();
#if __USE_AUDIO__ || __GENESIS_EDITOR__
	this->mSoundFeature = App::SoundFeature::Create();
#endif // __USE_AUDIO__ || __GENESIS_EDITOR__

#if __WIN32__
	//this->mNetworkFeature = App::NetworkFeature::Create();
#endif
	
	this->mParticleFeature = App::ParticleFeature::Create();
	this->mVegeFeature = App::VegetationFeature::Create();
	this->mResourceFeature = App::ResourceFeature::Create();
#if defined (__USE_PHYSX__) || defined (__GENESIS_EDITOR__)
	this->mPhysicsFeature = App::PhysicsFeature::Create();
#endif
	this->mAnimationFeature = App::AnimationFeature::Create();
#ifndef __SCRIPT_COMMIT__
	this->mScriptFeature = App::ScriptFeature::Create();
#endif

#if __OSX__ 
//	this->mFontFeature = App::FontFeature::Create();
    this->mALSystemFeature = App::ALSystemFeature::Create();
#endif


#ifdef __GENESIS_EDITOR__
	mFontFeature = App::FontFeature::Create();
	mFontFeature->SetFontPath("editor:Font/msyh.ttf");
	mFontFeature->SetShaderPath("editor:Shader/UI.shader");

	mGameServer->AttachGameFeature( this->mFontFeature.upcast<App::Feature>() );
#else
#	ifdef __PROFILER__
	Util::String fontName = "profile:font.ttf";
	Util::String shaderName = "profile:font.shader";
	if (IO::IoServer::Instance()->FileExists(IO::URI(fontName)) && IO::IoServer::Instance()->FileExists(IO::URI(shaderName)))
	{
		mFontFeature = App::FontFeature::Create();
		mFontFeature->SetFontPath(fontName);
		mFontFeature->SetShaderPath(shaderName);
		mGameServer->AttachGameFeature( this->mFontFeature.upcast<App::Feature>() );
	}
#	endif
#endif
	

	//// DInput 的IO输入,以前遗留的代码，现在注掉貌似也正常。
	//GPtr< Input::InputHardwareSource > hardwareSource = Input::InputHardwareSource::Create();
	//inputsourceList.Append( hardwareSource.upcast<Input::InputSource>() );
	App::InputSourceList inputsourceList;
	inputsourceList.Append(mInputSource);
	mInputFeature->InitSource(inputsourceList);

	// - setup ScriptSystem here
#ifndef __SCRIPT_COMMIT__
	setupSciptSystem();
#endif
	// - attach game feature
	mGameServer->AttachGameFeature(this->mInputFeature.upcast<App::Feature>());
#if __USE_AUDIO__ || __GENESIS_EDITOR__
	mGameServer->AttachGameFeature(this->mSoundFeature.upcast<App::Feature>());
#endif // __USE_AUDIO__ || __GENESIS_EDITOR__
	mGameServer->AttachGameFeature(this->mParticleFeature.upcast<App::Feature>());
	mGameServer->AttachGameFeature(this->mVegeFeature.upcast<App::Feature>());
	mGameServer->AttachGameFeature(this->mResourceFeature.upcast<App::Feature>());
#if defined (__USE_PHYSX__) || defined (__GENESIS_EDITOR__)
	mGameServer->AttachGameFeature(this->mPhysicsFeature.upcast<App::Feature>());
#endif
	mGameServer->AttachGameFeature(this->mAnimationFeature.upcast<App::Feature>());
#ifndef __SCRIPT_COMMIT__
	mGameServer->AttachGameFeature(this->mScriptFeature.upcast<App::Feature>());
#endif
// #if __WIN32__
// 	mGameServer->AttachGameFeature(this->mNetworkFeature.upcast<App::Feature>());
// #endif

#if __OSX__
//	mGameServer->AttachGameFeature(this->mFontFeature.upcast<App::Feature>());
    mGameServer->AttachGameFeature(this->mALSystemFeature.upcast<App::Feature>());
#endif
	//this->mBaseGameFeature->SetRenderDebug(true);

	// call this function when the baseGameFeature is attach
	//this->mBaseGameFeature->LoadActorTemplate();//"atp:ScriptActor.atp");


}

void
GameApplication::initGui()
{
	if (!mGuiServer->IsOpen())
	{
		Util::String coreFile = Util::String(GUI_DEFAULT_LOCATION) + Util::String(GUI_ROOT_CONFIG);
		if(mIoServer->FileExists(Util::StringAtom(coreFile)))
		{
			mGuiServer->SetLogDir(mEngineDir);
			mGuiServer->AddMediaLocaltion(GUI_DEFAULT_LOCATION);
			mGuiServer->Open();
		}
	}
	if (mGuiServer->IsOpen())
	{
		mGuiInputHandler = Input::GuiInputHandler::Create();
		mInputFeature->GetInputServer()->AttachInputHandler(Input::InputPriority::Gui, mGuiInputHandler.upcast<Input::InputHandler>());
	}
}

void GameApplication::exitGui()
{		
	mInputFeature->GetInputServer()->RemoveInputHandler(mGuiInputHandler.upcast<Input::InputHandler>());
	mGuiInputHandler = NULL;
	if (this->mGuiServer)
	{
		this->mGuiServer->Close();
	}
}

void
GameApplication::scriptRootLoad()
{
#ifndef __SCRIPT_COMMIT__
	App::Utility_ScriptRootLoad();
#endif
}

void
GameApplication::initGuiScript()
{
	if (mGuiServer->IsOpen())
	{
		mGuiServer->InitGuiRootScript();
	}
}
void GameApplication::exitGuiScript()
{
	if (this->mGuiServer && this->mGuiServer->IsOpen())
	{
		this->mGuiServer->ExitGuiScript();
	}
}




//------------------------------------------------------------------------------
/**
    Cleanup all added game features
*/
void
GameApplication::cleanupGameFeatures()
{   
	mFontFeature = NULL;
#ifndef __SCRIPT_COMMIT__
	this->mScriptFeature = 0;
#endif
	this->mInputFeature = 0;
	this->mBaseGameFeature = 0;
#if __USE_AUDIO__ || __GENESIS_EDITOR__
	this->mSoundFeature = 0;
#endif // __USE_AUDIO__ || __GENESIS_EDITOR__
#if __WIN32__
	//this->mNetworkFeature = 0;
#endif
	
	this->mParticleFeature = 0;
	this->mVegeFeature = 0;
	this->mResourceFeature = 0;
#if defined (__USE_PHYSX__) || defined (__GENESIS_EDITOR__)
	this->mPhysicsFeature = 0;
#endif
	this->mAnimationFeature = 0;
	this->mGraphicsFeature = 0;
#if __ANDROID__ || __OSX__
//	this->mFontFeature = 0;
    this->mALSystemFeature = 0;
#endif
}

//------------------------------------------------------------------------------
/**
*/
void 
GameApplication::setupAppFromCmdLineArgs()
{
    // allow rename of application
    const Util::CommandLineArgs& args = this->GetCmdLineArgs();
    if (args.HasArg("-appname"))
    {
        this->SetAppTitle(args.GetString("-appname"));
    }
}
//------------------------------------------------------------------------
void 
GameApplication::setupProjectAssigns()
{
	IO::AssignRegistry* pAssignRegistry = IO::AssignRegistry::Instance();

	if ( !mResDownloadUrl.IsEmpty() )
	{
		mbUseFileService = true;	

		// - resource server path of FileService
		pAssignRegistry->SetAssign(IO::Assign("fsSrvPath", mResDownloadUrl));
	}

	pAssignRegistry->SetAssign(IO::Assign("project",mResourceBaseDir ));
	pAssignRegistry->SetAssign(IO::Assign("home",mResourceBaseDir ));

	pAssignRegistry->SetAssign(IO::Assign("asset", "project:Asset") );	//assetname:Asset
	pAssignRegistry->SetAssign(IO::Assign("cmpileshd", "asset:compiledshaders"));
	pAssignRegistry->SetAssign(IO::Assign("sys", "project:System"));
	pAssignRegistry->SetAssign(IO::Assign("shd", "project:Shader"));
	pAssignRegistry->SetAssign(IO::Assign("script", "project:Script"));
#ifdef __PROFILER__
	pAssignRegistry->SetAssign(IO::Assign("profile", "project:Profile"));
#endif
}

void
GameApplication::setupSciptSystem()
{
	Util::String binPath = IO::AssignRegistry::Instance()->ResolveAssignsInString( App::s_csScriptBinaryAssign );
	Util::String emptyStr;
	Util::String monoDllPath = IO::AssignRegistry::Instance()->ResolveAssignsInString( App::s_csMonoDllAssign );
	GPtr<IO::Stream> pStream = IO::MemoryStream::Create();
	n_assert( pStream );
// 	GPtr<IO::ReadStream> readStreamMsg = IO::ReadStream::Create();
// 	n_assert( readStreamMsg );
// 	readStreamMsg->SetURI( App::s_csMonoDllAssign );
// 	readStreamMsg->SetStream( pStream );
// 	IO::IoInterface::Instance()->SendWait( readStreamMsg.upcast<Messaging::Message>() );
// 
// 	if ( !readStreamMsg->GetResult() )
// 	{
// 		n_warning( "ScriptGeneralManager::LoadAssemblies: can not open %s", App::s_csMonoDllAssign.AsCharPtr() );
// 	}
#if __ANDROID__
	if(!mbNeedReadApk)
	{
		App::StartupSciptSystem( binPath, emptyStr,  "/data/data/" + mPackageName+"/lib/libmono.so", mDebugScript );
	}
	else
	{
		App::StartupSciptSystem( "/data/data/"+mPackageName+"/Script", emptyStr,  "/data/data/" + mPackageName+"/lib/libmono.so", mDebugScript );
	}
#else
	App::StartupSciptSystem( binPath, emptyStr, monoDllPath, mDebugScript );
#endif
}

//------------------------------------------------------------------------------
/**
*/
void 
GameApplication::displayLOGO()
	{
#ifdef __WIN32__
		Util::String filepath = "sys:logo.bmp";
		if(!IO::IoServer::Instance()->FileExists(IO::URI(filepath)))
		{
			n_warning("\n Logo file does not exist: %s\n",filepath.AsCharPtr());
			return;
		}
		BITMAP bmp;
		HANDLE hBmp = LoadImageA(GetWindowInstance((HWND)mWinHandle),filepath.AsCharPtr(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		if (!hBmp)
		{
			n_warning("\n Loading Logo file fail!\nPlease check it: %s\n",filepath.AsCharPtr());
			return;
		}
		HDC hDC = ::GetDC((HWND)mWinHandle);
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hBmpOld = (HBITMAP)::SelectObject(hMemDC,hBmp);
		::GetObjectW(hBmp,sizeof(BITMAP),&bmp);
		::StretchBlt(hDC,0,0,mGameWidth,mGameHeight,hMemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
		::SelectObject(hMemDC,hBmpOld);
		::DeleteObject(hBmp);
		::DeleteObject(hBmpOld);
		::ReleaseDC((HWND)mWinHandle,hDC);
#endif
	}

void GameApplication::_renderDeviceLost()
{
	GameApplication::Instance()->mDeviceState = DS_Lost;
}

//------------------------------------------------------------------------------
#ifdef __GENESIS_EDITOR__
void GameApplication::BuildEditorVersion(void)
{
	return;
}
#endif
//------------------------------------------------------------------------------

void GameApplication::SetDeviceLost()
{
	//mGraphicsFeature->SetDeviceLost();
	mDeviceState = DS_Lost;
}
//------------------------------------------------------------------------------
#if __ANDROID__
//------------------------------------------------------------------------------
Util::String GameApplication::GetAppDicOnMobile()
{
	Util::String dic = "data/data/"+mPackageName;
	return dic;
}
#endif
//------------------------------------------------------------------------------
#if __OSX__
Util::String GameApplication::GetAppDicOnMobile()
{

}
#endif





} // namespace App
