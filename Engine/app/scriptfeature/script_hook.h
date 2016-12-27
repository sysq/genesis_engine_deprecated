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
#ifndef __scripthook_H__
#define __scripthook_H__


#ifdef __GENESIS_EDITOR__
#	define __SCRIPT_HOOK__ 1
#else
#	define __SCRIPT_HOOK__ 0
#endif

// declaration
#if __SCRIPT_HOOK__
namespace App
{
	class Actor;
	class Component;
	class Scene;
}

#define __HookClass(name) \
	class name##_Hook;\
	name##_Hook* GetHook_##name();\
	void SetHook_##name(name##_Hook*);\
	class name##_Hook : public Hook\
	{\
	public:\


#define __HookClassEnd }
#define __HookFunction(func, params) virtual void func##params = 0
#define __HookFunctionOverwrite(func, params) virtual void func##params

#define __ImplementHookClass(name)\
	static name##_Hook* g_Hook_##name=NULL;\
	void SetHook_##name(name##_Hook* hook)\
	{\
		g_Hook_##name = hook;\
	}\
	name##_Hook* GetHook_##name()\
	{\
		return g_Hook_##name;\
	}\

namespace ScriptHook
{
	class Hook
	{
	public:
		virtual ~Hook();
	};


	__HookClass(Actor)
		__HookFunction(SetParent, (App::Actor* self, App::Actor* parent));
		__HookFunction(AddChild, (App::Actor* self, App::Actor* child));
		__HookFunction(RemoveChild, (App::Actor* self, int child));

		__HookFunction(PropertyChanged, (App::Actor* self)); // common hook.
		__HookFunction(ComponentChanged, (App::Component* self));// component hook.
	__HookClassEnd;

	__HookClass(Scene)
		__HookFunction(RemoveActor, (App::Scene* scene, App::Actor* pActor));
		__HookFunction(AddActor, (App::Scene* scene, App::Actor* child));
	__HookClassEnd;

}
#endif


// base api for hook
#if __SCRIPT_HOOK__
#	define SCRIPT_API_HOOK_MESSEGE(name, func, params) \
	ScriptHook::##name##_Hook* p##name = ScriptHook::GetHook_##name();\
	if(NULL != p##name)\
	{\
		p##name->##func##params;\
	}\

#	define SET_SCRIPT_HOOK(name, hook) ScriptHook::SetHook_##name(hook)
#	define RESET_SCRIPT_HOOK(name) SET_SCRIPT_HOOK(name, NULL)

#else
#	define SCRIPT_API_HOOK_MESSEGE(name, func, params)
#	define SET_SCRIPT_HOOK(name, hook)
#	define RESET_SCRIPT_HOOK(name)

#endif

// advanced api for hook
#if __SCRIPT_HOOK__
#	define __Actor_Property_Changed(actor) SCRIPT_API_HOOK_MESSEGE(Actor, PropertyChanged,(actor))
#	define __Component_Property_Changed(com) SCRIPT_API_HOOK_MESSEGE(Actor, ComponentChanged,(com))
#else
#	define __Actor_Property_Changed(actor)
#	define __Component_Property_Changed(com)
#endif

#endif //__scripthook_H__