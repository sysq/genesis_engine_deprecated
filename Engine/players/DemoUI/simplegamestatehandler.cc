//------------------------------------------------------------------------------
//  application/statehandler.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "simplegamestatehandler.h"
#include "basegamefeature/basegamefeature.h"
#include "basegamefeature/managers/sceneschedulemanager.h"
#include "graphicfeature/graphicsfeature.h"
#include "appframework/gameserver.h"
#include "appframework/scene.h"
#include "appframework/actor.h"
#include "apputil/mouserayutil.h"
#include "graphicfeature/components/cameracomponent.h"
#include "graphicfeature/components/meshcomponent.h"
#include "graphicfeature/components/lightcomponent.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "graphicfeature/components/simulatephysicwatercomponent.h"
#include "graphicfeature/components/skycomponent.h"
#include "input/inputserver.h"
#include "input/inputkeyboard.h"
#include "input/inputmouse.h"
#include "app/inputfeature/inputfeature.h"

#include "graphicsystem/GraphicSystem.h"
#include "graphicsystem/Light/Light.h"
#include "graphicsystem/Camera/Camera.h"
#include "graphicsystem/Sky/SkyRenderer.h"
#include "graphicsystem/Sky/SkyRenderable.h"
#include "graphicsystem/Renderable/SimpleShapeRenderable.h"
#include "graphicsystem/Renderable/SimpleShapeRenderer.h"

#include "graphicfeature/components/skeletoncomponent.h"
#include "graphicfeature/components/skinnedmeshrendercomponent.h"
#include "graphicfeature/components/animationcomponent.h"

#include "apputil/intersectutil.h"
#include "apputil/manuresutil.h"

#include "resource/meshres.h"

#include "math/polar.h"
#include "math/ray.h"
#include "math/intersection.h"

#include "particlefeature/components/particlecomponent.h"
#include "particlefeature/components/particlerendercomponent.h"

#include "particles/particleemitter.h"
#include "particles/particletechnique.h"
#include "particles/particleaffector.h"
#include "particles/particleserver.h"
#include "particles/emitters/particlePointEmitter.h"
#include "particles/emitters/particleCircleEmitter.h"
#include "particles/emitters/particleBoxEmitter.h"
#include "particles/emitters/particleSphereSurfaceEmitter.h"

#include "particles/affectors/particleRandomiserAffector.h"
#include "particles/affectors/particleColorAffector.h"
#include "particles/affectors/particleJetAffector.h"
#include "particles/affectors/particleScaleAffector.h"
#include "particles/affectors/particleLinearForceAffector.h"
#include "particles/affectors/particleFollowerAffector.h"
#include "particles/affectors/particleTextureRotatorAffector.h"
#include "particles/affectors/particleVortexAffector.h"
#include "particles/affectors/particleTextureAnimatorAffector.h"
#include "particles/affectors/particleGravityAffector.h"


#include "particles/targets/particlebillboardtarget.h"
#include "particles/targets/particleBeamTarget.h"
#include "particles/targets/particleTrailTarget.h"
#include "particles/targets/particleRibbonTrailTarget.h"
#include "particles/targets/particleEntityTarget.h"

#include "vegetationfeature/components/vegetationrendercomponent.h"

#include "guifeature/gui.h"
#include "app/spritebacth/debugboard.h"

namespace Sample
{
__ImplementClass(Sample::SimpleStateHandler, 'SSHD', App::StateHandler);

using namespace Graphic;
using namespace App;
using namespace Particles;
using namespace Vegetation;

int keydown = 21;
Timing::Time curTime = 0;
int bWebFunc = false;


//------------------------------------------------------------------------------
/**
*/
SimpleStateHandler::SimpleStateHandler()
:mShowAll(false)
,mShowParticle(true)

,mEmtyWinBtn(NULL)
,mPressTxt(NULL)
,mRed(NULL)
,mGreen(NULL)
,mBlue(NULL)
,mColor(NULL)
,mColorText(NULL)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SimpleStateHandler::~SimpleStateHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called when the state associated with this state handler
    is entered. The parameter indicates the previously active state.

    @param  prevState   previous state
*/
void
SimpleStateHandler::OnStateEnter(const Util::String& prevState)
{         
	App::BaseGameFeature::Instance()->SetRenderDebug(true);

    // setup the game
    // start game world
    App::GameServer::Instance()->Start();

	CreateScene();

	outlist.Clear();
	//createParSys(outlist);
	//CreateParticleFromTemplate();


}
//void SimpleStateHandler::_testMouseButtonClick(MyGUI::Widget* _sender)
//{
//	int x = 0;
//}

//------------------------------------------------------------------------------
void SimpleStateHandler::CreateScene()
{
	App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
	//_initUI();
	//	new scene
	pSceneSchedule->OpenScene(  GetSceneName() , true);

	//init gyrostat
	if(0)
		mGyrostat = new Gyrostat();

	const Ptr<GraphicSystem>& gs = App::GraphicsFeature::Instance()->GetGraphicSystem();

	Ptr<App::Scene> pScene = pSceneSchedule->GetScene();
	n_assert( pScene );
	mScene = pScene;

	// 创建一个actor，挂接一个相机
	matrix44 cameraTransform;
	{
		Ptr<Actor> pActor = Actor::Create();
		pActor->SetLayerID(1);
		pScene->AddActor( pActor );
		pActor->SetName("Camera1");

		Ptr<CameraComponent> pCameraComponent = CameraComponent::Create();
		pActor->AttachComponent( pCameraComponent.upcast<Component>() );

		matrix44 Inittransform = matrix44::lookatrh(float4(1.27f, 1.2f, -3.4f, 1.f),float4(0.f, 0.f, 0.f, 1.f),float4(0.f,1.f,0.f,0.f));
		Inittransform = matrix44::inverse(Inittransform);

		pActor->SetTransform( Inittransform );

		cameraTransform = Inittransform;

		// 作为主相机
		pCameraComponent->SetCameraOrder(Graphic::Camera::Main);

		pCameraComponent->SetUseSkyBox(true);
		pCameraComponent->SetTexRes("tex:SkyBox_1.dds");//"tex:Sky_01_1024.dds"
		GraphicsFeature::Instance()->AssignDefaultCamera( pActor );
		pActor->Active();
		mCamera = pActor;
	}

	// sky
	if ( 0 )
	{
		Ptr<Actor> m_pSkyActor = Actor::Create();
		m_pSkyActor->SetName("Sky");
		m_pSkyActor->SetLayerID(1);

		Ptr<SkyComponent> pSkyComp = SkyComponent::Create();
		pSkyComp->SetShaderID(0, "shd:SkyDome.wjshader");
		pSkyComp->SetShaderID(1, "shd:SkyCloud.wjshader");
		pSkyComp->SetShaderID(2, "shd:SkyMoon.wjshader");

		pSkyComp->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		pSkyComp->_AddTextureParam(1, "_CloudMap", "sys:white.dds");
		pSkyComp->_AddTextureParam(1, "_NormalMap", "sys:white.dds");
		pSkyComp->_AddTextureParam(1, "_DensityMap", "sys:white.dds");
		pSkyComp->_AddTextureParam(2, "_MoonMap", "sys:white.dds");

		pSkyComp->SetTexture( 0, "_diffuseMap", "tex:SkyX_Starfield.dds" );
		pSkyComp->SetTexture( 1, "_CloudMap", "tex:Cloud1.dds" );
		pSkyComp->SetTexture( 1, "_NormalMap", "tex:c22n.dds" );
		pSkyComp->SetTexture( 1, "_DensityMap", "tex:c22.dds" );
		pSkyComp->SetTexture( 2, "_MoonMap", "tex:Moon.dds" );

		pSkyComp->SetTime(18);
		pSkyComp->SetAtmosphereAutoChange(false);

		m_pSkyActor->AttachComponent(pSkyComp.upcast<Component>());
		m_pSkyActor->Active();
		pScene->AddActor( m_pSkyActor );
	}

	// 创建actor，挂接一个光源
	if (true)
	{
		Ptr<Actor> pActor = Actor::Create();
		pActor->SetLayerID(1);
		pActor->SetName("Light1");
		pScene->AddActor( pActor );

		Ptr<LightComponent> pLightComponent = LightComponent::Create();
		pActor->AttachComponent( pLightComponent.upcast<Component>() );

		pLightComponent->SetLightType( Graphic::Light::eSunLight);

		Math::matrix44 lighttrans = Math::matrix44::identity();
		Math::polar orientation(Math::float4(2.f/3.f,2.f/3.f,-1.f/3.f,0.f));
		Math::matrix44 rot = Math::matrix44::rotationyawpitchroll(orientation.rho, orientation.theta, 0);
		Math::matrix44 rotx = Math::matrix44::rotationx(-0.5*PI);
		rotx = Math::matrix44::multiply(rot,rotx);
		lighttrans = Math::matrix44::multiply(rotx, lighttrans);

		pActor->SetTransform( lighttrans );
		pActor->Active();
		mLight = pActor;

	}
	

	//create Terrain 's floor
	{
		Ptr<Actor> pfloor = Actor::Create();
		pfloor->SetLayerID(0);
		pfloor->SetName("floor");
		pScene->AddActor(pfloor);

		Ptr<MeshComponent> pMeshComponent = MeshComponent::Create();

		if(1)
		{
			Resources::PositionData position;
			Resources::Index16Container indicies;
			Resources::TexCoordData texcoors;

			position.Append(Math::float3(-50,0,50));
			position.Append(Math::float3(50,0,50));
			position.Append(Math::float3(50,0,-50));
			position.Append(Math::float3(-50,0,-50));

			indicies.Append(0);
			indicies.Append(1);
			indicies.Append(2);

			indicies.Append(2);
			indicies.Append(3);
			indicies.Append(0);

			texcoors.Append(Math::float2(0,0));
			texcoors.Append(Math::float2(0,1));
			texcoors.Append(Math::float2(1,1));
			texcoors.Append(Math::float2(1,0));

			Math::Color32 color(0,0,0,0);

			Ptr<Resources::MeshRes> mesh = AppUtil::ManuResUtil::CreateManuMesh_WithTopology( "terrainFloor", position.Size(),	&position[0], color, indicies.Size(),&indicies[0]);
			n_assert( mesh.isvalid() );
			mesh->SetVertexData<Resources::TexCoordData>( &texcoors[0], 4 );

			pMeshComponent->SetMeshID( mesh->GetResourceId() );
		}
		else
		{
			pMeshComponent->SetMeshID( "msh:Grass2.mesh" );
		}


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();	
		pMeshRenderComponent->SetShaderID(0, "shd:particle_Simple.wjshader");	
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:grid.png" );
		//pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:Grass2.dds" );

		////[START] Add VegetationRenderComponent
		//{
		//	Ptr<VegetationRenderComponent> vegeRC = VegetationRenderComponent::Create();
		//	Ptr<VegetationObject>& vegeObj = vegeRC->CreateVegePrototype(Vegetation::eRT_Grass);
		//	vegeRC->SetShaderId(vegeObj, 0, "shd:Particle_SrcInvSrc.wjshader");
		//	vegeObj->AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		//	vegeObj->SetTexture(0, "_diffuseMap", "tex:grid.png" );

		//	INSTANCE_DATA_POS* inst_pos = vegeObj->CreateInstance(Vegetation::eRT_Grass);
		//	inst_pos->pos = Math::float3(1,1,1);
		//	inst_pos->scale = Math::float2(1,1);

		//	pfloor->AttachComponent( vegeRC.upcast<Component>() );
		//}
		////[END]

		pfloor->AttachComponent( pMeshComponent.upcast<Component>() );
		pfloor->AttachComponent( pMeshRenderComponent.upcast<Component>() );
		pfloor->Active();
	}

	//绘制世界参考坐标系
	if(0)
	{
		//Math::matrix44 worldM = Math::matrix44::identity();
		Ptr<Actor> pActor = Actor::Create();
		pActor->SetName("WorldCoords");
		pActor->SetLayerID(0);	
		mWorldCoordsActor = pActor;
		pActor->Active();

		Ptr<Actor> pCoordsX = Gyrostat::CreateGyrostatActor(Gyrostat::OSTrans ,"CoordsX",float3(1,0,0),0);
		pCoordsX->Active();
		pCoordsX->SetParent(pActor);
		Ptr<Actor> pCoordsY = Gyrostat::CreateGyrostatActor(Gyrostat::OSTrans ,"CoordsY",float3(0,1,0),0);
		pCoordsY->Active();
		pCoordsY->SetParent(pActor);
		Ptr<Actor> pCoordsZ = Gyrostat::CreateGyrostatActor(Gyrostat::OSTrans ,"CoordsZ",float3(0,0,1),0);
		pCoordsZ->Active();
		pCoordsZ->SetParent(pActor);
	}

}
//------------------------------------------------------------------------------
/**
    This method is called when the state associated with this state handler
    is left. The parameter indicates the next active state.

    @param  nextState   next state
*/
void
SimpleStateHandler::OnStateLeave(const Util::String& nextState)
{
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
Util::String
SimpleStateHandler::OnFrame()
{
	if( App::GameServer::Instance()->IsQuitRequested() )
	{
		if ( mIntersectBoundActor.isvalid() )
		{
			mIntersectBoundActor->Destory();
			mIntersectBoundActor = NULL;
		}
		if ( mWorldCoordsActor.isvalid() )
		{
			mWorldCoordsActor->Destory();
			mWorldCoordsActor = NULL;
		}
		
		App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();

		if (ParSystem)
		{
			ParSystem->DeActive();
			ParSystem = NULL;
		}
		
		pSceneSchedule->CloseScene();
		//SaveTemplate();

		return "Exit";
	}
	else
	{
		if(!_frameUI())
		{


			const Ptr<Input::InputServer>& inputServer = App::InputFeature::Instance()->GetInputServer();
			n_assert( inputServer.isvalid() );

			// update the camera from input	


			const Ptr<Input::InputKeyboard>& keyboard = inputServer->GetDefaultKeyboard();
			const Ptr<Input::InputMouse>& mouse = inputServer->GetDefaultMouse();

			if (keyboard->KeyDown(Input::InputKey::A))
			{
				if(mSelectActor.isvalid())
					mSelectActor->SetPosition(vector(mSelectActor->GetPosition().x()+2,mSelectActor->GetPosition().y(),
					mSelectActor->GetPosition().z()));
			}
			if(keyboard->KeyUp(Input::InputKey::D))		
			{
				if(mSelectActor.isvalid())
					mSelectActor->SetPosition(vector(mSelectActor->GetPosition().x()-2,mSelectActor->GetPosition().y(),
					mSelectActor->GetPosition().z()));
			}
			if(keyboard->KeyUp(Input::InputKey::Key0))
				SetKeyDown(0);
			if(keyboard->KeyUp(Input::InputKey::Key1))
				SetKeyDown(1);
			if(keyboard->KeyUp(Input::InputKey::Key2))
				SetKeyDown(2);
			if(keyboard->KeyUp(Input::InputKey::Key3))
				SetKeyDown(3);
			if(keyboard->KeyUp(Input::InputKey::Key4))
				SetKeyDown(4);
			if(keyboard->KeyUp(Input::InputKey::Key5))
				SetKeyDown(5);
			if(keyboard->KeyUp(Input::InputKey::Key6))
				SetKeyDown(6);
			if(keyboard->KeyUp(Input::InputKey::Key7))
				SetKeyDown(7);
			if(keyboard->KeyUp(Input::InputKey::Key8))
				SetKeyDown(8);
			if(keyboard->KeyUp(Input::InputKey::Key9))
				SetKeyDown(9);

			if(keyboard->KeyUp(Input::InputKey::End))
				SetKeyDown(keydown+1);
			if(keyboard->KeyUp(Input::InputKey::Home))
				SetKeyDown(keydown-1);
			//if(keyboard->KeyUp(Input::InputKey::Space))
			//	MessageBoxA(NULL,"GetSpace","",MB_OK);

			if(keyboard->KeyUp(Input::InputKey::P))
			{
				ChangeEmitter();
			}

			//const Ptr<Input::InputMouse>& mouse =  inputServer->GetDefaultMouse();
			if ( mouse->ButtonPressed( Input::InputMouseButton::LeftButton ) )
			{
				Math::float2 screenPos = mouse->GetScreenPosition();

				if (mSelectActor.isvalid() && mGyrostat->IsSelected())
				{
					// set move offset
					//float2 offsetPos(screenPos.x()-mMouseScreenPos.x(),screenPos.y()-mMouseScreenPos.y());
					if(screenPos.x()-mMouseScreenPos.x() > 0.001 || screenPos.y()-mMouseScreenPos.y() > 0.001 ||
						screenPos.x()-mMouseScreenPos.x()< -0.01 || screenPos.y()-mMouseScreenPos.y() < -0.001 )
					{	
						mGyrostat->MouseChanged(mMouseScreenPos,screenPos);
						mMouseScreenPos = screenPos;
					}


				}
			}
		}
		//check the response for WebCall
		if(bWebFunc)
			SetKeyDown(keydown);

		if(0)
		{
			ProcessAttach();
			UpdateLine();
			UpdateSoldier();
			UpdateWorldCoord();
			UpdateGyrostat();
			IntersectActor();
		}
		UpdateParticle();
		
		return this->GetName();
	}
}
//------------------------------------------------------------------------
void 
SimpleStateHandler::IntersectActor()
{
	Input::InputServer* pInputServer = Input::InputServer::Instance();
	// 得到鼠标点击的位置
	const Ptr<Input::InputMouse> mouse =  pInputServer->GetDefaultMouse();
	if ( !mouse->ButtonDown( Input::InputMouseButton::LeftButton ) )
	{

		if ( mSelectActor.isvalid() )
		{
			CreateOrUpdateBoundActor( mSelectActor );
		}
	}
	else
	{
		Math::float2 pixelPos = mouse->GetPixelPosition();	//	像素坐标
		Math::float2 screenPos = mouse->GetScreenPosition();	//	屏幕坐标 (0.0 .. 1.0)之间
		mMouseScreenPos = screenPos;

		Math::Ray worldRay;
		AppUtil::IntersectUtil::ComputeDefaultWorldMouseRay( screenPos, worldRay);

		AppUtil::IntersectResultList rsList;
		if( AppUtil::IntersectUtil::IntersectWorld(worldRay, 2, -1, false, rsList ) )
		{
			rsList.Sort();
			//temp set GyrostatGizmo 's layerid  6
			if (rsList[0].actor->GetLayerID() == 6)
				mGyrostat->SetSelectActor(rsList[0].actor);
			else
				mSelectActor = rsList[0].actor;
		}
		else
		{
			mSelectActor = NULL;
		}

		if ( mSelectActor.isvalid() )
		{
			const Ptr<Input::InputKeyboard> keyboard = pInputServer->GetDefaultKeyboard();
			bool bIsKeyDown = keyboard->KeyPressed(Input::InputKey::Control);
			if ( bIsKeyDown )
			{
				mShowAll = true;

				while( mSelectActor->GetParent() != mScene->GetRoot() )
				{
					mSelectActor = mSelectActor->GetParent();
				}		
			}
			else
			{
				mShowAll = false;
			}
		}
		else
		{
			mShowAll = false;
		}


		mGyrostat->SetParent(mSelectActor);
		CreateOrUpdateBoundActor( mSelectActor );

	}
}
//------------------------------------------------------------------------
void 
SimpleStateHandler::UpdateLine(void)
{

	if( !mLineActor.isvalid() )
		return;

	//mIntersectBoundActor->SetTransform( worldM );
	Ptr<MeshRenderComponent> mesh = mLineActor->FindComponent( MeshRenderComponent::RTTI ).downcast<MeshRenderComponent>();

	Ptr<Resources::MeshRes> meshRes = mesh->GetMesh();
	n_assert( mLineActor );
	static float s_angle = 0.0;

	Resources::PositionData postion;
	postion.Append( Math::float3(0.0, 0.0, 0.0 ) );
	
	float3 f;
	f.x() = Math::n_cos( s_angle )  * 5.0f;
	f.y() = 5.0;
	f.z() = Math::n_sin( s_angle ) * 5.0f;

	postion.Append( f );

	AppUtil::ManuResUtil::SetMesh_LineList( meshRes, postion.Size(), &postion[0], Math::Color32(0,255,0,0) );
	
	s_angle += 0.0001;
}
//------------------------------------------------------------------------
void 
SimpleStateHandler::UpdateSoldier(void)
{
	if ( !mSoldierActor.isvalid() )
	{
		return;
	}
	//const Ptr<Component>& comObj = mSoldierActor->FindComponent( MeshComponent::RTTI );
	//Ptr<MeshComponent> meshComponent = comObj.downcast<MeshComponent>();
	//SizeT cn = meshComponent->GetMesh()->GetSubMeshCount();
	//SizeT indexcount = meshComponent->GetMesh()->GetIndexCount();
	//Resources::SubMesh* sub = meshComponent->GetMesh()->GetSubMesh(1);

	static float s_change = 0.001;
	static float s_cur = 1.0;
	if ( s_cur < 1.0 )
	{
		s_change = 0.001;
	}
	else if ( s_cur > 3.0)
	{
		s_change = -0.001;
	}

	
	s_cur += s_change;
	

	mSoldierActor->SetScale( Math::vector( s_cur, s_cur, s_cur) );
}
//------------------------------------------------------------------------
void 
SimpleStateHandler::CreateOrUpdateBoundActor(const Ptr<Actor>& actor )
{
	if ( actor.isvalid() )
	{
		Math::matrix44 worldM = Math::matrix44::identity();

		Math::bbox worldBB;

		if ( mShowAll )
		{
			worldBB = actor->GetWorldBoundingBoxWithChild();
		}
		else
		{
			worldBB = actor->GetWorldBoundingBox();
		}

		// has intersect actor
		if ( mIntersectBoundActor.isvalid() )
		{
			mIntersectBoundActor->SetTransform( worldM );
			Ptr<MeshRenderComponent> mesh = mIntersectBoundActor->FindComponent( MeshRenderComponent::RTTI ).downcast<MeshRenderComponent>();

			Ptr<Resources::MeshRes> meshRes = mesh->GetMesh();
			AppUtil::ManuResUtil::SetMesh_LineList_Box(meshRes, worldBB, Math::Color32(255,0,0,0)  );
		}
		else
		{
			Ptr<Actor> pActor = Actor::Create();
			pActor->SetName("IntersectBox");
			pActor->SetLayerID(0);
			pActor->SetTransform( worldM );

			Ptr<Resources::MeshRes> mesh = AppUtil::ManuResUtil::CreateManuMesh_LineList_Box( "Intersect_Box_Mesh", worldBB, Math::Color32(255,0,0,0) );
			n_assert( mesh.isvalid() );

			Ptr<MeshComponent> pMeshComponent = MeshComponent::Create();
			pMeshComponent->SetMeshID( mesh->GetResourceId() );

			Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
			pMeshRenderComponent->SetShaderID(0, "shd:Line.wjshader");

			pActor->AttachComponent( pMeshComponent.upcast<Component>() );
			pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

			mIntersectBoundActor = pActor;
			mIntersectBoundActor->Active();
		}
	}
	else
	{
		// no actor intersect
		if( mIntersectBoundActor.isvalid() )
		{
			mIntersectBoundActor->Destory();
			mIntersectBoundActor = NULL;
		}

	}

}
void 
SimpleStateHandler::ProcessAttach()
{
	Input::InputServer* pInputServer = Input::InputServer::Instance();

	const Ptr<Input::InputKeyboard> keyboard = pInputServer->GetDefaultKeyboard();
	if ( keyboard->KeyDown(Input::InputKey::T) )
	{
		if ( mSoldierActorChild->GetParent() == mSoldierActor.get() )
		{
			mScene->AddActor( mSoldierActorChild );
		}
		else
		{
			mSoldierActor->AddChild( mSoldierActorChild );
		}

	}
}
//-------------------------------------------------------------------------------------
void SimpleStateHandler::UpdateWorldCoord(void)
{
	if (mWorldCoordsActor.isvalid())
	{
		//get camera
		//get ray		
		const Ptr<Actor>& pActorCam = GraphicsFeature::Instance()->GetDefaultCameraActor();
		if ( !pActorCam.isvalid() )
			return ;

		const Ptr<Component> obj = pActorCam->FindComponent( CameraComponent::RTTI );
		n_assert( obj.isvalid() );
		Ptr<CameraComponent> pCamera = obj.downcast<CameraComponent>();

		const Math::matrix44& viewM = pCamera->GetViewMat();
		Math::matrix44 invViewM = Math::matrix44::inverse(viewM);

		const Math::matrix44& projM = pCamera->GetProjMat();
		Math::matrix44 invProjM = Math::matrix44::inverse(projM);

		float nearWidth, nearHeight, nearPlaneDis, farPlaneDis;
		pCamera->GetProjParam( &nearWidth , &nearHeight , &nearPlaneDis , &farPlaneDis );

		Math::Ray worldray = AppUtil::MouseRayUtil::ComputeWorldMouseRay(Math::float2(0.05,0.05), invViewM, invProjM, nearPlaneDis );
		Math::float3 nearPoint =  worldray.PointAt(nearPlaneDis * 1.1f);
	
		mWorldCoordsActor->SetPosition(Math::vector(nearPoint.x(),nearPoint.y(),nearPoint.z()));
		mWorldCoordsActor->SetScale(Math::vector(0.01,0.01,0.01));
	}
}
//-------------------------------------------------------------------------------------
void 
SimpleStateHandler::UpdateGyrostat()
{
	if (mGyrostat == NULL)
		mGyrostat = new Gyrostat();

	mGyrostat->UpdateActors();
}
//---------------------------------------------------------------------------------
void SimpleStateHandler::ChangeEmitter()
{
	if (!ParSystem.isvalid())
		return;

	Ptr<ParticleTechnique> pTech =  ParSystem->GetTechnique(0);
	Ptr<ParticleEmitter> pEmit = pTech->GetEmitter(0);
	{
		//RemoveEmitter(0);
		Ptr<PointEmitter> emitter = PointEmitter::Create();
		pTech->AddEmitter(emitter.get(),0);
		pTech->RemoveEmitter(pEmit);

	}

}
//--------------------------------------------------------------------
void SimpleStateHandler::SetKeyDown(int key)
{
	keydown = (key>21)?0:(key<0?21:key);

	bWebFunc = false;

	if(ParticleAct.isvalid())
	{
		mScene->RemoveActor(ParticleAct);
		ParticleAct->Destory();
	}

	outlist.Clear();
	/*
	if(1)
	{
		//create from particle's template file
		ParticleServer::Instance()->OpenTemplateFile("atp:system_"+Util::String::FromInt(keydown)+".par");
		ParSystem = ParticleServer::Instance()->CreateFromTemplate("system_"+Util::String::FromInt(keydown));
		InitParticleTexutre(outlist);
	}
	else
	{
		//create from code generate
		createParSys(outlist);
		ParticleServer::Instance()->RemoveTemplate("system_"+Util::String::FromInt(keydown-1));
		ParticleServer::Instance()->AddTemplate( ParSystem->GetName(), ParSystem );
		ParticleServer::Instance()->SaveTemplateFile("atp:system_"+Util::String::FromInt(keydown)+".par");
	}

	CreateParticleFromTemplate();
	*/

}

//------------------------------------------------------------------------------
void SimpleStateHandler::CreateParticleFromTemplate()
{

	if (!ParSystem.isvalid())
		return;

	ParticleAct = Actor::Create();	

	if (keydown == 20 || keydown == 8 || keydown == 5 || keydown == 17 || keydown == 10)
		ParticleAct->SetPosition( Math::vector(0, 0, 0.0 ));
	else
		ParticleAct->SetPosition( Math::vector(0, 5, 0.0 ));

	//if ( keydown == 6 )
	//	ParticleAct->SetRotation(Math::quaternion(1,0,0,1));
	//if ( keydown == 12 )
	//	ParticleAct->SetScale(Math::vector(2,2,2));
	//mGyrostat->SetParent(ParticleAct);

	Ptr<ParticleComponent> pParticleComponent = ParticleComponent::Create();
	Ptr<ParticleRenderComponent> pParticleRender = ParticleRenderComponent::Create();

	pParticleComponent->SetParticleSystem(ParSystem);
	SizeT sizeLen = ParSystem->GetTechniqueCount();

	if (keydown == 20)
	{
		Ptr<MeshComponent> pMeshComponent = MeshComponent::Create();
		pMeshComponent->SetMeshID( "msh:Monster_000_yanlong_001.mesh" );
		ParticleAct->AttachComponent( pMeshComponent.upcast<Component>() );

		for (int i = 0;i < outlist.Size(); i++)
		{
			pParticleRender->SetShaderID(i, "shd:particle_transDragon.wjshader");
			pParticleRender->_AddTextureParam(i, "_diffuseMap", "sys:white.dds");
			n_assert( pParticleRender->HasTextureParam(i, "_diffuseMap") );
			pParticleRender->SetTexture(i, "_diffuseMap", outlist[i] );
		}
	}
	else if (keydown == 21)
	{
		/***/
		//ParticleAct->SetScale(Math::vector(10,10,10));
		Resources::PositionData position;
		Resources::Index16Container indicies;
		Resources::TexCoordData texcoors;

		//ParticleAct->SetScale(vector(10,10,10));
		Math::scalar xScale = ParticleAct->GetScale().x();
		Math::scalar yScale = ParticleAct->GetScale().y();
		Math::scalar iScale = 1;
		position.Append(Math::float3(-1* iScale * xScale ,iScale * yScale,0));
		position.Append(Math::float3(iScale * xScale,iScale * yScale,0));
		position.Append(Math::float3(iScale * xScale,-1*iScale * yScale,0));
		position.Append(Math::float3(-1*iScale * xScale,-1*iScale * yScale,0));

		indicies.Append(0);
		indicies.Append(1);
		indicies.Append(2);

		indicies.Append(2);
		indicies.Append(3);
		indicies.Append(0);

		texcoors.Append(Math::float2(0,0));
		texcoors.Append(Math::float2(0,1));
		texcoors.Append(Math::float2(1,1));
		texcoors.Append(Math::float2(1,0));

		Math::Color32 color(0,0,0,0);

		Ptr<Resources::MeshRes> mesh = AppUtil::ManuResUtil::CreateManuMesh_WithTopology( "vortexWall", position.Size(),	&position[0], color, indicies.Size(),&indicies[0]);
		n_assert( mesh.isvalid() );
		mesh->SetVertexData<Resources::TexCoordData>( &texcoors[0], 4 );		

		/***/
		Ptr<MeshComponent> pMeshComponent = MeshComponent::Create();
		pMeshComponent->SetMeshID( mesh->GetResourceId() );
		ParticleAct->AttachComponent( pMeshComponent.upcast<Component>() );

		//--------add matrials
		for (int i = 0;i < outlist.Size(); i++)
		{
			pParticleRender->SetShaderID(i, "sys:Template_Surface_AlphaBlend_Diffuse_Filter.wjshader");
			pParticleRender->SetTransParamName("_alphaFileter");
			pParticleRender->SetShaderConstantParam(i,"_alphaFileter",float4(0,0,0,1));

			if( i == 1)
				pParticleRender->SetShaderConstantParam(i,"_colorFileter",float4(1,0,1,1));
			else				
				pParticleRender->SetShaderConstantParam(i,"_colorFileter",float4(1,1,1,1));

			pParticleRender->_AddTextureParam(i, "_diffuseMap", "sys:white.dds");
			n_assert( pParticleRender->HasTextureParam(i, "_diffuseMap") );
			pParticleRender->SetTexture(i, "_diffuseMap", outlist[i] );

		}
	}
	else
	{
		for (int i = 0;i < outlist.Size(); i++)
		{
			if(keydown == 8 || keydown == 6 || keydown == 19 || keydown == 10)
				pParticleRender->SetShaderID(i, "sys:Template_Surface_AlphaBlend_Diffuse_Src_InvSrc.wjshader");//
			//else if ( keydown == 15)
			//	pParticleRender->SetShaderID(i, "shd:Particle_InvDestSrc.wjshader");
			else
				pParticleRender->SetShaderID(i, "sys:Template_Surface_AlphaBlend_Diffuse_One_One.wjshader");

			pParticleRender->_AddTextureParam(i, "_diffuseMap", "sys:white.dds");
			n_assert( pParticleRender->HasTextureParam(i, "_diffuseMap") );
			pParticleRender->SetTexture(i, "_diffuseMap", outlist[i] );

		}
	}
	
	ParticleAct->AttachComponent( pParticleComponent.upcast<Component>() );
	ParticleAct->AttachComponent( pParticleRender.upcast<Component>() );
	ParticleAct->Active();
	mScene->AddActor( ParticleAct );


}
//--------------------------------------------------------------
void SimpleStateHandler::UpdateParticle()
{
	Timing::Time time = GameTime::Instance()->GetFrameTime();
	curTime += time;

	if(curTime >= 0.5)
		curTime = 0;

	if(curTime != 0)
		return;


	if (keydown == 7)
	{

		Math::float3 curPos(Math::n_rand(2,-2),Math::n_rand(2,-2),Math::n_rand(2,-2));
		ParSystem->DeActive();

		for (IndexT i = 0; i<2; i++)
		{
			ParticleTechniquePtr pParTech = ParSystem->GetTechnique(i);
			ParticleEmitterPtr pParEmitter = pParTech->GetEmitter(0);
			Math::float3 emitPos = pParEmitter->GetPosition();
			emitPos += curPos;
			pParEmitter->SetPosition( emitPos );
		}
		ParSystem->Active();
	}

}
/************************************************************************/
void SimpleStateHandler::InitParticleTexutre(Util::Array<Util::StringAtom>& outarray)
{
	switch(keydown)
	{
		/***/
	default:
	case 0:
		{
			outarray.Append("tex:pu_fire_01.png");	
			outarray.Append("tex:pu_smoke_02_128x128.png");
		}

	case 1:
		{
			outarray.Append("tex:pump_flame_01.png");
		}
		break;
	case 2:
		{
			outarray.Append("tex:pu_ribbontrail.png");
		}
		break;
	case 3:
		{
			outarray.Append("tex:pu_beam_02.png");
		}
		break;

	case 4:
		{
			outarray.Append("tex:pu_rain_01_small.png");
		}
		break;
	case 5:
		{
			outarray.Append("tex:pump_flame_01.png");
		}
		break;
	case 6:
		{
			outarray.Append("tex:pump_snow_01.png");
		}
		break;
	case 7:
		{
			outarray.Append("tex:pu_beam_01.png");
			outarray.Append("tex:pu_beam_02.png");
		}
		break;
	case 8:
		{
			outarray.Append("tex:pu_sand_256.png");
		}
		break;
	case 9:
		{
			outarray.Append("tex:pump_streak_04.png");
		}
		break;
	case 10:
		{
			outarray.Append("tex:pump_smoke_07.png");
		}
		break;
	case 11:
		{
			outarray.Append("tex:pu_flare.png");
		}
		break;
	case 12:
		{
			outarray.Append("tex:pump_star_05.png");
		}
		break;
	case 13:
		{
			outarray.Append("tex:pu_dot_03_64x64.png");
		}
		break;

	case 14:
		{
			outarray.Append("tex:pu_flare_12_256x256.png");
		}
		break;


	case 15:
		{
			outarray.Append("tex:pu_flare_12_256x256.png");
			outarray.Append("tex:pump_streak_04.png");
		}
		break;
	case 16:
		{
			outarray.Append("tex:pump_swirl_01.png");
		}
		break;

	case 17:
		{				
			outarray.Append("tex:pu_flare.png");
		}
		break;

	case 18:
		{				
			outarray.Append("tex:pump_hourglass.png");				
			outarray.Append("tex:pump_watch.png");				
		}
		break;
	case 19:
		{
			outarray.Append("tex:interpolate.png");				
		}
		break;
	case 20:
		{				
			outarray.Append("tex:Monster_000_yanlong_001.dds");
		}
		break;
	case 21:
		{

			outarray.Append("tex:vortex.bmp");			
			outarray.Append("tex:circle.bmp");			
			outarray.Append("tex:lightning.bmp");
		}
		break;
	}

}

//---------------------------------------------------------------------------------
void SimpleStateHandler::createParSys(Util::Array<Util::StringAtom>& outarray)
{

	ParSystem = ParticleSystem::Create();
	
	switch(keydown)
	{
		/***/
	default:
	case 0:
		{
			
			Ptr<ParticleTechnique> technique0 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> emitter = PointEmitter::Create();
				emitter->SetMaxEmitDirection(N_PI_DOUBLE);
				//emitter->SetForceEmission(true);
				emitter->SetSize(Math::float3(6,6,6),Math::float3(6,6,6));
				emitter->SetEmitRate(150,150);
				emitter->SetVelocity(Math::float3(3,3,3),Math::float3(3,3,3));
				emitter->SetLiveTime(5,6);
				emitter->SetColor(Math::ColorF(1,0.5,0.3,1),Math::ColorF(1,0.5,0.3,1));
				emitter->SetSize(Math::float3(0.1,1,0),Math::float3(0.1,1,0));

				//Ptr<BoxEmitter> boxEmitter = BoxEmitter::Create();
				//boxEmitter->SetMaxEmitRate(300);
				//boxEmitter->SetSize(Math::float3(0.2,10,1),Math::float3(0.2,10,1));
				////emitter->SetColor(Math::ColorF(0.3,0.3,0.3,0.3),Math::ColorF(0.3,0.3,0.3,0.3));
				////emitter->SetVelocity(float3(100,-500,0), float3(100,-500,0));
				////emitter->SetDirection(Math::float3(0.2,1,0));
				//boxEmitter->SetLiveTime(1,2);
				//boxEmitter->SetEmitDirection(0,0);
				//boxEmitter->SetXRange(50);
				//boxEmitter->SetYRange(1);
				//boxEmitter->SetZRange(50);
				//technique0->AddEmitter( boxEmitter.get() );

				Ptr<JetAffector> jetAffector = JetAffector::Create();
				jetAffector->SetAcceleration(Math::float2(-1,-1));
				//jetAffector->SetDelayTime(0.5);
				technique0->AddAffector(jetAffector.get());

				Ptr<LinearForceAffector> lfAffector = LinearForceAffector::Create();
				lfAffector->SetForceVector(Math::float3(0,-3,0));
				technique0->AddAffector( lfAffector.get() );	

				Ptr<ScaleAffector> scaleAft = ScaleAffector::Create();
				scaleAft->SetScaleYSet(true);
				scaleAft->SetScaleY(-0.2);
				technique0->AddAffector( scaleAft.get() );	

				Ptr<ColorAffector> colorAft = ColorAffector::Create();
				colorAft->AddColor(0.0,Math::ColorF(1,0.5,0.3,1));
				colorAft->AddColor(0.8,Math::ColorF(0.18,0.05,0.01,1));
				colorAft->AddColor(1,Math::ColorF(0,0,0,1));
				colorAft->SetColorOperation(ColorAffector::CAO_SET);
				technique0->AddAffector( colorAft.get() );

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_ORIENTED_SELF);
				//target->SetOrientType(ParticleBillBoardTarget::OT_ORIENTED_SELF);
				technique0->AddEmitter( emitter.get() );
				technique0->SetTarget( target.get() );	
				technique0->SetParticleQuota(50);
			}
			ParSystem->AddTechnique( technique0 );
			outarray.Append("tex:pu_fire_01.png");		
			

			Ptr<ParticleTechnique> technique0_2 = ParticleTechnique::Create();
			{
				Ptr<SphereSurfaceEmitter> emitter = SphereSurfaceEmitter::Create();	
				emitter->SetRadius(1);
				emitter->SetMaxEmitDirection(N_PI_DOUBLE);
				//emitter->SetEmitRate(3,3);
				emitter->SetLiveTime(6,7);
				emitter->SetMaxVelocity(Math::float3(1,1,1));
				emitter->SetSize(Math::float3(1,1,1),Math::float3(2,2,2));
				emitter->SetColor(Math::ColorF(1,0.5,0.3,1),Math::ColorF(1,0.5,0.3,1));
				emitter->SetMinRotationSpeed(1);
				emitter->SetOrientation(Math::quaternion(0.1,0.1,0.1,1),Math::quaternion(1,1,1,1));

				//Ptr<ParticleAffector> pAffector = ParticleAffector::Create();
				//technique0_2->AddAffector( pAffector.get() );

				Ptr<ScaleAffector> scaleAffector = ScaleAffector::Create();
				scaleAffector->SetScaleXYZ(2);
				scaleAffector->SetScaleXYZSet(true);
				technique0_2->AddAffector(scaleAffector.get());

				//Ptr<TextureRotatorAffector> trAffector = TextureRotatorAffector::Create();	
				//trAffector->SetRandomRotation(-50,0);
				//technique0_2->AddAffector( trAffector.get() );

				Ptr<ColorAffector> colorAft = ColorAffector::Create();
				colorAft->AddColor(0.0,Math::ColorF(1,0.6,0.3,1));
				colorAft->AddColor(0.5,Math::ColorF(0.18,0.05,0.01,1));
				colorAft->AddColor(1,Math::ColorF(0,0,0,1));
				colorAft->SetColorOperation(ColorAffector::CAO_SET);
				technique0_2->AddAffector( colorAft.get() );

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				//target->SetOrientType(ParticleBillBoardTarget::OT_ORIENTED_SELF);
				technique0_2->AddEmitter( emitter.get() );
				technique0_2->SetTarget( target.get() );	
				technique0_2->SetParticleQuota(10);
			}
			ParSystem->AddTechnique( technique0_2 );
			outarray.Append("tex:pu_smoke_02_128x128.png");

			ParSystem->SetName(Util::String("system_0"));
			
		}
		break;
		/***/
	case 1:
		{
			// technique 1
			Ptr<ParticleTechnique> technique1 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> emitter = PointEmitter::Create();
				emitter->SetMaxEmitDirection(N_PI_DOUBLE);
				//emitter->SetAutoDirection(true);
				emitter->SetMaxVelocity(float3(5,5,5));
				//emitter->SetPosition(float3(0,100,-150));
				//emitter->SetLiveTime(100,100);


				Ptr<LinearForceAffector> lfAffector = LinearForceAffector::Create();
				lfAffector->SetForceVector(Math::float3(0,3,0));
				technique1->AddAffector( lfAffector.get() );

				Ptr<ParticleTrailTarget> target = ParticleTrailTarget::Create();
				target->SetMaxElements(30);
				//Ptr<ParticleBeamTarget> target = ParticleBeamTarget::Create();
				technique1->AddEmitter( emitter.get() );
				technique1->SetTarget( target.get() );	
				technique1->SetParticleQuota(5);
			}
			ParSystem->AddTechnique( technique1 );
			outarray.Append("tex:pump_flame_01.png");
			ParSystem->SetName(Util::String("system_1"));

		}
		break;
	case 2:
		{
			//---
			Ptr<ParticleTechnique> technique2 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> pEmitter = PointEmitter::Create();
				pEmitter->SetMinColor(Math::ColorF(0,0,0,0));
				pEmitter->SetMaxEmitDirection(N_PI_DOUBLE);
				//pEmitter->SetSize(Math::float3(0.5,0.5,0.5),Math::float3(0.5,0.5,0.5));
				pEmitter->SetLiveTime(40,40);
				pEmitter->SetVelocity(Math::float3(150,150,150),Math::float3(150,150,150));
				technique2->AddEmitter(pEmitter.get());

				Ptr<ParticleAffector> aft = ParticleAffector::Create();
				technique2->AddAffector(aft.get());

				Ptr<GravityAffector> pGraAft = GravityAffector::Create();
				pGraAft->SetGravity(9000);
				pGraAft->SetGravityPosition(Math::float3(0,20,10));
				technique2->AddAffector( pGraAft.get() );

				Ptr<GravityAffector> pGraAft2 = GravityAffector::Create();
				pGraAft2->SetGravity(9000);
				pGraAft2->SetGravityPosition(Math::float3(0,-20,-10));
				technique2->AddAffector( pGraAft2.get() );

				Ptr<RibbonTrailTarget> target = RibbonTrailTarget::Create();
				target->SetMaxElements(20);			
				target->SetTrailLength(50);
				technique2->SetTarget( target.get() );	
				technique2->SetParticleQuota(50);
			}
			ParSystem->AddTechnique( technique2 );
			outarray.Append("tex:pu_ribbontrail.png");
			ParSystem->SetName(Util::String("system_2"));
		}
		break;
	case 3:
		{
			//---
			Ptr<ParticleTechnique> technique3 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> pEmitter = PointEmitter::Create();
				pEmitter->SetSize(Math::float3(0.2,1,1),Math::float3(0.2,1,1));
				pEmitter->SetMaxEmitDirection(N_PI/4);
				pEmitter->SetOrientation(Math::quaternion(0.3,0.3,0.3,1),Math::quaternion(0.3,0.3,0.3,1));
				pEmitter->SetRotationSpeed(5,5);
				pEmitter->SetLiveTime(40,40);
				pEmitter->SetVelocity(Math::float3(10,10,10),Math::float3(10,10,10));
				pEmitter->SetPosition(Math::float3(0,5,0));
				pEmitter->SetColor(Math::ColorF(0.82,1,0.51,1),Math::ColorF(0.82,1,0.51,1));
				technique3->AddEmitter(pEmitter.get());

				Ptr<ParticleAffector> aft = ParticleAffector::Create();
				technique3->AddAffector(aft.get());

				Ptr<ParticleFollowerAffector> pfAft = ParticleFollowerAffector::Create();
				pfAft->SetMaxDistance(30);
				technique3->AddAffector(pfAft.get());

				Ptr<GravityAffector> pGraAft = GravityAffector::Create();
				pGraAft->SetGravity(90);
				pGraAft->SetGravityPosition(Math::float3(-5,5,0));
				technique3->AddAffector( pGraAft.get() );

				Ptr<GravityAffector> pGraAft2 = GravityAffector::Create();
				pGraAft2->SetGravity(90);
				pGraAft2->SetGravityPosition(Math::float3(5,-5,0));
				technique3->AddAffector( pGraAft2.get() );

				Ptr<ParticleBeamTarget> target = ParticleBeamTarget::Create();
				target->SetSegmentsCount(4);
				target->SetMaxElements(20);
				target->SetFlatness(true);
				technique3->SetTarget( target.get() );	
				technique3->SetParticleQuota(30);

			}
			ParSystem->AddTechnique( technique3 );
			outarray.Append("tex:pu_beam_02.png");
			ParSystem->SetName(Util::String("system_3"));
		}
		break;
	
	case 4:
		{
			// technique 3
			Ptr<ParticleTechnique> technique4 = ParticleTechnique::Create();
			{
				Ptr<BoxEmitter> emitter = BoxEmitter::Create();
				emitter->SetMaxEmitRate(300);
				emitter->SetSize(Math::float3(0.2,10,1),Math::float3(0.2,10,1));
				//emitter->SetColor(Math::ColorF(0.3,0.3,0.3,0.3),Math::ColorF(0.3,0.3,0.3,0.3));
				//emitter->SetVelocity(float3(100,-500,0), float3(100,-500,0));
				//emitter->SetDirection(Math::float3(0.2,1,0));
				emitter->SetLiveTime(1,2);
				emitter->SetEmitDirection(0,0);
				emitter->SetXRange(50);
				emitter->SetYRange(1);
				emitter->SetZRange(50);

				Ptr<ColorAffector> coloraffector = ColorAffector::Create();
				coloraffector->AddColor(0.0,Math::ColorF(0,0,0,0));
				coloraffector->AddColor(0.2,Math::ColorF(0.3,0.3,0.3,0.3));
				coloraffector->AddColor(1,Math::ColorF(0.3,0.3,0.3,0.3));
				technique4->AddAffector( coloraffector.get() );

				Ptr<ParticleAffector> aft = ParticleAffector::Create();
				technique4->AddAffector(aft);

				Ptr<LinearForceAffector> lfaf = LinearForceAffector::Create();
				lfaf->SetForceVector(Math::float3(100,-500,0));
				lfaf->SetTimeStep(0);
				technique4->AddAffector(lfaf.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_ORIENTED_SELF);
				target->SetTexCoordSize(2, 1);
				
				technique4->AddEmitter( emitter.get() );
				technique4->SetTarget( target.get() );	
				technique4->SetPosition( Math::float3(0, 80, 0.0 ) );
				technique4->SetParticleQuota(2000);
			}
			ParSystem->AddTechnique( technique4 );
			outarray.Append("tex:pu_rain_01_small.png");
			ParSystem->SetName(Util::String("system_4"));
		}
		break;
	case 5:
		{
			// technique 5
			Ptr<ParticleTechnique> technique5 = ParticleTechnique::Create();
			{
				Ptr<CircleEmitter> emitter = CircleEmitter::Create();
				//emitter->SetVelocity(float3(0,50,0), float3(0,50,0));
				emitter->SetRadius(5);
				emitter->SetMaxEmitRate(450);
				emitter->SetLiveTime(2,3);
				technique5->AddEmitter( emitter.get() );


				Ptr<ParticleFollowerAffector> parfollowAffector = ParticleFollowerAffector::Create();
				//parfollowAffector->SetMinDistance(3);
				parfollowAffector->SetMaxDistance(30);
				technique5->AddAffector( parfollowAffector.get() );

				Ptr<LinearForceAffector> lfaf = LinearForceAffector::Create();
				lfaf->SetForceVector(Math::float3(0,10,0));
				technique5->AddAffector(lfaf.get());

				Ptr<ScaleAffector> scaleAffector = ScaleAffector::Create();
				scaleAffector->SetScaleXYZ(2);
				scaleAffector->SetScaleXYZSet(true);
				technique5->AddAffector(scaleAffector.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique5->SetTarget( target.get() );	

				//technique5->SetPosition( Math::float3(20, 0, 0.0 ) );
				technique5->SetParticleQuota(1000);
			}

			ParSystem->AddTechnique( technique5 );
			outarray.Append("tex:pump_flame_01.png");

			ParSystem->SetName(Util::String("system_5"));
		}
		break;
	case 6:
		{
			// technique 7
			Ptr<ParticleTechnique> technique6 = ParticleTechnique::Create();
			{
				Ptr<BoxEmitter> emitter = BoxEmitter::Create();
				emitter->SetMaxEmitRate(500);
				//emitter->SetVelocity(Math::float3(0,1,0), Math::float3(0,1,0));
				emitter->SetVelocity(float3(0,-3,1), float3(-1,-4,-1));
				//emitter->SetLiveTime(5,6);
				emitter->SetEmitDirection(-1,-1);
				//emitter->SetDirection(float3(0.2,-1,0));
				emitter->SetXRange(50);
				emitter->SetYRange(1);
				emitter->SetZRange(50);
				emitter->SetLiveTime(5,6);


				Ptr<LinearForceAffector> lfaf = LinearForceAffector::Create();
				lfaf->SetForceVector(Math::float3(10,-20,0));
				lfaf->SetTimeStep(0.1);
				technique6->AddAffector(lfaf.get());

				//Ptr<LinearForceAffector> rfaf = LinearForceAffector::Create();
				//rfaf->SetForceVector(Math::float3(-400,0,0));
				//rfaf->SetTimeStep(0.3);
				//technique7->AddAffector(rfaf.get());

				Ptr<TextureRotatorAffector> lfAffector = TextureRotatorAffector::Create();	
				//lfAffector->SetUseOwnRotationSpeed(true);
				lfAffector->SetRandomRotation(2,3);
				technique6->AddAffector( lfAffector.get() );

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera_Y);
				technique6->AddEmitter( emitter.get() );
				technique6->SetTarget( target.get() );	
				technique6->SetParticleQuota(4000);
				technique6->SetPosition(Math::float3(0,10,0));
			}
			ParSystem->AddTechnique( technique6 );
			outarray.Append("tex:pump_snow_01.png");
			ParSystem->SetName(Util::String("system_6"));
		}
		break;
	case 7:
		{
			// technique 1
			Ptr<ParticleTechnique> technique7_1 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> emitter = PointEmitter::Create();
				//emitter->SetMaxEmitDirection(N_PI_DOUBLE);
				//emitter->SetAutoDirection(true);
				emitter->SetPosition(float3(10,1,-10));
				//emitter->SetLiveTime(100,100);

				Ptr<ParticleBeamTarget> target = ParticleBeamTarget::Create();
				//target->SetUpdateInterval(0.2);
				technique7_1->AddEmitter( emitter.get() );
				technique7_1->SetTarget( target.get() );	
				technique7_1->SetParticleQuota(2);
			}
			ParSystem->AddTechnique( technique7_1 );

			Ptr<ParticleTechnique> technique7_2 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> emitter = PointEmitter::Create();

				emitter->SetPosition(float3(10,1,-10));
				emitter->SetSize(float3(0.2,0.2,0.2),float3(0.2,0.2,0.2));

				Ptr<ParticleBeamTarget> target = ParticleBeamTarget::Create();
				target->SetFlatness(false);
				technique7_2->AddEmitter( emitter.get() );
				technique7_2->SetTarget( target.get() );	
				technique7_2->SetParticleQuota(2);
			}
			ParSystem->AddTechnique( technique7_2 );
			outarray.Append("tex:pu_beam_01.png");
			outarray.Append("tex:pu_beam_02.png");
			ParSystem->SetName(Util::String("system_7"));
		}
		break;
	case 8:
		{
			// technique 1
			Ptr<ParticleTechnique> technique8 = ParticleTechnique::Create();
			{
				Ptr<CircleEmitter> emitter = CircleEmitter::Create();
				emitter->SetRadius(1);
				emitter->SetMaxSize(Math::float3(2,2,2));
				//emitter->SetMaxEmitRate(450);
				emitter->SetLiveTime(3,3);
				emitter->SetVelocity(Math::float3(10,10,10),Math::float3(15,15,15));
				emitter->SetMaxEmitDirection(0);
				technique8->AddEmitter( emitter.get() );


				//Ptr<TextureRotatorAffector> lfAffector = TextureRotatorAffector::Create();	
				////lfAffector->SetUseOwnRotationSpeed(true);
				//lfAffector->SetRandomRotation(10,20);
				//technique9->AddAffector( lfAffector.get() );

				Ptr<ColorAffector> colorAft = ColorAffector::Create();
				Math::ColorF color(1,1,1,1);
				colorAft->AddColor(0.0,0.5f * color);
				colorAft->AddColor(0.2,0.4f * color);
				colorAft->AddColor(0.4,0.3f * color);
				colorAft->AddColor(0.6,0.2f * color);
				colorAft->AddColor(0.8,0.1f * color);
				colorAft->AddColor(1.0,0.0f * color);
				colorAft->SetColorOperation(ColorAffector::CAO_MULTIPLY);
				technique8->AddAffector( colorAft.get() );

				Ptr<VortexAffector> vortAft = VortexAffector::Create();
				vortAft->SetRotationVector(Math::vector(0,1,0) );
				vortAft->SetRotationSpeed(3);
				technique8->AddAffector(vortAft.get());

				Ptr<ScaleAffector> scaleAffector = ScaleAffector::Create();
				scaleAffector->SetScaleXYZ(4.5);
				scaleAffector->SetScaleXYZSet(true);
				technique8->AddAffector(scaleAffector.get());


				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique8->AddEmitter( emitter.get() );
				technique8->SetTarget( target.get() );	
				technique8->SetParticleQuota(1000);

			}
			ParSystem->AddTechnique( technique8 );

			outarray.Append("tex:pu_sand_256.png");
			ParSystem->SetName(Util::String("system_8"));

		}
		break;
	case 9:
		{
			//---
			Ptr<ParticleTechnique> technique9 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> emitter = PointEmitter::Create();
				emitter->SetVelocity(Math::float3(1,1,1),Math::float3(2,2,2));
				emitter->SetMaxEmitDirection(N_PI);
				emitter->SetColor(Math::ColorF(1,0.84,0.45,1), Math::ColorF(1,0.84,0.45,1));
				emitter->SetSize(Math::float3(1,20,20), Math::float3(1,20,20));
				emitter->SetLiveTime(1,3);
				technique9->AddEmitter(emitter.get());

				//Ptr<ParticleAffector> affect = ParticleAffector::Create();				
				//technique13->AddAffector(affect.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_ORIENTED_SELF);
				technique9->SetTarget( target.get() );	
				technique9->SetParticleQuota(100);
			}
			ParSystem->AddTechnique( technique9 );
			outarray.Append("tex:pump_streak_04.png");
			ParSystem->SetName(Util::String("system_9"));
		}
		break;
	case 10:
		{
			//---
			Ptr<ParticleTechnique> technique10 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> pEmitter = PointEmitter::Create();
				pEmitter->SetSize(Math::float3(4,4,4),Math::float3(4,4,4));
				pEmitter->SetMaxEmitDirection(N_PI/6);
				pEmitter->SetVelocity(Math::float3(0.9,1.4,0.9),Math::float3(0.9,1.4,0.9));
				technique10->AddEmitter(pEmitter.get());

				Ptr<ScaleAffector> sAft = ScaleAffector::Create();
				sAft->SetScaleXYZ(0.6);
				sAft->SetScaleXYZSet(true);
				technique10->AddAffector(sAft.get());

				Ptr<ParticleFollowerAffector> pfAft = ParticleFollowerAffector::Create();
				pfAft->SetMaxDistance(40);
				technique10->AddAffector(pfAft.get());

				Ptr<LinearForceAffector> lfAft = LinearForceAffector::Create();
				lfAft->SetForceVector(Math::float3(0,-0.5,0));
				technique10->AddAffector(lfAft.get());

				Ptr<TextureRotatorAffector> trAft = TextureRotatorAffector::Create();
				trAft->SetRotationSpeed(0.1);
				trAft->SetRandomRotation(1,4);
				technique10->AddAffector(trAft.get());

				Ptr<ColorAffector> cAft = ColorAffector::Create();
				cAft->AddColor(0.0,Math::ColorF(1 ,0.8, 0.6, 0));
				cAft->AddColor(0.2,Math::ColorF(1 ,0.9, 0.7, 1));
				cAft->AddColor(1.0,Math::ColorF(0 ,0.0, 0.0, 0));
				technique10->AddAffector(cAft.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique10->SetTarget( target.get() );	
				technique10->SetParticleQuota(40);

			}
			ParSystem->AddTechnique( technique10 );
			outarray.Append("tex:pump_smoke_07.png");
			ParSystem->SetName(Util::String("system_10"));
		}
		break;
	case 11:
		{
			//---
			Ptr<ParticleTechnique> technique11 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> pEmitter = PointEmitter::Create();
				//pEmitter->SetMaxColor(Math::ColorF(1,1,1,1));
				pEmitter->SetSize(Math::float3(2,2,2),Math::float3(2,2,2));
				pEmitter->SetMaxEmitDirection(N_PI_DOUBLE);
				pEmitter->SetLiveTime(5,5);
				pEmitter->SetEmitRate(150,150);
				pEmitter->SetVelocity(Math::float3(3,3,3),Math::float3(4,4,4));
				technique11->AddEmitter(pEmitter.get());

				Ptr<LinearForceAffector> lfAft = LinearForceAffector::Create();
				lfAft->SetForceVector(Math::float3(0,-7,0));
				technique11->AddAffector(lfAft.get());


				Ptr<ColorAffector> cAft = ColorAffector::Create();
				cAft->AddColor(0.0,Math::ColorF(1 ,1, 0, 1));
				cAft->AddColor(0.5,Math::ColorF(1 ,0, 0, 1));
				cAft->AddColor(0.9,Math::ColorF(0 ,0, 1, 1));
				cAft->AddColor(1.0,Math::ColorF(0 ,0, 0, 1));
				technique11->AddAffector(cAft.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique11->SetTarget( target.get() );	
				technique11->SetParticleQuota(100);
			}
			ParSystem->AddTechnique( technique11 );
			outarray.Append("tex:pu_flare.png");

			ParSystem->SetName(Util::String("system_11"));
		}
		break;
	case 12:
		{
			// technique 2
			Ptr<ParticleTechnique> technique12 =  ParticleTechnique::Create();//Particles::Clone( technique );
			{
				//Ptr<CircleEmitter> emitter = CircleEmitter::Create();
				Ptr<SphereSurfaceEmitter> emitter = SphereSurfaceEmitter::Create();
				emitter->SetRadius(5);
				emitter->SetMaxEmitDirection(N_PI_DOUBLE);
				//emitter->SetNormal(float3(1,1,1));
				technique12->AddEmitter(emitter.get());

				Ptr<ColorAffector> affector = ColorAffector::Create();
				affector->AddColor(0.0,Math::ColorF(1,1,1,1));
				affector->AddColor(0.5,Math::ColorF(1,0,0,1));
				affector->AddColor(1,Math::ColorF(0,1,0,1));
				technique12->AddAffector( affector.get() );

				Ptr<JetAffector> jetAffector = JetAffector::Create();
				jetAffector->SetAcceleration(Math::float2(1,20));
				jetAffector->SetDelayTime(3);
				technique12->AddAffector(jetAffector.get());

				Ptr<ScaleAffector> scaleAffector = ScaleAffector::Create();
				scaleAffector->SetScaleXYZ(1.5);
				scaleAffector->SetScaleXYZSet(true);
				technique12->AddAffector(scaleAffector.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique12->SetTarget( target.get() );	

				//technique2->SetParticleQuota(1);


			}
			ParSystem->AddTechnique( technique12 );
			outarray.Append("tex:pump_star_05.png");
			ParSystem->SetName(Util::String("system_12"));
		}
		break;
	case 13:
		{
			// technique 4
			Ptr<ParticleTechnique> technique13 = ParticleTechnique::Create();
			{
				Ptr<CircleEmitter> emitter = CircleEmitter::Create();
				emitter->SetRadius(10);
				//emitter->SetCircleAngle(10);

				emitter->SetMaxEmitRate(300);
				emitter->SetMaxVelocity(float3(1,1,1));
				//emitter->SetCircleAngle(1);
				//emitter->SetNormal(float3(0,1,0));
				//emitter->SetAutoDirection(true);
				//emitter->SetMaxSize(float3(2,2,2));
				//emitter->SetDirection(float3(0,1,0));
				technique13->AddEmitter( emitter.get() );

				Ptr<ParticleAffector> pAffector = ParticleAffector::Create();
				technique13->AddAffector( pAffector.get() );

				Ptr<RandomiserAffector> affector = RandomiserAffector::Create();
				//affector->SetMaxDeviationX(1);
				affector->SetMaxDeviationY(10);
				//affector->SetMaxDeviationZ(1);
				//affector->SetRandomDirection(false);
				technique13->AddAffector( affector.get() );

				Ptr<ColorAffector> coloraffector = ColorAffector::Create();
				coloraffector->AddColor(0.0,Math::ColorF(1,1,1,1));
				coloraffector->AddColor(0.5,Math::ColorF(1,0,0,1));
				coloraffector->AddColor(1,Math::ColorF(0,1,0,1));
				technique13->AddAffector( coloraffector.get() );

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);

				technique13->SetTarget( target.get() );	
				//technique4->SetParticleQuota(2000);
			}
			ParSystem->AddTechnique( technique13 );
			outarray.Append("tex:pu_dot_03_64x64.png");
			ParSystem->SetName(Util::String("system_13"));
		}
		break;
	
	case 14:
		{
			Ptr<ParticleTechnique> technique14 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> emitter = PointEmitter::Create();
				emitter->SetMaxEmitDirection(N_PI_DOUBLE);
				emitter->SetForceEmission(true);
				emitter->SetSize(Math::float3(3,3,3),Math::float3(3,3,3));
				//emitter->SetPosition(Math::float3(10,0,10));
				//emitter->SetAutoDirection(true);
				emitter->SetMaxVelocity(float3(10,10,10));

				Ptr<ParticleAffector> pAffector = ParticleAffector::Create();
				technique14->AddAffector( pAffector.get() );

				Ptr<TextureRotatorAffector> lfAffector = TextureRotatorAffector::Create();	
				//lfAffector->SetUseOwnRotationSpeed(true);
				lfAffector->SetRandomRotation(10,40);
				technique14->AddAffector( lfAffector.get() );

				//Ptr<ParticleBeamTarget> target = ParticleBeamTarget::Create();
				//Ptr<ParticleTrailTarget> target = ParticleTrailTarget::Create();
				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique14->AddEmitter( emitter.get() );
				technique14->SetTarget( target.get() );	
				//technique->SetParticleQuota(20);
				//technique->SetExternBoundingBox(Math::bbox());
			}
			ParSystem->AddTechnique( technique14 );
			outarray.Append("tex:pu_flare_12_256x256.png");
			ParSystem->SetName(Util::String("system_14"));
		}
		break;
	
	
	case 15:
		{
			//---
			Ptr<ParticleTechnique> technique15 = ParticleTechnique::Create();
			{
				Ptr<SphereSurfaceEmitter> emitter = SphereSurfaceEmitter::Create();
				//Ptr<PointEmitter> emitter = PointEmitter::Create();
				emitter->SetMaxEmitDirection(N_PI_DOUBLE);
				emitter->SetRadius(10);
				emitter->SetEmitRate(200,200);
				emitter->SetLiveTime(0.9,1.0);
				emitter->SetSize(Math::float3(7,7,7),Math::float3(7,7,7));
				emitter->SetColor(Math::ColorF(0.1, 0.1, 1, 1), Math::ColorF(0.1, 0.1, 1, 1));
				emitter->SetVelocity(Math::float3(0,0,0),Math::float3(0,0,0));
				//emitter->SetPosition(Math::float3(0, 5, 0.0 ));
				technique15->AddEmitter( emitter.get() );

				Ptr<ColorAffector> colorAft = ColorAffector::Create();
				colorAft->AddColor(0.0,Math::ColorF(0,0,0,0));
				colorAft->AddColor(0.2,Math::ColorF(0,0,0.2,1));
				colorAft->AddColor(0.9,Math::ColorF(0.8,0.8,1,1));
				colorAft->AddColor(1,Math::ColorF(1,1,1,1));
				technique15->AddAffector( colorAft.get() );

				Ptr<ScaleAffector> scaleAffector = ScaleAffector::Create();
				scaleAffector->SetScaleXYZ(-1);
				scaleAffector->SetScaleXYZSet(true);
				technique15->AddAffector(scaleAffector.get());

				Ptr<ParticleAffector> pAffector = ParticleAffector::Create();
				technique15->AddAffector( pAffector.get() );

				Ptr<GravityAffector> pGraAft = GravityAffector::Create();
				pGraAft->SetAffectSpecialisation(ParticleAffector::AFSP_TTL_INCREASE);
				pGraAft->SetGravity(900);				
				//pGraAft->SetGravityPosition(Math::float3(0, 5, 0.0 ));
				technique15->AddAffector( pGraAft.get() );

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique15->SetTarget( target.get() );	
				//technique11->SetParticleQuota(100);
			}
			ParSystem->AddTechnique( technique15 );
			outarray.Append("tex:pu_flare_12_256x256.png");

			Ptr<ParticleTechnique> technique15_2 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> emitter = PointEmitter::Create();
				emitter->SetVelocity(Math::float3(1,1,1),Math::float3(2,2,2));
				emitter->SetMaxEmitDirection(N_PI);
				emitter->SetColor(Math::ColorF(0.1, 0.1, 1, 1), Math::ColorF(0.1, 0.1, 1, 1));
				emitter->SetSize(Math::float3(1,30,30), Math::float3(1,30,30));
				emitter->SetLiveTime(1,1);
				//emitter->SetPosition(Math::float3(0, 5, 0.0 ));
				technique15_2->AddEmitter(emitter.get());

				Ptr<ColorAffector> colorAft = ColorAffector::Create();
				colorAft->AddColor(0.0,Math::ColorF(0,0,0,0));
				colorAft->AddColor(0.5,Math::ColorF(0.6,0.6,1,1));
				colorAft->AddColor(0.7,Math::ColorF(0.6,0.6,1,1));
				colorAft->AddColor(1,Math::ColorF(0,0,0,0));
				technique15_2->AddAffector( colorAft.get() );

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_ORIENTED_SELF);
				//target->SetOrientType(ParticleBillBoardTarget::Billboard_ORIENTED_SELF);
				technique15_2->SetTarget( target.get() );	
				technique15_2->SetParticleQuota(100);
			}
			ParSystem->AddTechnique( technique15_2 );
			outarray.Append("tex:pump_streak_04.png");

			ParSystem->SetName(Util::String("system_15"));
		}
		break;
	case 16:
		{
			//---
			Ptr<ParticleTechnique> technique16 = ParticleTechnique::Create();
			{
				Ptr<CircleEmitter> emitter = CircleEmitter::Create();
				emitter->SetRadius(3);
				emitter->SetSize(Math::float3(5,5,5), Math::float3(5,5,5));
				emitter->SetNormal(ConstDefine::FLOAT3_UNIT_Y);
				emitter->SetColor(Math::ColorF(1,0.2,0.2,1),Math::ColorF(1,0.2,0.2,1));
				technique16->AddEmitter( emitter.get() );

				Ptr<TextureRotatorAffector> lfAffector = TextureRotatorAffector::Create();	
				//lfAffector->SetUseOwnRotationSpeed(true);
				lfAffector->SetRandomRotation(10,20);
				technique16->AddAffector( lfAffector.get() );

				Ptr<ColorAffector> affector = ColorAffector::Create();
				affector->AddColor(0.0,Math::ColorF(1,0,0,1));
				affector->AddColor(0.8,Math::ColorF(1,1,1,1));
				affector->AddColor(1,Math::ColorF(1,1,1,0));
				technique16->AddAffector( affector.get() );

				Ptr<ScaleAffector> scaleAffector = ScaleAffector::Create();
				scaleAffector->SetScaleXYZ(-0.1);
				scaleAffector->SetScaleXYZSet(true);
				technique16->AddAffector(scaleAffector.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique16->SetTarget( target.get() );	
				technique16->SetParticleQuota(50);
			}		
			ParSystem->AddTechnique( technique16 );
			outarray.Append("tex:pump_swirl_01.png");

			ParSystem->SetName(Util::String("system_16"));
		}
		break;
	
	case 17:
		{
			//---
			Ptr<ParticleTechnique> technique17 = ParticleTechnique::Create();
			{
				Ptr<CircleEmitter> emitter = CircleEmitter::Create();
				emitter->SetRadius(3);	
				//emitter->SetNormal(Math::float3(0,1,0));
				emitter->SetEmitDirection(0,0);
				emitter->SetVelocity(Math::float3(0,2,0),Math::float3(0,2,0));
				emitter->SetColor(Math::ColorF(0.7,0.8,1,1), Math::ColorF(0.7,0.8,1,1));
				emitter->SetLiveTime(5,5);
				//emitter->SetForceEmission(true);
				//emitter->SetOrientation(Math::quaternion(1,1,1,1),Math::quaternion(1,1,1,1));
				//emitter->SetMass(20,20);
				//emitter->SetDelayTime(1);
				technique17->AddEmitter(emitter.get());

				//Ptr<ParticleAffector> affect = ParticleAffector::Create();				
				//technique14->AddAffector(affect.get());

				Ptr<VortexAffector> vortAft = VortexAffector::Create();
				vortAft->SetRotationVector(Math::vector(0,1,0) );
				vortAft->SetRotationSpeed(2);
				technique17->AddAffector(vortAft.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique17->SetTarget( target.get() );	
				technique17->SetParticleQuota(350);
			}
			ParSystem->AddTechnique( technique17 );
			outarray.Append("tex:pu_flare.png");
			ParSystem->SetName(Util::String("system_17"));
		}
		break;
	
	case 18:
		{
			//---
			Ptr<ParticleTechnique> technique18 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> pEmitter = PointEmitter::Create();
				pEmitter->SetSize(Math::float3(4,4,4),Math::float3(4,4,4));
				pEmitter->SetMaxEmitDirection(N_PI);
				pEmitter->SetVelocity(Math::float3(0,0,0),Math::float3(1,1,1));
				pEmitter->SetColor(Math::ColorF(1,0.88,0.26,1),Math::ColorF(1,0.88,0.26,1));
				//pEmitter->SetForceEmission(true);
				technique18->AddEmitter(pEmitter.get());

				Ptr<ParticleAffector> aft = ParticleAffector::Create();
				technique18->AddAffector(aft.get());

				//Ptr<ColorAffector> cAft = ColorAffector::Create();
				//cAft->AddColor(0.0,Math::ColorF(0 ,0, 0, 0));
				//cAft->AddColor(0.5,Math::ColorF(1 ,1, 1, 1));
				//cAft->AddColor(1.0,Math::ColorF(0 ,0, 0, 0));
				//technique16->AddAffector(cAft.get());

				Ptr<TextureRotatorAffector> trAft = TextureRotatorAffector::Create();
				trAft->SetRotationSpeed(0.1);
				trAft->SetRandomRotation(-3,3);
				technique18->AddAffector(trAft.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique18->SetTarget( target.get() );	
				technique18->SetParticleQuota(5);
			}
			ParSystem->AddTechnique( technique18 );
			outarray.Append("tex:pump_hourglass.png");

			Ptr<ParticleTechnique> technique18_2 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> pEmitter = PointEmitter::Create();
				pEmitter->SetSize(Math::float3(4,4,4),Math::float3(4,4,4));
				pEmitter->SetMaxEmitDirection(N_PI);
				pEmitter->SetVelocity(Math::float3(0,0,0),Math::float3(1,1,1));
				pEmitter->SetColor(Math::ColorF(0.82,1,0.51,1),Math::ColorF(0.82,1,0.51,1));
				//pEmitter->SetForceEmission(true);
				technique18_2->AddEmitter(pEmitter.get());
	
				Ptr<ParticleAffector> aft = ParticleAffector::Create();
				technique18_2->AddAffector(aft.get());

				//Ptr<ColorAffector> cAft = ColorAffector::Create();
				//cAft->AddColor(0.0,Math::ColorF(0 ,0, 0, 0));
				//cAft->AddColor(0.5,Math::ColorF(1 ,1, 1, 1));
				//cAft->AddColor(1.0,Math::ColorF(0 ,0, 0, 0));
				//technique16_2->AddAffector(cAft.get());

				Ptr<TextureRotatorAffector> trAft = TextureRotatorAffector::Create();
				trAft->SetRotationSpeed(0.1);
				trAft->SetRandomRotation(-3,3);
				technique18_2->AddAffector(trAft.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				technique18_2->SetTarget( target.get() );	
				technique18_2->SetParticleQuota(5);
			}
			ParSystem->AddTechnique( technique18_2 );
			outarray.Append("tex:pump_watch.png");
			
			ParSystem->SetName(Util::String("system_18"));
		}
		break;
	
	

	case 19:
		{
			//---
			Ptr<ParticleTechnique> technique19 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> pEmitter = PointEmitter::Create();
				pEmitter->SetMaxEmitDirection(N_PI_DOUBLE);
				pEmitter->SetLiveTime(7,10);
				pEmitter->SetVelocity(Math::float3(10,10,10),Math::float3(10,10,10));
				technique19->AddEmitter(pEmitter.get());

				Ptr<ParticleAffector> aft = ParticleAffector::Create();
				technique19->AddAffector(aft.get());


				Ptr<TextureAnimatorAffector> taAft = TextureAnimatorAffector::Create();
				taAft->SetStartRandom(true);
				taAft->SetTextureCoordsEnd(35);
				technique19->AddAffector(taAft.get());

				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				target->SetBillBoardType(ParticleBillBoardTarget::Billboard_Camera);
				target->SetTexCoordSize(6, 6);
				technique19->SetTarget( target.get() );	
			}
			ParSystem->AddTechnique( technique19 );
			outarray.Append("tex:interpolate.png");
			ParSystem->SetName(Util::String("system_19"));
		}
		break;
	case 20:
		{
			//---
			Ptr<ParticleTechnique> technique20 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> emitter = PointEmitter::Create();
				emitter->SetLiveTime(1,1);
				emitter->SetVelocity(Math::float3(0,0,0),Math::float3(0,0,0));
				emitter->SetOrientation(Math::quaternion(1,0,0,-N_PI_HALF),Math::quaternion(1,0,0,-N_PI_HALF));
				emitter->SetSize(Math::float3(0.005,0.001,0.001),Math::float3(0.006,0.001,0.001));
				technique20->AddEmitter( emitter.get() );

				Ptr<ColorAffector> colorAft = ColorAffector::Create();
				Math::ColorF color(1,1,1,1);
				colorAft->AddColor(0.0,0.5f * color);
				colorAft->AddColor(0.2,0.4f * color);
				colorAft->AddColor(0.4,0.3f * color);
				colorAft->AddColor(0.6,0.2f * color);
				colorAft->AddColor(0.8,0.1f * color);
				colorAft->AddColor(1.0,0.0f * color);
				colorAft->SetColorOperation(ColorAffector::CAO_SET);
				technique20->AddAffector( colorAft.get() );

				Ptr<ScaleAffector> scaleAffector = ScaleAffector::Create();
				scaleAffector->SetScaleX(0.01);
				scaleAffector->SetScaleXSet(true);
				scaleAffector->SetScaleY(0.002);
				scaleAffector->SetScaleYSet(true);
				scaleAffector->SetScaleZ(0.002);
				scaleAffector->SetScaleZSet(true);
				technique20->AddAffector(scaleAffector.get());

				Ptr<LinearForceAffector> lfAft = LinearForceAffector::Create();
				lfAft->SetForceVector(Math::float3(0,0,400));
				technique20->AddAffector(lfAft.get());

				Ptr<ParticleEntityTarget> target = ParticleEntityTarget::Create();

				technique20->SetTarget( target.get() );	
				technique20->SetParticleQuota(20);
			}
			ParSystem->AddTechnique( technique20 );
			//outarray.Append("tex:1head.dds");
			outarray.Append("tex:Monster_000_yanlong_001.dds");
			ParSystem->SetName(Util::String("system_20"));
		}
		break;
	case 21:
		{
			//---
			Ptr<ParticleTechnique> technique21 = ParticleTechnique::Create();
			technique21->SetEmitterTime(1);
			{
				Ptr<PointEmitter> pEmitter = PointEmitter::Create();
				//pEmitter->SetColor(Math::ColorF(1,0.5,0.3,1),Math::ColorF(1,0.5,0.3,1));
				pEmitter->SetSize(Math::float3(6,6,6),Math::float3(6,6,6));
				pEmitter->SetEmitRate(1,1);
				//pEmitter->SetInterMission(1);
				pEmitter->SetLiveTime(2,2);
				pEmitter->SetPosition(Math::float3(0,0.1,0));
				technique21->AddEmitter(pEmitter.get());

				Ptr<ColorAffector> colorAft = ColorAffector::Create();

				Math::ColorF color(1,1,1,1);
				colorAft->AddColor(0.0, 0.0f*color);
				colorAft->AddColor(0.1,0.2f * color);
				colorAft->AddColor(0.2,0.4f * color);
				colorAft->AddColor(0.3,0.6f * color);
				colorAft->AddColor(0.4,0.8f * color);
				colorAft->AddColor(0.5,1.0f * color);
				colorAft->AddColor(0.6,0.8f * color);
				colorAft->AddColor(0.7,0.6f * color);
				colorAft->AddColor(0.8,0.4f * color);
				colorAft->AddColor(0.9,0.2f * color);
				colorAft->AddColor(1.0,0.0f*color);
				colorAft->SetColorOperation(ColorAffector::CAO_SET);
				technique21->AddAffector( colorAft.get() );

				Ptr<ScaleAffector> sAft = ScaleAffector::Create();
				sAft->SetScaleXYZ(1.0);
				sAft->SetScaleXYZSet(true);
				technique21->AddAffector(sAft.get());

				Ptr<ParticleEntityTarget> target = ParticleEntityTarget::Create();
				technique21->SetTarget( target.get() );	
				technique21->SetParticleQuota(3);
			}
			ParSystem->AddTechnique( technique21 );
			outarray.Append("tex:vortex.bmp");
			//-----	

			/***/
			Ptr<ParticleTechnique> technique21_1 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> pEmitter = PointEmitter::Create();
				pEmitter->SetColor(Math::ColorF(1,0,1,1),Math::ColorF(1,0,1,1));
				pEmitter->SetSize(Math::float3(6.8,6.8,6.8),Math::float3(6.8,6.8,6.8));
				pEmitter->SetLiveTime(2,2);
				technique21_1->AddEmitter(pEmitter.get());

				Ptr<ColorAffector> colorAft = ColorAffector::Create();

				Math::ColorF color(1,1,1,1);
				colorAft->AddColor(0.0, 0.0f*color);
				colorAft->AddColor(0.1,0.f * color);
				colorAft->AddColor(0.2,0.4f * color);
				colorAft->AddColor(0.3,0.6f * color);
				colorAft->AddColor(0.4,0.8f * color);
				colorAft->AddColor(0.5,1.0f * color);
				colorAft->AddColor(0.6,0.8f * color);
				colorAft->AddColor(0.7,0.6f * color);
				colorAft->AddColor(0.8,0.4f * color);
				colorAft->AddColor(0.9,0.2f * color);
				colorAft->AddColor(1.0,0.0f*color);
				colorAft->SetColorOperation(ColorAffector::CAO_SET);
				technique21_1->AddAffector( colorAft.get() );

				Ptr<ScaleAffector> sAft = ScaleAffector::Create();
				sAft->SetScaleXYZ(1.0);
				sAft->SetScaleXYZSet(true);
				technique21_1->AddAffector(sAft.get());

				Ptr<ParticleEntityTarget> target = ParticleEntityTarget::Create();
				technique21_1->SetTarget( target.get() );	
				technique21_1->SetParticleQuota(1);
			}
			ParSystem->AddTechnique( technique21_1 );			
			outarray.Append("tex:circle.bmp");
			//-----------
			Ptr<ParticleTechnique> technique21_2 = ParticleTechnique::Create();
			{
				Ptr<PointEmitter> emitter = PointEmitter::Create();
				//emitter->SetNormal(Math::float3(0,1,0));
				//emitter->SetRadius(1.5);
				emitter->SetLiveTime(0.2,0.2);
				emitter->SetSize(float3(2,7.0,0),float3(2,7.0,0));
				emitter->SetPosition(Math::float3(0,0,0));
				//emitter->SetColor(Math::ColorF(0.2,0.2,0.33,1),Math::ColorF(0.2,0.2,0.33,1));

				Ptr<TextureAnimatorAffector> taAft = TextureAnimatorAffector::Create();
				taAft->SetStartRandom(true);
				taAft->SetTextureAnimationType(TextureAnimatorAffector::TAT_RANDOM);
				taAft->SetTextureCoordsStart(0);
				taAft->SetTextureCoordsEnd(16);
				technique21_2->AddAffector(taAft.get());

				Ptr<ColorAffector> colorAft = ColorAffector::Create();
				Math::ColorF color(0.8,0.8,1,0.8);
				colorAft->AddColor(0.0, Math::ColorF(0,0,0,0));
				colorAft->AddColor(0.9,Math::ColorF(0.8,0.8,1,0.8));
				colorAft->AddColor(1.0,Math::ColorF(0,0,0,0));
				colorAft->SetColorOperation(ColorAffector::CAO_SET);
				technique21_2->AddAffector( colorAft.get() );

				//Ptr<ParticleBeamTarget> target = ParticleBeamTarget::Create();
				Ptr<ParticleBillBoardTarget> target = ParticleBillBoardTarget::Create();
				//target->SetMaxElements(2);
				target->SetTexCoordSize(4, 4);
				technique21_2->AddEmitter( emitter.get() );
				technique21_2->SetTarget( target.get() );	
				technique21_2->SetParticleQuota(1);
			}
			ParSystem->AddTechnique( technique21_2 );
			outarray.Append("tex:lightning.bmp");
			ParSystem->SetName(Util::String("system_21"));
			/***/
		}
		break;
	}

}
//------------------------------------------------------------
void SimpleStateHandler::ChangeParticleUp()
{
	bWebFunc = true;
	keydown++;
}
//------------------------------------------------------------
void SimpleStateHandler::ChangeParticleDown()
{
	bWebFunc = true;
	keydown--;
}
//------------------------------------------------------------------------
void SimpleStateHandler::LoadTemplate()
{
	//ParticleServer::Instance()->OpenTemplateFile();
}

//--------------------------

// ================================ UI Test =====================================


MyGUI::Char _codeToChar(Input::InputKey::Code code)
{
	if (code >= Input::InputKey::A && code <= Input::InputKey::Z)
	{
		return ((MyGUI::Char)code) - (MyGUI::Char)Input::InputKey::A + (MyGUI::Char)'A';
	}
	else if (code >= Input::InputKey::Key0 && code <= Input::InputKey::Key9)
	{
		return ((MyGUI::Char)code) - (MyGUI::Char)Input::InputKey::Key0 + (MyGUI::Char)'0';
	}
	else if (Input::InputKey::Space == code)
	{
		return (MyGUI::Char)' ';
	}
	return 0;
}


bool SimpleStateHandler::_frameUI()
{

//#if _DEBUG & _ZHONGDAOHUAN
//	static int flag = 0;
//	if (flag)
//	{	
//		return false;//==================================================================================================
//	}
//#endif

	return false;//==================================================================================================




	const Ptr<Input::InputServer>& inputServer = App::InputFeature::Instance()->GetInputServer();
	n_assert( inputServer.isvalid() );

	// update the camera from input	

	bool ui_do = false;
	const Ptr<Input::InputKeyboard>& keyboard = inputServer->GetDefaultKeyboard();
	const Ptr<Input::InputMouse>& mouse = inputServer->GetDefaultMouse();

	static bool left_press = false;
	static bool right_press = false;
	static bool middle_press = false;
	static bool sys_cursor = true;
	MyGUI::InputManager* gui_input = MyGUI::InputManager::getInstancePtr();
	if (gui_input)
	{
		static Math::float2 mouse_xy(0, 0);
		int mouse_x = 0;
		int mouse_y = 0;
		static int mouse_z = 0;
		if(mouse->WheelForward())
		{
			mouse_z += 120;
		}
		if (mouse->WheelBackward())
		{
			mouse_z -= 120;
		}
		if (mouse_xy != mouse->GetPixelPosition() || 0 != mouse_z)
		{		
			mouse_xy = mouse->GetPixelPosition();
			mouse_x = (int)mouse_xy.x();
			mouse_y = (int)mouse_xy.y();
			ui_do |= gui_input->injectMouseMove(mouse_x, mouse_y, mouse_z);
		}
		else
		{
			mouse_x = (int)mouse_xy.x();
			mouse_y = (int)mouse_xy.y();
		}		

		if (mouse->ButtonUp(Input::InputMouseButton::LeftButton) && (left_press))
		{
			left_press = false;
			ui_do |= gui_input->injectMouseRelease(mouse_x, mouse_y, MyGUI::MouseButton::Left);
		}
		if (mouse->ButtonUp(Input::InputMouseButton::RightButton)&& (right_press))
		{
			right_press = false;
			ui_do |= gui_input->injectMouseRelease(mouse_x, mouse_y, MyGUI::MouseButton::Right);
		}
		if (mouse->ButtonUp(Input::InputMouseButton::MiddleButton) && (middle_press))
		{
			middle_press = false;
			ui_do |= gui_input->injectMouseRelease(mouse_x, mouse_y, MyGUI::MouseButton::Middle);
		}

		if (mouse->ButtonDown(Input::InputMouseButton::LeftButton) && (!left_press))
		{
			left_press = true;
			ui_do |= gui_input->injectMousePress(mouse_x, mouse_y, MyGUI::MouseButton::Left);
		}
		if (mouse->ButtonDown(Input::InputMouseButton::RightButton) && (!right_press))
		{
			right_press = true;
			ui_do |= gui_input->injectMousePress(mouse_x, mouse_y, MyGUI::MouseButton::Right);
		}
		if (mouse->ButtonDown(Input::InputMouseButton::MiddleButton) && (!middle_press))
		{
			middle_press = true;
			ui_do |= gui_input->injectMousePress(mouse_x, mouse_y, MyGUI::MouseButton::Middle);
		}
	}
	for (int x = Input::InputKey::Back; x < Input::InputKey::NumKeyCodes; ++x)
	{
		static bool press[Input::InputKey::NumKeyCodes] = {false};
		if(keyboard->KeyDown((Input::InputKey::Code)x))
		{
			mPressTxt->setCaption(Input::InputKey::ToString((Input::InputKey::Code)x).AsCharPtr());
			ui_do |= gui_input->injectKeyPress(GUIServer::KeyCodeWJtoMyGUI((Input::InputKey::Code)x), _codeToChar((Input::InputKey::Code)x));			
			press[x] = true;
		}
		else
		{
			if (press[x])
			{
				press[x] = false;
				gui_input->injectKeyRelease(GUIServer::KeyCodeWJtoMyGUI((Input::InputKey::Code)x));
			}
		}
	}
	return ui_do;
}

MyGUI::VectorWidgetPtr SimpleStateHandler::_addLayout(char* name)
{
	return MyGUI::LayoutManager::getInstance().loadLayout(name);
	//MyGUI::VectorWidgetPtr listWindowRoot = MyGUI::LayoutManager::getInstance().loadLayout(name);
}
void SimpleStateHandler::_initUI()
{
	ShowCursor(false);
	MyGUI::Widget* root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout").at(0);//->findWidget("_Main");
	MyGUI::TextBox* tb = root->findWidget("Text")->castType<MyGUI::TextBox>();
	tb->setCaption("I have nothing to say.");

	const MyGUI::VectorWidgetPtr& sample = _addLayout("wjgui_sample.layout");
	MyGUI::Widget* _main = sample.at(0)->findWidget("_Main");
	mEmtyWinBtn = _main->findWidget("EmptyWin")->castType<MyGUI::Button>();
	mEmtyWinBtn->eventMouseButtonClick += MyGUI::newDelegate(this, &SimpleStateHandler::_emptyWindowClick);
	
	MyGUI::Button* temp = _main->findWidget("R_One")->castType<MyGUI::Button>();
	if (temp)
	{
		mRadios.Append(temp);
	}
	temp = _main->findWidget("R_Two")->castType<MyGUI::Button>();
	if (temp)
	{
		mRadios.Append(temp);
	}

	mPressTxt = sample.at(0)->findWidget("BtnShow")->castType<MyGUI::EditBox>();


	_main->findWidget("NextPtc")->castType<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &SimpleStateHandler::_nextPtc);
	_main->findWidget("PrevPtc")->castType<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &SimpleStateHandler::_prevPtc);
	_main->findWidget("ClearPtc")->castType<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &SimpleStateHandler::_clearPtc);

	_main->findWidget("R_One")->castType<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &SimpleStateHandler::_radioClick);
	_main->findWidget("R_Two")->castType<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &SimpleStateHandler::_radioClick);
	_main->findWidget("C_A")->castType<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &SimpleStateHandler::_checkClick);
	_main->findWidget("C_B")->castType<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &SimpleStateHandler::_checkClick);

	App::DebugBoard::Open();
}
void SimpleStateHandler::_emptyWindowClick(MyGUI::Widget* _sender)
{

	//root.at(0)->findWidget("_Main")->castType<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &SimpleStateHandler::_testMouseButtonClick);
	
	if (mEmptyWin.empty())
	{
		mEmptyWin = _addLayout("EmptyWindow.layout");
		mEmtyWinBtn->setCaption(L"隐藏窗口");
		MyGUI::Widget* colordemo = MyGUI::LayoutManager::getInstance().loadLayout("ColourWindow.layout", "", mEmptyWin.at(0)).at(0)->findWidget("_Main");
		mRed = colordemo->findWidget("Red")->castType<MyGUI::ScrollBar>();
		mGreen = colordemo->findWidget("Green")->castType<MyGUI::ScrollBar>();
		mBlue = colordemo->findWidget("Blue")->castType<MyGUI::ScrollBar>();
		mColor = colordemo->findWidget("Colour");
		mColorText = colordemo->findWidget("ColorText")->castType<MyGUI::EditBox>();

		mRed->eventScrollChangePosition += MyGUI::newDelegate(this, &SimpleStateHandler::_notifyScrollChangePosition);
		mGreen->eventScrollChangePosition += MyGUI::newDelegate(this, &SimpleStateHandler::_notifyScrollChangePosition);
		mBlue->eventScrollChangePosition += MyGUI::newDelegate(this, &SimpleStateHandler::_notifyScrollChangePosition);
		_notifyScrollChangePosition(NULL, 0);
	}
	else
	{
	
		for (MyGUI::VectorWidgetPtr::iterator begin = mEmptyWin.begin(); begin < mEmptyWin.end(); ++begin)
		{
			if ((*begin)->getVisible())
			{
				(*begin)->setVisible(false);
				mEmtyWinBtn->setCaption(L"显示窗口");
			}
			else
			{
				(*begin)->setVisible(true);
				mEmtyWinBtn->setCaption(L"隐藏窗口");
			}
			
		}


	}
}

void SimpleStateHandler::_nextPtc(MyGUI::Widget* _sender)
{
	ChangeParticleDown();
}

void SimpleStateHandler::_prevPtc(MyGUI::Widget* _sender)
{
	ChangeParticleUp();
}
void SimpleStateHandler::_clearPtc(MyGUI::Widget* _sender)
{
	if(ParticleAct.isvalid())
	{
		mScene->RemoveActor(ParticleAct);
		ParticleAct->Destory();
	}
}
void SimpleStateHandler::_radioClick(MyGUI::Widget* _sender)
{
	MyGUI::Button* btn = _sender->castType<MyGUI::Button>();
	if (btn)
	{
		MyGUI::Button* child = NULL;
		for (int x = 0; x < mRadios.Size(); ++x)
		{
			mRadios[x]->setStateSelected(false);
		}
		btn->setStateSelected(true);
	}
}
void SimpleStateHandler::_checkClick(MyGUI::Widget* _sender)
{
	MyGUI::Button* btn = _sender->castType<MyGUI::Button>();
	if (btn)
	{
		btn->setStateSelected(!btn->getStateSelected());
	}
}

void SimpleStateHandler::_requestCreateWidgetItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item)
{
	MyGUI::VectorWidgetPtr& cell = MyGUI::LayoutManager::getInstance().loadLayout("CellView.layout", "", _item);//CellView.layout
}	

void SimpleStateHandler::_notifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position)
{
	float red = float(mRed->getScrollPosition()) / float(mRed->getScrollRange());
	float green = float(mGreen->getScrollPosition()) / float(mGreen->getScrollRange());
	float blue = float(mBlue->getScrollPosition()) / float(mBlue->getScrollRange());
	MyGUI::Colour colour(red, green, blue);
	mColor->setColour(colour);
	mColorText->setTextColour(colour);

	mRed->setColour(MyGUI::Colour(red, 0, 0));
	mGreen->setColour(MyGUI::Colour(0, green, 0));
	mBlue->setColour(MyGUI::Colour(0, 0, blue));
}

} // namespace Application