/****************************************************************************
Copyright (c) 2006, Radon Labs GmbH
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
#include "graphicfeature/graphicsfeature.h"
#include "appframework/actormanager.h"
#include "appframework/gameserver.h"
#include "basegamefeature/managers/timesource.h"
#include "components/cameracomponent.h"
#include "graphicsystem/base/ImageFilters.h"
#include "app/guifeature/guiserver.h"
#include "graphicsystem/Camera/Camera.h"
namespace App
{
	__ImplementClass(GraphicsFeature, 'FGFX' , App::Feature);
	__ImplementImageSingleton(GraphicsFeature);

	using namespace Graphic;

	//------------------------------------------------------------------------------
	/**
	*/
	GraphicsFeature::GraphicsFeature()
		: mMainWindow(NULL)
		, mWebVersion(false)
		, mWindowSizeChanged(false)
	{
		__ConstructImageSingleton;

		mFeatureName = "Graphics";
	}

	//------------------------------------------------------------------------------
	/**
	*/
	GraphicsFeature::~GraphicsFeature()
	{
		__DestructImageSingleton;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		GraphicsFeature::OnActivate()
	{
		n_assert(!this->IsActive());

		// call parent class
		Super::OnActivate();

		n_assert( !Graphic::GraphicSystem::HasInstance() );

#if __WIN32__
		n_assert( mMainWindow != NULL );
#endif

		mGraphicSystem = Graphic::GraphicSystem::Create();

		mGraphicSystem->Open(mMainWindow, mWindowWidth, mWindowHeight, RenderBase::PixelFormat::X8R8G8B8);

		mMainViewPort = mGraphicSystem->GetMainViewPortWindow();
#ifdef __OSX__

#else
		mMainViewPort->eventViewportChange  += Delegates::newDelegate(this, &GraphicsFeature::onMainViewPortChange);
#endif
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		GraphicsFeature::OnDeactivate()
	{
		n_assert(this->IsActive());

		//mDefaultCameraActor = NULL;
		ImageFiltrationSystem::Shutdown();
		n_assert( mGraphicSystem.isvalid() );
		mGraphicSystem->DestroyViewPortWindow(mMainViewPort);
		mMainViewPort = NULL;
		mGraphicSystem->Close();

		Super::OnDeactivate();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		GraphicsFeature::OnStart()
	{
		Super::OnStart();
	}
	////------------------------------------------------------------------------
	//void 
	//GraphicsFeature::AssignDefaultCamera(const GPtr<App::Actor>& camActor)
	//{
	//	mDefaultCameraActor = camActor;
	//}
	//------------------------------------------------------------------------------
	/**
	*/
	void
		GraphicsFeature::OnBeginFrame()
	{
		Graphic::GlobalMaterialParam* globalMaterialParam = Graphic::Material::GetGlobalMaterialParams();
		double curframe = GameTime::Instance()->GetTime();
		double wrapTime = GameTime::Instance()->GetWrapTime();

		Math::float4 g_time = globalMaterialParam->GetVectorParam(Graphic::eGShaderVecTime);
		g_time.x() = (float)curframe;
		g_time.y() = (float)wrapTime;

		globalMaterialParam->SetVectorParam( Graphic::eGShaderVecTime, g_time);
		mGraphicSystem->OnBeginFrame();
		Super::OnBeginFrame();


	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		GraphicsFeature::OnFrame()
	{    
		n_assert(mGraphicSystem.isvalid());
		mGraphicSystem->OnUpateFrame();
		Super::OnFrame();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		GraphicsFeature::OnEndFrame()
	{
		mGraphicSystem->OnEndFrame();
		Super::OnEndFrame();
	}
	//------------------------------------------------------------------------------
	/**
	*/
	GPtr<Graphic::ViewPortWindow>&
		GraphicsFeature::GetMainViewPort()
	{
		return mMainViewPort;
	}

	//------------------------------------------------------------------------
	void GraphicsFeature::SetMainWindow(void* hWnd)
	{
		mMainWindow = hWnd;
		//mGraphicSystem->CreateViewPortWindow((HWND)hWnd);
	}
	//--------------------------------------------------------------------------------
	void GraphicsFeature::SetWindowSize( int width, int height)
	{
		mWindowWidth = width;
		mWindowHeight = height;	
	}
	GPtr<Graphic::ViewPortWindow> GraphicsFeature::CreateViewPortWindow(WindHandle hWnd, const RenderBase::DisplayMode& mode)
	{
		return	mGraphicSystem->CreateViewPortWindow(hWnd, mode);
	}
	void GraphicsFeature::DestroyViewPortWindow(GPtr<Graphic::ViewPortWindow>& view)
	{
		n_assert(view != mMainViewPort);//[zhongdaohuan]主视口不应该由外部控制它的打开或销毁。
		mGraphicSystem->DestroyViewPortWindow(view);
	}


	void GraphicsFeature::onMainViewPortChange(Graphic::ViewPortWindow* sender)
	{
		ImageFiltrationSystem::ResizeWindow();
		if (GUIServer::HasInstance() && GUIServer::Instance()->IsOpen())
		{
			GUIServer::Instance()->OnWindowResized();
		}
	}

	GPtr<App::Actor> GraphicsFeature::GetDefaultCameraActor() const
	{
		Camera* camera = mGraphicSystem->GetSceneDefaultCamera();
		if(camera)
		{
			CameraComponent* cc = dynamic_cast<CameraComponent*>(camera->GetOwner());
			if (cc)
			{
				return GPtr<App::Actor>(cc->GetActor());
			}
		}
		return GPtr<App::Actor>();
	}

	void GraphicsFeature::OnDeviceLost()
	{
		mGraphicSystem->OnDeviceLost();


	}

	bool GraphicsFeature::CheckReset()
	{
		return mGraphicSystem->CheckReset();
	}

	void GraphicsFeature::OnDeviceReset()
	{
		mGraphicSystem->OnDeviceReset();

		eventDeviceReset(this);

		if (GUIServer::HasInstance())
		{
			if (GUIServer::Instance()->IsOpen())
			{
				GUIServer::Instance()->OnDeviceReseted();
			}
		}
	}

} // namespace Game
