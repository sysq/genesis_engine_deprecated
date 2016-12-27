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
#if __ANDROID__
#include "imejni.h"
#include <string.h>
#include <jni.h>

namespace App
{

	static char* s_classname= "org/genesis/lib/GenesisGLSurfaceView";

    void ShowKeyboardJNI(int bOpen) 
	{
        if (bOpen) 
		{
			OpenKeyboardJNI();
        } 
		else
		{
            CloseKeyboardJNI();
        }
    }

    void OpenKeyboardJNI() 
	{
        JniMethodInfo t;

        if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxGLSurfaceView", "openIMEKeyboard", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
		jclass		jcls = static_cast<jclass>(cls);
		jmethodID   jmth = static_cast<jmethodID>(methodID);
		jvalue* jargs    =  Utility_CreateCppArrFromMonoArr<jvalue>(args);
		JNIEnv* jevn = DemoPublishGameApplication::Instance()->GetJNIEvn();

		long ret = jevn->functions->CallStaticLongMethodA(jevn,jcls,jmth,jargs);
    }

    void CloseKeyboardJNI()
	{
        JniMethodInfo t;

        if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxGLSurfaceView", "closeIMEKeyboard", "()V")) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}
#endif