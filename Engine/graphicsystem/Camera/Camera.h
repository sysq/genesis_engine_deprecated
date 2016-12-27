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
#ifndef CAMERA_H_
#define CAMERA_H_

#include "rendersystem/RenderSystem.h"
#include "Camera/CameraSetting.h"
#include "base/GraphicObject.h"
#include "Light/Light.h"
#include "base/RenderToTexture.h"
#include "util/stl.h"
#include "base/ImageFilters.h"
#include "graphicsystem/RenderTargetSuite.h"
#include "graphicsystem/Material/Material.h"
#include "graphicsystem/Vision/RenderScene.h"
#include "Camera/RenderPipeline/RenderPipeline.h"
#include "foundation/util/scriptbind.h"

namespace Vis
{
	class VisQuery;
}

namespace Graphic
{
	class RenderPipelineManager;
	class Light;
	class RenderObject;
	class ViewPortWindow;
	class RenderScene;

	class Camera;
	class CameraListener
	{
	public:
		virtual ~CameraListener(){};
		virtual void OnRenderBegin(Camera* sender) = 0;
		virtual void OnRenderEnd(Camera* sender) = 0;
		virtual void OnCopyParam(Camera* sender, Camera* dest, const Camera* source) = 0;
		virtual void OnDiscard(Camera* sender) = 0;
		virtual void OnListenerChange(Camera* sender) = 0;
	};

	class _simple_scene : public IRenderScene
	{
	public:
		_simple_scene();
		virtual ~_simple_scene();
		// internal call
		virtual void _UpdateVisEntity(const GPtr<Vis::VisEntity>& visEnt );
		virtual void _AddRenderObject(RenderObject* obj);
		virtual void _RemoveRenderObject(RenderObject* obj);

		// internal call
		virtual void _AddRenderObjectNotCull(RenderObject* obj);
		virtual void _RemoveRenderObjectNotCull(RenderObject* obj);

		const Util::Array<RenderObject*>& _GetNotCullRenderObjects() const;
	private:
		Util::Array<RenderObject*>	m_notCullRenderObjects;
	};

	inline const Util::Array<RenderObject*>& _simple_scene::_GetNotCullRenderObjects() const
	{
		return m_notCullRenderObjects;
	}

	class Camera : public GraphicObject
	{
		__DeclareSubClass(Camera,GraphicObject);
	public:
		static float S_Camera_Far;	//	hack code for demo shadow map 
		enum RenderMode
		{
			ForwardMode,
			DeferredMode,
			CustomedMode
		};

		enum RenderSort
		{
			SortBackground = 1000,
			SortRTT = 2000,
			SortScene = 3000,
			SortTop = 4000,
			SortSuperTop = 5000,
			SortUI = 6000,		
			SortSuperUI = 7000,
			SortDefault = SortScene,
		};

		enum Pick
		{
			PickNone = BIT_FLAG_NONE,
			PickCullObjects = BIT_FLAG(0),
			PickNoCullObjects = BIT_FLAG(1),
			PickSelfObjects = BIT_FLAG(2),
			PickSunLight = BIT_FLAG(3),
			PickOtherLights = BIT_FLAG(4),
			PickAll = BIT_FLAG_ALL,
		};

		struct Viewport
		{
			SizeT x;
			SizeT y;
			SizeT width;
			SizeT height;
			float minZ;
			float maxZ;
		};

		enum DrawType
		{
			DrawTextured				= 0,
			DrawWire					= 1,
			DrawTexturedWire			= 2,
			DrawRenderPaths				= 3,
		};

		enum ViewType
		{
			VT_ortho,//正交投影
			VT_persp//透视投影
		};
		static const float PerspNormalFov;
		Camera();
		virtual ~Camera();

		virtual void Setup();

		virtual void Discard();

		void RenderBegin();

		void RenderEnd();

		GPtr<Vis::VisQuery> Cull() const;
		RenderScene* GetRenderScene() const;

		/// set render order
		void SetCameraOrder(CameraOrder order);
		/// get render order
		const CameraOrder& GetCameraOrder() const;		

		/// set draw mode
		void SetDrawType(DrawType mode);
		/// get drawmode
		const DrawType& GetDrawType() const;

		void SetViewType(ViewType type, float fovOrHeight);

		ViewType GetViewType() const;

		void SetRenderSort(uint sort);

		uint GetRenderSort() const;

		/// set
		void SetRenderMode(RenderMode mode);
		/// get
		RenderMode GetRenderMode() const;

		/// set new camera settings
		void SetCameraSetting(const CameraSetting& camSetting);
		/// get camera settings
		const CameraSetting& GetCameraSetting() const;

		/// get projection matrix
		const Math::matrix44& GetProjTransform() const;
		/// get view transform (inverse transform)
		const Math::matrix44& GetViewTransform() const;
		/// get view projection matrix 
		const Math::matrix44& GetViewProjTransform() const;
		/// get view frustum
		const Math::frustum& GetViewFrustum() const;

		void SetProj(ViewType vt, float fovOrHeight, float zNear, float zFar);

		/// setup a perspective view volume
		void SetPerspectiveFovRH(float fov, float aspect, float zNear, float zFar);
		/// setup a perspective oblique view volume with a clipplane
		void SetPerspectiveFovRH(float fov, float aspect, float zNear, float zFar, Math::float4 clipPlane, bool reflection = false);
		/// setup an orthogonal projection transform
		void SetOrthogonal(float w, float h, float zNear, float zFar);

		/// set shadow distence
		void SetShadowDistence(float shadowDis);
		/// get shadow distence
		const float GetShadowDistence() const;

		///
		GPtr<RenderPipelineManager>& GetRenderPipelineManager();

		ImageFilterManager& GetPostEffectFilters();
		///
		void OnTransformChanged();

		//const RenderBase::RenderResourceHandle& GetBackBuffer() const;

		/// set rendertotexture
		void SetRenderToTexture(const GPtr<RenderToTexture>& rtt);
		///set lightlit map
		void SetLightLitMap(const GPtr<RenderToTexture>& rtt);
		
		bool IsRenderDepthMap() const;

		bool IsRenderLightLitMap() const;
		/// get light lit material
		const GPtr<Material>& GetLightLitMaterial() const;
		/// get shadow blur material
		const GPtr<Material>& GetShadowBlurMaterial() const;

		/// rendered with custom material
		const bool IsRenderCustom() const;

		/// need set view port
		const bool UseViewport() const;

		/// set viewport
		void SetViewport(const Viewport& vp);
		/// get viewport
		const Viewport& GetViewport() const;

		void SetTargetWindow(ViewPortWindow* target);

		ViewPortWindow* GetTargetWindow() const;
		/// get custom material
		const GPtr<Material>& GetCustomMaterial() const;

		const bool IsRenderNormal() const;

		void SetDebugFrustum(bool enable, const Math::float4& color);

		/// get debug color
		const Math::float4& GetDebugColor() const;
		/// set debug color
		void SetDebugColor(const Math::float4& color);

		/// get cull mask
		const RenderLayer& GetCullMask() const;
		/// set cull mask
		void SetCullMask(const RenderLayer& rl);

		void SetPickMark(Pick mark);

		Pick GetPickMark() const;

		bool IsPick(Pick pck) const;

		void SetRenderSurface(bool bRenderSurface);

		const bool IsRenderSurface() const;

		void SetRenderNormal(bool bNormal);

		void SetRenderDepth(bool bDepth);

		void SetUseCustomMaterial(bool bUse);

		void SetCustomMaterial(const GPtr<Material>& pMat);

		void SetClipPlane(const Math::float4& plane);

		void SetUseWindowSize(bool bWinSize);

		const bool IsUsingWindowSize() const;

		void OnResizeWindow(int w, int h);

		void OnDeviceReset();

		void ResetProjMatrix(int w, int h);

		const Math::float4& GetClipPlane() const;

		// set custom pipeline
		bool SetCustomPipeline(const GPtr<RenderPipeline>& customPipeline);

		void CopyParam(const GPtr<Camera>& source);//不包括rendertarget
		void SetListener(CameraListener* listener);
		CameraListener* GetListener() const;
		void SetRenderScene(RenderScene* scene,bool bAddToRenderScene = false);

		//[zhongdaohuan]
		//这个队列里的的东西将不会产生裁剪而直接进入渲染队列。
		IRenderScene* GetCameraScene();
		const Util::Array<RenderObject*>& GetNotCullRenderObjects() const;

		void SetOwner(Core::RefCounted* owner);

		Core::RefCounted* GetOwner() const;

		void SetBeforeDrawEvent(bool enable);

		bool IsUseBeforeDrawEvent() const;

		const GPtr<QuadRenderable>&  GetQuadRenderable() const;

		//临时处理让摄影机不参与shadow map的渲染
		void SetRenderLightLitMap(bool enable);

		RenderTargetSuite* GetRenderTargetSuite() const;

		void SetUseCallBack(bool bUse);

		bool GetUseCallBack() const;

#if __GENESIS_EDITOR__

		enum CameraTarget
		{
			SCENE = 0,
			GAME
		};

		void SetCameraTarget(const CameraTarget bTarget);

		CameraTarget GetCameraTarget() const;
#endif

	private: 
		RenderTargetSuite::Target checkTarget() const;
		void registWindowEvent();

		void unregistWindowEvent();

		//void setupDeferred();
		void onTargetSizeChange(ViewPortWindow* sender);
		Core::RefCounted* m_owner;

		GPtr<RenderPipelineManager> m_renderPipelineManager;

		GPtr<RenderTargetSuite> m_targetSuite;
		//GPtr<RenderToTexture> m_backBuffer;
		GPtr<Material>        m_lightLitMaterial;
		GPtr<Material>		 m_customMaterial;
		// deferred lighting 
		GPtr<RenderToTexture> m_deferredNormalMap;
		GPtr<RenderToTexture> m_deferredParamMap;
		GPtr<RenderToTexture> m_deferredLightMap;

		RenderScene*				m_renderScene;
		ViewPortWindow*				m_targetWindow;
		CameraListener*				m_listener;
		_simple_scene				m_cameraScene;
		CameraSetting				m_setting;
		ImageFilterManager			m_postEffects;

		Math::float4				m_debugColor;
		Math::float4				m_ClipPlane;

		CameraOrder		m_renderOrder;
		RenderLayer		m_cullMask;
		DrawType		m_drawMode;
		ViewType		m_viewType;
		Viewport		m_viewPort;
		RenderMode		m_renderMode;
		Pick			m_pickMark;
		uint			m_renderSort;

		bool m_bSetup;
		bool m_bUseViewPort;
		bool m_bRenderDepthMap;
		bool m_bRenderLightLitMap;
		bool m_bRenderCustom;
		bool m_bRenderNormal;
		bool m_bRenderSurface;
		bool m_bUseWindowSize;
		bool m_bUseBeforeDrawEvent;
		bool m_bUseCallBack;

#if __GENESIS_EDITOR__
		CameraTarget m_CamTarget;
#endif

	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::matrix44&
		Camera::GetProjTransform() const
	{
		return m_setting.GetProjTransform();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::matrix44&
		Camera::GetViewTransform() const
	{
		return m_setting.GetViewTransform();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::matrix44&
		Camera::GetViewProjTransform() const
	{
		return m_setting.GetViewProjTransform();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Math::frustum&
		Camera::GetViewFrustum() const
	{
		n_error("Frustum's compution is not right");
		return m_setting.GetViewFrustum();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const CameraSetting&
		Camera::GetCameraSetting() const
	{
		return m_setting;
	}

	inline GPtr<RenderPipelineManager>& Camera::GetRenderPipelineManager()
	{
		return m_renderPipelineManager;
	}


	inline void Camera::SetCameraOrder(CameraOrder order)
	{
		m_renderOrder = order;
	}

	inline const CameraOrder& Camera::GetCameraOrder() const
	{
		return m_renderOrder;
	}

	inline void Camera::SetRenderMode(RenderMode mode)
	{
		m_renderMode = mode;
	}

	inline Camera::RenderMode Camera::GetRenderMode() const
	{
		return m_renderMode;
	}

	//inline void Camera::SetSwapTexture(const GPtr<RenderToTexture>& rtt)
	//{
	//	
	//}
	inline bool Camera::IsRenderDepthMap() const
	{
		return m_bRenderDepthMap;
	}
	inline bool Camera::IsRenderLightLitMap() const
	{
		return m_bRenderLightLitMap && m_lightLitMaterial.isvalid();
	}

	inline const GPtr<Material>& Camera::GetLightLitMaterial() const
	{
		return m_lightLitMaterial;
	}

	inline const bool Camera::IsRenderCustom() const
	{
		return m_bRenderCustom;
	}

	inline const GPtr<Material>& Camera::GetCustomMaterial() const
	{
		return m_customMaterial;
	}

	inline const bool Camera::IsRenderNormal() const
	{
		return m_bRenderNormal;
	}

	inline void Camera::SetDebugColor(const Math::float4& color)
	{
		m_debugColor = color;
	}

	inline const Math::float4& Camera::GetDebugColor() const
	{
		return m_debugColor;
	}

	inline void Camera::SetCullMask(const RenderLayer& rl)
	{
		m_cullMask = rl;
	}

	inline const RenderLayer& Camera::GetCullMask() const
	{
		return m_cullMask;
	}

	inline void Camera::SetPickMark(Pick mark)
	{
		m_pickMark = mark;
	}

	inline bool Camera::IsPick(Pick pck) const
	{
		return BIT_FLAG_IS_OPEN(m_pickMark, pck);
	}

	inline Camera::Pick Camera::GetPickMark() const
	{
		return m_pickMark;
	}

	inline const bool Camera::IsRenderSurface() const
	{
		return m_bRenderSurface;
	}

	inline void Camera::SetRenderSurface(bool bRenderSurface)
	{
		m_bRenderSurface = bRenderSurface;
	}

	inline const bool Camera::UseViewport() const
	{
		return m_bUseViewPort;
	}

	inline void Camera::SetViewport(const Viewport& vp)
	{
		m_viewPort     = vp;
		m_bUseViewPort = true;
	}

	inline const Camera::Viewport& Camera::GetViewport() const
	{
		return m_viewPort;
	}

	inline ViewPortWindow* Camera::GetTargetWindow() const
	{
		return m_targetWindow;
	}

	inline void Camera::SetRenderNormal(bool bNormal)
	{
		m_bRenderNormal = bNormal;
	}

	inline void Camera::SetUseCustomMaterial(bool bUse)
	{
		m_bRenderCustom = bUse;
	}

	inline void Camera::SetCustomMaterial(const GPtr<Material>& pMat)
	{
		m_customMaterial = pMat;
	}

	inline void Camera::SetClipPlane(const Math::float4& plane)
	{
		m_ClipPlane = plane;
	}

	inline const Math::float4& Camera::GetClipPlane() const
	{
		return m_ClipPlane;
	}

	inline void Camera::SetUseWindowSize(bool bWinSize)
	{
		m_bUseWindowSize = bWinSize;
	}

	inline const bool Camera::IsUsingWindowSize() const
	{
		return m_bUseWindowSize;
	}
	//--------------------------------------------------------------------------------
	inline  void  Camera::SetDrawType(Camera::DrawType mode)
	{
		m_drawMode = mode;
	}
	//--------------------------------------------------------------------------------
	inline const Camera::DrawType&  Camera::GetDrawType() const
	{
		return m_drawMode;
	}
	//--------------------------------------------------------------------------------
	inline Camera::ViewType Camera::GetViewType() const
	{
		return m_viewType;
	}

	inline uint Camera::GetRenderSort() const
	{
		return m_renderSort;
	}

	//---------------------------------------------------------------------------------
	inline ImageFilterManager& Camera::GetPostEffectFilters()
	{
		return m_postEffects;
	}
	inline const Util::Array<RenderObject*>& Camera::GetNotCullRenderObjects() const
	{
		return m_cameraScene._GetNotCullRenderObjects();
	}

	inline void Camera::SetListener(CameraListener* listener)
	{
		m_listener = listener;
		if(m_listener)
		{
			m_listener->OnListenerChange(this);
		}
	}

	inline CameraListener* Camera::GetListener() const
	{
		return m_listener;
	}
	//---------------------------------------------------------------------------------
	inline void Camera::SetShadowDistence(float shadowDis)
	{
		m_setting.SetShadowDistance(shadowDis);
	}

	inline const float Camera::GetShadowDistence() const
	{
		return m_setting.GetShadowDistance();
	}

	inline void Camera::SetOwner(Core::RefCounted* owner)
	{
		m_owner = owner;
	}

	inline Core::RefCounted* Camera::GetOwner() const
	{
		return m_owner;
	}

	inline void Camera::SetBeforeDrawEvent(bool enable)
	{
		m_bUseBeforeDrawEvent = enable;
	}

	inline bool Camera::IsUseBeforeDrawEvent() const
	{
		return m_bUseBeforeDrawEvent;
	}

	inline RenderScene* Camera::GetRenderScene() const
	{
		return m_renderScene;
	}

	inline void Camera::SetUseCallBack( bool bUse )
	{
		m_bUseCallBack = bUse;
	}

	inline bool Camera::GetUseCallBack() const
	{
		return m_bUseCallBack;
	}

	inline RenderTargetSuite* Camera::GetRenderTargetSuite() const
	{
		return m_targetSuite.get_unsafe();
	}
	//---------------------------------------------------------------------------------
#if __GENESIS_EDITOR__
	inline void Camera::SetCameraTarget(const CameraTarget bTarget)
	{
		m_CamTarget = bTarget;
	}

	inline Camera::CameraTarget Camera::GetCameraTarget() const
	{
		return m_CamTarget;
	}
#endif
}


#endif //CAMERA_H_
