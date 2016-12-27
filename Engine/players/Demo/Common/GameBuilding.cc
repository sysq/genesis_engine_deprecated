/** \file: GameBuilding.cc
* \brief: implement game building logic
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/12/07   1.0    SunHao   
*/
#include "stdneb.h"
#include "GameBuilding.h"
#include "core/rttimacros.h"

#include "graphicfeature/components/rendercomponent.h"

namespace Demo
{
	using namespace App;

	// - land implement -----------------------------------------------------------
	__ImplementClass( Demo::Estates, 'DMLD', Core::RefCounted )
	__ImplementClass( Demo::BuildingAnimationManager, 'DBAM', Core::RefCounted);

	Estates::~Estates()
	{
		if ( m_pBuilding.isvalid() )
		{
			m_pBuilding->Destory();
		}
	}

	void Estates::Init( App::TActorPtr pBuilding, TEstateType iType, const Math::float3& pos, Math::scalar radian  )
	{
		m_pBuilding  = pBuilding;
		m_iType		 = iType;
		m_Pos		 = pos;
		m_RollRadian = radian;

		if ( 0.f!=m_RollRadian &&
			NULL!=m_pBuilding )
		{
			m_pBuilding->Roll( m_RollRadian );
		}
	}

	void Estates::Rotate( Math::scalar radian )
	{
		m_RollRadian += radian;
		m_pBuilding->Roll( m_RollRadian );
	}

	// - landAgent implement -----------------------------------------------------------	
	TEstatesPtr LandAgent::Find( App::Actor::FastId iFastId )
	{
		TEstatesPtr pLand;
		IndexT idx =  m_Buildings.FindIndex( iFastId );
		if ( idx>=m_Buildings.Size() ||
			idx<0 )
		{
			return pLand;
		}

		pLand = m_Buildings.ValueAtIndex( idx );

		return pLand;
	}

	TEstatesPtr LandAgent::Find( IndexT iIndex )
	{
		if ( iIndex>=0 &&
			iIndex<m_Buildings.Size() )
		{
			return m_Buildings.ValueAtIndex( iIndex );
		}

		return Estates::Create();
	}

	bool LandAgent::Add( TEstatesPtr pBuilding )
	{
		if ( m_Buildings.Contains( pBuilding->GetId() ) )
		{
			return false;
		}

		m_Buildings.Add( pBuilding->GetId(), pBuilding );

		return true;
	}

	bool LandAgent::Add( App::TActorPtr pBuilding, TEstateType iType, const Math::float3& pos, Math::scalar radian )
	{
		TEstatesPtr pEsates = Estates::Create();
		pEsates->Init( pBuilding, iType, pos, radian );
		
		return Add( pEsates );
	}

	void LandAgent::Remove( App::Actor::FastId iFastId )
	{
		if ( m_Buildings.Contains(iFastId) )
		{
			m_Buildings.Erase( iFastId );
		}
	}

	SizeT LandAgent::Size( void )
	{
		return m_Buildings.Size();
	}

	void LandAgent::Clear( void )
	{
		m_Buildings.Clear();
	}

	//------------------------------------------------------------------------
	const float32 Default_Build_Height_Limit = 30;	//	默认的建筑高度
	const float32 Default_Build_Increase_Speed = 10;	//	建筑生成速度。 场景单位/秒
	const float32 Max_Build_Height = 10000;			//	建筑的最大高度

	//------------------------------------------------------------------------
	BuildingAnimationManager::BuildingAnimationManager()
		:m_animationShader("shd:house.wjshader")
		,m_animationShaderParamName("uHeightLimit")
	{

	}
	//------------------------------------------------------------------------
	BuildingAnimationManager::~BuildingAnimationManager()
	{

	}
	//------------------------------------------------------------------------
	void BuildingAnimationManager::Open()
	{
		//	针对每个建筑，填充一个建筑高度限制的数组
	}
	//------------------------------------------------------------------------
	void BuildingAnimationManager::BeignAnimtion( App::TActorPtr pBlock )
	{
		if ( !pBlock )
		{
			return;
		}

		float offset = pBlock->GetPosition().y();

		SizeT childCount = pBlock->GetChildCount();
		for(IndexT nChild = 0; nChild < childCount; ++nChild )
		{
			const Ptr<Actor>& child = pBlock->GetChild(nChild);
			if ( child && child->GetName() == "house" )
			{
				AddAnimationInfo( child, offset );
			}
		}
	}
	//------------------------------------------------------------------------
	void BuildingAnimationManager::AddAnimationInfo( const App::TActorPtr& actor, float32 offSet )
	{
		n_assert(actor);
		Ptr<RenderComponent> render = actor->FindComponent<RenderComponent>();
		if ( render )
		{
			bool bNeedAddAnim = false;

			SizeT shaderCount = render->GetShaderCount();
			for ( IndexT iSubMesh = 0; iSubMesh < shaderCount; ++iSubMesh )
			{
				Util::String shaderName = render->GetShaderID(iSubMesh).AsString(); 
				shaderName.ToLower();
				
				if ( shaderName == m_animationShader )
				{
					render->SetShaderConstantParam(iSubMesh, m_animationShaderParamName,  offSet );
					bNeedAddAnim = true;
				}
			}

			if ( bNeedAddAnim )
			{
				AnimInfo animInfo;
				animInfo.m_house = actor;
				animInfo.m_render = render;
				animInfo.m_curHeight = offSet;
				animInfo.m_maxHeight = GetMaxHouseHeight( actor->GetName(), offSet);
				m_AnimInfos.Append( animInfo );
			}
		}


		SizeT childCount = actor->GetChildCount();
		for(IndexT nChild = 0; nChild < childCount; ++nChild )
		{
			const Ptr<Actor>& child = actor->GetChild(nChild);
			AddAnimationInfo(child,  offSet);
		}
	}
	//------------------------------------------------------------------------
	void BuildingAnimationManager::OnFrame( float32 fTime )
	{
		if ( m_AnimInfos.IsEmpty() )
		{
			return;
		}

		float height_increase = 10 * fTime;

		AnimInfos continueAnims;

		SizeT count = m_AnimInfos.Size();
		for ( IndexT index = 0; index < count; ++index )
		{
			AnimInfo& animInfo = m_AnimInfos[index];
			n_assert( animInfo.m_house );
			n_assert( animInfo.m_render );
			animInfo.m_curHeight += height_increase;

			if ( animInfo.m_curHeight >= animInfo.m_maxHeight )
			{
				animInfo.m_curHeight = Max_Build_Height;	//	恢复成一个最大的默认值. 动画过程结束
			}
			else
			{
				continueAnims.Append( animInfo );	//	动画过程继续
			}

			SizeT shaderCount = animInfo.m_render->GetShaderCount();
			for ( IndexT iSubMesh = 0; iSubMesh < shaderCount; ++iSubMesh )
			{
				Util::String shaderName = animInfo.m_render->GetShaderID(iSubMesh).AsString(); 
				shaderName.ToLower();

				if ( shaderName == m_animationShader )
				{
					animInfo.m_render->SetShaderConstantParam(iSubMesh, m_animationShaderParamName,  animInfo.m_curHeight );
				}
			}
		}

		m_AnimInfos.Swap( continueAnims );
	}
	//------------------------------------------------------------------------
	float32 BuildingAnimationManager::GetMaxHouseHeight( const Util::String& housename , float32 offSet ) const
	{
		IndexT findIndex = m_heightLimit.FindIndex( housename );
		if ( findIndex != InvalidIndex )
		{
			return m_heightLimit.ValueAtIndex(findIndex) + offSet;
		}
		else
		{
			return Default_Build_Height_Limit + offSet;
		}

	}
	//------------------------------------------------------------------------
	void BuildingAnimationManager::Close()
	{
		m_AnimInfos.Clear();
	}

}
