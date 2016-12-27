#include "RegisterMonoModules.h"


#include <string>

extern "C"
{
	typedef void* gpointer;
	typedef int gboolean;
	void				mono_dl_register_symbol (const char* name, void *addr);
    extern int 			mono_ficall_flag;
	void				mono_aot_register_module(gpointer *aot_info);
	extern gboolean		mono_aot_only;
	extern gpointer*	mono_aot_module_UserDefCSharp_info; // UserDefCSharp.dll
	extern gpointer*	mono_aot_module_Mono_Security_info; // Mono.Security.dll
	extern gpointer*	mono_aot_module_System_info; // System.dll
	extern gpointer*	mono_aot_module_ScriptFrameworkLibrary_info; // ScriptFrameworkLibrary
	extern gpointer*	mono_aot_module_mscorlib_info; // mscorlib.dll
   	extern gpointer*	mono_aot_module_ScriptGUILibrary_info; // ScriptGUILibrary.dll
   	extern gpointer*	mono_aot_module_ScriptRuntimeLibrary_info; // mscorlib.dll
   	extern gpointer*	mono_aot_module_System_Configuration_info; // mscorlib.dll
   	extern gpointer*	mono_aot_module_System_Security_info; // mscorlib.dll
   	extern gpointer*	mono_aot_module_System_Xml_info; // mscorlib.dll
}


void RegisterMonoModules()
{
	mono_aot_only = true;
    //mono_ficall_flag = true;

}
