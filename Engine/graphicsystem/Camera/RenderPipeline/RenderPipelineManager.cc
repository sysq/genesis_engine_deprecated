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
#include "RenderPipelineManager.h"
#include "GraphicSystem.h"
#include "graphicsystem/Camera/RenderPipeline/DeferredLightingRenderPipeline.h"
#include "graphicsystem/Renderable/RenderObject.h"

namespace Graphic
{

	__ImplementClass(RenderPipelineManager,'RPMA',Core::RefCounted)

		RenderPipelineManager::RenderPipelineManager()
	{
		m_pipelineContext.m_renderPipelineManager = this;
		m_forwardPipeline = ForwardShadingRenderPipeline::Create();
	}

	RenderPipelineManager::~RenderPipelineManager()
	{
		//empty
	}

	void RenderPipelineManager::PreRender(Camera* camera)
	{
		// reset
		m_pipelineContext.m_renderConfig = RenderConfig();
		m_pipelineContext.m_camera = NULL;
		m_pipelineContext.m_targetWindows = NULL;
		m_pipelineContext.m_targetSuite = NULL;
		m_pipelineContext.m_callBacks.Clear(false);
		m_pipelineContext.m_visibleNodes.Clear();
		m_pipelineContext.m_activeLights.Reset();
		m_pipelineContext.m_renderDatas.Reset(eCO_Main == camera->GetCameraOrder());

		// init

		m_pipelineContext.m_camera = camera;
		m_pipelineContext.m_targetSuite = camera->GetRenderTargetSuite();
		const Camera::Viewport& vp = camera->GetViewport();

		if (m_pipelineContext.m_targetSuite)
		{
			m_pipelineContext.m_targetWindows = NULL;
			const GPtr<RenderToTexture>& rtt = m_pipelineContext.m_targetSuite->GetRenderToTexture();
			if (vp.width != rtt->GetWidth() || vp.height != rtt->GetHeight())
			{
				camera->OnResizeWindow(rtt->GetWidth(), rtt->GetHeight());
			}
		}
		else
		{
			m_pipelineContext.m_targetWindows = camera->GetTargetWindow();
			if (m_pipelineContext.m_targetWindows == NULL)
			{
				m_pipelineContext.m_targetWindows = GraphicSystem::Instance()->GetMainViewPortWindow();
			}
			m_pipelineContext.m_targetSuite = m_pipelineContext.m_targetWindows->_GetRTTSuite();

			const RenderBase::DisplayMode& dm = m_pipelineContext.m_targetWindows->GetDisplayMode();
			if (vp.width != dm.GetWidth() || vp.height != dm.GetHeight())
			{
				camera->OnResizeWindow(dm.GetWidth(), dm.GetHeight());
			}
		}

	}

	void RenderPipelineManager::AfterRender()
	{
		m_pipelineContext.m_targetSuite = NULL;
		m_pipelineContext.m_camera = NULL;
		m_pipelineContext.m_targetWindows = NULL;
	}

	void RenderPipelineManager::Open()
	{

	}

	void RenderPipelineManager::Close()
	{
		m_pipelineContext.m_camera = 0;
		m_pipelineContext.m_renderPipelineManager = 0;
		m_forwardPipeline = 0;
		m_customPipeline = 0;
	}
	void RenderPipelineManager::SetupDeferred()
	{
		m_deferredPipeline = DeferredLightingRenderPipeline::Create();
	}

	void RenderPipelineManager::SetCustomPipeline(const GPtr<RenderPipeline>& customPipeline)
	{
		this->m_customPipeline = customPipeline;
	}

	void RenderPipelineManager::OnRenderPipeline(Camera* camera)
	{
		GPtr<RenderPipelineManager>& renderpipemanager = camera->GetRenderPipelineManager();
		renderpipemanager->PreRender(camera);

		renderpipemanager->m_pipelineContext.m_renderDatas.SetUseFor(RenderDataCollection::Normal);
		AssignVisibleNodes(renderpipemanager->m_pipelineContext);

		if(camera->GetUseCallBack())
		{
			RenderCallBacks::Iterator begin = renderpipemanager->m_pipelineContext.m_callBacks.Begin();
			RenderCallBacks::Iterator end = renderpipemanager->m_pipelineContext.m_callBacks.End();
			while(begin != end)
			{
				(*begin)->OnWillRenderObject(camera);
				++begin;
			}	
		}

		RenderScene* camera_scene = camera->GetRenderScene();
		if (camera_scene->GetEnvironment() && camera_scene->GetEnvironment()->fogColor.w() > 0.0f)
		{
			renderpipemanager->m_pipelineContext.m_renderConfig.bFog = true;
		}
		else
		{
			renderpipemanager->m_pipelineContext.m_renderConfig.bFog = false;
		}

		AssignEffectiveLight(renderpipemanager->m_pipelineContext);
		AssignRenderDatas(renderpipemanager->m_pipelineContext);
		renderPipeline(renderpipemanager);

		renderpipemanager->AfterRender();
	}

	void RenderPipelineManager::renderPipeline(GPtr<RenderPipelineManager>& renderpipemanager)
	{
		Camera::RenderMode rm = renderpipemanager->m_pipelineContext.m_camera->GetRenderMode();
		switch (rm)
		{
		case Camera::ForwardMode:
			{
				renderpipemanager->m_forwardPipeline->Render(renderpipemanager->m_pipelineContext);
			}
			break;
		case Camera::DeferredMode:
			{
				renderpipemanager->m_deferredPipeline->Render(renderpipemanager->m_pipelineContext);
			}
			break;
		case Camera::CustomedMode:
			{
				// custom pipeline rendering
				n_assert(renderpipemanager->m_customPipeline.isvalid());
				renderpipemanager->m_customPipeline->Render(renderpipemanager->m_pipelineContext);
			}
			break;
		default:
			{
				n_error("error Camera::RenderMode !");
			}
			break;
		}
	}


	void RenderPipelineManager::AssignVisibleNodes(PipelineParamters& params)
	{
		Camera* camera = params.m_camera;

		Math::float4 camPos = camera->GetTransform().get_position();
		Math::float4 camDir = -camera->GetTransform().get_zaxis();

		if (camera->IsPick(Camera::PickCullObjects))
		{
			bool callbackable = camera->GetUseCallBack();
			GPtr<Vis::VisQuery> pVisQuery = camera->Cull();
			n_assert( pVisQuery.isvalid() );
			const Util::Array<GPtr<Vis::VisEntity> >& viEnityList = pVisQuery->GetQueryResult();
			for (IndexT i = 0; i < viEnityList.Size(); ++i)
			{
				const GPtr<Vis::VisEntity>& visEnt = viEnityList[i];
				n_assert( visEnt.isvalid() );
				Graphic::GraphicObject* obj = visEnt->GetUserData();
				n_assert( obj && obj->GetRtti()->IsDerivedFrom( RenderObject::RTTI ) );

				RenderObject* renderObj = static_cast<RenderObject*>(obj);

				uint mark = renderObj->GetRenderCullMark() & (uint)camera->GetCullMask();

				if (mark)
				{

#if __GENESIS_EDITOR__
					if (renderObj->IsEditorVisible() || camera->GetCameraTarget() == Camera::GAME)
					{
						if (callbackable && renderObj->GetNeedCullCallBack())
						{
							params.m_callBacks.Append(renderObj);
						}
						Math::float4 renderablePos = renderObj->GetTransform().get_position();
						Math::float4 camToObj = renderablePos - camPos;
						VisibleNode& node = params.m_visibleNodes.PushBack();
						node.object = renderObj;
						node.distance = Math::float4::dot3(camDir,camToObj);
					}
#else
					if (callbackable && renderObj->GetNeedCullCallBack())
					{
						params.m_callBacks.Append(renderObj);
					}
					Math::float4 renderablePos = renderObj->GetTransform().get_position();
					Math::float4 camToObj = renderablePos - camPos;
					VisibleNode& node = params.m_visibleNodes.PushBack();
					node.object = renderObj;
					node.distance = Math::float4::dot3(camDir,camToObj);
#endif

				}
			}
		}

		if (camera->IsPick(Camera::PickNoCullObjects))
		{
			const Util::Array<RenderObject*>& notCullList = camera->GetRenderScene()->GetNotCullRenderObjects();
			Util::Array<RenderObject*>::Iterator it = notCullList.Begin();
			Util::Array<RenderObject*>::Iterator end = notCullList.End();
			while(it != end)
			{
				uint mark = (*it)->GetRenderCullMark() & (uint)camera->GetCullMask();
				if (mark)
				{
#if __GENESIS_EDITOR__
					if ( (*it)->IsEditorVisible() || camera->GetCameraTarget() == Camera::GAME)
					{
						VisibleNode& node = params.m_visibleNodes.PushBack();
						node.object = *it;
						node.distance = 0;
					}
#else
					VisibleNode& node = params.m_visibleNodes.PushBack();
					node.object = *it;
					node.distance = 0;
#endif
					
				}
				++it;
			}
		}

		if (camera->IsPick(Camera::PickSelfObjects))
		{
			const Util::Array<RenderObject*>& notCullList = camera->GetNotCullRenderObjects();
			Util::Array<RenderObject*>::Iterator it = notCullList.Begin();
			Util::Array<RenderObject*>::Iterator end = notCullList.End();
			while(it != end)
			{
				uint mark = (*it)->GetRenderCullMark() & (uint)camera->GetCullMask();
				if (mark)
				{
					VisibleNode& node = params.m_visibleNodes.PushBack();
					node.object = *it;
					node.distance = 0;
				}
				++it;
			}
		}

	}

	void RenderPipelineManager::AssignRenderDatas(PipelineParamters& params)
	{
		RenderDataManager* renderDatas = &params.m_renderDatas;
		const RenderConfig* renderConfig = &params.m_renderConfig;
		VisibleNodeCollection::Iterator it = params.m_visibleNodes.Begin();
		VisibleNodeCollection::Iterator end = params.m_visibleNodes.End();
		while(it != end)
		{
			renderDatas->SetCurrentNode(it);
			it->object->AddToCollection(renderDatas, renderConfig);
			++it;
		}
		renderDatas->Sort();
	}

	void RenderPipelineManager::AssignEffectiveLight(PipelineParamters& params)
	{
		params.m_activeLights.CameraCull(params.m_camera);
		params.m_renderConfig.bLight = (params.m_activeLights.GetActiveLightCount() > 0);

		GlobalMaterialParam* pGMP = Material::GetGlobalMaterialParams();

		if (params.m_camera->IsRenderLightLitMap() && params.m_camera->IsRenderDepthMap())
		{
			const ActiveLightInfo* sun_light = params.m_activeLights.FindSunLight();
			params.m_renderConfig.bShadowMap = sun_light && sun_light->light->IsEnableShadow();
			if (params.m_renderConfig.bShadowMap)
			{
				renderShadowMap(sun_light);
				return;
			}
		}
		else
		{
			params.m_renderConfig.bShadowMap = false;
		}

		pGMP->SetVectorParam(eGShaderVecShadowMapSize, Math::float4(0.0f, 0.0f, 0.0f, 0.0f));
	}
	void RenderPipelineManager::renderShadowMap(const ActiveLightInfo* aLight)
	{
		RenderLayer cullmark = GraphicSystem::Instance()->GetRenderingCamera()->GetCullMask();
		aLight->light->RenderShadowMapBegin();
		const Light::ShadowMapCameraList& list = aLight->light->GetShadowMapCameraList();
		for (int i = 0; i < list.Size(); ++i)
		{
			GPtr<Camera> camera = list[i];
			GraphicSystem::Instance()->_PushRenderCamera(camera.get());
			camera->SetCullMask(cullmark);
			GPtr<RenderPipelineManager>& mgr = camera->GetRenderPipelineManager();
			mgr->PreRender(camera);

			mgr->m_pipelineContext.m_camera = camera.get();
			mgr->m_pipelineContext.m_renderDatas.SetUseFor(RenderDataCollection::Shadow);
			mgr->m_pipelineContext.m_renderConfig.bFog = false;
			AssignVisibleNodes(mgr->m_pipelineContext);
			AssignRenderDatas(mgr->m_pipelineContext);
			renderPipeline(mgr);
			GlobalMaterialParam* pGMP = Material::GetGlobalMaterialParams();
			n_assert(NULL != pGMP);
			const Math::float4& shadowMapParam = pGMP->GetVectorParam(Graphic::eGShaderVecShadowMapSize);
			Math::float4 srcRect(0, 0, shadowMapParam.x(), shadowMapParam.y());
			Math::float4 desRect = srcRect;
			desRect.x() = shadowMapParam.x() * i;
			desRect.z() = desRect.x() + shadowMapParam.x();
			GraphicSystem::Instance()->CopyRenderTarget(camera->GetRenderTargetSuite()->GetRenderToTexture()->GetTargetHandle(), srcRect,
				aLight->light->GetLightShadowMap()->GetTargetHandle(), desRect);
			
			mgr->AfterRender();
			GraphicSystem::Instance()->_PopRenderCamera();
		}
	}
}
