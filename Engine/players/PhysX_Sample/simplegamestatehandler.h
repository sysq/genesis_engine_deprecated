#pragma once

#include "appframework/statehandler.h"
#include "appframework/actor.h"


namespace Sample
{

	class SimpleStateHandler : public App::StateHandler
	{
		__DeclareClass(SimpleStateHandler);
	public:    

		/// constructor
		SimpleStateHandler();
		/// destructor
		virtual ~SimpleStateHandler();

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

		void CreateBox(const Math::float3& f3Dim);
		void CreateSphere();
		void CreateCapsule();
		void CreateTeapot();
		void CreateStatue();
		void CreateStacks(IndexT size);
		void CreateTower(IndexT size);
		void CreateConvex(); 
		void RemoveAll();

		void LoadRAW(int m, int n, const std::string& filename, float heightScale, float heightOffset, float *height);

	private:    
		Util::String mSceneName;

		App::Actor::FastId mMeshActorID;
		Util::String mCurMeshName;
		Util::String mCurShaderName;
		Util::String mCurTexName;

		Util::Array< Ptr<App::Actor> >   m_pBoxesActors;

		Ptr<App::Scene>           m_pScene;

		Ptr<App::Actor>           m_pCam;
		Ptr<App::Actor>			  m_pActLight;

	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		SimpleStateHandler::SetSceneName(const Util::String& n)
	{
		this->mSceneName = n;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Util::String&
		SimpleStateHandler::GetSceneName() const
	{

		return this->mSceneName;
	}
}