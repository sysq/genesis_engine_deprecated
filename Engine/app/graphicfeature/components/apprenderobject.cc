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
#include "apprenderobject.h"
#include "appframework/actor.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "graphicsystem/Camera/RenderPipeline/RenderData.h"
#include "graphicsystem/Material/materialinstance.h"
#include "shadercompiler/ShaderMarcro.h"
namespace App
{
	using namespace  Graphic;
	__ImplementClass(AppRenderObject,'AROB',Graphic::RenderObject)

	AppRenderObject::AppRenderObject()
		:mOwner(NULL)
	{

	}
	AppRenderObject::~AppRenderObject()
	{

	}
	void AppRenderObject::AddToCollection(Graphic::RenderDataCollection* collection, const Graphic::RenderConfig* renderConfig)
	{
		//check castshadows
		Graphic::RenderDataCollection::UseFor useFor = collection->GetUseFor();
		bool bCastShadow = this->GetCastShadow();
		bool bLMUsed = this->IsUseLM() && this->IsLMHandleValid();

		if (useFor == Graphic::RenderDataCollection::Shadow && (!bCastShadow || bLMUsed))
		{
			return;
		}

		const Graphic::RenderableResUnitArray& list = mOwner->GetRenderResource();
		Graphic::RenderableResUnitArray::Iterator begin = list.Begin();
		Graphic::RenderableResUnitArray::Iterator end = list.End();
		while(begin != end)
		{
			if (begin->Complete())
			{
				Graphic::Renderable* pRenderable = begin->GetRenderable();
				setShaderMask(pRenderable, renderConfig);
				collection->Push(pRenderable);
			}
			++begin;
		}
	}

	void AppRenderObject::OnWillRenderObject(Graphic::Camera* sender)
	{
		mOwner->OnWillRenderObject(sender);
	}

	void AppRenderObject::SetOwner(RenderComponent* owner)
	{
		mOwner = owner;
		if (mOwner->GetActor())
		{
			SetLayerID(mOwner->GetActor()->GetLayerID());
		}
	}

	void AppRenderObject::setShaderMask(Graphic::Renderable* pRenderable, const Graphic::RenderConfig* renderConfig)
	{

		const Graphic::MaterialInstance* material_instance = pRenderable->GetMaterial();

		//打开forward pass 里用到的 宏
		const Util::Array< GPtr<Graphic::MaterialPass> >& passList = material_instance->GetTech()->GetPassList();
		const GPtr<Graphic::MaterialPass>& pass = passList[Graphic::eForward - 1];
		const GPtr<ShaderProgramCompiler::ShaderMarcro>& pMarcro = pass->GetShaderMarcro();
		bool bTemplate = material_instance->GetTech()->IsTemplateTech();
		pMarcro->Reset();
		pRenderable->ResetShaderMask();
		if (bTemplate)
		{
			if (this->IsUseLM() && this->IsLMHandleValid())
			{
				pMarcro->TurnOn(ShaderProgramCompiler::ShaderMarcro::m_sLightMap);
			}

			if (this->GetReceiveShadow() && renderConfig->bShadowMap)
			{
				pMarcro->TurnOn(ShaderProgramCompiler::ShaderMarcro::m_sShadow);
			}

			if (renderConfig->bLight)
			{
				pMarcro->TurnOnLight();
			}

			if (renderConfig->bFog)
			{
				pMarcro->TurnOn(ShaderProgramCompiler::ShaderMarcro::m_sFog);
			}

			pRenderable->SetShaderMask(Graphic::eForward, pMarcro->GetShaderMask());
		}

	}

	void AppRenderObject::setModelParam(const Graphic::Renderable* renderable, Graphic::RenderPassType passType, const Graphic::Material* material)
	{
		GlobalMaterialParam* pGMP = Graphic::Material::GetGlobalMaterialParams();

		const Graphic::Camera* pCamera = Graphic::GraphicSystem::Instance()->GetRenderingCamera();

		Math::matrix44 mvp = Math::matrix44::multiply(pCamera->GetViewProjTransform(), GetTransform());
		pGMP->SetMatrixParam(eGShaderMatMVP, mvp);
		pGMP->SetMatrixParam(eGShaderMatM,GetTransform());

		{//判断是否需要设置逆矩阵
			//const Material* material = eCustomized == passType ? customizedMaterial : renderable->GetMaterial();
			//int passindex = eCustomized == passType ? eForward - 1 : passType -1;
			int passindex = (eCustomized == passType) ? 0 : passType -1;
			const Util::Array< GPtr<MaterialPass> >& passList = material->GetTech()->GetPassList();
			if(passindex >= passList.Size())			//我们现在不支持深度Pass，按照温洋的建议，先return掉，以后支持后，这块再改掉。
				return;
			const GPtr<MaterialPass>& pass = passList[passindex];
			if ( pass->isGlobalParamUsed( renderable->GetShaderMask(passType), eGShaderMatInverseM )//如果没有使用这几个全局的矩阵参数，就不求逆矩阵了，提高效率
				|| pass->isGlobalParamUsed( renderable->GetShaderMask(passType), eGShaderMatInverseTransposeM ) )
			{
				Math::matrix44 Inverse, InvTranspose;
				Inverse      = Math::matrix44::inverse(GetTransform());
				InvTranspose = Math::matrix44::transpose(Inverse);

				pGMP->SetMatrixParam(eGShaderMatInverseM,Inverse);
				pGMP->SetMatrixParam(eGShaderMatInverseTransposeM,InvTranspose);
			}

			if (pass->isGlobalParamUsed( renderable->GetShaderMask(passType), eGShaderMatMV ))
			{
				Math::matrix44 mv = Math::matrix44::multiply(pCamera->GetViewTransform(), GetTransform());
				pGMP->SetMatrixParam(eGShaderMatMV, mv);
			}
		}
	}
}