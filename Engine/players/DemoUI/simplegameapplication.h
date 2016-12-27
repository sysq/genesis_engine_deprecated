/** \file simplegameapplication.h
* \brief 文件内容说明
* Copyright (c) 2011,畅游天下WebJet事业部
* All rights reserved.
* Date        Ver    Who          Comment
* 2011:7:20   1.0    王兴博   
*/
#ifndef __simplegameapplication_H__
#define __simplegameapplication_H__

#include "appframework/feature_fwd_decl.h"
#include "appframework/gameapplication.h"
#include "basegamefeature/basegamefeature.h"
#include "graphicfeature/graphicsfeature.h"
#include "inputfeature/inputfeature.h"
#include "foundation/framesync/framesynctimer.h"
#include "input/inputwindowsource.h"
#include "vegetationfeature/vegetationfeature.h"
#include "guifeature/gui.h"
//------------------------------------------------------------------------------
namespace Sample
{
	class SimpleGameApplication : public App::GameApplication
	{
		__DeclareThreadSingleton(SimpleGameApplication);   
	public:
		/// constructor
		SimpleGameApplication();
		/// destructor
		virtual ~SimpleGameApplication();

		/// open the application
		virtual bool Open();

		virtual void Close();

		/// 
		virtual void Run(void);

		const Ptr<Input::InputWindowSource>& GetInputWindowsSource(void) const;

	public:
		void SetMonoDebug(bool debug);
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
		Ptr<App::GraphicsFeature> mGraphicsFeature;
		Ptr<App::GraphicObjectManager> mGraphicObjectManager;
		Ptr<App::BaseGameFeature> mBaseGameFeature;
		Ptr<App::InputFeature> mInputFeature;
		App::TParticleFeaturePtr mParticleFeature;


		// 获取窗口消息的InputSource
		Ptr<Input::InputWindowSource> mInputWindowsSource;
		HWND mWnd;
		Ptr<App::GUIServer> mGuiServer;
		bool mMonoDebug;
	};
	//------------------------------------------------------------------------
	inline 
	const Ptr<Input::InputWindowSource>& 
	SimpleGameApplication::GetInputWindowsSource(void) const
	{
		return mInputWindowsSource;
	}

	inline void SimpleGameApplication::SetMonoDebug(bool debug)
	{
		mMonoDebug = debug;
	}

} // namespace Test
//------------------------------------------------------------------------------







#endif // __simplegameapplication_H__

