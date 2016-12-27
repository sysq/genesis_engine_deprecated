#include "stdneb.h"
#include "PostEffectShowerDoorRenderer.h"
#include "GraphicSystem.h"
namespace Addons
{
	using namespace Graphic;
	__ImplementClass(PostEffectShowerDoorRenderer,'APSD',QuadRenderer)
		PostEffectShowerDoorRenderer::PostEffectShowerDoorRenderer() : m_bumpAmount(0.0880f),m_bRTBinded(false)
	{

	}

	PostEffectShowerDoorRenderer::~PostEffectShowerDoorRenderer()
	{
		GraphicSystem::Instance()->RemoveTexture(m_bumpMapHandle);
	}

	void PostEffectShowerDoorRenderer::Setup()
	{
		Ptr<RenderBase::Texture> tex = RenderBase::Texture::Create();
		tex->Setup();
		tex->SetTexturePath("sys:FieldstoneBumpDOT3.dds");
		Ptr<IO::Stream> texstream = IO::IoServer::Instance()->CreateStream(IO::URI(tex->GetTexturePath().Value()));
		tex->SetStream(texstream);
		m_bumpMapHandle = GraphicSystem::Instance()->CreateTexture(tex);
		
	}

	void PostEffectShowerDoorRenderer::RenderObj(const Ptr<Graphic::Renderable>& renderable, SurfacePassType surface /* = false */,const Ptr<Graphic::Material>& customizedMat /* = 0 */)
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
				for( SizeT paramIndex = 0; paramIndex < mpl.Size(); ++paramIndex)
				{
					Util::String paramName = mpl[paramIndex]->GetName();
					if (paramName == Util::String("_bump_amount"))
					{
						static_cast<MaterialParamFloat*>(mpl[paramIndex])->SetValue(m_bumpAmount);

					}
				}
				//init material,hack!
				if (!m_bRTBinded)
				{
					if (!m_bumpMapHandle.IsValid())
					{
						return;
					}
					for( SizeT i = 0; i < mpl.Size(); ++i)
					{
						Util::String paramName = mpl[i]->GetName();
						if (paramName == Util::String("_bumpMap"))
						{
							static_cast<MaterialParamTex2D*>(mpl[i])->SetHandle(m_bumpMapHandle);

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