/** \file: DemoGameStateHandler.h
* \brief: 
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/14   1.0    SunHao   
*/
#ifndef __DEMOGAMESTATEHANDLER_H__
#define __DEMOGAMESTATEHANDLER_H__

#include "appframework/statehandler.h"
#include "appframework/actor.h"
#include "math/float3.h"
#include "appframework/app_fwd_decl.h"
#include "addons/resource/resource_fwd_decl.h"
#include "DemoFwdDecl.h"

namespace Demo
{
	class DemoGameStateHandler : public App::StateHandler
	{
		__DeclareClass(DemoGameStateHandler);
	public:    

		/// constructor
		DemoGameStateHandler();
		/// destructor
		virtual ~DemoGameStateHandler();
		/// called when the state represented by this state handler is entered
		virtual void OnStateEnter(const Util::String& prevState);
		/// called when the state represented by this state handler is left
		virtual void OnStateLeave(const Util::String& nextState);
		/// called each frame as long as state is current, return new state
		virtual Util::String OnFrame();
		/// set level filename, required by setup mode LoadLevel
		void SetSceneName(const Util::String& n);
		/// get level name
		const Util::String& GetSceneName() const;

		bool ResetScene(const char* strSceneName);
		void HeldScene();

	private:    
		Util::String m_sSceneName;

		App::Actor::FastId m_iMeshActorID;
		Util::String m_sCurMeshName;
		Util::String m_sCurShaderName;
		Util::String m_sCurTexName;

		App::TActorPtr m_pSkyActor;
		App::TActorPtr m_pWaterActor;
		App::TActorPtr m_pTerrain;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline void DemoGameStateHandler::SetSceneName(const Util::String& sName)
	{
		this->m_sSceneName = sName;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Util::String& DemoGameStateHandler::GetSceneName() const
	{
		return this->m_sSceneName;
	}
}
#endif // __DEMOGAMESTATEHANDLER_H__