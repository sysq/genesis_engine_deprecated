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
#ifndef __guiserver_H__
#define __guiserver_H__

#include "addons/myguiengine/include/MyGUI_Gui.h"
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/string.h"
#include "input/base/inputkeyboardbase.h"
#include "input/base/inputmousebase.h"
#include "guifeature/guiroot.h"
#include "guifeature/guievent.h"
#include "foundation/delegates/delegatetype.h"


#define GUI_ROOT_CONFIG "gui_core.xml"
#define GUI_DEFAULT_LOCATION "asset:UIMedia/"
namespace MyGUI
{
	class GenesisPlatform;
}
namespace Graphic
{
	class ViewPortWindow;
	class MaterialInstance;
}

namespace App
{
	class GuiScene;

	typedef Delegates::delegates::CMultiDelegate1<void*> EventHandle_BeforeDrawVoid;

	class GUIServer : public Core::RefCounted
	{
		__DeclareClass(GUIServer);
		__DeclareImageSingleton(GUIServer);
	public:
		virtual ~GUIServer();
		void SetLogDir(const Util::String& dir);
		void AddMediaLocaltion(const Util::String& path);
		void SetCoreFile(const Util::String& core);
		void Open();
		bool IsOpen();
		void Close();
		void OnFrame();
		void OnWindowResized();
		void OnDeviceReseted();

		void SetTargetWindow(Graphic::ViewPortWindow* vpw);
		Graphic::ViewPortWindow* GetTargetWindow() const;

		MyGUI::Gui* GetGui() const;	
	MyGUI::GenesisPlatform* GetPlatform() const;
		GuiEvent& GetGuiEvent();

		MyGUI::IntSize GetScreenSize() const;

		void InitGuiRootScript();
		void ExitGuiScript();
		void ForceDestroyGuiScript();
		bool StartTick(bool start);

		bool GetVisible();
		void SetVisible(bool visible);

		void SetResolution(const MyGUI::IntSize& size);//0 means auto.
		const MyGUI::IntSize& GetResolution();
		bool AutoResolutionWidth();
		bool AutoResolutionHeight();

		Graphic::MaterialInstance* GetGuiMaterial() const;

		EventHandle_BeforeDrawVoid eventBeforeDrawUI;

		static MyGUI::KeyCode KeyCodeWJtoMyGUI(Input::InputKey::Code key);
		static MyGUI::MouseButton MouseButtonToMyGUI(Input::InputMouseButton::Code btn);
		void _renderUI();
	private:

		void _initMyGUI();
		void _initScript();
		void _beforeDraw();

		Graphic::ViewPortWindow* m_targetWindow;
		MyGUI::Gui* m_gui;
		MyGUI::GenesisPlatform* m_platform;
		GuiScene* m_guiScene;
		bool  m_visible;

		Util::String m_coreFile;			
		Util::String m_logDir;			
		GuiRoot	m_guiRoot;					//reference of a c# object.
		GuiEvent m_guiEvent;				//used for event between c++ and c#.

		bool m_opened;
		GUIServer();

	};

	inline Graphic::ViewPortWindow* GUIServer::GetTargetWindow() const
	{
		return m_targetWindow;
	}

	inline MyGUI::Gui* GUIServer::GetGui() const
	{
		return m_gui;
	}

	inline MyGUI::GenesisPlatform* GUIServer::GetPlatform() const
	{
		return m_platform;
	}
	inline GuiEvent& GUIServer::GetGuiEvent()
	{
		return m_guiEvent;
	}
	inline bool GUIServer::GetVisible()
	{
		return m_visible;
	}
	inline void GUIServer::SetVisible(bool visible)
	{
		m_visible = visible;
	}
}

#endif //__guiserver_H__