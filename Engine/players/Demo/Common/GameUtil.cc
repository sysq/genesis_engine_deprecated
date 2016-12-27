/** \file: GameUtil.cc
* \brief: 
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/12/06   1.0    SunHao   
*/
#include "stdneb.h"
#include "GameUtil.h"
#include "appframework/component.h"
#include "appframework/actor.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "app/apputil/manuresutil.h"

namespace Demo
{

	App::TActorPtr DrawLines( Resources::PositionData posData )
	{
		App::TActorPtr pActor = App::Actor::Create();

		Ptr<Resources::MeshRes> mesh = AppUtil::ManuResUtil::CreateManuMesh_LineList( "Line_mesh", posData.Size(), &posData[0], Math::Color32(0,255,0,0) );
		n_assert( mesh.isvalid() );
		

		Ptr<App::MeshRenderComponent> pMeshRenderComponent = App::MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( mesh->GetResourceId() );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:Line.wjshader");

		pActor->AttachComponent( pMeshRenderComponent.upcast<App::Component>() );

		return pActor;
	}

	Math::float3 CalcSplitZ(const Math::float3& one, const Math::float3& other, int iSplitCount, int iCount)
	{
		Math::float3 tem = one + other;
		tem.x() = tem.x()/2;
		tem.y() = tem.y()/2;
		float fSmall = 0.f;
		float fBig = 0.f;
		if ( one.z()>other.z() )
		{
			fSmall = other.z();
			fBig = one.z();
		}
		else
		{
			fSmall = one.z();
			fBig = other.z();
		}

		float fLen = fBig - fSmall;
		tem.z() = fSmall + fLen/iSplitCount* iCount; 

		return tem;
	}

	Math::float3 CalcSplitX(const Math::float3& one, const Math::float3& other, int iSplitCount, int iCount)
	{
		Math::float3 tem = one + other;
		tem.y() = tem.y()/2;
		tem.z() = tem.z()/2; 

		float fSmall = 0.f;
		float fBig = 0.f;
		if ( one.x()>other.x() )
		{
			fSmall = other.x();
			fBig = one.x();
		}
		else
		{
			fSmall = one.x();
			fBig = other.x();
		}

		float fLen = fBig - fSmall;
		tem.x() = fSmall + fLen/iSplitCount* iCount; 

		return tem;
	}

}