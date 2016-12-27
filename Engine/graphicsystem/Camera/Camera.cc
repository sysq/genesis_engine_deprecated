/****************************************************************************
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
#include "graphicsystem/Vision/RenderScene.h"
#include "Camera.h"
#include "GraphicSystem.h"
#include "materialmaker/parser/GenesisShaderParser.h"
#include "RenderPipeline/RenderPipelineManager.h"
#include "foundation/util/stl.h"
#include "graphicsystem/Renderable/RenderObject.h"

namespace Graphic
{

	_simple_scene::_simple_scene()
	{

	}

	_simple_scene::~_simple_scene()
	{

	}

	void _simple_scene::_UpdateVisEntity(const GPtr<Vis::VisEntity>& visEnt )
	{

	}
	void _simple_scene::_AddRenderObject(RenderObject* obj)
	{

	}
	void _simple_scene::_RemoveRenderObject(RenderObject* obj)
	{

	}

	// internal call
	//[zhongdaohuan]
	//这个队列里的的东西将不会产生裁剪而直接进入渲染队列。
	void _simple_scene::_AddRenderObjectNotCull(RenderObject* obj)
	{
		n_assert(obj);
		n_assert(!obj->_GetVisEnt().isvalid());
		m_notCullRenderObjects.Append(obj);	
	}
	void _simple_scene::_RemoveRenderObjectNotCull(RenderObject* obj)
	{
		n_assert(obj);
		n_assert(!obj->_GetVisEnt().isvalid());
		IndexT i = m_notCullRenderObjects.FindIndex(obj); 
		if( InvalidIndex != i )
		{
			m_notCullRenderObjects.EraseIndex(i);
		}
	}


	__ImplementClass(Camera,'CAGS',GraphicObject)
		/**
		*/
		const float Camera::PerspNormalFov = n_deg2rad(45.0f);
	float Camera::S_Camera_Far = 1000;	//	hack code for demo shadow map 
	Camera::Camera()
		:m_owner(NULL)
		,m_listener(NULL)
		,m_renderScene(NULL)
		,m_targetWindow(NULL)

		,m_bSetup(false)
		,m_bUseWindowSize(false)
		,m_bRenderDepthMap(false)
		,m_bRenderLightLitMap(true)
		,m_bRenderCustom(false)
		,m_bRenderNormal(false)
		,m_bUseViewPort(true)
		,m_bRenderSurface(false)
		,m_bUseBeforeDrawEvent(true)
		,m_bUseCallBack(true)

		,m_viewType(VT_persp)
		,m_renderOrder(eCO_InvalidCamera)
		,m_drawMode(DrawTextured)
		,m_renderMode(ForwardMode)
		,m_cullMask(eRLCameraRenderAll)

		,m_renderSort(Camera::SortDefault)
		,m_pickMark(Camera::PickAll)
#if __GENESIS_EDITOR__
		,m_CamTarget(GAME)
#endif
	{
		const RenderBase::DisplayMode& dm = GraphicSystem::Instance()->GetMainViewPortWindow()->GetDisplayMode();
		m_setting.SetupPerspectiveFovRH(Graphic::Camera::PerspNormalFov, float(dm.GetWidth())/float(dm.GetHeight()), 1.0f, S_Camera_Far);// 0.1f
		m_viewPort.width = dm.GetWidth();
		m_viewPort.height = dm.GetHeight();
		m_viewPort.x = 0;
		m_viewPort.y = 0;
		m_viewPort.minZ = 0.f;
		m_viewPort.maxZ = 1.f;
		m_renderPipelineManager = RenderPipelineManager::Create();
		m_renderPipelineManager->m_pipelineContext.m_camera = this;
		m_renderPipelineManager->m_pipelineContext.m_renderDatas.SetUseFor(RenderDataCollection::Normal);

		m_transform = matrix44::lookatrh(float4(1.27f, 1.2f, -800.4f, 1.f),float4(0.f, 0.f, 0.f, 1.f),float4(0.f,1.f,0.f,0.f));
		m_transform = matrix44::inverse(m_transform);
		OnTransformChanged();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	Camera::~Camera()
	{
		Discard();
	}

	void Camera::Discard()
	{
		m_bSetup = false;
		if (m_renderPipelineManager.isvalid())
		{
			m_renderPipelineManager->Close();
			m_renderPipelineManager = 0;
			SetRenderMode(ForwardMode);
			SetDrawType(DrawTextured);
		}

		if (m_lightLitMaterial.isvalid())
		{
			m_lightLitMaterial = 0;
		}

		if (m_deferredNormalMap.isvalid())
		{
			m_deferredNormalMap = 0;
		}

		if (m_deferredLightMap)
		{
			m_deferredLightMap = 0;
		}
		if (m_renderOrder == eCO_Main || m_renderOrder == eCO_PuppetMain)
		{
			unregistWindowEvent();
		}

		if(m_listener)
		{
			m_listener->OnDiscard(this);
		}
	}

	void Camera::Setup()
	{
		m_bSetup = true;
		if ( !m_renderPipelineManager )
		{
			m_renderPipelineManager = RenderPipelineManager::Create();
		}
		if (m_renderOrder == eCO_Main)
		{
			//			SetRenderMode(DeferredMode);
			unregistWindowEvent();
			ViewPortWindow* targetWindow = (NULL == m_targetWindow) ? GraphicSystem::Instance()->GetMainViewPortWindow() : m_targetWindow;
			m_bUseWindowSize = true;
			const RenderBase::DisplayMode& dm = targetWindow->GetDisplayMode();

			m_viewPort.width = dm.GetWidth();
			m_viewPort.height = dm.GetHeight();
			m_viewPort.x = 0;
			m_viewPort.y = 0;
			m_viewPort.minZ = 0.f;
			m_viewPort.maxZ = 1.f;
			m_bUseViewPort = true;

			m_bRenderNormal = true;
#if __WIN32__ && RENDERDEVICE_D3D9
			Util::StringAtom fileName("sys:Forward_lightLitMap.shader");
			m_lightLitMaterial = GenesisMaterialMaker::MakeFromShader( fileName );

#endif			
			registWindowEvent();

		}
		else if (m_renderOrder == eCO_Shadow)
		{
			m_bRenderCustom = true;
			m_bRenderNormal = false;
		}
		else
		{
			n_error("Custom RenderMode is not implemented right now!");
		}

	}

	void Camera::RenderBegin()
	{
		if (m_listener)
		{
			m_listener->OnRenderBegin(this);
		}
	}

	void Camera::RenderEnd()
	{
		if (m_listener)
		{
			m_listener->OnRenderEnd(this);
		}
	}

	GPtr<Vis::VisQuery> Camera::Cull() const
	{
		n_assert(m_renderScene);
		return m_renderScene->Cull(*this);
	}

	void Camera::SetTargetWindow(ViewPortWindow* target)
	{
		if (m_targetWindow != target)
		{
			unregistWindowEvent();
			m_targetWindow = target;
			registWindowEvent();
		}
	}

	// set custom pipeline
	bool Camera::SetCustomPipeline(const GPtr<RenderPipeline>& customPipeline)
	{
		bool isSucceed = false;

		if ( customPipeline.isvalid() )
		{
			m_renderPipelineManager->SetCustomPipeline(customPipeline);
			m_renderMode = CustomedMode;
			isSucceed = true;
		}
		return isSucceed;
	}

	IRenderScene* Camera::GetCameraScene()
	{
		return &m_cameraScene;
	}

	void Camera::registWindowEvent()
	{
		//if (m_renderOrder == eCO_Main)
		//{
		//	ViewPortWindow* targetWindow = (NULL == m_targetWindow) ? GraphicSystem::Instance()->GetMainViewPortWindow() : m_targetWindow;
		//	targetWindow->eventViewportChange += Delegates::newDelegate(this, &Camera::onTargetSizeChange); 
		//}
	}

	void Camera::unregistWindowEvent()
	{
		//if (m_renderOrder == eCO_Main || m_renderOrder == eCO_PuppetMain)
		//{
		//	ViewPortWindow* targetWindow = (NULL == m_targetWindow) ? GraphicSystem::Instance()->GetMainViewPortWindow() : m_targetWindow;
		//	if(targetWindow)
		//	{
		//		targetWindow->eventViewportChange -= Delegates::newDelegate(this, &Camera::onTargetSizeChange); 
		//	}
		//}
	}

	//void Camera::setupDeferred()
	//{
	//	const RenderBase::DisplayMode& dm = GraphicSystem::Instance()->GetMainViewPortWindow()->GetDisplayMode();

	//	m_deferredNormalMap = RenderToTexture::Create();
	//	m_deferredNormalMap->SetShareDepthStencilRT(m_renderToTexture->GetRenderTarget());
	//	m_deferredNormalMap->Setup(dm.GetWidth(),dm.GetHeight(),RenderBase::PixelFormat::G16R16F, 
	//		RenderBase::RenderTarget::ClearAll, Math::float4(0.0f,0.0f,0.0f,0.0f), 
	//		true,1.f);
	//	m_deferredParamMap = RenderToTexture::Create();//临时
	//	m_deferredParamMap->SetShareDepthStencilRT(m_renderToTexture->GetRenderTarget());
	//	m_deferredParamMap->Setup(dm.GetWidth(),dm.GetHeight(),RenderBase::PixelFormat::G16R16F,
	//		RenderBase::RenderTarget::ClearAll, Math::float4(0.f,0.f,0.f,0.f), 
	//		true,1.f);

	//	Material::GetGlobalMaterialParams()->SetTextureParam(eGShaderTexDfNormalMap, "g_DfNormalMap", m_deferredNormalMap->GetTextureHandle());
	//	Material::GetGlobalMaterialParams()->SetTextureParam(eGShaderTexDfDepthMap, "g_DfDepthMap", m_depthMap->GetTextureHandle());//m_deferredDepthMap
	//	Material::GetGlobalMaterialParams()->SetTextureParam(eGShaderTexDfParamMap, "g_DfParamMap", m_deferredParamMap->GetTextureHandle());

	//	m_deferredLightMap = RenderToTexture::Create();
	//	m_deferredLightMap->Setup(dm.GetWidth(),dm.GetHeight(),RenderBase::PixelFormat::A16B16G16R16F, 
	//		RenderBase::RenderTarget::ClearAll, Math::float4(0.0f,0.0f,0.0f,0.0f), 
	//		false, 1.f);
	//	Material::GetGlobalMaterialParams()->SetTextureParam(eGShaderTexDfLightMap0, "g_DfAndSpecMap0",m_deferredLightMap->GetTextureHandle());
	//}
	void Camera::onTargetSizeChange(ViewPortWindow* sender)
	{
		//n_assert( (NULL == m_targetWindow && GraphicSystem::Instance()->GetMainViewPortWindow()) || ( m_targetWindow == sender) );
		//if(m_bSetup)
		//{
		//	OnResizeWindow(sender->GetDisplayMode());
		//}
	}

	//------------------------------------------------------------------------------
	/**
	Set new the camera settings. This updates the internal matrices.
	*/
	void Camera::SetCameraSetting(const CameraSetting& camSetting)
	{
		m_setting = camSetting;
		m_setting.UpdateViewMatrix(matrix44::inverse(m_transform));

	}

	//------------------------------------------------------------------------------
	/**
	We need to keep track of modifications of the transformation matrix.
	*/
	void Camera::OnTransformChanged()
	{
		// keep track of view matrix
		m_setting.UpdateViewMatrix(matrix44::inverse(m_transform));
	}

	void Camera::SetDebugFrustum(bool enable, const float4& color)
	{

	}
	/// setup a perspective view volume
	void Camera::SetPerspectiveFovRH(float fov, float aspect, float zNear, float zFar)
	{

		m_viewType = VT_persp;
		m_setting.SetupPerspectiveFovRH(fov, aspect, zNear, zFar);
	}
	/// setup a perspective oblique view volume with a clipplane
	void Camera::SetPerspectiveFovRH(float fov, float aspect, float zNear, float zFar, Math::float4 clipPlane, bool reflection)
	{
		m_viewType = VT_persp;
		m_setting.SetupPerspectiveFovRH(fov, aspect, zNear, zFar, clipPlane, reflection);
	}
	/// setup an orthogonal projection transform
	void Camera::SetOrthogonal(float w, float h, float zNear, float zFar)
	{
		m_viewType = VT_ortho;
		m_setting.SetupOrthogonal((float)w, (float)h, zNear, zFar);
	}

	void Camera::SetProj(ViewType vt, float fovOrHeight, float zNear, float zFar)
	{
		ViewPortWindow* view = NULL;
		if (NULL == m_targetWindow)
		{
			const GPtr<ViewPortWindow>& p = GraphicSystem::Instance()->GetMainViewPortWindow();
			view = p.get_unsafe();
		}
		else
		{
			view = m_targetWindow;
		}
		//ViewPortWindow* view = (NULL == m_targetWindow) ? GraphicSystem::Instance()->GetMainViewPortWindow() : m_targetWindow;

		const RenderBase::DisplayMode& mode = view->GetDisplayMode();
		m_viewType = vt;
		if (VT_persp == m_viewType)
		{
			float aspect = (float)(mode.GetWidth()) / (float)mode.GetHeight();
			m_setting.SetupPerspectiveFovRH(fovOrHeight , aspect , zNear , zFar) ;
		}
		else if (VT_ortho == m_viewType)
		{
			float width = fovOrHeight * float(mode.GetWidth()) / float(mode.GetHeight());
			m_setting.SetupOrthogonal((float)width, (float)fovOrHeight, zNear, zFar);
		}

	}

	void Camera::CopyParam(const GPtr<Camera>& source)
	{
		Graphic::CameraSetting cs = source->GetCameraSetting();

		SetCameraSetting(cs);
		m_debugColor = source->m_debugColor;

		m_renderOrder = source->m_renderOrder;
		m_cullMask = source->m_cullMask;

		m_drawMode = source->m_drawMode;

		m_viewType = source->m_viewType;
		m_viewPort = source->m_viewPort;
		m_bUseViewPort = source->m_bUseViewPort;

		m_renderMode = source->m_renderMode;

		m_bRenderDepthMap = source->m_bRenderDepthMap;

		if (source->m_listener)
		{
			source->m_listener->OnCopyParam(source.get(), this, source.get());
		}
		if(m_listener)
		{
			m_listener->OnCopyParam(this, this, source.get());
		}
	}

	void Camera::ResetProjMatrix(int w, int h)
	{
		if (VT_persp == m_viewType)
		{
			float aspect = (float)w / (float)h;
			m_setting.SetupPerspectiveFovRH(m_setting.GetFov(), aspect, m_setting.GetZNear(), m_setting.GetZFar()) ;
		}
		else if (VT_ortho == m_viewType)
		{
			float width = m_setting.GetFarHeight() * (float)(w) / (float)h;
			m_setting.SetupOrthogonal((float)width, (float)m_setting.GetFarHeight(), m_setting.GetZNear(), m_setting.GetZFar());
		}
	}

	void Camera::OnResizeWindow(int w, int h)
	{
		m_viewPort.width = w;
		m_viewPort.height = h;
		ResetProjMatrix(m_viewPort.width, m_viewPort.height);
		if (m_targetSuite.isvalid())
		{
			m_targetSuite->OnChangeSize(w, h);
		}
	}

	void Camera::OnDeviceReset()
	{
		if (m_targetSuite.isvalid())
		{
			m_targetSuite->OnDeviceReset();
		}
	}

	void Camera::SetViewType(ViewType type, float fovOrHeight)
	{
		SetProj(type, fovOrHeight, m_setting.GetZNear() , m_setting.GetZFar());
	}

	void Camera::SetRenderSort(uint sort)
	{
		m_renderSort = sort;
		if (m_renderScene)
		{
			m_renderScene->_SortCameraList();
		}
	}

	void Camera::SetRenderScene(RenderScene* scene,bool bAddToRenderScene)
	{
		if(m_renderScene)
		{
			m_renderScene->_RemoveCamera(this);
		}
		m_renderScene = scene;
		if(m_renderScene && bAddToRenderScene )
		{
			m_renderScene->_AddCamera(this);
		}
	}

	void Camera::SetRenderToTexture(const GPtr<RenderToTexture>& rtt)
	{
		m_viewPort.width = rtt->GetWidth();
		m_viewPort.height = rtt->GetHeight();
		OnResizeWindow(m_viewPort.width, m_viewPort.height);
		m_targetWindow = NULL;
		m_targetSuite = NULL;
		m_targetSuite = RenderTargetSuite::Create();
		RenderTargetSuite::Target target = checkTarget();

		target = BIT_FlAG_APPEND(target, RenderTargetSuite::Target_RTT);
		target = BIT_FlAG_APPEND(target, RenderTargetSuite::Target_Swap);

		m_targetSuite->SetupFormRTT(rtt, target);
	}

	void Camera::SetLightLitMap(const GPtr<RenderToTexture>& rtt)
	{
		//empty.
	}

	void Camera::SetRenderDepth(bool bDepth)
	{
		if (m_bRenderDepthMap != bDepth)
		{
			if (m_targetSuite.isvalid())
			{
				RenderTargetSuite::Target target = m_targetSuite->GetTargetSetting();
				if (bDepth)
				{
					target = BIT_FlAG_APPEND(target, RenderTargetSuite::Target_Depth);
				}
				else
				{
					target = BIT_FlAG_DELETE(target, RenderTargetSuite::Target_Depth);
				}
				m_targetSuite->ChangeTargetSetting(target);	
			}	
		}
	
		m_bRenderDepthMap = bDepth;
	}
	void Camera::SetRenderLightLitMap(bool enable)
	{
		if (m_bRenderLightLitMap != enable)
		{
			if (m_targetSuite.isvalid())
			{
				RenderTargetSuite::Target target = m_targetSuite->GetTargetSetting();
				if (enable)
				{
					target = BIT_FlAG_APPEND(target, RenderTargetSuite::Target_LightLit);
				}
				else
				{
					target = BIT_FlAG_DELETE(target, RenderTargetSuite::Target_LightLit);
				}
				m_targetSuite->ChangeTargetSetting(target);	
			}	
		}
		m_bRenderLightLitMap = enable;
	}

	RenderTargetSuite::Target Camera::checkTarget() const
	{
		RenderTargetSuite::Target target = RenderTargetSuite::Target_None;
		if (m_bRenderDepthMap)
		{
			target = BIT_FlAG_APPEND(target, RenderTargetSuite::Target_Depth);
		}
		if (m_bRenderLightLitMap)
		{
			target = BIT_FlAG_APPEND(target, RenderTargetSuite::Target_LightLit);
		}
		if (m_bRenderDepthMap)
		{
			target = BIT_FlAG_APPEND(target, RenderTargetSuite::Target_Depth);
		}
		return target;
	}
}
