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
#if __USE_AUDIO__ || __GENESIS_EDITOR__
#include "stdneb.h"
#include "appframework/actor.h"
#include "appframework/editstatus.h"
#include "io/assignregistry.h"
#include "math/scalar.h"
#include "basegamefeature/managers/timesource.h"
#include "addons/resource/resourcemanager.h"

#include "SoundSourceComponent.h"
#include "soundfeature/components/SoundFilterComponent.h"
#include "soundfeature/SoundFeature.h"
#include "graphicfeature/graphicsfeatureprotocol.h"
#include "addons/soundsystem/SoundBufferOpenAL.h"

#if __ANDROID__
#include <SLES/OpenSLES.h>
#endif

namespace App
{

	__ImplementClass(SoundSource, 'SSCP', Component);

	using namespace Math;
	using namespace Graphic;
	//------------------------------------------------------------------------------
	SoundSource::SoundSource()
		: mStreamed(false)
		, mCompressed(true)
		, mSoundSystemSource(NULL)
		, mSoundRes(NULL)
		, mDecompress(false)
		, mStatistics(false)
		, mIsSetPlay(false)
		, mDirtyIndex(0)
		, mPlayOnActive(false)
		, mIsSoundDirty(false)
	{
		mSoundInfo.DopplerLevel = 1.0f;
		mSoundInfo.rolloffMode = ROLLOFF_INVERSE;
		mSoundInfo.priority = 128;
		mSoundInfo.position = 0;
		mSoundInfo.pan = 0.0f;
		mSoundInfo.pitch = 1.0f;
		mSoundInfo.playRate = 1000.0f;
		mSoundInfo.spread = 0.0f;
		mSoundInfo.minDistance = 1.0f;
		mSoundInfo.maxDistance = 5.0f;
		mSoundInfo.loop = false;
		mSoundInfo.mute = false;
		mSoundInfo.volume = 1.0f;


		mSoundRes = SoundRes::Create();
		n_assert(mSoundRes);
	}

	//------------------------------------------------------------------------------
	SoundSource::~SoundSource()
	{
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetupCallbacks()
	{
		mActor->RegisterComponentCallback(this, BeginFrame);
		mActor->RegisterComponentCallback(this, MoveAfter);
		return;
	}

	//------------------------------------------------------------------------------
	void SoundSource::_OnMoveAfter()
	{
		bool isPaused = false;
		if (!mSoundSystemSource)
			return;

		// 2D sound optimized
		if(!mSoundRes->Is3D())
			return;

		mSoundSystemSource->GetPaused(&isPaused);
		if (isPaused)
			return;

		Math::float4 pos = mActor->GetWorldPosition();
		Math::quaternion qua = mActor->GetWorldRotation();
		Math::float4 vel = Math::float4();
		
		mSoundSystemSource->SetModeAtts(pos, vel, qua);
		//mLatestPos = pos;
	}
	//------------------------------------------------------------------------------
	void SoundSource::_OnBeginFrame()
	{
#if __ANDROID__
#else 
		if (mIsSoundDirty)
		{
			n_assert(mSoundRes);

			if (mSoundRes->GetName() == "")
			{
				mIsSoundDirty = false;
				return;
			}

			if ( !mAudioRes.isvalid() )
			{	
				const Util::String resID = mSoundRes->GetName();
				GPtr<Resources::AudioResInfo> pAudioInfo = Resources::ResourceManager::Instance()->CreateAudioResInfo(resID, mSoundRes->GetPriority());
				if (pAudioInfo.isvalid() && pAudioInfo->GetRes().isvalid())
				{
					mAudioRes = pAudioInfo->GetRes().downcast<Resources::AudioRes>();
				}
				else
				{
					mAudioRes = NULL;
				}
			}
			else
			{
				/*The state of the resource is 'Failed' only if the user delete 
				the default audio resource.But this behavior is rare.So the 
				resource can only have two states('UnLoaded' and 'Loaded') when 
				the code is executed here.If the state of the resource is 'UnLoaded'
				,it means that the resource is loaded asynchronously and the loading
				request haven't been processed*/
				const Util::String resID = mSoundRes->GetName();
				GPtr<Resources::AudioResInfo> pAudioInfo = Resources::ResourceManager::Instance()->CreateAudioResInfo(resID, mSoundRes->GetPriority());
				if (pAudioInfo.isvalid() && pAudioInfo->GetRes().isvalid())
				{
					mAudioRes = pAudioInfo->GetRes().downcast<Resources::AudioRes>();
				}
				else
				{
					mAudioRes = NULL;
				}

				if (mAudioRes->GetState() == Resources::Resource::Loaded)
				{
					mIsSoundDirty = false;

					if( !mSoundRes->IsSoundValiad())
					{
						GENESISOUND_MODE mode = NULL;
						GENESIS_FILE_FORMAT fileFormat = NULL;
						/*If the audio file doesn't find in target path, the resource manager
						will use the default audio file(a empty wave file).In this case the 
						file name in the 'mAudioRes' is different from the file name in the
						'm_SoundRes'.But the address of the memory transmit to low level 
						sound system is the address of the default audio file.So we should
						use the file name in the 'mAudioRes' for file format determination.*/ 
						Util::String fileExtension =  mAudioRes->GetResourceId().AsString().GetFileExtension();
						fileExtension.ToLower();
						if ( "mp3" == fileExtension  )
						{
							fileFormat = fileFormat | GENESIS_FILE_FORMAT_MP3;
							mode = mode | GENESISSOUND_CREATESTREAM;
						}
						else if( "ogg" == fileExtension )
						{
							fileFormat = fileFormat | GENESIS_FILE_FORMAT_OGG;
							mode = mode | GENESISSOUND_CREATESTREAM;
						}
						else if ( "wav" == fileExtension)
						{
							fileFormat = fileFormat | GENESIS_FILE_FORMAT_WAV;
							mode = mode | GENESISSOUND_CREATESAMPLE;
						}

						mode = mode | GENESISSOUND_OPENMEMORY;

						/*Because switching the mode between 2D and 3D isn't supported
						when use hardware mix on win32 platform,so all the sound is
						created as 3D mode and simulate the switch between 2D and 3D
						by set the mode to GENESISSOUND_3D_HEADRELATIVE or 
						GENESISSOUND_3D_WORLDRELATIVE.*/
						mode = mode | GENESISSOUND_3D;

						if ( mode & GENESISSOUND_OPENMEMORY )
						{
							mSoundRes->CreateSoundBuffer(mAudioRes->GetPtr(), 
								mAudioRes->Size(), mode, fileFormat);
						}
						else
						{
							mSoundRes->CreateSoundBuffer(GetName().AsCharPtr(), 
								mAudioRes->Size(), mode,fileFormat);
						}
						if ( mIsSetPlay )
						{
							Play();
							mIsSetPlay = false;
						}
					}
				}
			}
		}
		else
		{
			if (mSoundSystemSource == NULL)
			{
				return;
			}

			Sound::SoundBuffer* soundBuffer = mSoundSystemSource->GetSoundBuffer();
			GENESISOUND_MODE mode = soundBuffer->GetBufferMode();
			if( mode & GENESISSOUND_CREATESTREAM)
			{
				GENESIS_FILE_FORMAT fileFormat = soundBuffer->GetFileFormat();
				int iBuffersProcessed = NULL;
				alGetSourcei(mSoundSystemSource->GetSource(), 
					AL_BUFFERS_PROCESSED, &iBuffersProcessed);
				
				if ( iBuffersProcessed != 0 )
				{
					GENESIS_FILE_FORMAT fileFormat = soundBuffer->GetFileFormat();

					const GPtr<Sound::SoundInterface>& SoundItf = SoundFeature::Instance()->GetSoundInterface();
					GPtr<Sound::SoundDecodeMsg> msg = Sound::SoundDecodeMsg::Create();
					msg->SetDecoder(this->GetSoundSystemSource());
					msg->SetFileFormat(fileFormat);
					msg->SetMsgHashCode(mSoundRes->GetName().HashCode());
					SoundItf->Send(msg);
				
					// 保存消息存根
					mAsynSoundQueue.AddBack(msg);
				}				
			}
		}

		for (SoundStubList::Iterator itr = mAsynSoundQueue.Begin(); 
			itr != mAsynSoundQueue.End(); )
		{
			if (!(*itr)->Handled())
			{
				++itr;
				continue;
			}

			SoundStubList::Iterator removeItr = itr;
			++itr;
			mAsynSoundQueue.Remove(removeItr);
		}
#endif
	}
	//------------------------------------------------------------------------------
	void SoundSource::OnActivate()
	{
		Super::OnActivate();
#if __EDIT_STATUS__
		if ( GetPlayOnActive() && EditStatus::IsPlayingGame() )
		{
			Play();
		}
#else
		if ( GetPlayOnActive())
		{
			Play();
		}
#endif

#if __ANDROID__
		
		SoundFeature::Instance()->AttachSoundSource(this);

#endif
		return;
	}

	//------------------------------------------------------------------------------
	void SoundSource::OnDeactivate()
	{
		Super::OnDeactivate();

		Stop();
#if __ANDROID__
		SoundFeature::Instance()->DetachSoundSource(this);
#endif
		return;
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetupAcceptedMessages()
	{
		Super::SetupAcceptedMessages();
	}
	//------------------------------------------------------------------------------
	void SoundSource::HandleMessage(const GPtr<Messaging::Message>& msg)
	{
		Super::HandleMessage(msg);
	}
	//------------------------------------------------------------------------------
	void SoundSource::AssignInfo(void)
	{
		SetPriority(mSoundInfo.priority);
		SetRolloffMode(mSoundInfo.rolloffMode);
		Set3D(Is3D());
		SetVolume(mSoundInfo.volume);
		SetLoop(mSoundInfo.loop);
		SetDopplerLevel(mSoundInfo.DopplerLevel);
		SetMute(mSoundInfo.mute);
		SetPitch(mSoundInfo.pitch);
		SetPlayRate(mSoundInfo.playRate);
		SetPan(mSoundInfo.pan);
		SetMaxDistance(mSoundInfo.maxDistance);
		SetMinDistance(mSoundInfo.minDistance);
		SetSpread(mSoundInfo.spread);
		//SetSoundInfo();

		return;
	}

	//------------------------------------------------------------------------------
	void SoundSource::Play(void)
	{
#if __ANDROID__
		if ( mSoundData.mIsLoaded )
		{
			SLresult result;

			result = (*mSoundData.mPlayItf)->SetPlayState(mSoundData.mPlayItf, SL_PLAYSTATE_STOPPED );
			n_assert(SL_RESULT_SUCCESS == result);

			SetVolume( mSoundInfo.volume );

			SetPlayRate( mSoundInfo.playRate );

			SetMute( mSoundInfo.mute );

			SetLoop( mSoundInfo.loop );

			SetPosition( mSoundInfo.position );

			n_warning("set playing state : SL_PLAYSTATE_PLAYING");
			result = (*mSoundData.mPlayItf)->SetPlayState(mSoundData.mPlayItf, SL_PLAYSTATE_PLAYING );
			n_assert(SL_RESULT_SUCCESS == result);

			return;
		}
#else
		if (!mActive)
		{
			return;
		}

		if (mIsSoundDirty)
		{
			mIsSetPlay = true;
			return;
		}

		// there are two conditions mIsSoundDirty can be false
		// 1.audio res file has been loaded
		// 2.audio res file name is ""
		// so we need to judge is the second condition or not before playing
		n_assert(mSoundRes);
		if (mSoundRes->GetName() == "")
		{
			return;
		}

		// when the sound res is loadded, sound Res may change, so we should judge the fmod sound
		// before playing
		if ( !mSoundRes->IsSoundValiad() )
		{
			return;
		}

		if ( !mSoundSystemSource )
		{
			mSoundSystemSource = mSoundRes->CreateSoundSystemSource();
			if (mSoundSystemSource)
			{
				AttachFilters();
				
				// assign property
				AssignInfo();

				// assign pos,rot,scale
				Math::float4 pos;
				Math::float4 vel;
				Math::quaternion qua;

				if(mSoundRes->Is3D())
				{
					pos = mActor->GetWorldPosition();
					qua = mActor->GetWorldRotation();
					vel = Math::float4();
				}
				else
				{
					pos = Math::float4( 0.0f, 0.0f, 0.0f, 0.0f);
					qua = Math::quaternion::identity();
					vel = Math::float4( 0.0f, 0.0f, 0.0f, 0.0f);
				}

				mSoundSystemSource->SetModeAtts(pos, vel, qua);
				//mLatestPos = pos;

				mSoundSystemSource->SetPaused(false);
				n_assert( SoundFeature::Instance() );
				SoundFeature::Instance()->AttachSoundSource(this);
			}
		}
		else
		{
			// when the mSoundSystemSource is not null, it means that we have execute playSound func
			// so the sound component can only be two state, one is playing , the other is pause
			if ( IsPlaying() )
			{
				return;
			}
			else
			{
				mSoundSystemSource->SetPaused(false);
			}
		}

		return;
#endif	
	}

	//------------------------------------------------------------------------------
	void SoundSource::Stop(void)
	{
#if __ANDROID__
		if ( mSoundData.mIsLoaded )
		{
			(*mSoundData.mPlayItf)->SetPlayState( mSoundData.mPlayItf, SL_PLAYSTATE_STOPPED);
		}
#else
		n_assert(SoundFeature::HasInstance());

		// Wait util msg is handled 
		IndexT hashCode = mSoundRes->GetName().HashCode();
		for (SoundStubList::Iterator itr = mAsynSoundQueue.Begin(); 
			itr != mAsynSoundQueue.End(); )
		{
			// 判定已发送的异步消息是否处理完毕
			if ((*itr)->GetMsgHashCode() == hashCode)
			{
				if(!(*itr)->Handled())
				{
					// 没有处理完,等待处理完成,并移除存根
                    SoundFeature::Instance()->GetSoundInterface()->Wait(*itr);
				}

				SoundStubList::Iterator removeItr = itr;
				++itr;
				mAsynSoundQueue.Remove(removeItr);
				continue;
			}
			
			++itr;
		}

		if (mSoundSystemSource)
		{	
			mSoundSystemSource->Stop();
		}

		mSoundSystemSource = NULL;
		mSoundRes->SetSoundSystemSource(NULL);

		SoundFeature::Instance()->DetachSoundSource(this);

		mIsSetPlay = false;

		return;
#endif
	}

	//------------------------------------------------------------------------------
	void SoundSource::Pause(void)
	{
#if __ANDROID__
		if ( mSoundData.mIsLoaded )
		{
			(*mSoundData.mPlayItf)->SetPlayState(mSoundData.mPlayItf, SL_PLAYSTATE_PAUSED);
			n_warning("pause music %s", GetName().AsCharPtr());
		}
#else
		if (mSoundSystemSource)
		{
			mSoundSystemSource->SetPaused(true);
		}
#endif
	}

	//------------------------------------------------------------------------------
	void SoundSource::SetDirty()
	{
		mDirtyIndex++;
		if (mDirtyIndex == 0)
		{
			mDirtyIndex = 1;
		}

		return;
	}

	//------------------------------------------------------------------------------
	void SoundSource::ClearDirty()
	{
		mDirtyIndex = 0;
		return;
	}

	//------------------------------------------------------------------------------
	void SoundSource::SetDirtyIndex(const int dirtyValue)
	{
		mDirtyIndex = dirtyValue;
		return;
	}

	//------------------------------------------------------------------------------
	float SoundSource::GetVolume() const
	{
		return mSoundInfo.volume;
	}

	//------------------------------------------------------------------------------
	void SoundSource::SetVolume(const float volume)
	{
		mSoundInfo.volume = Math::n_clamp(volume, 0.0f, 1.0f);

#if __ANDROID__
		SLresult result;
		if ( mSoundData.mIsLoaded && mSoundData.mVolumeItf )
		{
			if(Math::n_fequal(mSoundInfo.volume,0.0f, float3::epsilon))
			{
				(*mSoundData.mVolumeItf)->SetMute( mSoundData.mVolumeItf, SL_BOOLEAN_TRUE);
			}
			else
			{
				(*mSoundData.mVolumeItf)->SetMute( mSoundData.mVolumeItf, SL_BOOLEAN_FALSE);
				SLmillibel finalVolume = (SLmillibel)( -2000 * (1.0-mSoundInfo.volume));
				result = (*mSoundData.mVolumeItf)->SetVolumeLevel(mSoundData.mVolumeItf,finalVolume);
				n_warning("sound volume set failed");
			}
		}
		else
		{
			n_warning("device set volume interface can't get");
		}
#else
		if (mSoundSystemSource)
		{
			mSoundSystemSource->SetVolume(mSoundInfo.volume);
		}
#endif
	}

	//------------------------------------------------------------------------------
	void SoundSource::SetPlayOnActive(const bool playOnActive)
	{
		mPlayOnActive = playOnActive;
		return;
	}

	//------------------------------------------------------------------------------
	bool SoundSource::GetMute() const
	{
		return mSoundInfo.mute;
	}

	//------------------------------------------------------------------------------
	void SoundSource::SetMute(const bool mute)
	{
		mSoundInfo.mute = mute;
#if __ANDROID__
		SLresult result;
		if ( mSoundData.mIsLoaded && mSoundData.mMuteSoloItf )
		{
			result = (*mSoundData.mMuteSoloItf)->SetChannelMute( mSoundData.mMuteSoloItf, 0, mSoundInfo.mute ? SL_BOOLEAN_TRUE : SL_BOOLEAN_FALSE);
			result = (*mSoundData.mMuteSoloItf)->SetChannelMute( mSoundData.mMuteSoloItf, 1, mSoundInfo.mute ? SL_BOOLEAN_TRUE : SL_BOOLEAN_FALSE);
		}
		else
		{
			if ( mSoundData.mIsLoaded && mSoundData.mVolumeItf )
			{
				result = (*mSoundData.mVolumeItf)->SetMute(mSoundData.mVolumeItf, mSoundInfo.mute ? SL_BOOLEAN_TRUE : SL_BOOLEAN_FALSE );
				n_assert( SL_RESULT_SUCCESS == result );
			}
		}
#endif

		return;
	}

	//------------------------------------------------------------------------------
	bool SoundSource::GetLoop(void) const
	{
		return mSoundInfo.loop;
	}

	//------------------------------------------------------------------------------
	void SoundSource::SetLoop(const bool loop)
	{
		mSoundInfo.loop = loop;
#if __ANDROID__
		SLresult result;
		if ( mSoundData.mIsLoaded && mSoundData.mSeekItf )
		{
			result = (*mSoundData.mSeekItf)->SetLoop(mSoundData.mSeekItf, mSoundInfo.loop ? SL_BOOLEAN_TRUE : SL_BOOLEAN_FALSE, 0, SL_TIME_UNKNOWN);
			n_assert(SL_RESULT_SUCCESS == result);
		}
		else
		{
			n_warning("device not supported loop op");
		}
#else
		if (mSoundSystemSource)
		{
			mSoundSystemSource->SetLoop( loop );
		}
#endif
		return;
	}

	//------------------------------------------------------------------------------
	void SoundSource::UpdateSoundInfo()
	{

	}
	//------------------------------------------------------------------------------
	bool SoundSource::IsPlaying(void) const
	{
#if __ANDROID__
		if( mSoundData.mIsLoaded )
		{
			SLuint32 state;
			(*mSoundData.mPlayItf)->GetPlayState(mSoundData.mPlayItf, &state);
			n_warning("Sound is %s", SL_PLAYSTATE_PLAYING == state ? "playing" : "stopped");
			return SL_PLAYSTATE_PLAYING == state;
		}
#else
		if (mSoundSystemSource)
		{
			bool result;
			bool isPlaying = false;
			bool isPaused = false;

			result = mSoundSystemSource->IsPlaying(&isPlaying);
			result = mSoundSystemSource->GetPaused(&isPaused);

			return result && ((isPlaying) && (!isPaused));
		}
		return false;
#endif
	}

	//------------------------------------------------------------------------------
	bool SoundSource::IsPaused(void) const
	{
#if __ANDROID__
		if( mSoundData.mIsLoaded )
		{
			SLuint32 state;
			(*mSoundData.mPlayItf)->GetPlayState(mSoundData.mPlayItf, &state);
			return SL_PLAYSTATE_PAUSED == state;
		}
#else
		if (mSoundSystemSource)
		{
			bool result;
			bool isPaused = false;

			result = mSoundSystemSource->GetPaused(&isPaused);
			return ( result == true ) && isPaused;
		}
		return false;
#endif
	}

	//------------------------------------------------------------------------------
	bool SoundSource::Init(const Util::String &name, const bool &is3D)
	{
		bool bIsSetName = SetName(name);
		bool bIsSet3D = Set3D(is3D);
		return (bIsSetName && bIsSet3D);
	}

	//------------------------------------------------------------------------------
	bool SoundSource::Is3D(void) const
	{
		n_assert(mSoundRes);
		return mSoundRes->Is3D();
	}

	//------------------------------------------------------------------------------
	bool SoundSource::Set3D(const bool &is3D)
	{
		n_assert(mSoundRes);
		bool result = mSoundRes->Set3D(is3D);

		if(result && Is3D())
			_OnMoveAfter();

		return result;
	}

	//------------------------------------------------------------------------------
	const Util::String& SoundSource::GetName(void) const
	{
		return mSoundRes->GetName();
	}

	//------------------------------------------------------------------------------
	/**
	set the sound name, discard origin sound and set the sound to be dirty
	at next frame in the soundsource's _OnBeginFrame function create new sound
	*/
	bool SoundSource::SetName(const Util::String &name, Resources::Priority priority )
	{
#if __ANDROID__
		n_warning("prepare to load audio");
		// mSoundRes->SetName( name, priority);

		Sound::SoundSystemOpenSL* sl = dynamic_cast< Sound::SoundSystemOpenSL* >( Sound::SoundSystem::Instance() );

		sl->LoadSound( name , mSoundData);
		n_warning("load audio ok");
#else
		n_assert(mSoundRes);

		Util::String previousName = GetName();
		if (  previousName == name )
			return true;

		if ( name.IsEmpty())
			SetSoundDirty(false);	// when editor or script delete or stop sound , the name would be empty,mIsSoundDirty state should be false
		else
			SetSoundDirty(true);	// when chanding the sound name , the name is not empty , mIsSoundDirty state should be true

		if (mActor && mActor->PriorityDefinition())
			priority = mActor->GetPriority();

		mSoundRes->SetName(name, priority);	

		return true;
#endif
	}

	//------------------------------------------------------------------------------
	uint SoundSource::GetLength(void) const
	{
		if (mSoundRes)
		{
			return mSoundRes->GetLength();
		}

		return 0;
	}

	//------------------------------------------------------------------------------
	float SoundSource::GetPitch(void) const
	{
		return mSoundInfo.pitch;
	}

	//------------------------------------------------------------------------------
	void  SoundSource::SetPitch(const float pitch)
	{
		mSoundInfo.pitch = pitch;
#if __ANDROID__
		if ( mSoundData.mIsLoaded && mSoundData.mPitchItf )
		{
			(*mSoundData.mPitchItf)->SetPitch(mSoundData.mPitchItf, pitch);
		}
		else
		{
			n_warning("device don't support setPitch interface");
		}
#else
		if (mSoundSystemSource && mSoundRes.isvalid())
		{
			mSoundSystemSource->SetPitch(pitch);
		}
#endif
	}
	//------------------------------------------------------------------------------
	float SoundSource::GetPlayRate(void) const
	{
		return mSoundInfo.playRate;
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetPlayRate(const float playRate)
	{
		mSoundInfo.playRate = playRate;
#if __ANDROID__
		if( mSoundData.mIsLoaded && mSoundData.mPlayRateItf )
		{
			(*mSoundData.mPlayRateItf)->SetRate( mSoundData.mPlayRateItf, playRate);
		}
		else
		{
			printf("Device don't support setPlayRate interface");
		}
#endif
	}
	//------------------------------------------------------------------------------
	int SoundSource::GetPriority() const
	{
		return mSoundInfo.priority;
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetPriority(int priority)
	{
		priority = Math::n_clamp(priority, 0, 256);
		mSoundInfo.priority = priority;

		return;
	}
	//------------------------------------------------------------------------------
	bool SoundSource::IsSteamed() const
	{
		// TODO
		return mStreamed;
	}

	void SoundSource::SetSteamed(bool b)
	{
		mStreamed = b;
	}
	//------------------------------------------------------------------------------
	bool SoundSource::IsDecompress() const
	{
		// TODO
		return mDecompress;
	}

	void SoundSource::SetDecompress(bool b)
	{
		mDecompress = b;
	}
	//------------------------------------------------------------------------------
	bool SoundSource::IsCompressed() const
	{
		// TODO
		return mCompressed;
	}

	void SoundSource::SetCompressed(bool b)
	{
		mCompressed = b;
	}
	//------------------------------------------------------------------------------
	bool SoundSource::IsStatistics() const
	{
		return mStatistics;
	}

	void SoundSource::SetStatistics(bool b)
	{
		mStatistics = b;
	}
	//------------------------------------------------------------------------------
	float SoundSource::GetSpread(void) const
	{
		return mSoundInfo.spread;
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetSpread(float spread)
	{
		spread = Math::n_clamp(spread, 0.0f, 360.0f);
		mSoundInfo.spread = spread;
		return;
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetDopplerLevel(float level)
	{
		level = Math::n_clamp(level, 0.0f, 5.0f);
		mSoundInfo.DopplerLevel = level;
		return;
	}
	//------------------------------------------------------------------------------
	float SoundSource::GetDopplerLevel() const
	{
		return mSoundInfo.DopplerLevel;
	}
	//------------------------------------------------------------------------------
	float SoundSource::GetPan() const
	{
		return mSoundInfo.pan;
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetPan(float pan)
	{
		pan = Math::n_clamp(pan, -1.0f, 1.0f);
		mSoundInfo.pan = pan;

		return;
	}
	//------------------------------------------------------------------------------
	float SoundSource::GetMinDistance() const
	{
		return mSoundInfo.minDistance;
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetMinDistance(float minDistance)
	{
		minDistance = Math::n_clamp( minDistance, 0.3f, 999.9f);

		mSoundInfo.minDistance = minDistance;
		if (mSoundInfo.minDistance < 0.3)
		{
			mSoundInfo.minDistance = 0.3f;
		}
		if (mSoundInfo.maxDistance < (mSoundInfo.minDistance + 0.1f))
		{
			mSoundInfo.maxDistance = mSoundInfo.minDistance + 0.1f;
		}
		if (mSoundSystemSource)
		{
			mSoundSystemSource->Set3DMinMaxDistance(mSoundInfo.minDistance,
				mSoundInfo.maxDistance);
		}

		return;
	}
	//------------------------------------------------------------------------------
	float SoundSource::GetMaxDistance() const
	{
		return mSoundInfo.maxDistance;
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetMaxDistance(float maxDistance)
	{
		maxDistance = Math::n_clamp( maxDistance, 0.4f, 1000.0f);

		mSoundInfo.maxDistance = maxDistance;

		if (mSoundInfo.maxDistance < 0.4)
		{
			mSoundInfo.maxDistance = 0.4f;
		}
		if (mSoundInfo.minDistance > (mSoundInfo.maxDistance - 0.1f))
		{
			mSoundInfo.minDistance = mSoundInfo.maxDistance - 0.1f;
		}

		if (mSoundSystemSource)
		{
			mSoundSystemSource->Set3DMinMaxDistance(mSoundInfo.minDistance,
				mSoundInfo.maxDistance);
		}

		return;
	}
	//------------------------------------------------------------------------------
	RolloffMode SoundSource::GetRolloffMode() const
	{
		return mSoundInfo.rolloffMode;
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetRolloffMode(RolloffMode mode)
	{
		mSoundInfo.rolloffMode = mode;

		if(mSoundSystemSource == NULL)
			return;

		switch (mode)
		{
		case ROLLOFF_INVERSE :
			mSoundSystemSource->SetMode(GENESISSOUND_3D_INVERSEROLLOFF);
			break;
		case ROLLOFF_LINEAR :
			mSoundSystemSource->SetMode(GENESISSOUND_3D_LINEARROLLOFF);
			break;
		case ROLLOFF_LINEARSQUARE :
			mSoundSystemSource->SetMode(GENESISSOUND_3D_LINEARSQUAREROLLOFF);
			break;
		}

		if(Is3D())
			_OnMoveAfter();
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetPosition(uint position)
	{
		mSoundInfo.position = position;
#if __ANDROID__
		SLresult result;
		if ( mSoundData.mIsLoaded && mSoundData.mSeekItf )
		{
			result = (*mSoundData.mSeekItf)->SetPosition(mSoundData.mSeekItf, 0, SL_SEEKMODE_FAST);
			n_assert(SL_RESULT_SUCCESS == result);
		}
#endif
		return;
	}
	//------------------------------------------------------------------------------
	uint SoundSource::GetPosition(void) const
	{
		return mSoundInfo.position;
	}
	//------------------------------------------------------------------------------
	int SoundSource::GetDecompressFlag(void) const
	{
		n_assert(mSoundRes.isvalid());
		return mSoundRes->GetDecompressFlag();
	}
	//------------------------------------------------------------------------------
	void SoundSource::SetDecompressFlag(const int flag)
	{
		n_assert(mSoundRes.isvalid());
		mSoundRes->SetDecompressFlag(flag);
		return;
	}
	//------------------------------------------------------------------------
	void SoundSource::CopyFrom( const GPtr<Component>& pComponent )
	{
		if( !pComponent.isvalid()  )
			return;
		if( !pComponent->GetRtti()->IsDerivedFrom( *(this->GetRtti()) ) )
			return;
		// TODO:
		GPtr<SoundSource> pSource = pComponent.downcast<SoundSource>();

		mPlayOnActive = pSource->GetPlayOnActive();
		mSoundInfo.loop = pSource->GetLoop();
		mSoundInfo.mute = pSource->GetMute();
		mSoundInfo.volume = pSource->GetVolume();
		mSoundInfo.pitch = pSource->GetPitch();
		mSoundInfo.playRate = pSource->GetPlayRate();
		mSoundInfo.spread = pSource->GetSpread();
		mSoundInfo.pan = pSource->GetPan();
		mSoundInfo.minDistance = pSource->GetMinDistance();
		mSoundInfo.maxDistance = pSource->GetMaxDistance();
		mSoundInfo.rolloffMode = pSource->GetRolloffMode();

		mSoundRes->CopyFrom(*(pSource->GetSoundRes()));
		// when execute m_SoundRes->CopyFrom(*(pSource->GetSoundRes())); m_Sound would set be null,
		// mSound pointer would get reset when in the _OnBeginFrame() function is called
		// so we need to set the mSoundDirty to be true. So the mSound can be revalued.
		if (mSoundRes->GetName() != "")
		{
			SetSoundDirty(true);
		}
	}
	//--------------------------------------------------------------------------------
	void SoundSource::SetSoundDirty(bool bDirty)
	{
		Stop();

		n_assert(mSoundRes);
		mSoundRes->Discard();

		if ( mAudioRes.isvalid() )
			mAudioRes = NULL;

		mIsSoundDirty = bDirty;

	}
	//------------------------------------------------------------------------------
	void SoundSource::AttachFilters()
	{
		if ( !mSoundSystemSource )
			return;

		const Actor* actor = GetActor();
		Util::Array<GPtr<Component> > FilterArray = actor->FindComponents(SoundFilterComponent::RTTI);

		for (int i=0;i<FilterArray.Size();i++)
		{
			GPtr<SoundFilterComponent> pSoundFilter = FilterArray[i].downcast<SoundFilterComponent>();

			if (pSoundFilter.isvalid())
			{
				GPtr<Sound::SoundSystemDSP> dsp = pSoundFilter->GetDSP();
				if (dsp.isvalid())
				{
					uint dspType = dsp->getType();
					if (AL_FILTER_LOWPASS == dspType)
					{
						dsp->apply(mSoundSystemSource, NULL);
						pSoundFilter->SetDspParamter();
					} 
					else
					{
						int iSendIndex = dsp->GetSendIndex();
						dsp->apply(mSoundSystemSource, iSendIndex);
						pSoundFilter->SetDspParamter();
					}
				}
			}
		}
	}

	//------------------------------------------------------------------------------
	void SoundSource::OnDestroy()
	{
		Stop();
		Super::OnDestroy();
	}
	//------------------------------------------------------------------------------
	void SoundSource::GetReferenceResourceId(Util::Array<Resources::ReferenceResource>& list) const
	{
		list.Append(Resources::ReferenceResource(GetName(), Resources::RR_Unknown));
		Super::GetReferenceResourceId(list);
	}

	//------------------------------------------------------------------------------
}
#endif // __USE_AUDIO__ || __GENESIS_EDITOR__
