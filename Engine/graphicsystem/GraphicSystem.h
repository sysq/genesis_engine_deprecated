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
#ifndef GRAPHICSYSTEM_H_
#define GRAPHICSYSTEM_H_
#include "rendersystem/RenderSystem.h"
#include "Camera/Camera.h"
#include "Renderable/Renderable.h"
#include "Renderable/QuadRenderable.h"
#include "Light/Light.h"
#include "base/RenderToTexture.h"
#include "base/MultipleRenderToTexture.h"
#include "core/singleton.h"
#include "messaging/batchmessage.h"
#include "framesync/framesynchandlerthread.h"
#include "rendersystem/RenderSystemThreadHandler.h"
#include "rendersystem/base/RenderDeviceTypes.h"
#include "vis/visentity.h"
#include "vis/visserver.h"
#include "vis/visquery.h"
#include "graphicsystem/base/StreamBufferPool.h"
#include "ViewPortWindow.h"
#include "util/stack.h"
#include "foundation/delegates/delegatetype.h"


#define USE_RENDER_THREAD 0 

namespace Graphic
{
	class RenderObject;

	//typedef Util::Array<Camera*> CameraList;
	//typedef Util::Array< GPtr<Light> > LightList;
	typedef Util::Array<RenderScene*> RenderSceneList;
	typedef Delegates::delegates::CMultiDelegate1<Camera*> EventHandle_BeforeRenderCamera;


#if USE_RENDER_THREAD
	class GraphicSystem : public Interface::InterfaceBase
#else
	class GraphicSystem :  public Core::RefCounted
#endif
	{ 
#if USE_RENDER_THREAD
		__DeclareSubClass(GraphicSystem,InterfaceBase);
#else
		__DeclareClass(GraphicSystem);
#endif

		__DeclareImageSingleton(GraphicSystem);
	public:
		GraphicSystem();
		virtual ~GraphicSystem();

		float GetHorizontalTexelOffset();
		float GetVerticalTexelOffset();
		float GetMinimumDepthInputValue();
		float GetMaximumDepthInputValue();

		void Open(WindHandle hWnd, int width, int height, RenderBase::PixelFormat::Code format);
		void OnUpateFrame();
		void RenderAll();

		void OnDeviceLost();
		bool CheckReset();
		void OnDeviceReset();

		const Camera* GetRenderingCamera();

		GPtr<ViewPortWindow> CreateViewPortWindow( WindHandle hWnd, const RenderBase::DisplayMode& mode);
		void DestroyViewPortWindow(GPtr<ViewPortWindow>& view);
		const GPtr<ViewPortWindow>& FindViewPortWindow( WindHandle hWnd ) const;
		ViewPortWindow* GetMainViewPortWindow() const;
		void Close();

		//////////////////////////////////////////////////////////////////////////
		void OnBeginFrame();
		void OnEndFrame();

		/// ================================ Render Functions =================================================
		void SetViewport(const Camera::Viewport& vp);

		void SetVertexShaderConstantVectorF(const int& reg, const float4* val, const int& vec4count);
		void SetPixelShaderConstantVectorF(const int& reg, const float4* val, const int& vec4count);

		void SetVertexShaderConstantFloat(const int& reg, const float& val);
		void SetPixelShaderConstantFloat(const int& reg, const float& val);

		void SetVertexShaderConstantMatrixF(const int& reg, const matrix44* val, const int& matrixCount);
		void SetPixelShaderConstantMatrixF(const int& reg, const matrix44* val, const int& matrixCount);


		RenderBase::GPUProgramHandle CreateShaderProgram(GPtr<RenderBase::GPUProgram> program);
		void SetShaderProgram(RenderBase::GPUProgramHandle handle);
		void RemoveShaderProgram(RenderBase::GPUProgramHandle handle);

		//RenderBase::PrimitiveGroup* GetPrimitiveGroup(const RenderBase::PrimitiveHandle& handle) const;

		RenderBase::PrimitiveHandle CreatePrimitiveHandle(const RenderBase::VertexBufferData* vbd, const RenderBase::IndexBufferData* ibd = NULL);
		RenderBase::PrimitiveHandle CreatePrimitiveHandle(const VertexBufferData2* vbd2, const IndexBufferData2* ibd2 = NULL);

		void UpdatePrimitiveHandle(RenderBase::PrimitiveHandle& handle, const DynamicBuffer* vertices, const DynamicBuffer* indices = NULL);
		void ChangePrimitiveHandle(RenderBase::PrimitiveHandle& handle, const RenderBase::VertexBufferData* vbd, const RenderBase::IndexBufferData* ibd = NULL);
		void ChangePrimitiveHandle(RenderBase::PrimitiveHandle& handle, const VertexBufferData2* vbd2, const IndexBufferData2* ibd2 = NULL);

		void GetVertexComponents(const RenderBase::PrimitiveHandle& handle, RenderBase::VertexComponents& vcs);
		void GetVertexDesc(const RenderBase::PrimitiveHandle& handle, RenderBase::VERTEX_DESC& desc);
		void GetIndexDesc(const RenderBase::PrimitiveHandle& handle, RenderBase::INDEX_DESC& desc);
		void GetTextureDesc(const RenderBase::TextureHandle& handle, RenderBase::TEXTURE_DESC& desc);

		void DrawPrimitive(RenderBase::PrimitiveHandle handle, SizeT startVertice, SizeT numVertice, SizeT startIndice, SizeT numIndice);
		void DrawPrimitive(RenderBase::PrimitiveHandle handle);
		void RemovePrimitive(const RenderBase::PrimitiveHandle &handle);

		RenderBase::RenderStateDescHandle CreateRenderStateDesc( GPtr<RenderBase::RenderStateDesc> rsObject);
		void SetRenderState( GPtr<RenderBase::RenderStateDesc> rsObject, const uint& shaderMask);

		RenderBase::RenderTargetHandle CreateRenderTarget( GPtr<RenderBase::RenderTarget> rt, RenderBase::TextureHandle& texhandle );
		void SetRenderTarget(RenderBase::RenderTargetHandle handle,SizeT index = 0,uint clearflag = (1<<0));
		void SetRenderTarget(const GPtr<RenderToTexture>& rt,SizeT index = 0,uint clearflag = (1<<0));
		RenderBase::RenderTargetHandle GetRenderTarget(SizeT index = 0);

		void SetRenderTargetClearColor(const GPtr<RenderToTexture>& rt,const Math::float4& clearColor);
		void ReSizeRenderTarget(RenderBase::RenderTargetHandle handle, int width, int height);
		void ReSizeRenderTarget(GPtr<RenderToTexture> rt, int width, int height);
		void RemoveRenderTarget(const RenderBase::RenderTargetHandle &handle);
		void RemoveMultipleRenderTarget(const RenderBase::MultipleRenderTargetHandle& handle);
		void CopyRenderTarget(RenderBase::RenderTargetHandle srcHandle, const float4& srcRect,RenderBase::RenderTargetHandle desHandle, const float4& desRect);

		RenderBase::MultipleRenderTargetHandle CreateMultipleRenderTarget( GPtr<RenderBase::MultipleRenderTarget> mrt, Util::Array< RenderBase::RenderTargetHandle >& handles);
		void SetMultipleRenderTarget(RenderBase::MultipleRenderTargetHandle handle, bool resume = false);
		void SetMultipleRenderTarget(GPtr<MultipleRenderToTexture> mrt, bool resume = false);
		void ReSizeMultipleRenderTarget(RenderBase::MultipleRenderTargetHandle handle, int width, int height);
		void ReSizeMultipleRenderTarget(GPtr<MultipleRenderToTexture> mrt, int width, int height);

		RenderBase::TextureHandle CreateTexture( GPtr<RenderBase::Texture> tex);
		void SetTexture(SizeT index,RenderBase::TextureHandle handle);
		void SetTexture(SizeT index,const GPtr<RenderToTexture>& rtt);
		void UpdateTexture(RenderBase::TextureHandle texHandle, RenderBase::Texture::UpdateFunction texMapInfo, void* tag);
		void UpdateTexture(RenderBase::TextureHandle texHandle, GPtr<RenderBase::Texture> texture);
		void ChangeTexture(RenderBase::TextureHandle texHandle, GPtr<RenderBase::Texture> texture);
		void RemoveTexture(const RenderBase::TextureHandle &handle);

		//////////////////////////////////////////////////////////////////////////
		//Fixed Pipeline Functions
		void SetClipPlane(SizeT index, const Math::float4& cplane);

		Camera* GetCameraByType(CameraOrder co) const;
		GPtr<RenderBase::RenderDisplay> GetRenderDisplay();

		// Apply display parameters
		void ApplyDisplayParams();

		const RenderBase::GraphicCardCapability GetGraphicCardCapability();

		void SetWireFrameMode(bool wireframe = false);

		typedef void (*DebugDrawCallBack)();
		inline void SetFontCallBack(DebugDrawCallBack call_back)
		{
			m_debugDrawCallBack = call_back;
		}



		EventHandle_BeforeRenderCamera      m_BeforeDrawEvent;

		void AddQuadRenderable(QuadRenderable* const& quad);

		void ResetAllQuadRenderable() const;

		void SetDeviceLostCallBack(RenderBase::deviceLostCallBackFunc func);
		
		void PushTargetWindow(ViewPortWindow* target);

		void RenderCamera(Camera* camera);

		void SortRenderScene();

		void AddRenderScene(RenderScene* pScene);

		void RemoveRenderScene(RenderScene* pScene);

		Camera* GetSceneDefaultCamera() const;
		RenderSceneList& GetRenderSceneList();

		const GPtr<RenderToTexture>& GetDefaultWhiteRTT() const;

		//Internal Call!!!!!!!!!!!!!!!!

		void _ViewPortDirty();

		MemoryBlock _GetBlock(SizeT sizeInByte);
		const StreamBufferPool* _GetPool() const;

		void _PushRenderCamera(Camera* const& pCamera);

		void _PopRenderCamera();

		void _SetRenderWindow(ViewPortWindow* target);

		void _SetBackBuffer(ViewPortWindow* target);
	private:

		struct WinCache
		{
			ViewPortWindow* window;
			bool nocleare;
			inline WinCache()
				:window(0)
				,nocleare(true)
			{
			}
		};
		void _SetRenderTarget(const RenderBase::RenderTargetHandle& handle, SizeT index);

		WinCache* _FindInCache(ViewPortWindow* target) const;
		void _SetupDefaultObjects();
		void _DestroyDefaultObjects();
		void _PushTargetWindow(ViewPortWindow* target);
		void _ClearWindowCache();
		void _ApplyBackBuffer();
		void _ApplyWindows();
		void _RenderCamera(Camera* camera);

		void _OnDeviceLost();
		void _OnDeviceReset();
		GPtr<ViewPortWindow> _CreateDefaultViewPortWindow( WindHandle hWnd , const RenderBase::DisplayMode& mode);
		void _RemoveRenderResource(const RenderBase::RenderResourceHandle &handle);


		void OpenRenderSystem(WindHandle hWnd);
		void StartRenderSystem(int width, int height);
		void CloseRenderSystem();

		DebugDrawCallBack m_debugDrawCallBack;
		GPtr<RenderBase::RenderDisplay> m_graphicDisplay;

#if USE_RENDER_THREAD
	public:
		//////////////////////////////////////////////////////////////////////////
		/// add batched message
		void AddBatchMessage(const GPtr<Messaging::Message>& msg);
		/// enter lock-step mode
		void EnterLockStepMode();
		/// leave lock-step mode
		void LeaveLockStepMode();
		/// call when game thread arrives at frame sync point
		void GameThreadWaitForFrameSync();
	private:
		GPtr<RenderBase::RenderSystemThreadHandler> m_renderThreadHandler;
		GPtr<FrameSync::FrameSyncHandlerThread> m_frameSyncHandlerThread;
		GPtr<Messaging::BatchMessage> m_batchMessage;
		Threading::ThreadId m_mainThreadId;
#else
	private:
		GPtr<RenderBase::RenderSystem> mRenderSystem;
#endif
		enum _viewport
		{
			vp_default,
		};

		enum _renderscene
		{
			rs_default,
		};

		GPtr<StreamBufferPool> m_streamBufferPool;

		typedef Util::Array< GPtr<ViewPortWindow> > ViewPorts;
		typedef Util::Array<WinCache> ViewportWindows;
		typedef Util::Array<RenderBase::RenderTargetHandle> RenderTargets;

		ViewPorts							m_ViewPortLists;
	
		Util::Stack<Camera*>				m_renderingCameraStack;
		ViewportWindows						m_windowCache;
		IndexT								m_nViewPort;

		bool								m_ViewPortDirty;

		GPtr<RenderToTextureManager>        m_RenderToTexturesManager;

		Util::Array< QuadRenderable* >		m_AllQuadRenderable;

		RenderSceneList                     m_RenderSceneLists;

		GPtr<RenderToTexture>				m_DefaultWhiteRTT;

		RenderTargets						m_RenderTargetCache;
	};


	inline GPtr<RenderBase::RenderDisplay> GraphicSystem::GetRenderDisplay()
	{
		return m_graphicDisplay;
	}

	inline const RenderBase::GraphicCardCapability GraphicSystem::GetGraphicCardCapability()
	{
		return mRenderSystem->GetGraphicCardCapability();
	}

	inline void GraphicSystem::SetWireFrameMode(bool wireframe /* = false */)
	{
		mRenderSystem->SetWireFrameMode(wireframe);
	}

	inline float GraphicSystem::GetHorizontalTexelOffset()
	{
		return mRenderSystem->GetHorizontalTexelOffset();
	}

	inline float GraphicSystem::GetVerticalTexelOffset()
	{
		return mRenderSystem->GetVerticalTexelOffset();
	}

	inline float GraphicSystem::GetMinimumDepthInputValue()
	{
		return mRenderSystem->GetMinimumDepthInputValue();
	}

	inline float GraphicSystem::GetMaximumDepthInputValue()
	{
		return mRenderSystem->GetMinimumDepthInputValue();
	}

	inline void GraphicSystem::_ViewPortDirty()
	{
		m_ViewPortDirty = true;
	}

	inline const StreamBufferPool* GraphicSystem::_GetPool() const
	{
		return m_streamBufferPool.get();
	}

	inline void GraphicSystem::SetDeviceLostCallBack(RenderBase::deviceLostCallBackFunc func)
	{
		mRenderSystem->SetDeviceLostCallBack(func);
	}

	inline const GPtr<RenderToTexture>& GraphicSystem::GetDefaultWhiteRTT() const
	{
		return m_DefaultWhiteRTT;
	}

	//#if RENDERDEVICE_OPENGLES
	//	inline void GraphicSystem::SetDeviceLost()
	//	{
	//		mRenderSystem->SetDeviceLost();
	//	}
	//
	//	inline bool GraphicSystem::IsDeviceLost() const
	//	{
	//		return mRenderSystem->IsDeviceLost();
	//	}
	//#endif

	inline void GraphicSystem::AddQuadRenderable(QuadRenderable* const& quad)
	{
		IndexT res = m_AllQuadRenderable.FindIndex(quad);

		if (res == InvalidIndex)
		{
			m_AllQuadRenderable.Append(quad);
		}
	}

	inline void GraphicSystem::_PushRenderCamera(Camera* const& pCamera)
	{
		m_renderingCameraStack.Push(pCamera);
	}

	inline void GraphicSystem::_PopRenderCamera()
	{
		m_renderingCameraStack.Pop();
	}

	inline Camera* GraphicSystem::GetSceneDefaultCamera() const
	{
		if(m_RenderSceneLists.Size())
		{
			return m_RenderSceneLists[rs_default]->GetDefaultCamera();
		}

		return NULL;
	}

	inline RenderSceneList& GraphicSystem::GetRenderSceneList()
	{
		return m_RenderSceneLists;
	}
}
#endif// GRAPHICSYSTEM_H_
