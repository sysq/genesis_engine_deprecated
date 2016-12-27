#include "stdneb.h"
#include "PostEffectBlurRenderer.h"
#include "GraphicSystem.h"
namespace Addons
{
	using namespace Graphic;
	__ImplementClass(PostEffectBlurRenderer,'APEB',QuadRenderer)
	PostEffectBlurRenderer::PostEffectBlurRenderer()
	{
		m_bRTBinded = false;
	}

	PostEffectBlurRenderer::~PostEffectBlurRenderer()
	{
		m_blurVertAndHori.Clear();
	}

	void PostEffectBlurRenderer::Setup()
	{
		Ptr<Graphic::RenderToTexture> blurVert = Graphic::RenderToTexture::Create();
		blurVert->Setup(512,512,RenderBase::PixelFormat::A32B32G32R32F, RenderBase::RenderTarget::ClearAll, Math::float4(0.f,0.f,0.f,1.f),  false);
		
		Ptr<Graphic::RenderToTexture> blurHori = Graphic::RenderToTexture::Create();
		blurHori->Setup(512,512,RenderBase::PixelFormat::A32B32G32R32F, RenderBase::RenderTarget::ClearAll, Math::float4(0.f,0.f,0.f,1.f),  false);

		m_blurVertAndHori.Append(blurVert);
		m_blurVertAndHori.Append(blurHori);
	}

	void PostEffectBlurRenderer::RenderObj(const Ptr<Graphic::Renderable>& renderable, bool surface /* = false */,const Ptr<Graphic::Material>& customizedMat /* = 0 */)
	{
		for (SizeT i = 0; i< 3; ++i)
		{
			
			const Ptr<Graphic::QuadRenderable>& quadrenderable = renderable.downcast<Graphic::QuadRenderable>();
			n_assert(quadrenderable.isvalid())

			if (i == 2)
			{
				Graphic::GraphicSystem::Instance()->SetRenderTarget(quadrenderable->GetOwnerCamera()->GetRenderToTexture());
			}
			else
			{
				Graphic::GraphicSystem::Instance()->SetRenderTarget(m_blurVertAndHori[i]);
			}

				GlobalMaterialParam* pGMP = Material::GetGlobalMaterialParams();
			pGMP->SetMatrixParam(eGShaderMatM,matrix44::identity());

			if (surface)
			{
				const MaterialParamList& mpl = quadrenderable->GetMaterial()->GetParamList();

				//init material,hack!
				if (!m_bRTBinded)
				{
					for( SizeT i = 0; i < mpl.Size(); ++i)
					{
						Util::String paramName = mpl[i]->GetName();
						if (paramName == Util::String("_blurVertMap"))
						{
							static_cast<MaterialParamTex2D*>(mpl[i])->SetHandle(m_blurVertAndHori[0]->GetTextureHandle());
							
						}
						if (paramName == Util::String("_blurHoriMap"))
						{
							static_cast<MaterialParamTex2D*>(mpl[i])->SetHandle(m_blurVertAndHori[1]->GetTextureHandle());
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