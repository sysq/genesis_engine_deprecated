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
#include "stdneb.h"
#include "jnihelper.h"
#include <string.h>
#include "players/GenesisA/simplegameapplication.h"

namespace App
{
    //////////////////////////////////////////////////////////////////////////
    // java vm helper function
    //////////////////////////////////////////////////////////////////////////

	static bool getEnv(JNIEnv **env)
	{
		bool bRet = true;
		*env = Genesis::DemoPublishGameApplication::Instance()->GetJNIEvn();       
		return bRet;
	}
    bool JniHelper::GetEnv(JNIEnv **env)
    {
        return getEnv(env);
    }

    static jclass getClassID_(const char *className, JNIEnv *env)
    {
        JNIEnv *pEnv = env;
        jclass ret = 0;

        do 
        {
            if (! pEnv)
            {
                if (! getEnv(&pEnv))
                {
                    break;
                }
            }
            
            ret = pEnv->FindClass(className);
            if (! ret)
            {
                 n_warning("Failed to find class of %s", className);
                break;
            }
        } while (0);

        return ret;
    }

    static bool getStaticMethodInfo_(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
    {
        jmethodID methodID = 0;
        JNIEnv *pEnv = 0;
        bool bRet = false;

        do 
        {
            if (! getEnv(&pEnv))
            {
                break;
            }

            jclass classID = getClassID_(className, pEnv);

            methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
            if (! methodID)
            {
                n_warning("Failed to find static method id of %s", methodName);
                break;
            }

            methodinfo.classID = classID;
            methodinfo.env = pEnv;
            methodinfo.methodID = methodID;

            bRet = true;
        } while (0);

        return bRet;
    }

    static bool getMethodInfo_(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
    {
        jmethodID methodID = 0;
        JNIEnv *pEnv = 0;
        bool bRet = false;

        do 
        {
            if (! getEnv(&pEnv))
            {
                break;
            }

            jclass classID = getClassID_(className, pEnv);

            methodID = pEnv->GetMethodID(classID, methodName, paramCode);
            if (! methodID)
            {
                n_warning("Failed to find method id of %s", methodName);
                break;
            }

            methodinfo.classID = classID;
            methodinfo.env = pEnv;
            methodinfo.methodID = methodID;

            bRet = true;
        } while (0);

        return bRet;
    }

    static std::string jstring2string_(jstring jstr)
    {
        if (jstr == NULL)
        {
            return "";
        }
        
        JNIEnv *env = 0;

        if (! getEnv(&env))
        {
            return 0;
        }

        const char* chars = env->GetStringUTFChars(jstr, NULL);
        std::string ret(chars);
        env->ReleaseStringUTFChars(jstr, chars);

        return ret;
    }
//////////////////////////////////////////////////////////////



jclass JniHelper::GetClassID(const char *className, JNIEnv *env)
{
    return getClassID_(className, env);
}

bool JniHelper::GetStaticMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
{
    return getStaticMethodInfo_(methodinfo, className, methodName, paramCode);
}

bool JniHelper::GetMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
{
    return getMethodInfo_(methodinfo, className, methodName, paramCode);
}

std::string JniHelper::jstring2string(jstring str)
{
    return jstring2string_(str);
}
}
#endif
