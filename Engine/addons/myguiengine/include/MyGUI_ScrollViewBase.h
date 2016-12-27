/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_SCROLL_VIEW_BASE_H__
#define __MYGUI_SCROLL_VIEW_BASE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class MYGUI_EXPORT ScrollViewBase
	{
	protected:
		ScrollViewBase();
		virtual ~ScrollViewBase();

		void updateScrollSize();
		void updateScrollPosition();

		// размер данных
		virtual IntSize getContentSize();

		// смещение данных
		virtual IntPoint getContentPosition();
		virtual void setContentPosition(const IntPoint& _value);

		// размер окна, через которые видно данные
		virtual IntSize getViewSize();

		// размер на который прокручиваются данные при щелчке по скролу
		virtual size_t getHScrollPage();
		virtual size_t getVScrollPage();

		virtual Align getContentAlign();

		virtual void eraseContent();
	protected:
		void bindDrag();// expand by genesis-3d
		bool dragClent(int _left, int _top, MouseButton _id);// expand by genesis-3d
		bool isDraging() const;// expand by genesis-3d
		void notifyClentPress(Widget* _sender, int _left, int _top, MouseButton _id);// expand by genesis-3d
		void notifyClentRelease(Widget* _sender, int _left, int _top, MouseButton _id);// expand by genesis-3d
		void notifyClentDrag(Widget* _sender, int _left, int _top, MouseButton _id);// expand by genesis-3d
		void notifyClentLostFocus(Widget* _sender, Widget* _newFocus);// expand by genesis-3d
		virtual void onClientDragBegin(Widget* _sender, int _left, int _top, MouseButton _id);// expand by genesis-3d
		virtual void onClientDragEnd(Widget* _sender);// expand by genesis-3d
		virtual void onClientDrag(Widget* _sender, int _left, int _top, MouseButton _id);// expand by genesis-3d

		ScrollBar* mVScroll;
		ScrollBar* mHScroll;
		Widget* mClient;

		bool mVisibleHScroll;
		bool mVisibleVScroll;

		size_t mVRange;
		size_t mHRange;

		// expand by genesis-3d
		bool mDraging;
		//----------------------------
		// изменяется ли содержимое при ресайзе
		bool mChangeContentByResize;
	};

} // namespace MyGUI

#endif // __MYGUI_SCROLL_VIEW_BASE_H__
