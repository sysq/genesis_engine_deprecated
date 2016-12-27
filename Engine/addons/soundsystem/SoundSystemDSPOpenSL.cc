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
#include "SoundSystemDSPOpenSL.h"
#include "soundsystem/Framework.h"
#include "soundsystem/SoundSystemSourceOpenAL.h"

namespace Sound
{
	__ImplementClass(Sound::SoundSystemDSPOpenSL, 'SSSL', Sound::SoundSystemDSP);

	//------------------------------------------------------------------------------
	SoundSystemDSPOpenSL::SoundSystemDSPOpenSL()
		: m_uiFilter(NULL)
		, m_uiEffectSlot(NULL)
		, m_type(NULL)
		, m_sendIndex(0)
	{}
	//------------------------------------------------------------------------------
	SoundSystemDSPOpenSL::~SoundSystemDSPOpenSL()
	{
		uninit();
	}
	//------------------------------------------------------------------------------
	bool SoundSystemDSPOpenSL::remove()
	{
		return false;
	}
	//------------------------------------------------------------------------------
	bool SoundSystemDSPOpenSL::setParameter( int index, float value , bool integer)
	{
		n_printf("setParameter failed, Filter is invalid on Android\n");
		return false;
	}
	//------------------------------------------------------------------------------
	bool SoundSystemDSPOpenSL::setParameters( int index, float* pan)
	{
		n_printf("setParameter failed, Filter is invalid on Android\n");
		return false;
	}

	//------------------------------------------------------------------------------
	bool SoundSystemDSPOpenSL::init(ALuint type)
	{
		n_printf("init failed, Filter is invalid on Android\n");
		return false;
	}

	//------------------------------------------------------------------------------
	bool SoundSystemDSPOpenSL::uninit()
	{
		n_printf("uninit failed, Filter is invalid on Android\n");
		return false;
	}

	bool SoundSystemDSPOpenSL::apply( SoundSystemSource* sss, int sendIndex)
	{
		n_printf("apply failed, Filter is invalid on Android\n");
		return false;
	}

	bool SoundSystemDSPOpenSL::unapply( SoundSystemSource* sss )
	{
		n_printf("unapply failed, Filter is invalid on Android\n");
		return false;
	}
}
#endif // __USE_AUDIO__ || __GENESIS_EDITOR__