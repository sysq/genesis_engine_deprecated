#include "stdneb.h"
#include "PostEffectOldTVRenderer.h"
#include "GraphicSystem.h"
namespace Addons
{
	using namespace Graphic;
	__ImplementClass(PostEffectOldTVRenderer,'APOT',QuadRenderer)
		PostEffectOldTVRenderer::PostEffectOldTVRenderer() : m_timeX(0.0880f),m_bRTBinded(false)
	{

	}

	PostEffectOldTVRenderer::~PostEffectOldTVRenderer()
	{
		GraphicSystem::Instance()->RemoveTexture(m_noiseVolumeMapHandle);
		GraphicSystem::Instance()->RemoveTexture(m_random3DMapHandle);
	}

	void PostEffectOldTVRenderer::Setup()
	{
		Ptr<RenderBase::Texture> tex = RenderBase::Texture::Create();
		tex->Setup();
		tex->SetTexturePath("sys:NoiseVolume.dds");
		Ptr<IO::Stream> texstream = IO::IoServer::Instance()->CreateStream(IO::URI(tex->GetTexturePath().Value()));
		tex->SetStream(texstream);
		m_noiseVolumeMapHandle = GraphicSystem::Instance()->CreateTexture(tex);

		tex = RenderBase::Texture::Create();
		tex->Setup();
		tex->SetTexturePath("sys:Random3D.dds");
		texstream = IO::IoServer::Instance()->CreateStream(IO::URI(tex->GetTexturePath().Value()));
		tex->SetStream(texstream);
		m_random3DMapHandle = GraphicSystem::Instance()->CreateTexture(tex);
		
	}

	void PostEffectOldTVRenderer::RenderObj(const Ptr<Graphic::Renderable>& renderable, SurfacePassType surface /* = false */,const Ptr<Graphic::Material>& customizedMat /* = 0 */)
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
					if (paramName == Util::String("time_0_X"))
					{
						static_cast<MaterialParamFloat*>(mpl[paramIndex])->SetValue(m_timeX);
						continue;
					}

					if (paramName == Util::String("sin_time_0_X"))
					{
						static_cast<MaterialParamFloat*>(mpl[paramIndex])->SetValue(m_sinTimeX);
						continue;
					}
				}
				//init material,hack!
				if (!m_bRTBinded)
				{
					if (!m_noiseVolumeMapHandle.IsValid())
					{
						return;
					}
					for( SizeT i = 0; i < mpl.Size(); ++i)
					{
						Util::String paramName = mpl[i]->GetName();
						if (paramName == Util::String("_noiseMap"))
						{
							static_cast<MaterialParamTex3D*>(mpl[i])->SetHandle(m_noiseVolumeMapHandle);
							continue;
						}

						if (paramName == Util::String("_randMap"))
						{
							static_cast<MaterialParamTex3D*>(mpl[i])->SetHandle(m_random3DMapHandle);
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