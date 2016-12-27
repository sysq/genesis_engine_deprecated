#include "stdneb.h"
#include "PostEffectTwirlRenderer.h"
#include "GraphicSystem.h"
namespace Addons
{
	using namespace Graphic;
	__ImplementClass(PostEffectTwirlRenderer,'APET',QuadRenderer)
		PostEffectTwirlRenderer::PostEffectTwirlRenderer() : m_twirlRotation(0.0f)
	{

	}

	PostEffectTwirlRenderer::~PostEffectTwirlRenderer()
	{

	}

	void PostEffectTwirlRenderer::Setup()
	{
		
	}

	void PostEffectTwirlRenderer::RenderObj(const Ptr<Graphic::Renderable>& renderable, bool surface /* = false */,const Ptr<Graphic::Material>& customizedMat /* = 0 */)
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
					if (paramName == Util::String("_twirl_rotation"))
					{
						static_cast<MaterialParamFloat*>(mpl[paramIndex])->SetValue(m_twirlRotation);

					}
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