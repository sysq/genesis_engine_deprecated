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
#if __ANDROID__


#include "stdneb.h"
#include "SoundSystemOpenSL.h"
#include "SoundSystemDSPOpenSL.h"
#include "Framework.h"
#include "io/memorystream.h"
#include "io/assignregistry.h"
#include "app/appframework/gameapplication.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include <sys/types.h>
#include "players/GenesisA/simplegameapplication.h"

namespace Sound
{
	__ImplementClass(Sound::SoundSystemOpenSL, 'SBOS', Sound::SoundBuffer);
	bool SoundSystemOpenSL::InitSoundSystem()
	{
		SLresult result;
		// create engine
		result = slCreateEngine(&mEngineObject, 0, NULL, 0, NULL, NULL);
		n_assert(SL_RESULT_SUCCESS == result);

		// realize the engine
		result = (*mEngineObject)->Realize(mEngineObject, SL_BOOLEAN_FALSE);
		n_assert(SL_RESULT_SUCCESS == result);

		// get the engine interface
		result = (*mEngineObject)->GetInterface(mEngineObject, SL_IID_ENGINE, &mEngineItf); 
		n_assert(SL_RESULT_SUCCESS == result);

		// create output mix
		const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
		const SLboolean req[1] = {SL_BOOLEAN_FALSE};
		result = (*mEngineItf)->CreateOutputMix(mEngineItf, &mOutputMixObject, 1, ids, req);
		n_assert(SL_RESULT_SUCCESS == result);

		n_warning("prepare to get outputmix object");

		// realize the output mix
		result = (*mOutputMixObject)->Realize(mOutputMixObject, SL_BOOLEAN_FALSE);
		n_assert(SL_RESULT_SUCCESS == result);

		n_warning("after to get outputmix object");

		// get the environmental reverb interface
		result = (*mOutputMixObject)->GetInterface(mOutputMixObject, SL_IID_ENVIRONMENTALREVERB, &mEnvItf);
		if (SL_RESULT_SUCCESS == result) 
		{
			SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
			result = (*mEnvItf)->SetEnvironmentalReverbProperties(mEnvItf, &reverbSettings);
		}
		else
		{
			n_warning("SL_IID_ENVIRONMENTALREVERB interface not supported\n");
		}

		n_warning("SLES System prepare OK");
		return true;
	}

	void SoundSystemOpenSL::ReleaseSoundSystem()
	{
		if (mListenerObject)
		{
			(*mListenerObject)->Destroy(mListenerObject);
			mListenerObject = NULL;
		}

		// Çå³ýÉè±¸
		if(mOutputMixObject)
		{
			(*mOutputMixObject)->Destroy(mOutputMixObject);
			mOutputMixObject = NULL;
		}

		if(mEngineObject)
		{
			(*mEngineObject)->Destroy(mEngineObject);
			mEngineObject = NULL;
		}
	}

	void SoundSystemOpenSL::LoadSound(const Util::String& infileName, SoundData& soundData )
	{
		SLresult result;

		IndexT hashCode = infileName.HashCode();
		SoundDataMap::iterator it = mSoundAssetMap.find( hashCode );
		if ( mSoundAssetMap.end() != it )
		{
			soundData = it->second;
			return;
		}

		// aseet:Sound/XX.mp3
		int splitIndex = infileName.FindCharIndex(':',0);

		// Data/Asset/Sound/xx.mp3
		Util::String finalPath = "Data/Asset/";
		finalPath += infileName.ExtractRange(splitIndex+1, infileName.Length() - splitIndex - 1);

		// use asset manager to open asset by filename		
		AAssetManager* assetMgr = Genesis::DemoPublishGameApplication::Instance()->GetJNIAssetManager();
		n_assert(NULL != assetMgr);

		// config audio source
		n_warning("try to load file %s", finalPath.AsCharPtr());
		AAsset* asset = AAssetManager_open(assetMgr, finalPath.AsCharPtr(), AASSET_MODE_UNKNOWN);

		// the asset might not be found
		if ( NULL == asset )
		{
			n_warning("asset get failed");
			return;
		}

	    // open asset as file descriptor
		off_t start, length;
		int fd = AAsset_openFileDescriptor(asset, &start, &length);
		n_assert( 0 <= fd );
		AAsset_close(asset);

		// configure audio source
		SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
		SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
		SLDataSource audioSrc = {&loc_fd, &format_mime};

		// config audio sink
		SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, mOutputMixObject};
		SLDataSink audioSnk = {&loc_outmix, NULL};

		// create audio player
		const int interface_req = 5;
		const SLInterfaceID ids[interface_req] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME, SL_IID_PITCH, SL_IID_PLAYBACKRATE};
		const SLboolean req[interface_req] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE};
		result = (*mEngineItf)->CreateAudioPlayer(mEngineItf, &(soundData.mPlayerObj), &audioSrc, &audioSnk, interface_req, ids, req);
		n_assert(SL_RESULT_SUCCESS == result);

		// realize the player
		result = (*(soundData.mPlayerObj))->Realize(soundData.mPlayerObj, SL_BOOLEAN_FALSE);
		if (SL_RESULT_SUCCESS != result) {
			n_error("Can't load sound file with name %s, error code %d\n", finalPath.AsCharPtr(), result);
			(*(soundData.mPlayerObj))->Destroy(soundData.mPlayerObj);
			soundData.mPlayerObj = NULL;
			soundData.mIsLoaded = false;
			return;
		}
		
		n_warning("load file :%s success\n", finalPath.AsCharPtr());
		// get the play interface
		result = (*(soundData.mPlayerObj))->GetInterface(soundData.mPlayerObj, SL_IID_PLAY, &(soundData.mPlayItf));
		n_assert(SL_RESULT_SUCCESS == result);

		// get the seek interface
		result = (*(soundData.mPlayerObj))->GetInterface(soundData.mPlayerObj, SL_IID_SEEK, &(soundData.mSeekItf));
		n_assert(SL_RESULT_SUCCESS == result);

		// get the volume interface
		result = (*(soundData.mPlayerObj))->GetInterface(soundData.mPlayerObj, SL_IID_VOLUME, &(soundData.mVolumeItf));
		if ( SL_RESULT_SUCCESS != result )
		{
			soundData.mVolumeItf = NULL;
			n_warning( "audio volume not supported\n" );
		}

		// get mute solo interface
		result = (*(soundData.mPlayerObj))->GetInterface(soundData.mPlayerObj, SL_IID_MUTESOLO, &(soundData.mMuteSoloItf));
		if ( SL_RESULT_SUCCESS != result )
		{
			soundData.mMuteSoloItf = NULL;
			n_warning( "audio mute not supported\n" );
		}

		// get pitch interface
		result = (*(soundData.mPlayerObj))->GetInterface(soundData.mPlayerObj, SL_IID_PITCH, &(soundData.mPitchItf));
		if ( SL_RESULT_SUCCESS != result )
		{
			soundData.mPitchItf = NULL;
			n_warning( "audio pitch not supported\n" );
		}

		// get play rate interfaace
		result = (*(soundData.mPlayerObj))->GetInterface(soundData.mPlayerObj, SL_IID_PLAYBACKRATE, &(soundData.mPlayRateItf));
		if ( SL_RESULT_SUCCESS != result )
		{
			soundData.mPlayRateItf = NULL;
			n_warning("audio play rate not supported");
		}

		soundData.mIsLoaded = true;

		// cache it
		mSoundAssetMap.insert( std::make_pair( hashCode, soundData ) );

		return;
	}

	bool SoundSystemOpenSL::SetListenerAttrs( int listener, const Math::vector& position, const Math::vector& velocity, const Math::vector& up, const Math::vector& forward )
	{
		return true;
	}

	bool SoundSystemOpenSL::ReleaseSingleBuffer( SoundBuffer *buffer )
	{
		return true;
	}

	bool SoundSystemOpenSL::ReleaseSingleSource( SoundSystemSource *source )
	{
		return true;
	}

	bool SoundSystemOpenSL::PlaySound( SoundBuffer* soundBuffer, SoundSystemSource **ssSource, bool paused )
	{
		return true;
	}

	bool SoundSystemOpenSL::createDSPByType( ALuint type, GPtr<SoundSystemDSP>& dsp )
	{
		dsp = SoundSystemDSPOpenSL::Create();
		dsp->init(type);

		return false;
	}

	bool SoundSystemOpenSL::CreateSoundBuffer( const char* nameOrData, SizeT dataSize, SoundBuffer **soundBuffer, GENESISOUND_MODE mode ,GENESIS_FILE_FORMAT fileFormat)
	{
		return true;
	}
}

#endif // __ANDROID__
#endif // __USE_AUDIO__ || __GENESIS_EDITOR__