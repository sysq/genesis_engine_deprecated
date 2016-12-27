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
#include "scriptgui.h"
#include "guiserver.h"
#include "graphicsystem/GraphicSystem.h"
#include "guiutility.h"
#include "Jni/imejni.h"
#if __OSX__
#include "OCAndCPlusInterface.h"
#endif
namespace App
{
	MyGUI::Widget* ScriptGui::s_pCurFocusEditBox = NULL;
    void ScriptGui::SetFocusedEditboxCaption(const MyGUI::UString& data)
    {
        if (s_pCurFocusEditBox) {
            MyGUI::EditBox* pEditBox = dynamic_cast<MyGUI::EditBox*>(s_pCurFocusEditBox);
            pEditBox->setCaption(data);
        }
        
    }
	void ScriptGui::recursiveGetEditBoxWidget(MyGUI::VectorWidgetPtr& outVec,MyGUI::Widget* pWidget)
	{
		if ( !pWidget )
		{
			return;
		}
		//itself
		if ( pWidget->isType<MyGUI::EditBox>() )
		{
			outVec.push_back(pWidget);
		}
		//children
		size_t nSize = pWidget->getChildCount();
		for ( size_t i = 0; i < nSize; i++ )
		{
			MyGUI::Widget* pChildWidget = pWidget->getChildAt(i);
			recursiveGetEditBoxWidget(outVec,pChildWidget);
		}
	}
	void ScriptGui::_ShowKeyboard(bool bShow)
	{
#if __ANDROID__
		// call to java by the way jni
		IMEJni::ShowKeyboardJNI(bShow);
#elif __OSX__
		// call to object c by the way ThirdLibIosSenderListener interface
        if (bShow) {
            MyGUI::EditBox* pEditBox = dynamic_cast<MyGUI::EditBox*>(s_pCurFocusEditBox);
            MyGUI::UString ustr = pEditBox->getCaption();
            pEditBox->setCaption("");
            const char* cstr = ustr.asUTF8_c_str();
            OCAndCPlusInterface::showKeyboard(bShow,cstr);
        } else {
            OCAndCPlusInterface::showKeyboard(bShow,NULL);
        }
        

#else
		n_warning("not implement this platform!");
#endif
	}
	template<typename Delegate, typename Function>
	void _reg_event(MyGUI::Widget* widget, EventType::_type event_type, bool advise, Delegate& _delegate, Function& _function);
	void ScriptGui::_OnKeySetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new)
	{
		s_pCurFocusEditBox = _sender;
		//show keyboard
		//if ( _new && _new->isType<MyGUI::EditBox>() )
		//{ //old focus is editbox,don't need show keyboard
		//	return;
		//}
		_ShowKeyboard(true);
	}
	void ScriptGui::_OnKeyLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _old)
	{
		if ( _sender == s_pCurFocusEditBox )
		{
			s_pCurFocusEditBox = NULL;
		}
		//hide keyboard
		//if ( _old && _old->isType<MyGUI::EditBox>() )
		//{// new focus is editbox,don't need hide keyboard
		//	return;
		//}
		_ShowKeyboard(false);
	}
	void ScriptGui::_InternalRegisterEditBoxGetLostFocus(MyGUI::Widget*pWidget)
	{
		if ( !pWidget )
		{
			return;
		}
		_reg_event(pWidget, EventType::KeySetFocus, true, pWidget->eventKeySetFocus, _OnKeySetFocus);
		_reg_event(pWidget, EventType::KeyLostFocus, true, pWidget->eventKeyLostFocus, _OnKeyLostFocus);		

	}
	MyGUI::Widget* ScriptGui::loadLayout(MyGUI::Widget* parent, const std::string& file_name)
	{
		MyGUI::LayoutManager* lm = MyGUI::LayoutManager::getInstancePtr();
		if (!lm)
		{
			return nullptr;
		}

		MyGUI::VectorWidgetPtr insts = MyGUI::LayoutManager::getInstancePtr()->loadLayout(
			file_name,
			"",
			static_cast<MyGUI::Widget*>(parent));

		//interal register event
#if __ANDROID__ || __OSX__
		MyGUI::VectorWidgetPtr allEditBox;
		for ( MyGUI::VectorWidgetPtr::iterator it = insts.begin(); it != insts.end(); it++ )
		{
			recursiveGetEditBoxWidget(allEditBox,*it);
		}
		for ( MyGUI::VectorWidgetPtr::iterator it = allEditBox.begin(); it != allEditBox.end(); it++ )
		{
			_InternalRegisterEditBoxGetLostFocus(*it);
		}
#endif
		

		if (1 == insts.size())
		{
			return insts.at(0);
		}
		else if (1 < insts.size())
		{
			MyGUI::Widget* parent_new = MyGUI::Gui::getInstancePtr()->createWidgetT("Widget","Widget", 0, 0, 0, 0, MyGUI::Align::Default, "Static", (*insts.begin())->getName());
			MyGUI::VectorWidgetPtr::iterator begin = insts.begin();
			MyGUI::VectorWidgetPtr::iterator end = insts.begin();
			while (begin < end)
			{
				(*begin)->attachToWidget(parent_new, MyGUI::WidgetStyle::Popup);
			}
			return parent_new;
		}
		MyGUI::Widget* messing = MyGUI::Gui::getInstancePtr()->createWidgetT("Widget","Widget", 0, 0, 0, 0, MyGUI::Align::Default, "Static", "MissingLayout");
		return messing;
	}
	void ScriptGui::destroyWidget(MyGUI::Widget* widget)
	{
		MyGUI::Gui::getInstancePtr()->destroyWidget(widget);
	}
	MyGUI::Widget* ScriptGui::findWidget(MyGUI::Widget* parent, const std::string& widget_name)
	{
		return parent->findWidget(widget_name);
	}
	MyGUI::IntSize ScriptGui::getScreenSize()
	{
		return GUIServer::Instance()->GetScreenSize();
	}

	void ScriptGui::setResolution(const MyGUI::IntSize& size)
	{
		GUIServer::Instance()->SetResolution(size);
	}
	const MyGUI::IntSize& ScriptGui::getResolution()
	{
		return GUIServer::Instance()->GetResolution();
	}
	bool ScriptGui::autoResolutionWidth()
	{
		return GUIServer::Instance()->AutoResolutionWidth();
	}
	bool ScriptGui::autoResolutionHeight()
	{
		return GUIServer::Instance()->AutoResolutionHeight();
	}
}