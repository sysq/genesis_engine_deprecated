#include "stdneb.h"
#include "PostEffectGoldenShineRenderer.h"
#include "GraphicSystem.h"
namespace Addons
{
	using namespace Graphic;
	__ImplementClass(PostEffectGoldenShineRenderer,'APGS',QuadRenderer)
		PostEffectGoldenShineRenderer::PostEffectGoldenShineRenderer()
	{
		m_bRTBinded = false;
	}

	PostEffectGoldenShineRenderer::~PostEffectGoldenShineRenderer()
	{

	}

	void PostEffectGoldenShineRenderer::Setup()
	{
		
	}

	void PostEffectGoldenShineRenderer::RenderObj(const Ptr<Graphic::Renderable>& renderable, SurfacePassType surface /* = false */,const Ptr<Graphic::Material>& customizedMat /* = 0 */)
	{
		if (customizedMat.isvalid())
		{
			return;
		}
		int numDraCall = 2;
		for (SizeT i = 0; i< numDraCall; ++i)
		{
			
			const Ptr<Graphic::QuadRenderable>& quadrenderable = renderable.downcast<Graphic::QuadRenderable>();
			n_assert(quadrenderable.isvalid())

				GlobalMaterialParam* pGMP = Material::GetGlobalMaterialParams();

			if (i == numDraCall - 1)
			{
				Graphic::GraphicSystem::Instance()->SetRenderTarget(quadrenderable->GetOwnerCamera()->GetRenderToTexture(),0,RenderBase::RenderTarget::ClearAll);
			}
			else
			{
				Graphic::GraphicSystem::Instance()->SetRenderTarget(quadrenderable->GetOwnerCamera()->GetSwapTexture(),0,RenderBase::RenderTarget::ClearAll);
			}

			pGMP->SetMatrixParam(eGShaderMatM,matrix44::identity());

			if (surface)
			{

				const MaterialParamList& mpl = quadrenderable->GetMaterial()->GetParamList();

				//init material,hack!
				if (!m_bRTBinded)
				{
					Ptr<Graphic::Camera> glowCam = GraphicSystem::Instance()->GetCameraByType(Graphic::Camera::Glow);
					if (!glowCam.isvalid() || !glowCam->GetRenderToTexture().isvalid())
					{
						return;
					}
					for( SizeT i = 0; i < mpl.Size(); ++i)
					{
						Util::String paramName = mpl[i]->GetName();
						if (paramName == Util::String("_glowMap"))
						{
							static_cast<MaterialParamTex2D*>(mpl[i])->SetHandle(glowCam->GetRenderToTexture()->GetTextureHandle());
						}
					}

					m_bRTBinded = true;
				}
				//hack end


				const Util::Array<Ptr<MaterialPass>>& passList = quadrenderable->GetMaterial()->GetTech()->GetPassList();
			
				const Ptr<MaterialPass>& pass = passList[i];
				SetMaterialParams(mpl,pass);
				const Ptr<RenderBase::RenderStateObject>& rso = pass->GetRenderStateObject();
				if (rso.isvalid())
				{
					GraphicSystem::Instance()->SetRenderState(rso);
				}
				else
				{
					n_error("you must set renderstate in quad!");
				}
				GraphicSystem::Instance()->SetShaderProgram(pass->GetGPUProgramHandle());
			}

			GraphicSystem::Instance()->DrawPrimitive(quadrenderable->GetQuadHandle());
		}
		
	}
}