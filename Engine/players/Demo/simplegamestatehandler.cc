#include "stdneb.h"
#include "simplegamestatehandler.h"
#include "basegamefeature/basegamefeature.h"
#include "basegamefeature/managers/sceneschedulemanager.h"
#include "graphicfeature/graphicsfeature.h"
#include "appframework/gameserver.h"
#include "appframework/scene.h"
#include "appframework/actor.h"
#include "graphicfeature/components/cameracomponent.h"
#include "graphicfeature/components/lightcomponent.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "graphicfeature/components/skeletoncomponent.h"
#include "graphicfeature/components/skinnedmeshrendercomponent.h"
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
#include "graphicsystem/Sky/SkyRenderable.h"
#include "graphicfeature/components/skycomponent.h"
#include "graphicfeature/components/watercomponent.h"
#include "graphicfeature/components/LakeComponent.h"
#include "app/scriptfeature/inc/script_utility.h"

namespace Demo 
{
	__ImplementClass(Demo::SimpleStateHandler, 'SIHD', App::StateHandler);

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

		bool isNewScene = false;
		const Util::String sceneName = GetSceneName();
		if ( sceneName.IsValid() )
		{
			// open scene
			pSceneSchedule->OpenScene(  sceneName , false );
		}
		else
		{
			// new scene
			pSceneSchedule->OpenScene( "asset:scene/newscene.scene",true);
			isNewScene = true;
		}
		
		Ptr<App::Scene> pScene = pSceneSchedule->GetScene();
		n_assert( pScene );

		// 创建一个actor，挂接一个相机
		if ( isNewScene )
		{
			matrix44 cameraTransform;
			{
				Ptr<Actor> pActor = Actor::Create();
				pScene->AddActor( pActor );

				Ptr<CameraComponent> pCameraComponent = CameraComponent::Create();
				pCameraComponent->SetCameraOrder( Graphic::Camera::Main );
				pActor->AttachComponent( pCameraComponent.upcast<Component>() );

				matrix44 Inittransform = matrix44::lookatrh(float4(0.f, 0.f, 15.0f, 1.f),float4(0.f, 0.f, 0.f, 1.f),float4(0.f,1.f,0.f,0.f));
				Inittransform = matrix44::inverse(Inittransform);
				pActor->SetTransform( Inittransform );

				cameraTransform = Inittransform;

				pActor->SetName("Camera");

				// 作为主相机
				pCameraComponent->SetCameraOrder( Graphic::Camera::Main );
				GraphicsFeature::Instance()->AssignDefaultCamera( pActor );

				pActor->Active();
			}

			// 创建actor，挂接一个光源
			{
				Ptr<Actor> pActor = Actor::Create();
				pScene->AddActor( pActor );

				Ptr<LightComponent> pLightComponent = LightComponent::Create();
				pActor->AttachComponent( pLightComponent.upcast<Component>() );

				pLightComponent->SetLightType( Graphic::Light::eSunLight);

				Math::matrix44 lighttrans = Math::matrix44::identity();
				Math::polar orientation(Math::float4(2.f/3.f,2.f/3.f,-1.f/3.f,0.f));
				Math::matrix44 rot = Math::matrix44::rotationyawpitchroll(orientation.rho, orientation.theta, 0);
				Math::matrix44 rotx = Math::matrix44::rotationx( float(-0.5*PI) );
				rotx = Math::matrix44::multiply(rot,rotx);
				lighttrans = Math::matrix44::multiply(rotx,lighttrans);

				pActor->SetName("Light");

				pActor->SetTransform( lighttrans );
				pActor->Active();
			}
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
			App::SceneScheduleManager* pSceneSchedule = App::SceneScheduleManager::Instance();
			pSceneSchedule->CloseScene();
			return "Exit";
		}
		else
		{

			const Ptr<Input::InputServer>& inputServer = App::InputFeature::Instance()->GetInputServer();
			n_assert( inputServer.isvalid() );

			// update the camera from input	
			const Ptr<Input::InputKeyboard>& keyboard = inputServer->GetDefaultKeyboard();
			Input::InputServer* pInputServer = Input::InputServer::Instance();
			return this->GetName();
		}
	}

} // namespace Application