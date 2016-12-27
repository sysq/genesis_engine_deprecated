#include "stdneb.h"
#include "PostEffectSimpleHDRRenderer.h"
#include "GraphicSystem.h"
namespace Addons
{
	using namespace Graphic;
	__ImplementClass(PostEffectSimpleHDRRenderer,'APEH',QuadRenderer)
		PostEffectSimpleHDRRenderer::PostEffectSimpleHDRRenderer()
	{
		m_bRTBinded = false;
	}

	PostEffectSimpleHDRRenderer::~PostEffectSimpleHDRRenderer()
	{
		m_blurVertAndHori.Clear();
	}

	void PostEffectSimpleHDRRenderer::Setup()
	{
		Ptr<Graphic::RenderToTexture> blurVert = Graphic::RenderToTexture::Create();
		blurVert->Setup(512,512,RenderBase::PixelFormat::A32B32G32R32F, RenderBase::RenderTarget::ClearAll, Math::float4(0.f,0.f,0.f,1.f),  false);

		Ptr<Graphic::RenderToTexture> blurHori = Graphic::RenderToTexture::Create();
		blurHori->Setup(512,512,RenderBase::PixelFormat::A32B32G32R32F, RenderBase::RenderTarget::ClearAll, Math::float4(0.f,0.f,0.f,1.f),  false);

		const RenderBase::DisplayMode& dm = RenderBase::RenderDisplay::Instance()->GetDisplayMode();
		m_sceneColor = Graphic::RenderToTexture::Create();
		m_sceneColor->Setup(dm.GetWidth(),dm.GetHeight(),RenderBase::PixelFormat::A32B32G32R32F, RenderBase::RenderTarget::ClearAll, Math::float4(0.f,0.f,0.f,1.f), false,1.f);

		m_blurVertAndHori.Append(blurVert);
		m_blurVertAndHori.Append(blurHori);
	}

	void PostEffectSimpleHDRRenderer::RenderObj(const Ptr<Graphic::Renderable>& renderable, bool surface /* = false */,const Ptr<Graphic::Material>& customizedMat /* = 0 */)
	{
		for (SizeT i = 0; i< 7; ++i)
		{

			const Ptr<Graphic::QuadRenderable>& quadrenderable = renderable.downcast<Graphic::QuadRenderable>();
			n_assert(quadrenderable.isvalid())

				GlobalMaterialParam* pGMP = Material::GetGlobalMaterialParams();
			pGMP->SetMatrixParam(eGShaderMatM,matrix44::identity());

			if (i == 6)
			{
				Graphic::GraphicSystem::Instance()->SetRenderTarget(quadrenderable->GetOwnerCamera()->GetRenderToTexture());
			}
			else if (i == 5)
			{
				Graphic::GraphicSystem::Instance()->SetRenderTarget(m_sceneColor);
			}
			else
			{
				Graphic::GraphicSystem::Instance()->SetRenderTarget(m_blurVertAndHori[i%2]);
			}

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
							continue;
						}
						if (paramName == Util::String("_blurHoriMap"))
						{
							static_cast<MaterialParamTex2D*>(mpl[i])->SetHandle(m_blurVertAndHori[1]->GetTextureHandle());
							continue;
						}
						if (paramName == Util::String("_sceneColorMap"))
						{
							static_cast<MaterialParamTex2D*>(mpl[i])->SetHandle(m_sceneColor->GetTextureHandle());
							continue;
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