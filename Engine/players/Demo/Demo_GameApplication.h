/** \file: DemoGameApplication.h
* \brief: application for Demo
* Copyright (c) 2011,畅游天下WebJet事业部
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/14   1.0    SunHao   
*/
#ifndef __DEMOAPPLICATION_H__
#define __DEMOAPPLICATION_H__

#include "appframework/gameapplication.h"
#include "input/inputwindowsource.h"
#include "appframework/feature_fwd_decl.h"
#include "DemoFwdDecl.h"
#include "graphicsystem/GraphicObjectManager.h"

namespace Demo
{

	class DemoGameApplication : public App::GameApplication
	{
		__DeclareThreadSingleton(DemoGameApplication);   
	public:
		/// constructor
		DemoGameApplication();
		/// destructor
		virtual ~DemoGameApplication();
		/// open the application
		virtual bool Open();
		/// close it
		virtual void Close();
		/// run it
		virtual void Run( void );
		/// get method
		const Ptr<Input::InputWindowSource>& GetInputWindowsSource( void ) const;

	protected:
		/// setup application state handlers
		virtual void SetupStateHandlers();
		/// setup game features
		virtual void SetupGameFeatures();
		/// cleanup game features
		virtual void CleanupGameFeatures(); 
		/// set working path
		virtual void SetupProjectAssigns();

	private:

		App::TGraphicsFeaturePtr mGraphicsFeature;
		Ptr<App::GraphicObjectManager> mGraphicObjectManager;
		App::TBaseGameFeaturePtr mBaseGameFeature;
		App::TInputFeaturePtr mInputFeature;

		App::TParticleFeaturePtr mParticleFeature;
		App::TVegetationFeaturePtr mVegeFeature;
		Ptr<App::SoundFeature> mSoundFeature;
		Ptr<App::PhysicsFeature> mPhysicsFeature;
		Ptr<App::AnimationFeature> mAnimationFeature;
		Ptr<App::ScriptFeature> mScriptFeature;

		/// get window InputSource
		Ptr<Input::InputWindowSource> m_InputWindowsSource;
		HWND m_Wnd;
		HWND m_TestWnd;
	};

	//------------------------------------------------------------------------
	inline const Ptr<Input::InputWindowSource>& DemoGameApplication::GetInputWindowsSource(void) const
	{
		return m_InputWindowsSource;
	}
}

#endif // __DEMOAPPLICATION_H__