#pragma once

#include "appframework/gameapplication.h"
#include "appframework/feature_fwd_decl.h"
#include "foundation/framesync/framesynctimer.h"
#include "input/inputwindowsource.h"
#include "graphicsystem/graphicobjectmanager.h"
#include "appframework/feature_fwd_decl.h"

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
		Ptr<App::BaseGameFeature> mBaseGameFeature;
		Ptr<App::InputFeature> mInputFeature;
		Ptr<App::GraphicObjectManager> mGraphicObjectManager;

		// 获取窗口消息的InputSource
		Ptr<Input::InputWindowSource> mInputWindowsSource;
		App::TParticleFeaturePtr mParticleFeature;
		HWND mWnd;

	};
	//------------------------------------------------------------------------
	inline 
		const Ptr<Input::InputWindowSource>& 
		SimpleGameApplication::GetInputWindowsSource(void) const
	{
		return mInputWindowsSource;
	}

} 