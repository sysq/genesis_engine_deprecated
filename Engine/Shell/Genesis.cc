//
//  Genesis.c
//  GenesisEngineLib
//
//  Created by 宋 琦 on 13-9-18.
//  Copyright (c) 2013年 webJet. All rights reserved.
//
#include "Genesis.h"
#include "stdneb.h"
#include "RenderSystem.h"
#include "util/array.h"
#include "util/string.h"
#include "math/float3.h"
#include "math/float2.h"
#include "math/matrix44.h"
#include "core/coreserver.h"
#include "gles/OpenGLES.h"
#include "gles/GPUProgramGLES.h"
#include "gles/PrimitiveGroupObjectGLES.h"
#include "gles/VertexBufferObjectGLES.h"
#include "gles/IndexBufferObjectGLES.h"
#include "io/stream.h"

#include "io/memorystream.h"
#include "io/textreader.h"
#include "io/textwriter.h"
#include "io/ioserver.h"
#include "io/uri.h"
#include "core/sysfunc.h"

#include "players/GenesisiOS/simplegameapplication.h"


#include "input/inputwindowsource.h"
#include "input/osx/osxinputsource.h"
#include "input/osx/osxtouchevent.h"
#include "input/mobilekeyboardevent.h"

#include "addons/shadercompiler/ShadercompilerConfig.h"
#include "addons/myguiengine/include/MyGUI_UString.h"

#include "app/guifeature/scriptgui.h"


using namespace RenderBase;
using namespace GLES;
using namespace Core;
using namespace IO;

namespace EngineShell
{

static iOSGame::iOSGameApplication* g_pApp = NULL;


void InitEngine( const int& w, const int& h, const char* path,const char* appWritableDic,const char* scene,bool bUsePrecompileShader )
{
    g_pApp = n_new(iOSGame::iOSGameApplication);
    
    g_pApp->SetCompanyName( "CYOU-INC.COM" );
	g_pApp->SetAppTitle( "Genesis iOS" );
    
	//…Ë÷√◊ ‘¥¬∑æ∂
	g_pApp->SetResourceBaseDir(path);
    g_pApp->SetAppDic(appWritableDic);
	g_pApp->SetDebugScript(false);
	g_pApp->SetGui(true);
	g_pApp->SetGameResolution(w, h);
    
	GPtr<Input::InputSource> input = Input::InputWindowSource::Create();
	g_pApp->SetInput(input);
	ShaderProgramCompiler::ShaderCompilerConfig::usePreCompileShader = bUsePrecompileShader;    
    
	g_pApp->Open();
    
	g_pApp->Start();

    g_pApp->OpenScene(scene);
}

void Update()
{
    if (g_pApp->IsOpen())
    {
        g_pApp->Run();
    }
}


void TouchPoint( const TouchDataVector& touchDatas, const InputAciton& action )
{
    OSXInput::OSXTouchEvent touchEvent;
    Input::MobileTouchEvent::MotionType Motion;
    int count = 0;
    switch(action)
    {
        case IA_DOWN: Motion = Input::MobileTouchEvent::MOTION_EVENT_ACTION_DOWN;break;
        case IA_MOVE: Motion = Input::MobileTouchEvent::MOTION_EVENT_ACTION_MOVE;break;
        case IA_UP: Motion = Input::MobileTouchEvent::MOTION_EVENT_ACTION_UP;break;
        case IA_CANCEL: Motion = Input::MobileTouchEvent::MOTION_EVENT_ACTION_CANCEL;break;
        default:Motion = (Input::MobileTouchEvent::MotionType)-1;break;
    }
	touchEvent.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_MOTION);
	touchEvent.SetMotionType(Motion);
	touchEvent.SetSourceType(OSXInput::OSXTouchEvent::INPUT_SOURCE_TOUCHSCREEN);
    for( TouchDataVector::const_iterator it = touchDatas.begin();
        it != touchDatas.end();
        it++ )
    {
        touchEvent.SetPointerPos( it->id, Math::float2( it->x, it->y ) );
    }
    const GPtr<Input::InputSource>& pInputSource = g_pApp->GetInputSource();
	if (pInputSource.isvalid())
	{
		pInputSource.downcast<OSXInput::OSXInputSource>()->OnOSXProc(&touchEvent);
	}
}

void OnResumed()
{
    g_pApp->OnResumed();
}
void OnStopped()
{
    g_pApp->OnStopped();
}
    
void UIInsertText(const char* wstr)
{
#if __OSX__
    MyGUI::UString uiStr(wstr);
    
    App::ScriptGui::SetFocusedEditboxCaption(uiStr);
    MyGUI::InputManager::getInstance().setKeyFocusWidget(nullptr);
#elif __ANDROID__
        std::wstring stdWstr = uiStr.asWStr();
       for ( IndexT i = 0; i < stdWstr.length(); i++ )
    	{
    		Input::MobileKeyboardEvent keyboardEvent;
    		keyboardEvent.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_KEY);
    		keyboardEvent.SetMotionType(Input::MobileKeyboardEvent::MOTION_EVENT_CHAR);
    		Input::Char characterCode = (Input::Char)stdWstr.at(i);
    		keyboardEvent.SetChar(characterCode);
    
    		const GPtr<Input::InputSource>& pInputSource = g_pApp->GetInputSource();
    		if (pInputSource.isvalid())
    		{
    			pInputSource.downcast<OSXInput::OSXInputSource>()->OnOSXProc(&keyboardEvent);
    		}
    	}
#endif

}
void UIDeleteBackward()
{
    Input::MobileKeyboardEvent keyboardEvent;
	keyboardEvent.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_KEY);
	keyboardEvent.SetMotionType(Input::MobileKeyboardEvent::MOTION_EVENT_KEY_DOWN);
	keyboardEvent.SetKeycode(Input::InputKey::Back);
    
	const GPtr<Input::InputSource>& pInputSource = g_pApp->GetInputSource();
	if (pInputSource.isvalid())
	{
		pInputSource.downcast<OSXInput::OSXInputSource>()->OnOSXProc(&keyboardEvent);
        
		//send keyrelease
		keyboardEvent.SetMotionType(Input::MobileKeyboardEvent::MOTION_EVENT_KEY_UP);
		pInputSource.downcast<OSXInput::OSXInputSource>()->OnOSXProc(&keyboardEvent);
	}
}
    
    
}

