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

#include <jni.h>

#include <unistd.h>
#include <android/log.h>

#include "simplegameapplication.h"
#include "input/inputwindowsource.h"
#include "input/android/androidinputsource.h"
#include "input/android/androidtouchevent.h"
#include "input/mobilekeyboardevent.h"

#include "rendersystem/gles/OpenGLES.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "addons/shadercompiler/ShadercompilerConfig.h"

#include "app/Jni/Jnihelper.h"
#include "addons/myguiengine/include/MyGUI_UString.h"
#include "app/guifeature/scriptgui.h"

using namespace Genesis;

static DemoPublishGameApplication* g_pApp = NULL;
static bool g_bPause = false;
static Util::String g_gamedir = "";
static Util::String g_scenename = "";
static Util::String g_pakagename = "";
static Util::String g_apklocation = "";


JNIEnv* gEnv = NULL;


enum StorageType
{
	Compressed = 0,
	PackageName,
	SDCard,
};

static int g_StorageType = SDCard;
static bool g_usePrecompilerShader = true;

void Init(jint width, jint height,jfloat dpiw,jfloat dpih,JNIEnv* env, jobject assetManager)
{
	g_pApp->SetCompanyName( "CYOU" );
	g_pApp->SetAppTitle( "Genesis Android" );

	//设置资源路径
	g_pApp->SetResourceBaseDir(g_gamedir);
	g_pApp->SetDebugScript(false);
	g_pApp->SetGui(true);
	g_pApp->SetGameResolution(width, height);
	g_pApp->SetJNIEnv(env);
	g_pApp->SetDPI(dpiw,dpih);
	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
	g_pApp->SetJNIAssetManager(mgr);

	if(g_StorageType==Compressed)
	{
		g_pApp->SetNeedReadAPK(true);
	}
	else
	{
		g_pApp->SetNeedReadAPK(false);
	}
	g_pApp->SetAPKPath(g_apklocation);
	g_pApp->SetPackageName(g_pakagename);
	GPtr<Input::InputSource> input = Input::InputWindowSource::Create();
	g_pApp->SetInput(input);
	ShaderProgramCompiler::ShaderCompilerConfig::usePreCompileShader = g_usePrecompilerShader;

	LOGI("Before Open Scene");
	g_pApp->Open();

	LOGI("After Core App Open");

	g_pApp->Start();

	//设置scene文件
	g_pApp->OpenScene(g_scenename);
	LOGI("After Open Scene");

	GLES::GLESImpl::Instance()->CheckError();
}

void CleanUp()
{
	if (g_pApp != NULL)
	{
		g_pApp->Exit();
		n_delete(g_pApp);
		g_pApp = NULL;
	}
}

void ExitGame()
{
	CleanUp();
	jclass c = gEnv->functions->FindClass(gEnv,"org/genesis/lib/ExitAct");
	jmethodID exitfoo = gEnv->functions->GetStaticMethodID(gEnv,c,"exit","()V");
	gEnv->functions->CallStaticVoidMethod(gEnv,c ,exitfoo);
}


void RenderFrame()
{
	GLES::GLESImpl::Instance()->CheckError();

	if (g_pApp)
	{
		if (g_pApp->IsOpen())
		{
			g_pApp->Run();
		}

		if (g_pApp->IsQuit())  
		{
			ExitGame();
		}
	}

	GLES::GLESImpl::Instance()->CheckError();
}



void ProcessInputMulTouch(JNIEnv * pEnv, Input::MobileTouchEvent& event, const jintArray& pIDs, const jfloatArray& pXs, const jfloatArray& pYs)
{
	jsize nPointers = pEnv->GetArrayLength(pIDs);
	jint* pPointers = pEnv->GetIntArrayElements(pIDs, NULL);

	jfloat* pPosX = pEnv->GetFloatArrayElements(pXs, NULL);
	jfloat* pPosY = pEnv->GetFloatArrayElements(pYs, NULL);


	if (pEnv->ExceptionCheck())
	{
		n_delete_array(pPointers);

		n_error("--------------------JNI ERROR: Touches Move--------------------");

		return;
	}

	for (IndexT i = 0; i < nPointers; ++i)
	{
		Math::float2 pos( pPosX[i], pPosY[i] );

		event.SetPointerPos(pPointers[i], pos);
	}

	const GPtr<Input::InputSource>& pInputSource = g_pApp->GetInputSource();

	if (pInputSource.isvalid())
	{
		pInputSource.downcast<AndroidInput::AndroidInputSource>()->OnAndroidProc(&event);
	}

	pEnv->ReleaseIntArrayElements(pIDs, pPointers, 0);
	pEnv->ReleaseFloatArrayElements(pXs, pPosX, 0);
	pEnv->ReleaseFloatArrayElements(pYs, pPosY, 0);
}

void ProcessInputTouch(JNIEnv * pEnv, Input::MobileTouchEvent& event, const jint& pID, const jfloat& pX, const jfloat& pY)
{
	if (pEnv->ExceptionCheck())
	{
		n_error("--------------------JNI ERROR: Touches Move--------------------");
		return;
	}

	Math::float2 pos( pX, pY );

	event.SetPointerPos(pID, pos);

	const GPtr<Input::InputSource>& pInputSource = g_pApp->GetInputSource();

	if (pInputSource.isvalid())
	{
		pInputSource.downcast<AndroidInput::AndroidInputSource>()->OnAndroidProc(&event);
	}

}

extern "C" {

	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeConfig(JNIEnv*  env, jobject thiz, jstring gamedir, jstring scenename,jboolean bShader);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeStorage(JNIEnv*  env, jobject thiz, jint type, jstring pakagename, jstring apklocation);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h,jfloat dpiw,jfloat dpih,jobject assetManager);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeRender(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeOnPause(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeOnResume(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeOnStop(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeTouchesBegin(JNIEnv * env, jobject obj, jint pIDs, jfloat pXs, jfloat pYs);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeTouchesMove(JNIEnv * env, jobject obj, jintArray pIDs, jfloatArray pXs, jfloatArray pYs);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeTouchesEnd(JNIEnv * env, jobject obj, jint pIDs, jfloat pXs, jfloat pYs);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeTouchesCancel(JNIEnv * env, jobject obj, jintArray pIDs, jfloatArray pXs, jfloatArray pYs);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeKeyDown(JNIEnv * env, jobject obj, jint pKeyCode);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeKeyUp(JNIEnv * env, jobject obj, jint pKeyCode);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeInsertText(JNIEnv* env, jobject obj, jstring text);
	JNIEXPORT void JNICALL Java_org_genesis_lib_GenesisRenderer_nativeDeleteBackward(JNIEnv* env, jobject thiz);
	JNIEXPORT jstring JNICALL Java_org_genesis_lib_GenesisRenderer_nativeGetContentText();
};	

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeConfig(JNIEnv*  env, jobject thiz, jstring gamedir, jstring scenename,jboolean bShader)
{
	const char* dirP = env->GetStringUTFChars(gamedir, NULL);
	g_gamedir = dirP;
	const char* sceneP = env->GetStringUTFChars(scenename, NULL);
	g_scenename = sceneP;
	env->ReleaseStringUTFChars(gamedir, dirP);
	env->ReleaseStringUTFChars(scenename, sceneP);
	g_usePrecompilerShader = bShader;
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeStorage(JNIEnv*  env, jobject thiz, jint type, jstring pakagename , jstring apklocation)
{
	const char* pakage = env->GetStringUTFChars(pakagename, NULL);
	g_pakagename = pakage;
	env->ReleaseStringUTFChars(pakagename, pakage);

	const char* loc = env->GetStringUTFChars(apklocation, NULL);
	g_apklocation = loc;
	env->ReleaseStringUTFChars(apklocation, loc);

	g_StorageType = type;
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeInit(JNIEnv* env, jobject thiz, jint w, jint h, jfloat dpiw,jfloat dpih , jobject assetManager)
{
	gEnv = env;
	if (g_pApp != NULL)
	{
		g_pApp->SetDeviceLost();
	} 
	else
	{
		Threading::ThreadRunTimeInfo::SetupMyThreadRunTime("GL Thread");

		g_pApp = n_new(DemoPublishGameApplication);		
		Init(w, h,dpiw,dpih, env, assetManager);
	}

}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeRender(JNIEnv * env, jobject obj)
{
	if (g_pApp != NULL && !g_bPause)
	{
		RenderFrame();
	}

}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeTouchesBegin(JNIEnv * env, jobject obj, jint pIDs, jfloat pXs, jfloat pYs)
{
	AndroidInput::AndroidTouchEvent event;
	event.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_MOTION);
	event.SetMotionType(Input::MobileTouchEvent::MOTION_EVENT_ACTION_DOWN);
	event.SetSourceType(AndroidInput::AndroidTouchEvent::INPUT_SOURCE_TOUCHSCREEN);

	ProcessInputTouch(env, event, pIDs, pXs, pYs);
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeTouchesMove(JNIEnv * pEnv, jobject obj, jintArray pIDs, jfloatArray pXs, jfloatArray pYs)
{
	AndroidInput::AndroidTouchEvent event;
	event.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_MOTION);
	event.SetMotionType(Input::MobileTouchEvent::MOTION_EVENT_ACTION_MOVE);
	event.SetSourceType(AndroidInput::AndroidTouchEvent::INPUT_SOURCE_TOUCHSCREEN);

	ProcessInputMulTouch(pEnv, event, pIDs, pXs, pYs);
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeTouchesEnd(JNIEnv * env, jobject obj, jint pIDs, jfloat pXs, jfloat pYs)
{
	AndroidInput::AndroidTouchEvent event;
	event.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_MOTION);
	event.SetMotionType(Input::MobileTouchEvent::MOTION_EVENT_ACTION_UP);
	event.SetSourceType(AndroidInput::AndroidTouchEvent::INPUT_SOURCE_TOUCHSCREEN);

	ProcessInputTouch(env, event, pIDs, pXs, pYs);
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeTouchesCancel(JNIEnv * env, jobject obj, jintArray pIDs, jfloatArray pXs, jfloatArray pYs)
{
	AndroidInput::AndroidTouchEvent event;
	event.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_MOTION);
	event.SetMotionType(Input::MobileTouchEvent::MOTION_EVENT_ACTION_CANCEL);
	event.SetSourceType(AndroidInput::AndroidTouchEvent::INPUT_SOURCE_TOUCHSCREEN);

	ProcessInputMulTouch(env, event, pIDs, pXs, pYs);
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeOnPause(JNIEnv * env, jobject obj)
{
	LOGI("-----------------------------nativeOnPause-----------------------------");

	g_bPause = true;
	g_pApp->OnStopped();
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeOnResume(JNIEnv * env, jobject obj)
{
	LOGI("-----------------------------nativeOnResume-----------------------------");
	if(g_bPause==true)
	{
		g_pApp->OnResumed();
		g_bPause = false;
	}
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeKeyDown(JNIEnv * env, jobject obj, jint pKeyCode)
{
	LOGI("-----------------------------nativeKeyDown-----------------------------");

	switch (pKeyCode)
	{
	case Input::InputKey::KEYCODE_BACK:
		g_pApp->OnExited();
		return;
		break;
	default:
		break;
	}

	n_warning("android key down");
	Input::MobileKeyboardEvent keyboardEvent;
	keyboardEvent.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_KEY);
	keyboardEvent.SetMotionType(Input::MobileKeyboardEvent::MOTION_EVENT_KEY_DOWN);

	Input::InputKey::Code keyCode;
	keyCode = Input::InputKey::TranslateAndoridToWin32((Input::InputKey::AndroidKeyCode)pKeyCode);

	keyboardEvent.SetKeycode(keyCode);

	const GPtr<Input::InputSource>& pInputSource = g_pApp->GetInputSource();
	if (pInputSource.isvalid())
	{
		pInputSource.downcast<AndroidInput::AndroidInputSource>()->OnAndroidProc(&keyboardEvent);
	}
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeKeyUp(JNIEnv * env, jobject obj, jint pKeyCode)
{
	LOGI("-----------------------------nativeKeyUp-----------------------------");

	n_warning("android key up");
	Input::MobileKeyboardEvent keyboardEvent;
	keyboardEvent.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_KEY);
	keyboardEvent.SetMotionType(Input::MobileKeyboardEvent::MOTION_EVENT_KEY_UP);

	Input::InputKey::Code keyCode;
	keyCode = Input::InputKey::TranslateAndoridToWin32((Input::InputKey::AndroidKeyCode)pKeyCode);

	keyboardEvent.SetKeycode(keyCode);

	const GPtr<Input::InputSource>& pInputSource = g_pApp->GetInputSource();
	if (pInputSource.isvalid())
	{
		pInputSource.downcast<AndroidInput::AndroidInputSource>()->OnAndroidProc(&keyboardEvent);
	}
}

JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeOnStop(JNIEnv * env, jobject obj)
{
	LOGI("-----------------------------nativeOnStop-----------------------------");
}
JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeInsertText(JNIEnv* env, jobject obj, jstring text)
{
	const char* pszText = env->GetStringUTFChars(text, NULL);
	n_warning("insert text:%s",pszText);
	
	//jni GetStringUTFChars get string is utf8
	MyGUI::UString guiString(pszText);
	std::wstring wstr = guiString.asWStr();
	
	MyGUI::UString uiStr(wstr);

	App::ScriptGui::SetFocusedEditboxCaption(uiStr);
	MyGUI::InputManager::getInstance().setKeyFocusWidget(nullptr);
	//// first run,and then modify
	//for ( IndexT i = 0; i < wstr.length(); i++ )
	//{
	//	Input::MobileKeyboardEvent keyboardEvent;
	//	keyboardEvent.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_KEY);
	//	keyboardEvent.SetMotionType(Input::MobileKeyboardEvent::MOTION_EVENT_CHAR);
	//	Input::Char characterCode = (Input::Char)wstr.at(i);
	//	keyboardEvent.SetChar(characterCode);

	//	const GPtr<Input::InputSource>& pInputSource = g_pApp->GetInputSource();
	//	if (pInputSource.isvalid())
	//	{
	//		pInputSource.downcast<AndroidInput::AndroidInputSource>()->OnAndroidProc(&keyboardEvent);
	//	}
	//}
	env->ReleaseStringUTFChars(text, pszText);
	

}
JNIEXPORT void Java_org_genesis_lib_GenesisRenderer_nativeDeleteBackward(JNIEnv* env, jobject thiz)
{
	
	n_warning("delete backward text");
	Input::MobileKeyboardEvent keyboardEvent;
	keyboardEvent.SetType(Input::MoibleInputEvent::INPUT_EVENT_TYPE_KEY);
	keyboardEvent.SetMotionType(Input::MobileKeyboardEvent::MOTION_EVENT_KEY_DOWN);
	keyboardEvent.SetKeycode(Input::InputKey::Back);

	const GPtr<Input::InputSource>& pInputSource = g_pApp->GetInputSource();
	if (pInputSource.isvalid())
	{
		pInputSource.downcast<AndroidInput::AndroidInputSource>()->OnAndroidProc(&keyboardEvent);

		//send keyrelease
		keyboardEvent.SetMotionType(Input::MobileKeyboardEvent::MOTION_EVENT_KEY_UP);
		pInputSource.downcast<AndroidInput::AndroidInputSource>()->OnAndroidProc(&keyboardEvent);

	}
}

JNIEXPORT jstring Java_org_genesis_lib_GenesisRenderer_nativeGetContentText()
{
	n_warning("get content text");
	JNIEnv * env = 0;
	if ( !App::JniHelper::GetEnv(&env) )
	{
		return 0;
	}
	const char * pszText = "jiu shi ai ni";
	MyGUI::UString ustr = "";
	if ( App::ScriptGui::s_pCurFocusEditBox )
	{
		MyGUI::EditBox* pEditBox = dynamic_cast<MyGUI::EditBox*>(App::ScriptGui::s_pCurFocusEditBox);
		
		if ( pEditBox )
		{
			ustr = pEditBox->getCaption();
		}		
	}
	const char*  strUtf8 = ustr.asUTF8_c_str();
	return env->NewStringUTF(strUtf8);
	
}