/****************************************************************************
Copyright (c) 2011-2013,WebJet Business Division,CYOU
 
http://www.genesis-3d.com.cn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "stdneb.h"
#include "profilesystem/ProfileSystem.h"
#include "scriptfeature/inc/script_feature.h"
#include "scriptfeature/mono_script.h"
#include "scriptfeature/inc/script_instance.h"
#include "scriptfeature/inc/script_component.h"

namespace App
{
	__ImplementClass( App::ScriptFeature, 'SCSR', App::Feature );
	__ImplementImageSingleton( App::ScriptFeature );

	//------------------------------------------------------------------------
	ScriptFeature::ScriptFeature()
		:m_nSwapIndex(0)
	{
		__ConstructImageSingleton;

		mFeatureName = "Script";
	}
	//------------------------------------------------------------------------
	ScriptFeature::~ScriptFeature()
	{
		for ( int i = 0; i<m_ScriptInstanceArraies.Size(); i++ )
		{
			for ( int j = 0; i<m_ScriptInstanceArraies[i].Size(); j++)
			{
				m_ScriptInstanceArraies[i][j] = NULL;
			}
			m_ScriptInstanceArraies[i].Clear();
		}
		m_ScriptInstanceArraies.Clear();
		__DestructImageSingleton;
	}
	//------------------------------------------------------------------------
	void ScriptFeature::OnActivate()
	{
		Feature::OnActivate();

		m_ScriptInstanceArraies.Reserve( 20 );
	}
	//------------------------------------------------------------------------
	void ScriptFeature::OnDeactivate()
	{
		m_ScriptInstanceArraies.Clear();
		m_pScriptComs.Clear();

		Feature::OnDeactivate();
	}
	//------------------------------------------------------------------------
	void ScriptFeature::OnBeginFrame()
	{
#if __EDIT_STATUS__
		if (!EditStatus::IsPlayingGame())
		{
			return;
		}
#endif
		PROFILER_ADDDTICKBEGIN(scriptsTime);
		Utility_ScriptRootTick();
		for( IndexT i=0; i < m_ScriptInstanceArraies.Size(); ++i )
		{

			for( IndexT j=0; j < m_ScriptInstanceArraies[i].Size(); ++j )
			{
				if ( m_ScriptInstanceArraies[i][j].isvalid() )
				{
					
					m_ScriptInstanceArraies[i][j]->OnBeginFrame();
				}
			}
		}
		PROFILER_ADDDTICKEND(scriptsTime);
	}
	//------------------------------------------------------------------------
	void ScriptFeature::OnFrame()
	{
#if __EDIT_STATUS__
		if (!EditStatus::IsPlayingGame())
		{
			return;
		}
#endif
		PROFILER_ADDDTICKBEGIN(scriptsTime);
		for( IndexT i=0; i < m_ScriptInstanceArraies.Size(); ++i )
		{
			for( IndexT j=0; j < m_ScriptInstanceArraies[i].Size(); ++j)
			{
				if (m_ScriptInstanceArraies[i][j].isvalid())
				{
					m_ScriptInstanceArraies[i][j]->OnFrame();
				}
			}
		}
		PROFILER_ADDDTICKEND(scriptsTime);
	}
	//------------------------------------------------------------------------
	void ScriptFeature::OnEndFrame()
	{
#if __EDIT_STATUS__
		if (!EditStatus::IsPlayingGame())
		{
			return;
		}
#endif
		PROFILER_ADDDTICKBEGIN(scriptsTime);
		for ( IndexT i=0; i<m_ScriptInstanceArraies.Size(); ++i )
		{

			for ( IndexT j=0; j<m_ScriptInstanceArraies[i].Size(); ++j )
			{
				if ( m_ScriptInstanceArraies[i][j].isvalid() )
				{
					m_ScriptInstanceArraies[i][j]->OnEndFrame();
				}
			}
		}

		//release delay object
		m_CriticalSection.Enter();
		int currentIndex = m_nSwapIndex;
		m_nSwapIndex = !m_nSwapIndex;
		m_CriticalSection.Leave();
		for ( int i = 0 ; i < m_DelayReleaseArray[currentIndex].Size(); ++ i )
		{
			m_DelayReleaseArray[currentIndex][i]->Release();
		}
		m_DelayReleaseArray[currentIndex].Clear(false);
		PROFILER_ADDDTICKEND(scriptsTime);
	}
	//------------------------------------------------------------------------
	void ScriptFeature::OnStopped()
	{
#if __EDIT_STATUS__
		if (!EditStatus::IsPlayingGame())
		{
			return;
		}
#endif
		for ( IndexT i=0; i<m_ScriptInstanceArraies.Size(); ++i )
		{

			for ( IndexT j=0; j<m_ScriptInstanceArraies[i].Size(); ++j )
			{
				if ( m_ScriptInstanceArraies[i][j].isvalid() )
				{
					m_ScriptInstanceArraies[i][j]->OnStopped();
				}
			}
		}
		Utility_ScriptRootStop();
	}
	//------------------------------------------------------------------------
	void ScriptFeature::OnResumed()
	{
#if __EDIT_STATUS__
		if (!EditStatus::IsPlayingGame())
		{
			return;
		}
#endif
		for ( IndexT i=0; i<m_ScriptInstanceArraies.Size(); ++i )
		{

			for ( IndexT j=0; j<m_ScriptInstanceArraies[i].Size(); ++j )
			{
				if ( m_ScriptInstanceArraies[i][j].isvalid() )
				{
					m_ScriptInstanceArraies[i][j]->OnResumed();
				}
			}
		}
		Utility_ScriptRootResume();
	}
	//------------------------------------------------------------------------
	void ScriptFeature::AttachScriptInstances(const ScriptInstances &scriptInstances)
	{
		IndexT found = m_ScriptInstanceArraies.FindIndex( scriptInstances );

		if ( found == InvalidIndex )
		{
			m_ScriptInstanceArraies.Append( scriptInstances );
#if __EDIT_STATUS__
			if (!EditStatus::IsPlayingGame())
			{
				return;
			}
#endif
			ScriptInstances::Iterator it = scriptInstances.Begin();
			ScriptInstances::Iterator end = scriptInstances.End();
			while(it != end)
			{
				if ((*it).isvalid())
				{
					(*it)->OnLoad();
				}
				++it;
			}
		}
	}
	//------------------------------------------------------------------------
	void ScriptFeature::AttachScriptCom(App::ScriptComponent* pCom)
	{
		IndexT found = m_pScriptComs.FindIndex( pCom );

		if ( found==InvalidIndex )
		{
			m_pScriptComs.Append( pCom );
		}
	}

	void ScriptFeature::RemoveScriptInstances( const ScriptInstances& scriptInstances )
	{	
		IndexT found = m_ScriptInstanceArraies.FindIndex( scriptInstances );
		if( found!=InvalidIndex )
		{
			m_ScriptInstanceArraies.EraseIndex( found );
#if __EDIT_STATUS__
			if (!EditStatus::IsPlayingGame())
			{
				return;
			}
#endif
			ScriptInstances::Iterator it = scriptInstances.Begin();
			ScriptInstances::Iterator end = scriptInstances.End();
			while(it != end)
			{
				if ((*it).isvalid())
				{
					(*it)->OnExit();
				}
				++it;
			}
		}
	}

	void ScriptFeature::RemoveScriptCom( ScriptComponent* const pCom )
	{
		IndexT found = m_pScriptComs.FindIndex( pCom );
		if( found!=InvalidIndex )
		{
			m_pScriptComs.EraseIndex( found );
		}
	}

	void ScriptFeature::RemoveScript( ScriptComponent* const pCom, const ScriptInstances& scriptInstances )
	{
		RemoveScriptCom(pCom);
		RemoveScriptInstances(scriptInstances);
	}

	void ScriptFeature::AttachScript( ScriptComponent* const pCom, const ScriptInstances& scriptInstances )
	{
		AttachScriptCom(pCom);
		AttachScriptInstances(scriptInstances);
	}

	void ScriptFeature::PushObjectToDelayRelease( RefCounted* obj )
	{
		m_CriticalSection.Enter();
		m_DelayReleaseArray[m_nSwapIndex].Append( obj );
		m_CriticalSection.Leave();
	}

	void ScriptFeature::_ClearObjectBuffers()
	{
		m_CriticalSection.Enter();
		for ( int i = 0 ; i < SC_SWAP_BUFFER_SIZE; ++ i )
		{
			for ( int j = 0 ; j < m_DelayReleaseArray[i].Size(); ++ i )
			{
				m_DelayReleaseArray[i][j]->Release();
			}
			m_DelayReleaseArray[i].Clear(false);
		}
		m_CriticalSection.Leave();
	}

	void ScriptFeature::OnStop()
	{
		_ClearObjectBuffers();
		Feature::OnStop();
	}

}