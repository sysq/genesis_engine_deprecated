/** \file: InputSystem.h
* \brief: IO
* Copyright (c) 2011,畅游天下WebJet事业部
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/17   1.0    SunHao   
*/
#ifndef __INPUTSYSTEM_H__	
#define __INPUTSYSTEM_H__

#include "app/appframework/manager.h"
#include "app/apputil/mayacamerautil.h"


namespace Demo
{
	class DemoInputManager: public App::Manager
	{
		__DeclareSubClass(DemoInputManager, App::Manager );
	public:
		DemoInputManager();
		~DemoInputManager();

		/// @App::Manager::OnActivate called when attached to game server
		virtual void OnActivate();

		/// @App::Manager::OnBeginFrame called before frame by the game server
		virtual void OnBeginFrame();
	public:
		AppUtil::MayaCameraUtil m_mayaCameraUtil;
	};

}

#endif // __INPUTSYSTEM_H__
