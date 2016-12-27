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
#ifndef __ANDROID__

#include "resource/resource_stdneb.h"
#include "soundsystem/SoundInterface.h"
#include "messaging/asynhandlerthread.h"
#include "SoundSystemSourceOpenAL.h"

using namespace Messaging;
using namespace IO;

namespace Sound
{
	__ImplementClass(SoundInterface, 'SDIF', Interface::InterfaceBase);
	__ImplementImageSingleton(Sound::SoundInterface);

	__ImplementClass(Sound::SoundHandler, 'SDHD', Interface::InterfaceHandlerBase);

	__ImplementClass(Sound::SoundMsg, 'SDMG', Messaging::Message);
	__ImplementMsgId(SoundMsg);

	__ImplementClass(Sound::SoundDecodeMsg, 'SDCM', Messaging::Message);
	__ImplementMsgId(SoundDecodeMsg);

	SoundInterface::SoundInterface()
	{
		__ConstructImageSingleton;
	}
	//------------------------------------------------------------------------
	SoundInterface::~SoundInterface()
	{
		__DestructImageSingleton;		
	}
	//------------------------------------------------------------------------
	void SoundInterface::Open()
	{
		// setup the message handler thread object
		GPtr<AsynHandlerThread> handlerThread = AsynHandlerThread::Create();
		handlerThread->SetName("SoundInterface Thread");
		handlerThread->SetCoreId(System::Cpu::AudioThreadCore);    
		handlerThread->AttachHandler(SoundHandler::Create());
		this->SetHandlerThread(handlerThread.cast<Messaging::HandlerThreadBase>());

		Super::Open();
	}

	//------------------------------------------------------------------------
	SoundHandler::SoundHandler()
	{

	}
	//------------------------------------------------------------------------

	SoundHandler::~SoundHandler()
	{
		n_assert(!this->IsOpen());
	}
	//------------------------------------------------------------------------
	/// open the handler
	void 
		SoundHandler::Open()
	{
		Super::Open();
		n_assert( SoundInterface::Instance() );
	}
	//------------------------------------------------------------------------
	/// close the handler
	void 
		SoundHandler::Close()
	{
		Super::Close();
	}
	//------------------------------------------------------------------------
	/// handle a message, return true if handled
	bool 
		SoundHandler::HandleMessage(const GPtr<Messaging::Message>& msg)
	{
		n_assert(msg.isvalid());
		if (msg->CheckId(SoundDecodeMsg::Id))
		{
			this->OnDecode( msg.downcast<SoundDecodeMsg>() );
		}
		else
		{
			// unknown message
			return false;        
		}
		// fallthrough: message was handled
		return true;
	}
	//------------------------------------------------------------------------
	void 
		SoundHandler::OnDecode( const GPtr<SoundDecodeMsg>& msg )
	{
		n_assert(msg.isvalid());

		// 初始化消息为处理失败,已处理消息
		msg->SetResult(false);
		msg->SetHandled(false);

		// 开始解码
		GPtr<SoundSystemSource> s3 = msg->GetDecoder();

		if (!s3.isvalid())
        {
            msg->SetResult(false);
            msg->SetHandled(true);
			return;
        }

		GPtr<SoundSystemSourceOpenAL> s3Al = s3.downcast<SoundSystemSourceOpenAL>();

		s3Al->Decode( msg->GetFileFormat());

		msg->SetResult(true);
		msg->SetHandled(true);
	}


}


#endif	//	__ANDROID__
#endif	//	__USE_AUDIO__ || __GENESIS_EDITOR__ 