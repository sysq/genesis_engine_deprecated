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
#include "terrainfeature/components/TerrainRenderObject.h"
#include "terrainfeature/components/TerrainRenderComponent.h"
#include "terrainfeature/components/TerrainNode.h"
#include "graphicsystem/Camera/RenderPipeline/RenderData.h"
#include "graphicsystem/GraphicSystem.h"


namespace App
{
	using namespace Graphic;
	using namespace RenderBase;
	static bool g_bUseBaseMap(1);
	const IndexT sBrushIdxBase = 2;
	const IndexT sColorMapMaterialIdx = 1;
	__ImplementClass(TerrainRenderObject,'TROB',AppRenderObject)
		TerrainRenderObject::TerrainRenderObject()
		: m_TerrainNode(NULL)
	{

	}

	TerrainRenderObject::~TerrainRenderObject()
	{

	}

	void TerrainRenderObject::Set(Owner* comObject, TerrainNode* node,  const Math::matrix44& world)
	{
		n_assert(comObject&&node);
		mOwner = comObject;
		m_TerrainNode = node;
		SetTransform(world);
		SetBoundingBox( node->GetLocalBoundingBox() );
		SetLayerID(mOwner->GetActor()->GetLayerID());
#ifdef __GENESIS_EDITOR__
		SetEditorVisible(mOwner->IsEditorVisible());
#endif
	}

	void TerrainRenderObject::AddToCollection(Graphic::RenderDataCollection* collection, const Graphic::RenderConfig* renderConfig)
	{
		const Graphic::RenderableResUnitArray& res = GetOwner<Owner>()->GetRenderResource();
		int count = res.Size();

		if (Graphic::RenderDataCollection::Normal != collection->GetUseFor())
		{
			Graphic::Renderable* pRenderable = res[0].GetRenderable();
			Super::setShaderMask(pRenderable, renderConfig);
			collection->Push(pRenderable);
		}
		else if (g_bUseBaseMap && m_TerrainNode->IsUseBaseMap())
		{
			Graphic::Renderable* pRenderable = res[0].GetRenderable();
			Super::setShaderMask(pRenderable, renderConfig);
			collection->Push(pRenderable);
			//colormap Material
			if ( sColorMapMaterialIdx <= count )
			{
				pRenderable = res[sColorMapMaterialIdx].GetRenderable();
				Super::setShaderMask(pRenderable, renderConfig);
				collection->Push(pRenderable);
			}			

		}
		else
		{
			//other texture Material			
			for (int index = sBrushIdxBase; index< count; ++index)
			{
				Graphic::Renderable* pRenderable = res[index].GetRenderable();
				Super::setShaderMask(pRenderable, renderConfig);
				collection->Push(res[index].GetRenderable());
			}
			//colormap Material
			if ( sColorMapMaterialIdx <= count )
			{
				Graphic::Renderable* pRenderable = res[sColorMapMaterialIdx].GetRenderable();
				Super::setShaderMask(pRenderable, renderConfig);
				collection->Push(res[sColorMapMaterialIdx].GetRenderable());

				// make sure colormap is after other material
				IndexT lastIdx = count - 1;
				if ( lastIdx >= 0 )
				{
					MaterialInstance* pMatIns = res[lastIdx].GetRenderable()->GetMaterial();
					if ( pMatIns )
					{
						MaterialInstance* pMatInsColor = res[sColorMapMaterialIdx].GetRenderable()->GetMaterial();
						pMatInsColor->SetSort(Graphic::MatSort_TerrainEnd);

					}

				}
			}
		}
	}
	void TerrainRenderObject::Render(const Graphic::Renderable* renderable, Graphic::RenderPassType passType, const Graphic::Material* customizedMaterial)
	{
		const Graphic::Material* material = (NULL == customizedMaterial) ? renderable->GetMaterial() : customizedMaterial;
		const RenderableType* rd = renderable->cast_fast<RenderableType>();
		//const RenderBase::PrimitiveHandle& priHandle = getOwner()->GetPrimitive(m_patchIndex);
		const RenderBase::PrimitiveHandle& priHandle = m_TerrainNode->GetPrimitiveHandle();

		if (m_TerrainNode->GetRenderEnable() && priHandle.IsValid())
		{
			GlobalMaterialParam* pGMP = Material::GetGlobalMaterialParams();

			int passindex = (eCustomized == passType) ? 0 : passType -1;
			const Util::Array< GPtr<MaterialPass> >& passList = material->GetTech()->GetPassList();
			if(passindex >= passList.Size())			
				return;
			const GPtr<MaterialPass>& pass = passList[passindex];

			const Graphic::Camera* pCamera = Graphic::GraphicSystem::Instance()->GetRenderingCamera();

			Math::matrix44 mvp = Math::matrix44::multiply(pCamera->GetViewProjTransform(), GetTransform());
			pGMP->SetMatrixParam(eGShaderMatMVP, mvp);
			pGMP->SetMatrixParam(eGShaderMatM,GetTransform());

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

			if ( IsUseLM() && IsLMHandleValid() )
			{
				// lightmap support
				RenderBase::TextureHandle lightmapHandle = getOwner()->GetLMTextureHandle( 0 );

				int lmMode = (int)getOwner()->GetLightmapMode();
				if (lmMode > 0)
				{
					// lightmap support
					RenderBase::TextureHandle lightmapHandle = getOwner()->GetLMTextureHandle( 1 );
					pGMP->SetTextureParam(eGShaderTexLightmap2, lightmapHandle);
			}

				// set lightmap params
				// set lightmap params
				pGMP->SetVectorParam_X(eGShaderMatLMEnable, (float)(lmMode + 1.0f));
				pGMP->SetVectorParam(eGShaderMatLMTexParam, float4(1.0f,1.0f,0.0f,0.0f));
			}
			else
			{
				pGMP->SetVectorParam(eGShaderMatLMEnable, float4(0.0f,0.0f,0.0f,0.0f));
			}


			if (passType && material->GetTech()->IsTemplateTech())
			{
				rd->RenderForward(passType, priHandle, customizedMaterial);					
			}
			else
			{
				rd->RenderForward(eForward, priHandle, customizedMaterial);
			}

			// reset lightmap params
			pGMP->SetVectorParam(eGShaderMatLMEnable, float4(0.0f,0.0f,0.0f,0.0f));
		}
	}

	inline bool TerrainRenderObject::IsUseLM() const
	{
		return getOwner()->IsUseLM();
	}

	inline bool TerrainRenderObject::IsLMHandleValid() const
	{
		return getOwner()->IsLMTextureHandleValid();
	}

	inline TerrainRenderObject::Owner* TerrainRenderObject::getOwner() const
	{
		return GetOwner<Owner>();
	}

	Resources::Index16Container::value_type* TerrainRenderObject::GetTerrainNodeIndexData()
	{
		return GetTerrainNode()->GetIndexData();
	}

	SizeT TerrainRenderObject::GetTerrainNodeIndexDataCount()
	{
		return GetTerrainNode()->GetIndexDataCount();
	}

	const Resources::PositionData::Elem* TerrainRenderObject::GetTerrainNodePosData()
	{
		return GetTerrainNode()->GetPositionData();
	}

	SizeT TerrainRenderObject::GetTerrainNodePosCount()
	{
		return GetTerrainNode()->GetPostionDataCount();
	}




}