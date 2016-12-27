#include "stdneb.h"
#include "simplegamestatehandler.h"
#include "basegamefeature/basegamefeature.h"
#include "basegamefeature/managers/sceneschedulemanager.h"
#include "graphicfeature/graphicsfeature.h"
#include "appframework/gameserver.h"
#include "appframework/scene.h"
#include "appframework/actor.h"
#include "graphicfeature/components/cameracomponent.h"
#include "graphicfeature/components/meshcomponent.h"
#include "graphicfeature/components/lightcomponent.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "graphicfeature/components/simulatephysicwatercomponent.h"
#include "input/inputserver.h"
#include "input/inputkeyboard.h"
#include "app/inputfeature/inputfeature.h"
#include "input/inputkey.h"

#include "graphicsystem/GraphicSystem.h"
#include "graphicsystem/Light/Light.h"
#include "graphicsystem/Camera/Camera.h"
#include "graphicsystem/Sky/SkyRenderer.h"
#include "graphicsystem/Sky/SkyRenderable.h"
#include "graphicsystem/Renderable/SimpleShapeRenderable.h"
#include "graphicsystem/Renderable/SimpleShapeRenderer.h"
#include "math/polar.h"

#include "app/physXfeature/PhysicsBodyComponent.h"
#include "app/physXfeature/PhysicsBoxShapeComponent.h"
#include "app/physXfeature/PhysicsSphereShapeComponent.h"
#include "app/physXfeature/PhysicsPlaneShapeComponent.h"
#include "app/physXfeature/PhysicsTerrianComponent.h"
#include "app/physXfeature/PhysicsCapsuleShapeComponent.h"
#include "app/physXfeature/PhysicsMeshShapeComponent.h"
#include "app/physXfeature/PhysicsServer.h"

#include "apputil/manuresutil.h"
#include "terrainfeature/components/TerrainRenderComponent.h"
#include "terrainsystem/HeightMap.h"

#include "resource/meshres.h"

#include <fstream>
#include <ios>

static float g_BoxSize(4.0f);

namespace Sample 
{
	__ImplementClass(Sample::SimpleStateHandler, 'SSHD', App::StateHandler);

	using namespace Graphic;
	using namespace App;

	//------------------------------------------------------------------------------
	/**
	*/
	SimpleStateHandler::SimpleStateHandler()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	SimpleStateHandler::~SimpleStateHandler()
	{
		// empty

		m_pScene = NULL;
		//m_pBoxesActors.Clear();
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

		App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();

	// open scene
		pSceneSchedule->OpenScene(  GetSceneName() , false );
		m_pScene = pSceneSchedule->GetScene();
		n_assert( m_pScene );

		{
			Ptr<Actor> pActor = Actor::Create();
			pActor->SetLayerID(1);
			pActor->SetName("Light1");
			m_pScene->AddActor( pActor );

			Ptr<LightComponent> pLightComponent = LightComponent::Create();
			pActor->AttachComponent( pLightComponent.upcast<Component>() );

			pLightComponent->SetLightType( Graphic::Light::eSunLight);
			//pLightComponent->SetLightIntensity(5.f);
			Math::matrix44 lighttrans = Math::matrix44::identity();
			Math::polar orientation(Math::float4(2.f/3.f,2.f/3.f,-1.f/3.f,0.f));
			Math::matrix44 rot = Math::matrix44::rotationyawpitchroll(orientation.rho, orientation.theta, 0);
			Math::matrix44 rotx = Math::matrix44::rotationx(-0.5*PI);
			rotx = Math::matrix44::multiply(rot, rotx);
			lighttrans = Math::matrix44::multiply(rotx, lighttrans);

			pActor->SetTransform( lighttrans );
			pActor->Active();
			m_pActLight = pActor;
		}

		//create Terrain 's floor
		if (true)
		{
			Ptr<Actor> pfloor = Actor::Create();
			pfloor->SetLayerID(0);
			pfloor->SetName("floor");
			m_pScene->AddActor(pfloor);

			Resources::PositionData position;
			Resources::Index16Container indicies;
			Resources::TexCoordData texcoors;
			Resources::NormalData   normals;

			position.Append(Math::float3(-800,0,800));
			position.Append(Math::float3(800,0,800));
			position.Append(Math::float3(800,0,-800));
			position.Append(Math::float3(-800,0,-800));

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

			normals.Append(Math::float3(0.0f,1.0f,0.0f));
			normals.Append(Math::float3(0.0f,1.0f,0.0f));
			normals.Append(Math::float3(0.0f,1.0f,0.0f));
			normals.Append(Math::float3(0.0f,1.0f,0.0f));

			Math::Color32 color(0,0,0,0);

			Ptr<Resources::MeshRes> mesh = AppUtil::ManuResUtil::CreateManuMesh_WithTopology( "terrainFloor", position.Size(),	&position[0], color, indicies.Size(),&indicies[0]);
			n_assert( mesh.isvalid() );
			mesh->SetVertexData<Resources::TexCoordData>( &texcoors[0], 4 );
			mesh->SetVertexData<Resources::NormalData>( &normals[0], 4 );

			Ptr<MeshComponent> pMeshComponent = MeshComponent::Create();
			pMeshComponent->SetMeshID( mesh->GetResourceId() );

			Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();	
			pMeshRenderComponent->SetShaderID(0, "shd:Simple.wjshader");	
			pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
			n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
			pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:grid.png" );

			Math::matrix44 trans = Math::matrix44::identity();
			trans.set_position(Math::float4(0.0f, 0.0f, 0.0f, 1.0f));

			Ptr<PhysicsPlaneShapeComponent> pPPSC = PhysicsPlaneShapeComponent::Create();
			//pPPSC->AddShapes(1);
			pPPSC->SetStaticFriction(200.0f);
			pPPSC->SetDynamicFriction(200.0f);
			pPPSC->SetRestitution(0.0f);

			//Ptr<PhysicsBodyComponent> pBC = PhysicsBodyComponent::Create();
			//pBC->SetBodyStatic(true);

			pfloor->AttachComponent( pMeshComponent.upcast<Component>() );
			pfloor->AttachComponent( pMeshRenderComponent.upcast<Component>() );
			//pfloor->AttachComponent( pBC.upcast<Component>() );
			pfloor->AttachComponent( pPPSC.upcast<Component>() );
			

			pfloor->SetTransform(trans);
			pfloor->Active();
		}

		if (false)
		{
			Ptr<Actor> pActor = Actor::Create();
			pActor->SetLayerID(1);
			pActor->SetName("TerrainPhysicSmaple");
			m_pScene->AddActor( pActor );
			pActor->SetPosition( Math::vector(0, -40.0, 0 ) );

			// 1. create terrain
			Ptr<TerrainRenderComponent> pTerrain = TerrainRenderComponent::Create();
			n_assert(pTerrain.isvalid());
            pTerrain->SetTerrainName(pActor->GetGUID().AsString());

			// please first set resolution, second set size!!
			const int kResolution = 256;
			pTerrain->SetResolution(kResolution);	//1st set
			pTerrain->SetTerrainSize(float3(200, 60, 200));	//2nd set

			// set height map. for example, import a exist height map file.
			Ptr<Terrain::HeightMap> heightmap = pTerrain->GetTerrainData()->GetHeightMap();
			float *heights = n_new_array(float, heightmap->GetWidth()*heightmap->GetHeight());
			Memory::Clear(heights, sizeof(float)*heightmap->GetWidth()*heightmap->GetHeight());

			LoadRAW(heightmap->GetWidth(), heightmap->GetHeight(), "heightmap17_257.raw", 1.0f, 0.0f, heights);
			for (int i=0; i< heightmap->GetWidth()*heightmap->GetHeight(); ++i)
			{
				heights[i] /= 255.0f;
			}
			pTerrain->GetTerrainData()->GetHeightMap()->SetHeights(0, 0, heightmap->GetWidth(), heightmap->GetHeight(), heights, false);

			n_delete_array(heights);

			pTerrain->SetBaseTexture("sys:white.dds");
			//pTRComponent->SetBlendTexture(0, "_controlMap", "tex:blend_hm17.dds");
			pTerrain->SetBlendTexture(0, "_controlMap", "sys:white.dds");
			//pTRComponent->SetBlendTexture(0, "_blendMap0", "tex:grass.dds");
			//pTRComponent->SetBlendTexture(0, "_blendMap1", "tex:dirt.dds");
			//pTRComponent->SetBlendTexture(0, "_blendMap2", "tex:stone.dds");

			Ptr<PhysicsTerrainComponent> pTerrainCom = PhysicsTerrainComponent::Create();

			pActor->AttachComponent( pTerrain.upcast<Component>() );
			pActor->AttachComponent( pTerrainCom.upcast<Component>() );
			pActor->Active();
		}	

		for (IndexT i = 0; i<m_pScene->GetActorCount(); ++i)
		{
			const Ptr<Actor>& pActor = m_pScene->GetActor(i);

			if (pActor->GetName() == "Camera")
			{
				m_pCam = pActor;
				break;
			}
		}

		App::PhysicsServer::Instance()->ConnectToVRD();
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
			App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
			//pSceneSchedule->SaveScene();

			pSceneSchedule->CloseScene();
			return "Exit";
		}
		else
		{

			const Ptr<Input::InputServer>& inputServer = App::InputFeature::Instance()->GetInputServer();
			n_assert( inputServer.isvalid() );

			// update the camera from input	
			const Ptr<Input::InputKeyboard>& keyboard = inputServer->GetDefaultKeyboard();

			if (keyboard->KeyDown(Input::InputKey::Key1))
			{
				CreateCapsule();
				//CreateBox( Math::float3(4.0, 4.0, 4.0) );
			}

			else if (keyboard->KeyDown(Input::InputKey::Key2))
			{
				//CreateConvex();
				CreateBox( Math::float3(8.0, 8.0, 8.0) );
			}
			/*else if (keyboard->KeyDown(Input::InputKey::Key3))
			{
				CreateStatue();
			}
			else if (keyboard->KeyDown(Input::InputKey::Key4))
			{
				CreateTeapot();
			}*/
			else if (keyboard->KeyDown(Input::InputKey::Key5))
			{
				CreateStacks(30);
			}
			else if (keyboard->KeyDown(Input::InputKey::Key6))
			{
				CreateTower(100);
			}
			else if (keyboard->KeyDown(Input::InputKey::Key0))
			{
				RemoveAll();
			}
			
			const Ptr<CameraComponent>& pCC = m_pCam->FindComponent(CameraComponent::RTTI).downcast<CameraComponent>();

			float posY = pCC->GetCameraObject()->GetTransform().get_position().y();
						
			//n_dbgout("Cam PosY: %f \n", posY);

			Input::InputServer* pInputServer = Input::InputServer::Instance();
			return this->GetName();
		}
	}


	void SimpleStateHandler::CreateBox(const Math::float3& f3Dim)
	{
		Ptr<App::Actor> pBox = App::Actor::Create();

		Ptr<MeshComponent> pMC = MeshComponent::Create();
		pMC->SetMeshID("msh:Cube.mesh");

		Ptr<MeshRenderComponent> pMRC = MeshRenderComponent::Create();
		pMRC->SetShaderID(0, "shd:Simple.wjshader");
		pMRC->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		pMRC->SetTexture(0, "_diffuseMap", "tex:crate.dds");

		pBox->AttachComponent(pMC.upcast<Component>());
		pBox->AttachComponent(pMRC.upcast<Component>());

		const Ptr<CameraComponent>& pCC = m_pCam->FindComponent(CameraComponent::RTTI).downcast<CameraComponent>();
		const Math::float4&  pos4 = pCC->GetCameraObject()->GetTransform().get_position();
		Math::float4  dir4 = pCC->GetCameraObject()->GetTransform().get_zaxis();

		Math::matrix44 trans = Math::matrix44::identity();
		Math::matrix44 rotx = Math::matrix44::rotationx(scalar(-0.5f*PI));
		trans = Math::matrix44::multiply(rotx, trans);
		trans.set_position(pos4);
		//trans.set_position(Math::float4(50.0f, 100.0f, 50.0f,1.0f));
		pBox->SetTransform(trans);
		//pBox->SetScale(Math::vector(0.1f, 0.1f, 0.1f));
		

		dir4 = Math::float4::normalize(dir4);

		Math::float3 dir3 = -Math::float3(dir4.x(), dir4.y(), dir4.z());
		dir3 *= 100.0f;

		Ptr<PhysicsBoxShapeComponent> pBSC = PhysicsBoxShapeComponent::Create();
		//pBSC->AddShapes(1);
		pBSC->SetStaticFriction(200.0f);
		pBSC->SetDynamicFriction(200.0f);
		pBSC->SetRestitution(0.0f);
		pBSC->SetDimension(f3Dim);
		pBSC->SetRenderShapeShow(true);
		

		Ptr<PhysicsBodyComponent> pBC = PhysicsBodyComponent::Create();
		//pBC->SetPhysicsID("phy:sample.nxb");
		pBC->SetLinearVelocity(dir3);
		pBC->SetMass(1.0f);
		//pBC->SetCollisionMode(PhysicsBodyComponent::CCD_DYNAMIC);

		pBox->AttachComponent(pBSC.upcast<Component>());
		pBox->AttachComponent(pBC.upcast<Component>());

		m_pScene->AddActor(pBox);

		pBox->Active();

		m_pBoxesActors.Append(pBox);
	}

	void SimpleStateHandler::CreateStacks(IndexT size)
	{
		
		float Offset = -size * (g_BoxSize * 2.0f) * 0.5f;

		Math::float4 pos4(0.0f, 0.0f, 0.0f, 1.0f);

		while(size)
		{
			for (IndexT i = 0; i<size; ++i)
			{
				pos4.x() = Offset + i * (g_BoxSize * 2.0f);
				
				Ptr<App::Actor> pBox = App::Actor::Create();

				Ptr<MeshComponent> pMC = MeshComponent::Create();
				pMC->SetMeshID("msh:Cube.mesh");

				Ptr<MeshRenderComponent> pMRC = MeshRenderComponent::Create();
				pMRC->SetShaderID(0, "shd:Simple.wjshader");
				pMRC->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
				pMRC->SetTexture(0, "_diffuseMap", "tex:rustymetal.dds");

				pBox->AttachComponent(pMC.upcast<Component>());
				pBox->AttachComponent(pMRC.upcast<Component>());

				Math::matrix44 trans = Math::matrix44::identity();
				Math::matrix44 rotx = Math::matrix44::rotationx(scalar(-0.5f*PI));
				trans = Math::matrix44::multiply(rotx, trans);
				trans.set_position(pos4);
				pBox->SetTransform(trans);

				Ptr<PhysicsBoxShapeComponent> pBSC = PhysicsBoxShapeComponent::Create();
				//pBSC->AddShapes(1);
				pBSC->SetStaticFriction(200.0f);
				pBSC->SetDynamicFriction(200.0f);
				pBSC->SetRestitution(0.0f);
				pBSC->SetDimension(Math::float3(4.f,4.f,4.f));

				Ptr<PhysicsBodyComponent> pBC = PhysicsBodyComponent::Create();
				pBC->SetMass(50.0f);
				//pBC->SetCollisionMode(PhysicsBodyComponent::CCD);

				pBox->AttachComponent(pBSC.upcast<Component>());
				pBox->AttachComponent(pBC.upcast<Component>());

				m_pScene->AddActor(pBox);

				pBox->Active();

				m_pBoxesActors.Append(pBox);
			}

			Offset   += g_BoxSize;
			pos4.y() += (g_BoxSize * 2.0f);
			size--;
		}
	}

	void SimpleStateHandler::CreateTower(IndexT size)
	{
		const Ptr<CameraComponent>& pCC = m_pCam->FindComponent(CameraComponent::RTTI).downcast<CameraComponent>();

		float posX = pCC->GetCameraObject()->GetTransform().get_position().x();
		float posZ = pCC->GetCameraObject()->GetTransform().get_position().z();

		Math::float4 pos4(posX, 0.0f, posZ, 1.0f);

		while (size)
		{
			Ptr<App::Actor> pBox = App::Actor::Create();

			Ptr<MeshComponent> pMC = MeshComponent::Create();
			pMC->SetMeshID("msh:Cube.mesh");

			Ptr<MeshRenderComponent> pMRC = MeshRenderComponent::Create();
			pMRC->SetShaderID(0, "shd:Simple.wjshader");
			pMRC->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
			pMRC->SetTexture(0, "_diffuseMap", "tex:crate.dds");

			pBox->AttachComponent(pMC.upcast<Component>());
			pBox->AttachComponent(pMRC.upcast<Component>());

			Math::matrix44 trans = Math::matrix44::identity();
			Math::matrix44 rotx = Math::matrix44::rotationx(scalar(-0.5f*PI));
			trans = Math::matrix44::multiply(rotx, trans);
			trans.set_position(pos4);
			pBox->SetTransform(trans);

			Ptr<PhysicsBoxShapeComponent> pBSC = PhysicsBoxShapeComponent::Create();
			//pBSC->AddShapes(1);
			pBSC->SetStaticFriction(200.0f);
			pBSC->SetDynamicFriction(200.0f);
			pBSC->SetRestitution(0.0f);
			pBSC->SetDimension(Math::float3(4.f,4.f,4.f));

			Ptr<PhysicsBodyComponent> pBC = PhysicsBodyComponent::Create();
			pBC->SetMass(50.0f);

			pBox->AttachComponent(pBSC.upcast<Component>());
			pBox->AttachComponent(pBC.upcast<Component>());

			m_pScene->AddActor(pBox);

			pBox->Active();

			m_pBoxesActors.Append(pBox);

			pos4.y() += g_BoxSize * 2.0f;

			size--;
		}
	}

	void SimpleStateHandler::CreateCapsule()
	{
		Ptr<App::Actor> pBox = App::Actor::Create();

		Ptr<MeshComponent> pMC = MeshComponent::Create();
		pMC->SetMeshID("msh:capsule.mesh");
		

		Ptr<MeshRenderComponent> pMRC = MeshRenderComponent::Create();
		pMRC->SetShaderID(0, "shd:color.wjshader");
		pMRC->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		pMRC->SetTexture(0, "_diffuseMap", "tex:crate.dds");
		pMRC->SetVisible(false);

		pBox->AttachComponent(pMC.upcast<Component>());
		pBox->AttachComponent(pMRC.upcast<Component>());

		const Ptr<CameraComponent>& pCC = m_pCam->FindComponent(CameraComponent::RTTI).downcast<CameraComponent>();
		const Math::float4&  pos4 = pCC->GetCameraObject()->GetTransform().get_position();
		Math::float4  dir4 = pCC->GetCameraObject()->GetTransform().get_zaxis();

		Math::matrix44 trans = Math::matrix44::identity();
		Math::matrix44 rotx = Math::matrix44::rotationx(scalar(-0.5f*PI));
		trans = Math::matrix44::multiply(rotx, trans);
		trans.set_position(pos4);
		//trans.set_position(Math::float4(50.0f, 100.0f, 50.0f,1.0f));
		pBox->SetTransform(trans);


		dir4 = Math::float4::normalize(dir4);

		Math::float3 dir3 = -Math::float3(dir4.x(), dir4.y(), dir4.z());
		dir3 *= 100.0f;

		Ptr<PhysicsCapsuleShapeComponent> pBSC = PhysicsCapsuleShapeComponent::Create();
		//pBSC->AddShapes(1);
		pBSC->SetStaticFriction(200.0f);
		pBSC->SetDynamicFriction(200.0f);
		pBSC->SetRestitution(0.0f);
		pBSC->SetHeight(4.f);
		pBSC->SetRadius(1.f);
		pBSC->SetRenderShapeShow(true);

		Ptr<PhysicsBodyComponent> pBC = PhysicsBodyComponent::Create();
		//pBC->SetPhysicsID("phy:capsule.nxb");
		pBC->SetLinearVelocity(dir3);
		pBC->SetMass(1.0f);
		
		//pBC->SetCollisionMode(PhysicsBodyComponent::CCD_DYNAMIC);

		pBox->AttachComponent(pBSC.upcast<Component>());
		pBox->AttachComponent(pBC.upcast<Component>());

		m_pScene->AddActor(pBox);

		pBox->Active();

		m_pBoxesActors.Append(pBox);
	}

	void SimpleStateHandler::CreateSphere()
	{
		Ptr<App::Actor> pSphere = App::Actor::Create();

		Ptr<MeshComponent> pMC = MeshComponent::Create();
		pMC->SetMeshID("msh:Sphere.mesh");

		Ptr<MeshRenderComponent> pMRC = MeshRenderComponent::Create();
		pMRC->SetShaderID(0, "shd:Simple.wjshader");
		pMRC->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		pMRC->SetTexture(0, "_diffuseMap", "tex:oshHelm_d.dds");

		pSphere->AttachComponent(pMC.upcast<Component>());
		pSphere->AttachComponent(pMRC.upcast<Component>());

		const Ptr<CameraComponent>& pCC = m_pCam->FindComponent(CameraComponent::RTTI).downcast<CameraComponent>();
		const Math::float4&  pos4 = pCC->GetCameraObject()->GetTransform().get_position();
		Math::float4  dir4 = pCC->GetCameraObject()->GetTransform().get_zaxis();

		Math::matrix44 trans = Math::matrix44::identity();
		trans.scale(Math::float4(0.7f, 0.7f, 0.7f, 1.0f));
		Math::matrix44 rotx = Math::matrix44::rotationx(scalar(-0.5f*PI));
		trans = Math::matrix44::multiply(rotx, trans);
		trans.set_position(pos4);
		pSphere->SetTransform(trans);

		dir4 = Math::float4::normalize(dir4);

		Math::float3 dir3 = -Math::float3(dir4.x(), dir4.y(), dir4.z());
		dir3 *= 100.0f;

		Ptr<PhysicsShpereComponent> pSC = PhysicsShpereComponent::Create();
		//pSC->AddShapes(1);
		pSC->SetStaticFriction(5000.0f);
		pSC->SetDynamicFriction(5000.0f);
		pSC->SetRestitution(0.5f);
		pSC->SetRenderShapeShow(true);

		Ptr<PhysicsBodyComponent> pBC = PhysicsBodyComponent::Create();
		pBC->SetLinearVelocity(dir3);

		pSphere->AttachComponent(pSC.upcast<Component>());
		pSphere->AttachComponent(pBC.upcast<Component>());

		m_pScene->AddActor(pSphere);

		pSphere->Active();

		m_pBoxesActors.Append(pSphere);
	}

	void SimpleStateHandler::CreateConvex()
	{
		Ptr<App::Actor> pBox = App::Actor::Create();

		Ptr<MeshComponent> pMC = MeshComponent::Create();
		pMC->SetMeshID("msh:teapot.mesh");

		Ptr<MeshRenderComponent> pMRC = MeshRenderComponent::Create();
		pMRC->SetShaderID(0, "shd:color.wjshader");
		pMRC->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		//pMRC->SetShaderID(1, "shd:color.wjshader");
		//pMRC->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");

		pBox->AttachComponent(pMC.upcast<Component>());
		pBox->AttachComponent(pMRC.upcast<Component>());

		const Ptr<CameraComponent>& pCC = m_pCam->FindComponent(CameraComponent::RTTI).downcast<CameraComponent>();
		const Math::float4&  pos4 = pCC->GetCameraObject()->GetTransform().get_position();
		Math::float4  dir4 = pCC->GetCameraObject()->GetTransform().get_zaxis();

		Math::matrix44 trans = Math::matrix44::identity();
		Math::matrix44 rotx = Math::matrix44::rotationx(scalar(-0.5f*PI));
		trans = Math::matrix44::multiply(rotx, trans);
		trans.set_position(pos4);
		//trans.set_position(Math::float4(50.0f, 100.0f, 50.0f,1.0f));
		pBox->SetTransform(trans);
		//pBox->SetScale(Math::vector(10.0f, 10.0f, 10.0f));

		dir4 = Math::float4::normalize(dir4);

		Math::float3 dir3 = -Math::float3(dir4.x(), dir4.y(), dir4.z());
		dir3 *= 100.0f;

		Ptr<PhysicsMeshShapeComponent> pMSC = PhysicsMeshShapeComponent::Create();
		//pMSC->AddShapes(1);
		pMSC->SetConvex(true);

		Ptr<PhysicsBodyComponent> pBC = PhysicsBodyComponent::Create();
		//pBC->SetPhysicsID("phy:teapot.nxb");
		pBC->SetLinearVelocity(dir3);
		pBC->SetMass(1.0f);
		//pBC->SetCollisionMode(PhysicsBodyComponent::CCD_DYNAMIC);
		
		pBox->AttachComponent(pMSC.upcast<Component>());
		pBox->AttachComponent(pBC.upcast<Component>());

		m_pScene->AddActor(pBox);

		pBox->Active();

		m_pBoxesActors.Append(pBox);
	}

	void SimpleStateHandler::CreateStatue()
	{
		Ptr<App::Actor> pBox = App::Actor::Create();

		Ptr<MeshComponent> pMC = MeshComponent::Create();
		pMC->SetMeshID("msh:statue.mesh");

		Ptr<MeshRenderComponent> pMRC = MeshRenderComponent::Create();
		pMRC->SetShaderID(0, "shd:Metal_2.wjshader");
		pMRC->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		pMRC->_AddTextureParam(0, "_cubeMap", "tex:whitecube.dds");
		pMRC->SetTexture(0, "_diffuseMap", "tex:cubeCathDiff.dds");
		pMRC->SetTexture(0, "_cubeMap", "tex:Sky_01_1024.dds");

		pBox->AttachComponent(pMC.upcast<Component>());
		pBox->AttachComponent(pMRC.upcast<Component>());

		const Ptr<CameraComponent>& pCC = m_pCam->FindComponent(CameraComponent::RTTI).downcast<CameraComponent>();
		const Math::float4&  pos4 = pCC->GetCameraObject()->GetTransform().get_position();
		Math::float4  dir4 = pCC->GetCameraObject()->GetTransform().get_zaxis();

		Math::matrix44 trans = Math::matrix44::identity();
		Math::matrix44 rotx = Math::matrix44::rotationx(scalar(-0.5f*PI));
		trans = Math::matrix44::multiply(rotx, trans);
		trans.set_position(pos4);
		pBox->SetTransform(trans);
		pBox->SetScale(Math::vector(300.0f, 300.0f, 300.0f));

		dir4 = Math::float4::normalize(dir4);

		Math::float3 dir3 = -Math::float3(dir4.x(), dir4.y(), dir4.z());
		dir3 *= 100.0f;

		Ptr<PhysicsBoxShapeComponent> pBSC = PhysicsBoxShapeComponent::Create();
		//pBSC->AddShapes(1);
		pBSC->SetStaticFriction(200.0f);
		pBSC->SetDynamicFriction(200.0f);
		pBSC->SetRestitution(0.0f);

		Ptr<PhysicsBodyComponent> pBC = PhysicsBodyComponent::Create();
		pBC->SetLinearVelocity(dir3);
		pBC->SetMass(1000.0f);
		//pBC->SetCollisionMode(PhysicsBodyComponent::CCD_DYNAMIC);

		pBox->AttachComponent(pBSC.upcast<Component>());
		pBox->AttachComponent(pBC.upcast<Component>());

		m_pScene->AddActor(pBox);

		pBox->Active();

		m_pBoxesActors.Append(pBox);
	}

	void SimpleStateHandler::CreateTeapot()
	{
		Ptr<App::Actor> pBox = App::Actor::Create();

		Ptr<MeshComponent> pMC = MeshComponent::Create();
		pMC->SetMeshID("msh:Teapot.mesh");

		Ptr<MeshRenderComponent> pMRC = MeshRenderComponent::Create();
		pMRC->SetShaderID(0, "shd:Metal_2.wjshader");
		pMRC->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		pMRC->_AddTextureParam(0, "_cubeMap", "tex:whitecube.dds");
		pMRC->SetTexture(0, "_diffuseMap", "tex:cubeCathDiff.dds");
		pMRC->SetTexture(0, "_cubeMap", "tex:room-dxt5.dds");

		pBox->AttachComponent(pMC.upcast<Component>());
		pBox->AttachComponent(pMRC.upcast<Component>());

		const Ptr<CameraComponent>& pCC = m_pCam->FindComponent(CameraComponent::RTTI).downcast<CameraComponent>();
		const Math::float4&  pos4 = pCC->GetCameraObject()->GetTransform().get_position();
		Math::float4  dir4 = pCC->GetCameraObject()->GetTransform().get_zaxis();

		Math::matrix44 trans = Math::matrix44::identity();
		Math::matrix44 rotx = Math::matrix44::rotationx(scalar(-0.5f*PI));
		trans = Math::matrix44::multiply(rotx, trans);
		trans.set_position(pos4);
		pBox->SetTransform(trans);
		pBox->SetScale(Math::vector(0.4f, 0.4f, 0.4f));

		dir4 = Math::float4::normalize(dir4);

		Math::float3 dir3 = -Math::float3(dir4.x(), dir4.y(), dir4.z());
		dir3 *= 100.0f;

		Ptr<PhysicsBoxShapeComponent> pBSC = PhysicsBoxShapeComponent::Create();
		//pBSC->AddShapes(1);
		pBSC->SetStaticFriction(200.0f);
		pBSC->SetDynamicFriction(200.0f);
		pBSC->SetRestitution(0.0f);

		Ptr<PhysicsBodyComponent> pBC = PhysicsBodyComponent::Create();
		pBC->SetLinearVelocity(dir3);
		pBC->SetMass(1000.0f);
		//pBC->SetCollisionMode(PhysicsBodyComponent::CCD_DYNAMIC);

		pBox->AttachComponent(pBSC.upcast<Component>());
		pBox->AttachComponent(pBC.upcast<Component>());

		m_pScene->AddActor(pBox);

		pBox->Active();

		m_pBoxesActors.Append(pBox);
	}

	void SimpleStateHandler::RemoveAll()
	{
		for (IndexT i = 0; i<m_pBoxesActors.Size(); ++i)
		{
			m_pBoxesActors[i]->Deactive();
		}

		m_pBoxesActors.Clear();
	}

	void SimpleStateHandler::LoadRAW(int m, int n, const std::string &filename, float heightScale, float heightOffset, float *height)
	{
		// A height for each vertex
		std::vector<unsigned char> in( m * n );


		// Open the file.
		std::ifstream inFile;
		inFile.open(filename.c_str(), std::ios_base::binary);
		if(!inFile)
		{
			return;
		}

		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();

		for(int i = 0; i < m; ++i)
		{
			for(int j = 0; j < n; ++j)
			{
				int k = i * n + j;
				height[k] = (float)in[k] * heightScale + heightOffset;
			}
		}
	}

} // namespace Application