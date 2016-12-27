/** \file: DemoGameStateHandler.cc
* \brief: implement DemoGameStateHandler
* Copyright (c) 2011,WebJet Enterprise Department ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/14   1.0    SunHao   
*/

#include "stdneb.h"
#include "math/polar.h"
#include "DemoGameStateHandler.h"
#include "basegamefeature/basegamefeature.h"
#include "basegamefeature/managers/sceneschedulemanager.h"
#include "graphicfeature/graphicsfeature.h"
#include "appframework/gameserver.h"
#include "appframework/scene.h"
#include "appframework/actor.h"
#include "graphicfeature/components/cameracomponent.h"
#include "graphicfeature/components/lightcomponent.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "input/inputserver.h"
#include "graphicsystem/GraphicSystem.h"
#include "graphicsystem/Light/Light.h"
#include "graphicsystem/Camera/Camera.h"
#include "addons/resource/meshres.h"
#include "apputil/manuresutil.h"
#include "graphicfeature/graphicsfeature.h"
#include "graphicfeature/graphicsfeatureprotocol.h"
#include "resource/resourceserver.h"
#include "input/inputmouse.h"
#include "input/inputkeyboard.h"
#include "math/ray.h"
#include "apputil/intersectutil.h"
#include "app/appframework/app_fwd_decl.h"
#include "input/input_fwd_decl.h"
#include "foundation/util/array.h"
#include "apputil/manuresutil.h"
#include "graphicsystem/Sky/SkyRenderer.h"
#include "graphicsystem/Sky/SkyRenderable.h"
#include "graphicfeature/components/skycomponent.h"
#include "water/SimpleGrid.h"
#include "water/PerlinNoise.h"
#include "graphicfeature/components/watercomponent.h"
#include "graphicfeature/components/LakeComponent.h"
#include "GameLogic.h"

#include "graphicsystem/Material/GlobalShaderParam.h"

namespace Demo
{
	__ImplementClass(Demo::DemoGameStateHandler, 'SSHD', App::StateHandler);

	//------------------------------------------------------------------------------
	/**
	*/
	DemoGameStateHandler::DemoGameStateHandler()
		: m_sSceneName()
		, m_iMeshActorID(0)
		, m_sCurMeshName()
		, m_sCurShaderName()
		, m_sCurTexName()
		, m_pSkyActor( NULL )
		, m_pWaterActor( NULL )
		, m_pTerrain( NULL )
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	DemoGameStateHandler::~DemoGameStateHandler()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	This method is called when the state associated with this state handler
	is entered. The parameter indicates the previously active state.

	@param  prevState   previous state
	*/
	void DemoGameStateHandler::OnStateEnter(const Util::String& prevState)
	{         
		using namespace App;

		App::BaseGameFeature::Instance()->SetRenderDebug(true);

		// setup the game
		// start game world
		App::GameServer::Instance()->Start();
		App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();

		// sky
		if ( true )
		{
			m_pSkyActor = Actor::Create();
			m_pSkyActor->SetName("Sky");
			m_pSkyActor->SetLayerID(1);

			Ptr<SkyComponent> pSkyComp = SkyComponent::Create();
			pSkyComp->SetMaterialByShaderID(0, "shd:SkyDome.wjshader");
			pSkyComp->SetMaterialByShaderID(1, "shd:SkyCloud.wjshader");
			pSkyComp->SetMaterialByShaderID(2, "shd:SkyMoon.wjshader");

			pSkyComp->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
			pSkyComp->_AddTextureParam(1, "_CloudMap", "sys:white.dds");
			pSkyComp->_AddTextureParam(1, "_NormalMap", "sys:white.dds");
			pSkyComp->_AddTextureParam(1, "_DensityMap", "sys:white.dds");
			pSkyComp->_AddTextureParam(2, "_MoonMap", "sys:white.dds");

			pSkyComp->SetTexture( 0, "_diffuseMap", "tex:SkyX_Starfield.DDS" );
			pSkyComp->SetTexture( 1, "_CloudMap", "tex:Cloud1.DDS" );
			pSkyComp->SetTexture( 1, "_NormalMap", "tex:c22n.DDS" );
			pSkyComp->SetTexture( 1, "_DensityMap", "tex:c22.DDS" );
			pSkyComp->SetTexture( 2, "_MoonMap", "tex:Moon.DDS" );

			pSkyComp->SetTime( 11.0f );			//	设置时间
			pSkyComp->SetAtmosphereAutoChange(false);	//	停止天空光照停止运动
			pSkyComp->SetTimeMultiplier(0.4f);	//	继续让云运动

			m_pSkyActor->AttachComponent(pSkyComp.upcast<Component>());
			m_pSkyActor->Active();
		}

		//lake
		if (true)
		{
			m_pWaterActor = Actor::Create();

			Ptr<LakeComponent> pLakeComponent = LakeComponent::Create();
			pLakeComponent->SetMeshID("msh:waterplane.mesh");
			pLakeComponent->SetMaterialByShaderID(0, "shd:Lake.wjshader");
			pLakeComponent->_AddTextureParam(0, "_waveMap1", "sys:white.dds");
			pLakeComponent->_AddTextureParam(0, "_waveMap2", "sys:white.dds");
			pLakeComponent->SetTexture(0, "_waveMap1", "tex:wave0.dds");
			pLakeComponent->SetTexture(0, "_waveMap2", "tex:wave1.dds");

			m_pWaterActor->AttachComponent( pLakeComponent.upcast<Component>() );

			m_pWaterActor->SetPosition(Math::vector(-220.f, -130, -200.f));
			m_pWaterActor->Active();
			
		}

		// fog
		{
			Graphic::Camera::S_Camera_Far = 1000.0f;
			Graphic::GlobalMaterialParam* globalMaterialParam = Graphic::Material::GetGlobalMaterialParams();
			// 设置fog颜色。  最后一位是0.0
			globalMaterialParam->SetVectorParam( Graphic::eGShaderVecFogColor, Math::float4(0.645f,0.688f,0.7716f,0.0f) );
			// 设置fog参数。 fog start 和fog end
			globalMaterialParam->SetVectorParam( Graphic::eGShaderVecFogParam, Math::float4(0.3f,0.7f,0.0f,1.0f) );

		}

		// open scene
		pSceneSchedule->OpenScene(  GetSceneName() , false );
		Ptr<App::Scene> pScene = pSceneSchedule->GetScene();
		n_assert( pScene );

		GameLogic::GameStart();
		GameLogic::Instance()->SetSceneName( GetSceneName() );
		GameLogic::Instance()->GameInit();

}

	//------------------------------------------------------------------------------
	/**
	This method is called when the state associated with this state handler
	is left. The parameter indicates the next active state.

	@param  nextState   next state
	*/
	void DemoGameStateHandler::OnStateLeave(const Util::String& nextState)
	{
		App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
		pSceneSchedule->OpenScene(  GetSceneName() , false );
		Ptr<App::Scene> pScene = pSceneSchedule->GetScene();
		
		if ( m_pWaterActor.isvalid() )
		{
			m_pWaterActor->Destory();
			m_pWaterActor = NULL;
		}
		if ( m_pSkyActor.isvalid() )
		{
			m_pSkyActor->Destory();
			m_pSkyActor = NULL;
		}

		// - close scene
		pScene->Close();

		GameLogic::GameClose();

		// stop game world
		App::GameServer::Instance()->Stop();


	}

	//------------------------------------------------------------------------------
	/**
	This method is called once a frame while the state is active. The method
	must return a state identifier. If this is different from the current
	state, a state switch will occur after the method returns. 

	@return     a state identifier
	*/
	Util::String DemoGameStateHandler::OnFrame()
	{
		if( App::GameServer::Instance()->IsQuitRequested() )
		{
			return "Exit";
		}

		Input::InputServer* pInputServer = Input::InputServer::Instance();
		Ptr< Input::InputMouse> mouse =  pInputServer->GetDefaultMouse();
		Ptr< Input::InputKeyboard> keyboard = pInputServer->GetDefaultKeyboard();

		GameLogic::Instance()->Update( 0.01f );

		if ( m_pSkyActor && keyboard )
		{
			if ( keyboard->KeyDown( Input::InputKey::P ) )
			{
				Ptr<App::SkyComponent> pSkyComp = m_pSkyActor->FindComponent<App::SkyComponent>();
				if ( pSkyComp )
				{
					pSkyComp->SetAtmosphereAutoChange( !pSkyComp->IsAtmosphereAutoChange() );	//	开启/关闭大气效果
				}
			}
		}

		if ( mouse->ButtonDown( Input::InputMouseButton::LeftButton ) )
		{
			Math::float2 screenPos = mouse->GetScreenPosition();

			Math::Ray worldRay;
			AppUtil::IntersectUtil::ComputeDefaultWorldMouseRay( screenPos, worldRay );

			AppUtil::IntersectResultList rsList;
			if ( AppUtil::IntersectUtil::IntersectWorld(worldRay, GROUND_LAYER_ID, 0xFFFFFFFF, false, rsList ) )
			{
				rsList.Sort();

				App::TActorPtr pActor = rsList[0].actor;
				Math::float3 intersectPoint = worldRay.PointAt( rsList[0].intersectPoint );

				if ( MouseState::eMouseState_Building==GameLogic::Instance()->GetMouseState() )
				{
					GameLogic::Instance()->BuildOnGrids( intersectPoint );
				}
				else
				{
					GameLogic::Instance()->CancelSelected();
					GameLogic::Instance()->Select( intersectPoint );
				}
			}
		}
		else if ( MouseState::eMouseState_Building==GameLogic::Instance()->GetMouseState() )
		{
			Math::float2 screenPos = mouse->GetScreenPosition();

			Math::Ray worldRay;
			AppUtil::IntersectUtil::ComputeDefaultWorldMouseRay( screenPos, worldRay );

			AppUtil::IntersectResultList rsList;
			if ( AppUtil::IntersectUtil::IntersectWorld(worldRay, GROUND_LAYER_ID, 0xFFFFFFFF, false, rsList ) )
			{
				rsList.Sort();
				App::TActorPtr pActor = rsList[0].actor;
				Math::float3 intersectPoint = worldRay.PointAt( rsList[0].intersectPoint );

				GameLogic::Instance()->ShowPreview( Math::vector(intersectPoint.x(), intersectPoint.y(), intersectPoint.z()) );
			}		

		}


		return this->GetName();
	}

} // namespace Application
