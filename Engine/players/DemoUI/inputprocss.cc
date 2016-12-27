/** \file defaultinputprocss.cc
* \brief 文件内容说明
* Copyright (c) 2011,畅游天下WebJet事业部
* All rights reserved.
* Date        Ver    Who          Comment
* 2011:9:16   1.0    王兴博   
*/
#include "stdneb.h"
#include "inputprocss.h"
#include "app/inputfeature/inputfeature.h"
#include "app/graphicfeature/graphicsfeature.h"
#include "input/inputmouse.h"
#include "input/inputkeyboard.h"
#include "input/inputgamepad.h"


namespace App
{
	using namespace AppUtil;
	using namespace Math;
	using namespace Input;

	__ImplementClass(App::GyrostProcessInput, 'GRPI', App::Manager);
	//------------------------------------------------------------------------
	GyrostProcessInput::GyrostProcessInput()
	{

	}
	//------------------------------------------------------------------------
	GyrostProcessInput::~GyrostProcessInput()
	{

	}
	//------------------------------------------------------------------------
	void GyrostProcessInput::OnActivate()
	{
		// setup the camera util object
		this->m_mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(0.0f, 3.0f, 10.0f), vector(0.0f, 1.0f, 0.0f));
		this->m_mayaCameraUtil.Update();

		Super::OnActivate();
	}
	//------------------------------------------------------------------------
	void GyrostProcessInput::OnBeginFrame()
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
		const Ptr<Input::InputGamePad>& gamePad = inputServer->GetDefaultGamePad(0);

		// standard input handling: manipulate camera
 		this->m_mayaCameraUtil.SetOrbitButton( mouse->ButtonPressed(InputMouseButton::RightButton) );
 		this->m_mayaCameraUtil.SetPanButton( mouse->ButtonPressed(InputMouseButton::MiddleButton) );
 		//this->m_mayaCameraUtil.SetZoomButton( mouse->ButtonPressed(InputMouseButton::LeftButton) );
 		this->m_mayaCameraUtil.SetZoomInButton( mouse->WheelForward() );
 		this->m_mayaCameraUtil.SetZoomOutButton( mouse->WheelBackward() );
 		this->m_mayaCameraUtil.SetMouseMovement( mouse->GetMovement() );

		// process gamepad input
		float zoomIn = 0.0f;
		float zoomOut = 0.0f;
		float2 panning(0.0f, 0.0f);
		float2 orbiting(0.0f, 0.0f);
	/*
		if (gamePad->IsConnected())
		{
			const float gamePadZoomSpeed = 5.0f;
			const float gamePadOrbitSpeed = 1.0f;
			const float gamePadPanSpeed = 10.0f;
			if (gamePad->ButtonDown(GamePad::AButton))
			{
				this->mayaCameraUtil.Reset();
			}
			if (gamePad->ButtonDown(GamePad::StartButton) ||
				gamePad->ButtonDown(GamePad::BackButton))
			{
				this->SetQuitRequested(true);
			}
			float frameTime = (float) this->GetFrameTime();
			zoomIn       += gamePad->GetAxisValue(GamePad::RightTriggerAxis) * frameTime * gamePadZoomSpeed;
			zoomOut      += gamePad->GetAxisValue(GamePad::LeftTriggerAxis) * frameTime * gamePadZoomSpeed;
			panning.x()  -= gamePad->GetAxisValue(GamePad::RightThumbXAxis) * frameTime * gamePadPanSpeed;
			panning.y()  += gamePad->GetAxisValue(GamePad::RightThumbYAxis) * frameTime * gamePadPanSpeed;
			orbiting.x() -= gamePad->GetAxisValue(GamePad::LeftThumbXAxis) * frameTime * gamePadOrbitSpeed;
			orbiting.y() += gamePad->GetAxisValue(GamePad::LeftThumbYAxis) * frameTime * gamePadOrbitSpeed;
		}
	*/
 		// process keyboard input
 		if (keyboard->KeyDown(InputKey::Escape))
 		{
 			//this->SetQuitRequested(true);
 		}
 		if (keyboard->KeyDown(InputKey::Space))
 		{
 			this->m_mayaCameraUtil.Reset();
 		}
 		if (keyboard->KeyPressed(InputKey::Left)||keyboard->KeyPressed(InputKey::A))
 		{
 			panning.x() += 0.1f;
 		}
 		if (keyboard->KeyPressed(InputKey::Right)||keyboard->KeyPressed(InputKey::D))
 		{
 			panning.x() -= 0.1f;
 		}
		if (keyboard->KeyPressed(InputKey::Up))
		{
			panning.y() -= 0.1f;
		}
		if (keyboard->KeyPressed(InputKey::Down))
		{
			panning.y() += 0.1f;
		}
 		if (keyboard->KeyPressed(InputKey::S))
 		{
 			zoomIn -= 0.1f;
 		}
 		if (keyboard->KeyPressed(InputKey::W))
 		{
 			zoomOut -= 0.1f;
 		}
		if (keyboard->KeyPressed(InputKey::F3))
		{
			GraphicsFeature::Instance()->SetRenderDebug(!GraphicsFeature::Instance()->HasRenderDebug());
		}
	 
 		this->m_mayaCameraUtil.SetPanning(panning);
 		this->m_mayaCameraUtil.SetOrbiting(orbiting);
 		this->m_mayaCameraUtil.SetZoomIn(zoomIn);
 		this->m_mayaCameraUtil.SetZoomOut(zoomOut);
 		this->m_mayaCameraUtil.Update();

		App::GraphicsFeature::Instance()->TransformDefaultCamera(this->m_mayaCameraUtil.GetCameraTransform());
	}
}
