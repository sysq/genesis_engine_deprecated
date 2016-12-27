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
#include "scriptfeature/inc/script_utility.h"
#include "io/stream.h"
#include "io/textreader.h"
#include "util/string.h"
#include "app/basegamefeature/managers/sceneschedulemanager.h"

namespace App
{

	void ICallReg_ScriptRuntime_IO( void );

	static MonoArray* ICall_IO_ReadAllFile( MonoString* pMonoString )
	{
		Util::String str = Utility_MonoStringToCppString( pMonoString );
		MonoArray* ret = NULL;
		GPtr<IO::Stream> stream = IO::IoServer::Instance()->ReadFile( str.AsCharPtr() );
		if(!stream.isvalid())
		{
			return NULL;
		}
		stream->SetAccessMode(IO::Stream::ReadAccess);
		if (stream->Open()&&stream->GetSize() > 0)
		{
			IO::Stream::Size size = stream->GetSize();
			char* buf  = (char*)stream->Map();
			ret = Utility_CppByteArrToMonoByteArr(buf,size);
	
			stream->Unmap();
		}
	
		return ret;
	}

	static MonoArray* ICall_IO_ReadFile( MonoString* pMonoString ,unsigned int size)
	{
		Util::String str = Utility_MonoStringToCppString( pMonoString );
		MonoArray* ret = NULL;

		GPtr<IO::Stream> stream = IO::IoServer::Instance()->ReadFile( str.AsCharPtr() );
		if(!stream.isvalid())
		{
			return NULL;
		}
		stream->SetAccessMode(IO::Stream::ReadAccess);
		if (stream->Open()&&stream->GetSize() > 0)
		{
			
			if ( size > (unsigned int)stream->GetSize())
			{
				size = stream->GetSize();
			}
	
			char* buf  = (char*)stream->Map();
			ret = Utility_CppByteArrToMonoByteArr(buf,size);
			stream->Unmap();
			
		}
		return ret;
	}
	
	void ICallReg_ScriptRuntime_IO( void )
	{
		static const InternalCallDetail s_cScriptBindInternalCallDetail[] = {
			{ "ScriptRuntime.IO::ICall_IO_ReadAllFile",					 (void*)&ICall_IO_ReadAllFile },
			{ "ScriptRuntime.IO::ICall_IO_ReadFile",					 (void*)&ICall_IO_ReadFile },
		};

		int size = sizeof(s_cScriptBindInternalCallDetail)/sizeof(InternalCallDetail);
		for( int ii=0; ii<size; ii++ )
		{
			mono_add_internal_call( s_cScriptBindInternalCallDetail[ii].__pScriptFunName, 
				s_cScriptBindInternalCallDetail[ii].__pCppFunPtr );
		}
	}

}