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
#ifndef __mobilekeyboardevent_H__
#define __mobilekeyboardevent_H__

#include "input/mobileinputevent.h"
#include "inputkey.h"
#include "inputchar.h"

namespace Input
{
class MobileKeyboardEvent : public Input::MoibleInputEvent
{
public:

	enum MotionType
	{
		MOTION_EVENT_KEY_DOWN = 0,
		MOTION_EVENT_KEY_UP,
		MOTION_EVENT_CHAR,
	};

	MobileKeyboardEvent();

	~MobileKeyboardEvent();

public:

	void SetMotionType(const MotionType type);
	const MotionType GetMotionType() const;
	void SetKeycode(InputKey::Code keyc);
	InputKey::Code GetKeycode();
	void SetChar(Char& keyc);
	Char GetChar();

	

protected:
	MotionType		m_MotionType;
	InputKey::Code	keyCode;
	Char			character;


};

inline MobileKeyboardEvent::MobileKeyboardEvent()
{
		
}

inline MobileKeyboardEvent::~MobileKeyboardEvent()
{
	
}

inline void MobileKeyboardEvent::SetMotionType(const MotionType type)
{
	m_MotionType = type;
}

inline const MobileKeyboardEvent::MotionType MobileKeyboardEvent::GetMotionType() const
{
	return m_MotionType;
}

inline void MobileKeyboardEvent::SetKeycode(InputKey::Code keyc)
{
	keyCode = keyc;
}
inline InputKey::Code MobileKeyboardEvent::GetKeycode()
{
	return keyCode;
}
inline void MobileKeyboardEvent::SetChar(Char& keyc)
{
	character = keyc;
}
inline Char MobileKeyboardEvent::GetChar()
{
	return character;
}

}

#endif