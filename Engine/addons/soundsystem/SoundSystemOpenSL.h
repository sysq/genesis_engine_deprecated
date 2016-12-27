#pragma once

#include "SoundSystem.h"

#include <vector>
#include <map>
#include <string>

struct SLObjectItf_;
typedef const struct SLObjectItf_ * const * SLObjectItf;
struct SLEngineItf_;
typedef const struct SLEngineItf_ * const * SLEngineItf;
struct SLPlayItf_;
typedef const struct SLPlayItf_ * const * SLPlayItf;
struct SLSeekItf_;
typedef const struct SLSeekItf_ * const * SLSeekItf;
struct SLEnvironmentalReverbItf_;
typedef const struct SLEnvironmentalReverbItf_ * const * SLEnvironmentalReverbItf;
struct SLVolumeItf_;
typedef const struct SLVolumeItf_ * const * SLVolumeItf;
struct SLPlaybackRateItf_;
typedef const struct SLPlaybackRateItf_ * const * SLPlaybackRateItf;
struct SLPitchItf_;
typedef const struct SLPitchItf_ * const * SLPitchItf;
struct SLMuteSoloItf_;
typedef const struct SLMuteSoloItf_ * const * SLMuteSoloItf;

namespace Sound
{
	struct SoundData //保存可播放的音乐结构体
	{
		SLObjectItf			mPlayerObj;
		SLPlayItf			mPlayItf;
		SLSeekItf			mSeekItf;
		SLVolumeItf			mVolumeItf;
		SLMuteSoloItf		mMuteSoloItf;
		SLPitchItf			mPitchItf;
		SLPlaybackRateItf   mPlayRateItf;

		bool				mIsLoaded;

		SoundData():
			mPlayerObj(0),
			mPlayItf(0),
			mSeekItf(0),
			mVolumeItf(0),
			mMuteSoloItf(0),
			mPitchItf(0),
			mPlayRateItf(0),
			mIsLoaded(false)
		{}
	};

	class SoundSystemOpenSL :  public SoundSystem
	{
		__DeclareSubClass(SoundSystemOpenSL, Sound::SoundSystem);
	public:

	private:
		typedef std::multimap< int, SoundData >	SoundDataMap;
	private:
		SLObjectItf						mEngineObject;
		SLObjectItf						mOutputMixObject;
		SLObjectItf						mListenerObject;
		
		SLEngineItf						mEngineItf;
		SLEnvironmentalReverbItf		mEnvItf;

		SoundDataMap					mSoundAssetMap;
	public:
		SoundSystemOpenSL(){}
		virtual ~SoundSystemOpenSL(){}
	public:
		void LoadSound( const Util::String& fileName, SoundData& soundData);

		virtual bool InitSoundSystem();
		virtual void ReleaseSoundSystem();

		virtual bool CreateSoundBuffer(const char* nameOrData, SizeT dataSize, SoundBuffer **soundBuffer, GENESISOUND_MODE mode, GENESIS_FILE_FORMAT fileFormat);

		virtual bool createDSPByType(ALuint type, GPtr<SoundSystemDSP>& dsp);	// create a filter

		virtual bool PlaySound(SoundBuffer* soundBuffer, SoundSystemSource **ssSource, bool paused);

		virtual bool ReleaseSingleSource(SoundSystemSource *source);
		virtual bool ReleaseSingleBuffer(SoundBuffer *buffer);

		virtual bool SetListenerAttrs(int listener,//该参数暂时无用，留作以后开发多listener功能时使用
			const Math::vector& position, 
			const Math::vector& velocity, 
			const Math::vector& up,
			const Math::vector& forward);

		virtual void SetBufferInfo(SoundBuffer::BufferInfo &bf){}
	};
}
