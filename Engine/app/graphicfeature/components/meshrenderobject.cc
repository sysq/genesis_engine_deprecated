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
#include "meshrenderobject.h"
#include "graphicsystem/GraphicSystem.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "graphicsystem/Camera/RenderPipeline/RenderData.h"
#include "appframework/actor.h"


namespace App
{
	using namespace  Graphic;
	__ImplementClass(MeshRenderObject,'MROB',AppRenderObject)

	MeshRenderObject::MeshRenderObject()
	{

	}
	MeshRenderObject::~MeshRenderObject()
	{

	}

	void MeshRenderObject::setLightParam()
	{
		GlobalMaterialParam* pGMP = Graphic::Material::GetGlobalMaterialParams();

		if ( IsUseLM() && IsLMHandleValid() )
		{
			// lightmap support
			const Math::float4& lmTexParam = getOwner()->GetLMTexParam();
			RenderBase::TextureHandle lmHandle = getOwner()->GetLMTextureHandle( 0 );
			pGMP->SetTextureParam(eGShaderTexLightmap, lmHandle);

			int lmMode = (int)getOwner()->GetLightmapMode();
			if (lmMode > 0)
			{
				// lightmap support
				RenderBase::TextureHandle lightmapHandle = getOwner()->GetLMTextureHandle( 1 );
				pGMP->SetTextureParam(eGShaderTexLightmap2, lightmapHandle);
			}

			// set lightmap params
			pGMP->SetVectorParam_X(eGShaderMatLMEnable, (float)(lmMode + 1.0f));
//#if __OSX__
//			//hack for ios.
//			//for android or pc: o.LightmapUV.xy = v.texcoord1.xy * g_LMTexParam.xy + g_LMTexParam.zw;// for android.
//			//for ios o.LightmapUV.x the same as android and pc.
//			//but o.LightmapUV.y not the same :  o.LightmapUV.y = 1 - (v.texcoord1.y * g_LMTexParam.y + g_LMTexParam.w);
//			//so........
//			pGMP->SetVectorParam(eGShaderMatLMTexParam, float4(lmTexParam.x(), -lmTexParam.y(), lmTexParam.z(), -lmTexParam.w() + 1.0f));
//#else
			pGMP->SetVectorParam(eGShaderMatLMTexParam, lmTexParam);
//#endif

		}
		else
		{
			pGMP->SetVectorParam(eGShaderMatLMEnable, float4(0.0f,0.0f,0.0f,0.0f));
			pGMP->SetVectorParam(eGShaderMatLMTexParam, float4(1.0f,1.0f,0.0f,0.0f));
		}
	}

	void MeshRenderObject::resetLightmap()
	{
		// reset lightmap params
		Graphic::Material::GetGlobalMaterialParams()->SetVectorParam(eGShaderMatLMEnable, float4(0.0f,0.0f,0.0f,0.0f));
	}

	void MeshRenderObject::Render(const Graphic::Renderable* renderable, Graphic::RenderPassType passType, const Graphic::Material* customizedMaterial)
	{
		n_assert(NULL != renderable);
		n_assert(mOwner);

		const RenderBase::PrimitiveHandle& priHandle = getOwner()->GetPrimitiveHandle();
		if (priHandle.IsValid())//( (customizedMat.isvalid() || mat.isvalid()) && priHandle.IsValid() )
		{		
			const Graphic::Material* material = (NULL == customizedMaterial) ? renderable->GetMaterial() : customizedMaterial;
			const RenderableType* rd = renderable->cast_fast<RenderableType>();
			setModelParam(renderable, passType, material);
			setLightParam();
			//render passType
			if (passType && material->GetTech()->IsTemplateTech())
			{
				rd->RenderForward(passType, priHandle, customizedMaterial);
			}
			else
			{
				rd->RenderForward(eForward, priHandle, customizedMaterial);
			}	
			resetLightmap();
		}
	}

	const Util::StringAtom& MeshRenderObject::GetMeshName() const
	{
		return getOwner()->GetMeshID();
	}

	inline bool MeshRenderObject::IsUseLM() const
	{
		return getOwner()->IsUseLM();
	}

	inline bool MeshRenderObject::IsLMHandleValid() const
	{
		return getOwner()->IsLMTextureHandleValid();
	}
}