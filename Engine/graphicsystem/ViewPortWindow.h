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
#ifndef __ViewPortWindow_H__
#define __ViewPortWindow_H__
#include "Camera/Camera.h"
#include "rendersystem/base/RenderDisplay.h"
#include "foundation/delegates/delegatetype.h"
#include "graphicsystem/RenderTargetSuite.h"

namespace RenderBase
{
	class RenderWindow;
}

namespace Graphic
{
	enum ViewportType
	{
		VPT_Unkown = -1,
		VPT_CustomBegin = 0,
		VPT_CustomEnd = 999999,
		VPT_MAIN,
	};
	typedef Delegates::delegates::CMultiDelegate1<ViewPortWindow*> EventHandle_ViewportChange;
	class ViewPortWindow : public Core::RefCounted
	{
		__DeclareClass(ViewPortWindow);

		//public:
		//enum ViewPortPerspec

	public:
		ViewPortWindow();
		virtual ~ViewPortWindow();

		//void SetDisplay();

		void ChangeSize(SizeT width, SizeT height, bool force = false);

		const RenderBase::DisplayMode& GetDisplayMode(void) const;

		void SetDeviceWindow(RenderBase::RenderWindow*);
		RenderBase::RenderWindow* GetRenderWindow(void) const;

		void Setup();

		void Destroy();

		void ChangeAntiAliasQuality(RenderBase::AntiAliasQuality::Code alias);

		RenderBase::AntiAliasQuality::Code GetAntiAliasQuality() const;

		void BeginRender();

		void ApplyWindow();

		void OnDeviceReset();

		ViewportType GetType() const;

		bool IsValid() const;

		void SetNeedUpdate(bool need);
		bool GetNeedUpdate() const;

		const GPtr<RenderToTexture>& GetBackBuffer() const; 


		// internal call

		RenderTargetSuite* _GetRTTSuite() const;

		bool _IsDirty() const;
		void _OnChangeSize();

		void _SetType(ViewportType type);
		void _SetDisplayMode(const RenderBase::DisplayMode& mode);

		EventHandle_ViewportChange eventViewportChange;
	private:

		void setupBackBuffer();

		GPtr<RenderToTexture>			m_backBuffer;
		GPtr<RenderTargetSuite>			mRTSuite;			
		ViewportType					mViewportType;
		RenderBase::RenderWindow*		mpWindow;
		RenderBase::DisplayMode			mDisplayMode;
		bool							mDirty;
		bool							mUpdate;
	};

	//--------------------------------------------------------------------------------
	inline void ViewPortWindow::_SetDisplayMode(const RenderBase::DisplayMode& mode)
	{
		mDisplayMode = mode;
	}
	//--------------------------------------------------------------------------------
	inline const RenderBase::DisplayMode& ViewPortWindow::GetDisplayMode(void) const
	{
		return mDisplayMode;
	}
	//--------------------------------------------------------------------------------
	inline RenderBase::RenderWindow* ViewPortWindow::GetRenderWindow(void) const
	{
		return mpWindow;
	}

	inline bool ViewPortWindow::_IsDirty() const
	{
		return mDirty;
	}

	inline ViewportType ViewPortWindow::GetType() const
	{
		return mViewportType;
	}

	inline void ViewPortWindow::_SetType(ViewportType type)
	{
		mViewportType = type;
	}

	inline void ViewPortWindow::SetNeedUpdate(bool need)
	{
		mUpdate = need;
	}

	inline bool ViewPortWindow::GetNeedUpdate() const
	{
		return mUpdate;
	}

	inline const GPtr<RenderToTexture>& ViewPortWindow::GetBackBuffer() const
	{
		return m_backBuffer;
	}

	inline bool ViewPortWindow::IsValid() const
	{
		return m_backBuffer.isvalid() && (NULL != mpWindow);
	}

	inline RenderTargetSuite* ViewPortWindow::_GetRTTSuite() const
	{
		return mRTSuite.get_unsafe();
	}

	inline RenderBase::AntiAliasQuality::Code 
		ViewPortWindow::GetAntiAliasQuality() const
	{
		 return mRTSuite->GetAntiAliasQuality();
	}
}
#endif // __ViewPortWindow_H__
