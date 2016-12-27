/** \file: InputSystem.cc
* \brief: implement input system
* Copyright (c) 2011,畅游天下WebJet事业部
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/17   1.0    SunHao   
*/

#include "stdneb.h"
#include "InputSystem.h"
#include "Gamelogic.h"

#include "math/float2.h"
#include "app/inputfeature/inputfeature.h"
#include "app/graphicfeature/graphicsfeature.h"
#include "app/basegamefeature/managers/timesource.h"
#include "input/inputmouse.h"
#include "input/inputkeyboard.h"
#include "input/inputgamepad.h"
#include "InputSystem.h"
#include "DemoGameStateHandler.h"
#include "Gamelogic.h"

namespace Demo
{
	void DisplayShortCut( void )
	{
		return ;
	}
	using namespace AppUtil;
	using namespace Math;
	using namespace Input;

	__ImplementClass(DemoInputManager, 'DMPI', App::Manager);
	
	//------------------------------------------------------------------------
	DemoInputManager::DemoInputManager()
		: m_mayaCameraUtil()
	{
		// empty
	}

	//------------------------------------------------------------------------
	DemoInputManager::~DemoInputManager()
	{
		// empty
	}

	//------------------------------------------------------------------------
	void DemoInputManager::OnActivate()
	{
		// setup the camera util object
		this->m_mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(0.0f, 3.0f, 10.0f), vector(0.0f, 1.0f, 0.0f));
		this->m_mayaCameraUtil.Update();

		Super::OnActivate();
	}

	//------------------------------------------------------------------------
	void DemoInputManager::OnBeginFrame()
	{
		if( !App::InputFeature::HasInstance() || !App::GraphicsFeature::HasInstance() )
		{
			return;
		}

		if ( !App::InputFeature::Instance()->IsActive() || !App::GraphicsFeature::Instance() )
		{
			return;
		}

		const Ptr<Input::InputServer>& inputServer = App::InputFeature::Instance()->GetInputServer();
		n_assert( inputServer.isvalid() );

		// update the camera from input	
		const Ptr<Input::InputKeyboard>& keyboard = inputServer->GetDefaultKeyboard();
		const Ptr<Input::InputMouse>& mouse = inputServer->GetDefaultMouse();

		Math::float2 move = mouse->GetMovement();

		// standard input handling: manipulate camera
		this->m_mayaCameraUtil.SetZoomInButton( mouse->WheelForward() );
		this->m_mayaCameraUtil.SetZoomOutButton( mouse->WheelBackward() );
		this->m_mayaCameraUtil.SetMouseMovement( move );

		// process mouse input
		if ( mouse->ButtonPressed(InputMouseButton::LeftButton) )
		{
		}

		if ( mouse->ButtonPressed(InputMouseButton::RightButton) )
		{
			GameLogic::Instance()->CancelPreparedBuild();
			this->m_mayaCameraUtil.SetOrbitButton( true );
		}

		// ----- key board -----------------------------
		// -=------process keyboard move --------------------
		float2 orbiting( 0.0f, 0.0f );
		float2 panning( 0.0f, 0.0f );
		Timing::Time fTick = App::GameTime::Instance()->GetFrameTime();
		static double fVelocity = 10.f;
		Math::scalar fOffset = static_cast<Math::scalar>( fVelocity * fTick );
		
		if (keyboard->KeyDown(InputKey::Space))
		{
			this->m_mayaCameraUtil.Reset();
		}
		// - move left
		if ( keyboard->KeyPressed(InputKey::A) )
		{
			panning.x() += 5*fOffset;
		}
		// - move right
		if ( keyboard->KeyPressed(InputKey::D) )
		{
			panning.x() -= 5*fOffset;
		}
		// - zoom in
		if ( keyboard->KeyPressed(InputKey::S) )
		{
			this->m_mayaCameraUtil.SetZoomIn( -1*fOffset );
		}
		// - zoom out
		if ( keyboard->KeyPressed(InputKey::W) )
		{
			this->m_mayaCameraUtil.SetZoomIn( fOffset );
		}
		// - go up
		if ( keyboard->KeyPressed(InputKey::Q) )
		{
			panning.y() += 5 * fOffset;
		}
		// - go down
		if ( keyboard->KeyPressed(InputKey::E) )
		{
			panning.y() -= 5 * fOffset;
		}
		// - rotate actor
		if ( keyboard->KeyDown(InputKey::K) )
		{
			Math::scalar radian = static_cast<Math::scalar>( PI/2 );
			GameLogic::Instance()->RotateSelectedBuilding( radian );
		}
		// - rotate actor
		if ( keyboard->KeyDown(InputKey::L) )
		{
			Math::scalar radian = static_cast<Math::scalar>( -PI/2 );
			GameLogic::Instance()->RotateSelectedBuilding( radian );
		}
		// - show grid
		if ( keyboard->KeyDown(InputKey::G) )
		{
			static bool s_bShowing = false;
			if ( !s_bShowing )
			{
				GameLogic::Instance()->ShowGirds();
			}
			else
			{
				GameLogic::Instance()->HideGirds();
			}

			s_bShowing = !s_bShowing;
		}
		// - remove selected building
		if ( !keyboard->KeyPressed( InputKey::Control) && 
			keyboard->KeyPressed(InputKey::R) )
		{
			GameLogic::Instance()->RemoveSelected();			
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key1) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 1 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 11 );
			}
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key2) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 2 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 12 );
			}
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key3) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 3 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 13 );
			}
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key4) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 4 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 14 );
			}
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key5) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 5 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 15 );
			}
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key6) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 6 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 16 );
			}
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key7) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 7 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 17 );
			}
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key8) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 8 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 18 );
			}
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key9) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 9 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 19 );
			}
		}
		// - prepare to build a building
		if ( keyboard->KeyDown(InputKey::Key0) )
		{
			if ( !keyboard->KeyPressed( InputKey::Shift) )
			{
				GameLogic::Instance()->PrepareToBuild( 10 );
			}
			else
			{
				GameLogic::Instance()->PrepareToBuild( 20 );
			}
		}
		// - save scene
		if ( keyboard->KeyPressed(InputKey::Shift) && 
			keyboard->KeyDown(InputKey::S) )
		{
			GameLogic::Instance()->SaveScene();
		}
		// - saveLogic
		if ( keyboard->KeyDown(InputKey::F2) )
		{
			GameLogic::Instance()->SaveLogic();
		}

		if ( keyboard->KeyDown(InputKey::F3) )
		{
			GameLogic::Instance()->LoadLogic( "logic:preBuild.glogic");
		}

		if ( keyboard->KeyPressed(InputKey::F4) )
		{
			App::GraphicsFeature::Instance()->SetRenderDebug(!App::GraphicsFeature::Instance()->HasRenderDebug());
		}

		this->m_mayaCameraUtil.SetPanning( panning );
		this->m_mayaCameraUtil.SetOrbiting( orbiting );
		this->m_mayaCameraUtil.Update();

		App::GraphicsFeature::Instance()->TransformDefaultCamera(this->m_mayaCameraUtil.GetCameraTransform());
	}
}
