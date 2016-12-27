
#include "stdneb.h"
#include "system/appentry.h"
#include "foundation/memory/memory.h"
#include "DemoPublish_GameApplication.h"
#include "WebJetWebData.h"
#include "foundation/io/advFileService/advFileService.h"
#include "addons/fileservice/libwfs/src/lfs_util.h"
#include "WJPluginPacketFactoryManager.h"
#include "WJPacketRecevier.h"
#include "PageExportFunction.h"
#include "WJCallFuncProxy.h"
#include "LogFile.h"

using namespace DemoPublish;
using namespace WebJet;
 
#define DLLEXP extern "C" __declspec(dllexport)


#define HANDLE_API(r,n,p)  DLLEXP r n p;
#include "WinWebInterface.h"

static DemoPublishGameApplication* g_App = NULL;
static PacketRecevier* g_pRec = NULL;
//CRITICAL_SECTION g_RecvCS;

int g_nLevel0FileTaskID = - 1 ;
WebJetWebData g_WebData ;


bool g_bUseFS = false ;
bool g_bAllOpenned = false ;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
//插件调用引擎层的函数实现
bool CallFuncProxy::CallEngineMethod( const char* sMethodName,const char* sGUID,const FuncParamList& argList )
{
	FuncParamList resList;
	assert(sMethodName);
	//调用Toggle方法,带有一个传入参数，无返回值
	if (strcmp("Toggle",sMethodName)==0)
	{
		FuncParameter arg0;
		//获取传入参数
		if(!argList.GetParam(&arg0,0))
		{
			assert(0 && "if(!argList.GetParam(&arg0,0))");
		}
		unsigned int iNum = arg0.GetTypeValue<unsigned int>();
		Demo::Toggle(iNum);
	}
	//调用GetCameraPos返回相机位置,没有传入参数，带有3个float返回值
	else if (strcmp("GetCameraPos",sMethodName)==0)
	{
		float x,y,z;
		//GetCamerPos返回一个含有3个float值的字符串，需要解析出来
		sscanf(Demo::GetCarmerPos(),"%f %f %f",&x,&y,&z);
		FuncParameter fx(x),fy(y),fz(z);		
		resList.Push(fx);
		resList.Push(fy);
		resList.Push(fz);
		
		
	}
	else if (strcmp("LoadLogic",sMethodName)==0)
	{
		assert(argList.size());
		Demo::LoadLogic(argList[0].GetTypeValue<std::string>().c_str());
		
	}


	if(!CallFuncProxy::Instance()->CallFromEngine(g_pRec,sMethodName,sGUID,resList))
	{
		assert(0 && "if(!CallFuncProxy::Instance()->CallFromEngine(g_pRec,sMethodName,resList))");
	}
	return true;
}



static CLogFile g_LogFile;
void OpenAll()
{
	///
	char sLogName[256];
	sprintf(sLogName,"%s%s",g_WebData.sHomeDir,"\\EngineLog.txt");	
	OpenLog(sLogName,false);
	LogMsg("\n\n============================================================================================");

	g_App = new DemoPublishGameApplication() ;
	assert(g_App) ;

	if(!g_App)
		return ;

	WebJetWebData* pData = &g_WebData ;

	//g_App = n_new(WebSimpleGameApplication);
	g_App->SetCompanyName("Radon Labs GmbH");
	g_App->SetAppTitle("Test Game");
	//g_App.SetCmdLineArgs(args);
	//g_App.GetGraphicsFeature()->SetWebVersion(false,hWnd);
	g_App->SetWebVersion(true,pData->hWnd);
	g_App->SetResourceBaseDir(pData->sHomeDir);
	g_App->SetResDownloadUrl(pData->sResUrlBase);
	g_App->SetSceneName(pData->sSceneName);
	g_App->SetPipeName(pData->sPacketPipeName);	
	g_App->SetGameResolution(pData->uGameWidth,pData->uGameHeight);
	g_App->Open();
	
	PluginPacketFactoryManager::Instance()->registerAllFactory();
	//InitializeCriticalSection(&g_RecvCS);

	g_pRec = new PacketRecevier(g_App->GetPipeName(),PluginPacketFactoryManager::Instance());
	assert(g_pRec);
	g_pRec->Start();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void  WinWebInit(WebJetWebData* pData)
{
	if(!pData)
	{
		assert(0 && "pData is NULL in WinWebInit");
		return;
	}

	g_WebData = *pData ;

	Util::String fsCfg = "-id=xxyz -file_server=\"http://127.0.0.1/files\" -disk_cache=\"C:\\WebJetPluginInstall\\FileService\\Cache\"" ;
	g_bUseFS = false ;

	if( 1 && "Just for demo." )
	{
		FILE* pF = fopen( "fscfg.ini" , "r" ) ;
		if( pF )
		{
			char bf[1024] ;
			fgets( bf , 1024 , pF ) ;

			if( strlen( bf ) > 0 )
			{
				if( strncmp( bf , "-id" , 3 ) == 0 )
				{
					g_bUseFS = true ;
					fsCfg = bf ;
				}
			}

			fclose( pF ) ;
		}
	}

	if( g_bUseFS )
	{
		//wFileService::setFileServiceLocation( "C:\\WebJetPluginInstall\\FileService" ) ;

		/// Create file service .
		//IO::AdvFileServiceSystem::Create() ;
		//IO::AdvFileServiceSystem::Instance()->Setup( fsCfg ) ;

		/// Download level 0 files .
		//g_nLevel0FileTaskID = IO::AdvFileServiceSystem::Instance()->UpdateFileByLevel( 0 , 0 ) ;
	}


	

}

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void  WinWebUpdate()
{

	if( g_bUseFS )
	{
		if( !g_bAllOpenned )
		{
			//int stt = IO::AdvFileServiceSystem::Instance()->GetTaskState( g_nLevel0FileTaskID ) ;
			//if( stt == IO::AdvFileServiceSystem::Succeed || stt == IO::AdvFileServiceSystem::Failed )
			{
				OpenAll() ;
				g_bAllOpenned = true ;
			}
		}
	}
	else
	{
		if( !g_bAllOpenned )
		{
			OpenAll() ;
			g_bAllOpenned = true ;
		}
	}

	if( !g_bAllOpenned )
		return ;
	
	g_App->Run();

	//EnterCriticalSection(&g_RecvCS);
	if(g_pRec)
		g_pRec->Update();
	//LeaveCriticalSection(&g_RecvCS);
		
}

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void WinWebCleanup()
{
	if(g_pRec)
	{
		g_pRec->Terminate();
		delete g_pRec;
	}
	
	if (g_App)
	{
		g_App->End();
		/*g_App->Close();*/
		n_delete(g_App);
		g_App = NULL;
	}

	
	//DeleteCriticalSection(&g_RecvCS);
	//n_delete(g_pRec);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
LRESULT WinWebInputMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (g_App)
	{
		return g_App->GetInputWindowsSource()->OnWinProc(hWnd,uMsg,wParam,lParam);
	}
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}