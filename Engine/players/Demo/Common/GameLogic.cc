/* \file: GameLogic.cc
* \brief: Demo Game logic
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/11/17   1.0    SunHao   
*/

#include "stdneb.h"
#include "util/keyvaluepair.h"
#include "basegamefeature/managers/sceneschedulemanager.h"
#include "addons/resource/meshres.h"
#include "addons/serialization/serializeserver.h"
#include "appframework/component.h"
#include "app/graphicfeature/components/meshrendercomponent.h"
#include "app/apputil/manuresutil.h"
#include "app/appframework/actormanager.h"

#include "GameLogic.h"
#include "GameData.h"
#include "DemoFwdDecl.h"
#include "GameUtil.h"

namespace Demo
{
	// - MouseState implement -----------------------------------------------------------------------------------------------
	void MouseState::SetState( int32 iState )
	{
		if ( iState>=eMouseState_Normal &&
			iState<eMouseState_Num )
		{
			m_eState = iState;
		}
	}	
	// - Grids implement -----------------------------------------------------------------------------------------------
	Grids::Grids()
		: m_fHalfLen( 0.f )
		, m_fYPos( 0.f )
		, m_iGridCount( 0 )
		, m_vertex0()
		, m_vertex1()
		, m_vertex2()
		, m_vertex3()
		, m_CalcResult()
		, m_selectRecord()
		, m_GridRegist()
	{
		//empty
	}

	Grids::~Grids()
	{
		if ( m_Grids )
		{
			m_Grids->Destory();
		}
	}

	void Grids::Init( float fLen, float fYpos, int iGridCount )
	{
		m_fHalfLen	 = fLen/2;
		m_fYPos		 = fYpos;
		m_iGridCount = iGridCount;

		m_vertex0.set( m_fHalfLen, m_fYPos, m_fHalfLen );
		m_vertex1.set( m_fHalfLen, m_fYPos, -1 * m_fHalfLen );
		m_vertex2.set( -1 * m_fHalfLen, m_fYPos, -1 * m_fHalfLen );
		m_vertex3.set( -1 * m_fHalfLen, m_fYPos, m_fHalfLen );

		ResetSelectRecord();
	}

	void Grids::ShowGirds( void )
	{
		/*
	
	vertex2	________|________vertex1
			|		|		|
			|		|		|
			|		|		|
	  ------------------------------>x
			|		|		|
			|		|		|
	vertex3	|_______|_______|vertex0
					|
				   \|/z
		*/
		
		//TODO: still have some problem, one grid is wrong 2011.12.12
		if ( !m_Grids.isvalid() )
		{
			int iSplitSize = m_iGridCount;

			Resources::PositionData posData;
			for ( int ii=0; ii<iSplitSize-1; ii++ )
			{

				Math::float3 Offset01 = CalcSplitZ( m_vertex0, m_vertex1, iSplitSize, ii );
				Math::float3 Offset01_next = CalcSplitZ( m_vertex0, m_vertex1, iSplitSize, ii+1 );
				Math::float3 Offset32 = CalcSplitZ( m_vertex3, m_vertex2, iSplitSize, ii );
				Math::float3 Offset32_next = CalcSplitZ( m_vertex3, m_vertex2, iSplitSize, ii+1 );

				posData.Append( Offset01 );
				posData.Append( Offset01_next );
				posData.Append( Offset32_next );
				posData.Append( Offset32 );
				posData.Append( Offset01 );
			}

			for ( int ii=0; ii<iSplitSize-1; ii++ )
			{
				Math::float3 Offset30 = CalcSplitX( m_vertex3, m_vertex0, iSplitSize, ii );
				Math::float3 Offset30_next = CalcSplitX( m_vertex3, m_vertex0, iSplitSize, ii+1 );
				Math::float3 Offset21 = CalcSplitX( m_vertex2, m_vertex1, iSplitSize, ii );
				Math::float3 Offset21_next = CalcSplitX( m_vertex2, m_vertex1, iSplitSize, ii+1 );

				posData.Append( Offset30 );
				posData.Append( Offset30_next );
				posData.Append( Offset21_next );
				posData.Append( Offset21 );
				posData.Append( Offset30 );
			}

			App::TActorPtr pActor = DrawLines( posData );

			pActor->SetName("LineTest");
			pActor->SetLayerID( NO_LOGIC_LAYER_ID );
			
			m_Grids = pActor;
			m_Grids->Active();
		}
		else
		{
			m_Grids->Active();
		}

	}

	void Grids::HideGirds( void )
	{
		if ( m_Grids.isvalid() )
		{
			m_Grids->Deactive();
		}
	}

	SizeT Grids::Size( void )
	{
		return m_GridRegist.Size();
	}

	void Grids::Clear( void )
	{
		ResetSelectRecord();
		m_GridRegist.Clear();
		m_CalcResult.Clear();
	}

	Math::float3 Grids::IsCanBuild( const Math::float3& pos, int iMeasure )
	{
		int iSplitSize = m_iGridCount;
		float fXPre	 = 0.f;
		float fXNext = 0.f;

		float fZPre  = 0.f;
		float fZNext = 0.f;

		int iZCentreIdx = -1;
		int iXCentreIdx = -1;

		int iZFirstIdx = -1;
		int iXFirstIdx = -1;

		Math::float3 centre;

		// - clear the result
		m_CalcResult.Clear();

		// to see where this building would locate
		for ( int ii=0; ii<iSplitSize; ii++ )
		{
			Math::float3 pre = CalcSplitX( m_vertex3, m_vertex0, iSplitSize, ii );
			Math::float3 next = CalcSplitX( m_vertex3, m_vertex0, iSplitSize, ii+1 );

			if ( pre.x()<pos.x() && 
				next.x()>pos.x() )
			{
				fXPre  = pre.x();
				fXNext = next.x();
				iXCentreIdx  = ii;
				break;
			}
		}
		
		for ( int ii=0; ii<iSplitSize; ii++ )
		{
			Math::float3 pre = CalcSplitZ( m_vertex0, m_vertex1, iSplitSize, ii );					
			Math::float3 next = CalcSplitZ( m_vertex0, m_vertex1, iSplitSize, ii+1 );

			if ( pre.z()<pos.z() && 
				next.z()>pos.z() )
			{
				fZPre  = pre.z();
				fZNext = next.z();
				iZCentreIdx  = ii;
				break;
			}
		}				

		if ( -1==iXCentreIdx ||
			-1==iZCentreIdx )
		{
			return centre;
		}

		// calcuate the first one,what's the first one? See below
		/*
		____________________________________
		|			|			|			|
		|	the		|			|			|
		|	first	|			|			|
		|	one		|			|			|
		|			|			|			|
		|			|			|			|
		_____________________________________
		|			|			|			|
		|			|			|			|
		|			|	centre	|			|
		|			|			|			|
		|			|			|			|
		|			|			|			|
		-------------------------------------	
		|			|			|			|
		|			|			|			|
		|			|			|			|
		|			|			|			|
		|			|			|			|
		|			|			|			|
		-------------------------------------	
		*/

		iXFirstIdx = iXCentreIdx - (int)(iMeasure/2);
		iZFirstIdx = iZCentreIdx - (int)(iMeasure/2);

		// check if there are already a building 
		for ( int ii=iXFirstIdx; ii<(iXFirstIdx+iMeasure); ii++ )
		{
			for ( int jj=iZFirstIdx; jj<(iZFirstIdx+iMeasure); jj++ )
			{
				if ( m_selectRecord[ii][jj] )
				{
					return centre;
				}
			}
		}

		// take these place
		for ( int ii=iXFirstIdx; ii<(iXFirstIdx+iMeasure); ii++ )
		{
			for ( int jj=iZFirstIdx; jj<(iZFirstIdx+iMeasure); jj++ )
			{
				m_CalcResult.Append( TGridIndex(ii, jj) );
			}
		}

		centre.set( (fXPre+fXNext)/2.f, pos.y(), (fZPre+fZNext)/2.f );
		return centre;
	}

	App::Actor::FastId Grids::GetActorId( Math::float3 pos )
	{
		int iSplitSize = m_iGridCount;
		float fXPre	 = 0.f;
		float fXNext = 0.f;

		int iZIdx = -1;
		int iXIdx = -1;

		for ( int ii=0; ii<iSplitSize; ii++ )
		{
			Math::float3 pre = CalcSplitX( m_vertex3, m_vertex0, iSplitSize, ii );
			Math::float3 next = CalcSplitX( m_vertex3, m_vertex0, iSplitSize, ii+1 );

			if ( pre.x()<pos.x() && 
				next.x()>pos.x() )
			{
				iXIdx  = ii;
				break;
			}
		}

		for ( int ii=0; ii<iSplitSize; ii++ )
		{
			Math::float3 pre = CalcSplitZ( m_vertex0, m_vertex1, iSplitSize, ii );					
			Math::float3 next = CalcSplitZ( m_vertex0, m_vertex1, iSplitSize, ii+1 );

			if ( pre.z()<pos.z() && 
				next.z()>pos.z() )
			{
				iZIdx  = ii;
				break;
			}
		}				

		if ( -1==iXIdx ||
			-1==iZIdx )
		{
			return INVALID_ACTOR_FASTID;
		}

		TGridIndex gridIdx(iXIdx, iZIdx);
		if ( !m_GridRegist.Contains(gridIdx) )
		{
			 return INVALID_ACTOR_FASTID;
		}

		return m_GridRegist[gridIdx];
	}

	bool Grids::Build( App::Actor::FastId iActorId )
	{
		if ( m_CalcResult.IsEmpty() )
		{
			return false;
		}

		for ( IndexT ii=0; ii<m_CalcResult.Size(); ii++ )
		{
			int iRow = m_CalcResult[ii].Row();
			int iCol = m_CalcResult[ii].Col();

			m_selectRecord[iRow][iCol] = true;

			m_GridRegist.Add( m_CalcResult[ii], iActorId );
		}

		return true;
	}

	void Grids::Remove( App::Actor::FastId iActorId )
	{
		TGridIndex idx( 0, 0 );
		App::Actor::FastId iFastId = 0;
		for ( IndexT ii=0; ii<m_GridRegist.Size(); )
		{
			idx = m_GridRegist.KeyAtIndex( ii );
			iFastId = m_GridRegist.ValueAtIndex( ii );
			if ( iFastId==iActorId )
			{
				m_selectRecord[idx.Row()][idx.Col()] = false;
				m_GridRegist.EraseAtIndex( ii );
			}
			else
			{
				ii++;
			}
		}
	}

	void Grids::ResetSelectRecord( void )
	{
		if ( m_selectRecord.Capacity()< m_iGridCount )
		{
			m_selectRecord.Resize( m_iGridCount, Util::Array<bool>() );
			for ( int ii=0; ii<m_iGridCount; ii++ )
			{
				m_selectRecord[ii].Resize( m_iGridCount, false );
			}
		}
		else
		{
			for ( SizeT ii=0; ii<m_selectRecord.Size(); ii++ )
			{
				for ( SizeT jj=0; jj<m_selectRecord[ii].Size(); jj++ )
				{
					m_selectRecord[ii][jj] = false;
				}
			}
		}
	}
	
	// - Preview implement -------------------------------------------------------------------------------------------------
	void Preview::Create( int iBuildingType )
	{
		if ( m_pActor.isvalid() )
		{
			m_pActor->Deactive( true );
			m_pActor->Destory();
			m_pActor = NULL;
		}

		TBuildingDataPtr pBuildingData =  GameData::Instance()->BuildingData( iBuildingType );
		if ( !pBuildingData.isvalid() )
		{
			return ;
		}
		App::TActorPtr pActor = App::ActorManager::Instance()->CreateFromTemplate( pBuildingData->TemplateName() );
		if ( !pActor.isvalid() )
		{
			return;
		}

		m_pActor = pActor;
		m_pActor->Active( true );
	}

	void Preview::SetPos( const Math::vector& pos ) 
	{
		if ( m_pActor.isvalid() )
		{
			m_pActor->SetPosition( pos );
		}
	}

	void Preview::ShowPreView( void )
	{
		if ( m_pActor.isvalid() )
		{
			m_pActor->Active( true ); 
		}
	}
	
	void Preview::HidePreView( void ) 
	{
		if ( m_pActor.isvalid() )
		{
			m_pActor->Deactive( true ); 
		}
	}

	// - LogicSerialization implement --------------------------------------------------------------------------------------
	__ImplementClass( Demo::LogicSerialization, 'DMLS', Core::RefCounted )

	void LogicSerialization::SetGame( GameLogic *pGame )
	{
		m_pGame = pGame;
	}

	void LogicSerialization::LoadGame( GameLogic* pGame )
	{
		pGame->Restart();

		for ( IndexT ii=0; ii<m_Lands.Size(); ii++ )
		{
			TEstatesPtr pLand = m_Lands[ii];
			pGame->PrepareToBuild( pLand->GetType() );
			pGame->BuildOnGrids( pLand->GetPos(), pLand->GetRadian() );
		}

		m_Lands.Clear( false );
	}

	App::Version LogicSerialization::GetVersion( void )const
	{
		return 1;//2011.12.19
	}

	void LogicSerialization::Load( App::Version ver, App::AppReader* pReader )
	{
		Util::String sSceneName;
		pReader->SerializeString( LOGIC_SERIALIZATION_SCENE_NAME.AsCharPtr(), sSceneName );

		SizeT iBuildingCount;
		pReader->SerializeInt( LOGIC_SERIALIZATION_BUILDING_COUNT.AsCharPtr(), iBuildingCount );

		if ( iBuildingCount>0 )
		{
			for ( IndexT index=0; index<iBuildingCount; ++index )
			{
				int iType;
				float fRadian;
				Math::float3 pos;

				pReader->SerializeInt( LOGIC_SERIALIZATION_BUILDING_TYPE.AsCharPtr(), iType );
				pReader->SerializeFloat( LOGIC_SERIALIZATION_BUILDING_RADIAN.AsCharPtr(), fRadian );
				pReader->SerializeFloat3( LOGIC_SERIALIZATION_BUILDING_POS.AsCharPtr(), pos );

				TEstatesPtr pEstate = Estates::Create();
				pEstate->Init( NULL, iType, pos, fRadian );
				m_Lands.Append( pEstate );
			}
		}
				
	}

	void LogicSerialization::Save( App::AppWriter* pWriter )const
	{
		// - make a simple save

		// - name, count
		Util::String sSceneName = m_pGame->GetSceneName();
		pWriter->SerializeString( LOGIC_SERIALIZATION_SCENE_NAME.AsCharPtr(), sSceneName );
		
		SizeT iBuildingCount = m_pGame->m_landAgent.Size();
		pWriter->SerializeInt( LOGIC_SERIALIZATION_BUILDING_COUNT.AsCharPtr(), iBuildingCount );

		// - building
		{
			for ( IndexT index=0; index<iBuildingCount; ++index )
			{
				TEstatesPtr pLand = m_pGame->m_landAgent.Find( index );
				n_assert( pLand.isvalid() );
				pWriter->SerializeInt( LOGIC_SERIALIZATION_BUILDING_TYPE.AsCharPtr(), pLand->GetType() );
				pWriter->SerializeFloat( LOGIC_SERIALIZATION_BUILDING_RADIAN.AsCharPtr(), (float)pLand->GetRadian() );
				pWriter->SerializeFloat3( LOGIC_SERIALIZATION_BUILDING_POS.AsCharPtr(), pLand->GetPos() );
			}
		}

	}
	
	// - GameLogic implement -----------------------------------------------------------------------------------------------
	__ImplementClass( Demo::GameLogic, 'DMGL', Core::RefCounted )
	__ImplementThreadSingleton( Demo::GameLogic );

	GameLogic::GameLogic()
		: m_sSceneName()
		, m_MouseState()
		, m_landAgent()
		, m_Preview()
		, m_SelectedId( 0 )
		, m_Grids()
	{
		__ConstructThreadSingleton

		m_AnimMgr = BuildingAnimationManager::Create();
	}

	GameLogic::~GameLogic()
	{
		m_AnimMgr->Close();
		__DestructThreadSingleton
	}

	void GameLogic::Select( Math::float3 intersectPoint )
	{
		App::Actor::FastId fastId = m_Grids.GetActorId( intersectPoint );
		if ( INVALID_ACTOR_FASTID==fastId )
		{
			return ;
		}

		TEstatesPtr pEstate =  m_landAgent.Find( fastId );
		if ( !pEstate.isvalid() )
		{
			return ;
		}

		m_SelectedId = fastId;

		//zhangming
		App::TActorPtr pActor = pEstate->GetBuilding();
		SizeT childCount = pActor->GetChildCount();
		if ( childCount<=0 )
		{
			return ;
		}

		for ( SizeT childIndex=0; childIndex<childCount; childIndex++ )
		{
			App::TActorPtr pChildActorL1 = pActor->GetChild(childIndex);
			Util::String childName = pChildActorL1->GetName();
			if ( "ground"!=childName )
			{
				continue;
			}

			SizeT childCountL1 = pChildActorL1->GetChildCount();
			if ( childCountL1<=0 )
			{
				continue;
			}

			for ( SizeT childIndexL1=0; childIndexL1<childCountL1; childIndexL1++ )
			{
				App::TActorPtr pChildActorL2 = pChildActorL1->GetChild(childIndexL1);
				Ptr<App::Component> pComponent = pChildActorL2->FindComponent(App::MeshRenderComponent::RTTI);
				if ( !pComponent.isvalid() )
				{
					continue;
				}

				Ptr<App::MeshRenderComponent> meshRenderCom = pComponent.downcast<App::MeshRenderComponent>();
				if ( NULL==meshRenderCom)
				{
					continue;
				}

				SizeT matInstanceCount = meshRenderCom->GetMaterialInstanceCount();
				for (SizeT matIndex = 0; matIndex < matInstanceCount; matIndex++)
				{
					Ptr<Graphic::MaterialInstance> pMatInstance = meshRenderCom->GetMaterial(matIndex);
					const Graphic::MaterialParamList& mpl = pMatInstance->GetParamList();
					SizeT MaterialParamCount = mpl.Size();
					for (SizeT paramIndex = 0;paramIndex < MaterialParamCount; paramIndex++)
					{
						Graphic::MaterialParam * pMatParam = mpl[paramIndex];
						if (pMatParam->GetName() == "_diffuseColor")
						{
							meshRenderCom->SetShaderConstantParam(matIndex,"_diffuseColor",Math::float4(0.5f, 0.2f, 0.2f, 0.0f));
							break;
						}
					}
				}
				
			}
		}
			
	}

	void GameLogic::RemoveSelected( void )
	{
		if ( 0==m_SelectedId )
		{
			return ;
		}

		// - delete it in game logic
		m_landAgent.Remove( m_SelectedId );	
		m_Grids.Remove( m_SelectedId );
		m_SelectedId = 0;
		
		// - remove it from scene
		App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
		Ptr<App::Scene> pScene = pSceneSchedule->GetScene();
		n_assert( pScene );

		// - remove the Actor from scene
		App::TActorPtr pRemove = pScene->FindActor( m_SelectedId );
		if ( pRemove.isvalid() )
		{
			pScene->RemoveActor( pRemove );
			pRemove->Destory();
		}

	}

	void GameLogic::CancelSelected( void )
	{
		//zhangming
		TEstatesPtr pEstate =  m_landAgent.Find( m_SelectedId );
		if ( !pEstate.isvalid() )
		{
			return ;
		}

		App::TActorPtr pActor = pEstate->GetBuilding();
		SizeT childCount = pActor->GetChildCount();
		if (childCount<=0 )
		{
			return ;
		}

		for (SizeT childIndex = 0; childIndex < childCount; childIndex++)
		{
			App::TActorPtr pChildActorL1 = pActor->GetChild(childIndex);
			Util::String childName = pChildActorL1->GetName();
			if (childName!="ground")
			{
				continue;
			}

			SizeT childCountL1 = pChildActorL1->GetChildCount();
			if ( childCountL1<=0 )
			{
				continue;
			}

			for ( SizeT childIndexL1=0; childIndexL1<childCountL1; childIndexL1++ )
			{
				App::TActorPtr pChildActorL2 = pChildActorL1->GetChild( childIndexL1 );
				Ptr<App::Component> pComponent = pChildActorL2->FindComponent( App::MeshRenderComponent::RTTI );
				if ( !pComponent.isvalid() )
				{
					continue;
				}

				Ptr<App::MeshRenderComponent> meshRenderCom = pComponent.downcast<App::MeshRenderComponent>();
				if ( NULL==meshRenderCom )
				{
					continue;
				}

				SizeT matInstanceCount = meshRenderCom->GetMaterialInstanceCount();
				for (SizeT matIndex = 0; matIndex < matInstanceCount; matIndex++)
				{
					Ptr<Graphic::MaterialInstance> pMatInstance = meshRenderCom->GetMaterial(matIndex);
					const Graphic::MaterialParamList& mpl = pMatInstance->GetParamList();
					SizeT MaterialParamCount = mpl.Size();
					for (SizeT paramIndex = 0;paramIndex < MaterialParamCount; paramIndex++)
					{
							Graphic::MaterialParam * pMatParam = mpl[paramIndex];
							if (pMatParam->GetName() == "_diffuseColor")
							{
								meshRenderCom->SetShaderConstantParam(matIndex,"_diffuseColor",Math::float4(0.0f, 0.0f, 0.0f, 0.0f));
								break;
							}
					}
				}
			}
		}

		m_SelectedId = 0;
	}

	App::TActorPtr GameLogic::GetSelected( void )
	{
		TEstatesPtr pEstate =  m_landAgent.Find( m_SelectedId );
		return pEstate->GetBuilding();
	}

	void GameLogic::RotateSelectedBuilding( Math::scalar radian )
	{
		TEstatesPtr pBuilding = m_landAgent.Find( m_SelectedId );
		if ( !pBuilding.isvalid() )
		{
			return ;
		}

		pBuilding->Rotate( radian );
	}

	void GameLogic::SetMouseState( int32 iState )
	{
		int32 iCurState = GetMouseState();
		if ( (MouseState::eMouseState_Building==iCurState) &&
			(MouseState::eMouseState_Building!=iState) )
		{
			m_Preview.HidePreView();
		}

		m_MouseState.SetState( iState );
	}	

	void GameLogic::SetMouseNormal( void )
	{
		SetMouseState( MouseState::eMouseState_Normal );
	}

	void GameLogic::ShowPreview( const Math::vector& pos )
	{
		m_Preview.SetPos( pos );
	}

	void GameLogic::PrepareToBuild( int iType )
	{
		m_iBuildingType = iType;

		m_Preview.Create( iType );

		SetMouseState( MouseState::eMouseState_Building );
	}

	void GameLogic::BuildOnGrids( Math::float3 pos )
	{
		BuildOnGrids( pos, 0.f );
	}

	void GameLogic::BuildOnGrids( Math::float3 pos, Math::scalar radian )
	{
		App::TActorPtr pBuilding = Build( pos );
		if ( !pBuilding.isvalid() )
		{
			return;
		}

		App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
		Ptr<App::Scene> pScene = pSceneSchedule->GetScene();
		n_assert( pScene );

		pScene->AddActor( pBuilding );
		pBuilding->Active( true );
		pBuilding->SetLayerID( BUILDING_LAYER_ID );


		m_landAgent.Add( pBuilding, m_iBuildingType, pos, radian );

		m_AnimMgr->BeignAnimtion( pBuilding );

		SetMouseNormal();
		
	}

	void GameLogic::CancelPreparedBuild( void )
	{
		SetMouseNormal();

		CancelSelected();
	}

	void GameLogic::Update( float32 fTime/*=0.f*/ )
	{
		m_AnimMgr->OnFrame( fTime );
	}

	void GameLogic::ToggleScene( void )
	{
		// TODO:implement
	}

	void GameLogic::SaveScene( void )
	{
		App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
		pSceneSchedule->SaveScene( GetSceneName() );
	}

	void GameLogic::SaveLogic( void )
	{
		Serialization::SerializationServer* pSerialzeMgr = Serialization::SerializationServer::Instance();
		n_assert( pSerialzeMgr );

		Ptr<Serialization::SerializeWriter> pWriter = pSerialzeMgr->OpenWriteFile( "logic:newGame.gLogic" );
		if ( pWriter )
		{
			Ptr<LogicSerialization> pLogicSer = LogicSerialization::Create();
			pLogicSer->SetGame( this );
			pWriter->SerializeObject( pLogicSer );
			pSerialzeMgr->CloseWriteFile( pWriter );
		}		
	}

	void GameLogic::LoadLogic( const Util::String& sFileName )
	{
		Serialization::SerializationServer* pSerialzeMgr = Serialization::SerializationServer::Instance();
		n_assert( pSerialzeMgr );

		Ptr<Serialization::SerializeReader> pReader = pSerialzeMgr->OpenReadFile( sFileName );
		if ( NULL!=pReader )
		{
			try
			{
				Ptr<LogicSerialization> pLogicSer = pReader->SerializeObject<LogicSerialization>();
				pLogicSer->LoadGame( this );
				CancelPreparedBuild();
			}
			catch(...)
			{
				n_error("SceneScheduleManager::OpenScene(): read %s scene failed/n", sFileName.AsCharPtr() );
			}

			pSerialzeMgr->CloseReadFile( pReader );		
		}

	}

	void GameLogic::Restart( void )
	{
		m_iBuildingType = 0;
		m_AnimMgr->Close();
		m_landAgent.Clear();
		SetMouseNormal();
		m_Preview.HidePreView();
		m_Grids.Clear();
		m_SelectedId = 0;
	}

	bool GameLogic::GameStart( void )
	{
		GameLogic::Create();
		GameLogic::Instance()->AddRef();

		GameData::Create();
		GameData::Instance()->AddRef();

		return true;
	}

	bool GameLogic::GameClose(void)
	{
		GameLogic::Instance()->Release();
		GameData::Instance()->Release();
		return true;
	}

	void GameLogic::GameInit( void )
	{
		bool bLoadData = false;
		bLoadData = GameData::Instance()->LoadData();
		n_assert( bLoadData );

		// len, y pos, grid count
		m_Grids.Init( 1100.0f, -127.0f, 220 );

		n_assert(m_AnimMgr);
		m_AnimMgr->Open();
	}

	App::TActorPtr GameLogic::Build( Math::float3 pos )
	{
		App::TActorPtr pBuilding;

		// for clear code style
		do 
		{

			TBuildingDataPtr pBuildingData =  GameData::Instance()->BuildingData( m_iBuildingType );
			if ( !pBuildingData.isvalid() )
			{
				break;
			}

			int iMeasure = pBuildingData->Measure();

			Math::float3 centre = m_Grids.IsCanBuild( pos, iMeasure );
			if (  !centre.all() )
			{
				break;
			}

			App::TActorPtr pActor= App::ActorManager::Instance()->CreateFromTemplate( pBuildingData->TemplateName() );
			if ( !pActor.isvalid() )
			{
				break;
			}

			m_Grids.Build( pActor->GetFastId() );

			// plant a building
			Math::vector vecPos( centre.x(), centre.y(), centre.z() );
			pActor->SetPosition( vecPos );

			pBuilding = pActor;
		}while( false );

		return pBuilding;
	}

	void GameLogic::ShowGirds( void )
	{
		m_Grids.ShowGirds();
	}

	void GameLogic::HideGirds( void )
	{
		m_Grids.HideGirds();
	}

}