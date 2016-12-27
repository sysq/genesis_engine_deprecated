/** \file: DemoPublishGameApplication.h
* \brief: Export the game
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/28   1.0    SunHao   
*/

#ifndef _WEBWebSimpleGameApplication_H_
#define  _WEBWebSimpleGameApplication_H_
#include "DemoPublish_GameApplication.h"

#include "appframework/gameapplication.h"
#include "basegamefeature/basegamefeature.h"
#include "graphicfeature/graphicsfeature.h"
#include "inputfeature/inputfeature.h"
#include "foundation/framesync/framesynctimer.h"
#include "input/inputhardwaresource.h"
#include "input/inputwindowsource.h"
#include "DemoGameStateHandler.h"
//------------------------------------------------------------------------------
namespace DemoPublish
{
	class DemoPublishGameApplication : public App::GameApplication
	{
		__DeclareThreadSingleton(DemoPublishGameApplication);   
	public:
		/// constructor
		DemoPublishGameApplication();
		/// destructor
		virtual ~DemoPublishGameApplication();

		/// open the application
		virtual bool Open();


		//inline Ptr<App::GraphicsFeature> GetGraphicsFeature(){return mGraphicsFeature;}

		void SetWebVersion(bool bWebVersion,void* hWebWindow);


		virtual void Close();

		/// 
		virtual void Run(void);

		const Ptr<Input::InputWindowSource>& GetInputWindowsSource(void) const;

		void End();

		//设定dll目录
		void SetResourceBaseDir(const char* sPath)
		{
			m_sResourceBaseDir = sPath;
		}

		//设定资源下载URL
		void SetResDownloadUrl(const char* sPath)
		{
			m_sResDownloadUrl = sPath;
		}

		//设定场景名称
		void SetSceneName(const char* sSceneName)
		{
			m_sSceneName = sSceneName;
		}

		//设定管道名称
		void SetPipeName(const char* sPipeName)
		{
			m_sPipeName = sPipeName;
		}

		const char* GetPipeName()
		{
			return m_sPipeName.AsCharPtr();
		}

		void SetGameResolution(unsigned int uGameWidth,unsigned int uGameHeight)
		{
			muGameWidth = uGameWidth;
			muGameHeight = uGameHeight;
		}
		
		unsigned int GetGameWidth()
		{
			return muGameWidth;
		}

		unsigned int GetGameHeight()
		{
			return muGameHeight;

		}
	protected:
		/// setup application state handlers
		virtual void SetupStateHandlers();
		/// setup game features
		virtual void SetupGameFeatures();
		/// cleanup game features
		virtual void CleanupGameFeatures(); 
		/// 设置工程路径
		virtual void SetupProjectAssigns();

	private:
		bool mbWebVersion;
		void* mhWebWindow;
		unsigned int muGameWidth;
		unsigned int muGameHeight;

		Ptr<App::GraphicsFeature> mGraphicsFeature;
		Ptr<App::GraphicObjectManager> mGraphicObjectManager;
		Ptr<App::BaseGameFeature> mBaseGameFeature;
		Ptr<App::InputFeature> mInputFeature;

		Ptr<Demo::DemoGameStateHandler> gameState;

		// 获取窗口消息的InputSource
		Ptr<Input::InputWindowSource> mInputWindowsSource;
		HWND mWnd;
		HWND mExternWnd;
		
		//资源放置的基础目录
		Util::String m_sResourceBaseDir;
		//资源下载的基础url
		Util::String m_sResDownloadUrl;
		//场景名称
		Util::String m_sSceneName;
		//Packet Pipe
		Util::String m_sPipeName;


	};
	//------------------------------------------------------------------------
	inline 
		const Ptr<Input::InputWindowSource>& 
		DemoPublishGameApplication::GetInputWindowsSource(void) const
	{
		return mInputWindowsSource;
	}

} // namespace Test
//------------------------------------------------------------------------------



#endif