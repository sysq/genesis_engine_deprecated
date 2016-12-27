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
#ifndef __inputkey_H__
#define __inputkey_H__


#include "core/types.h"
#include "util/string.h"
#include "util/dictionary.h"

//------------------------------------------------------------------------------
namespace Input
{
class InputKey
{
public:
    /// key codes
    enum Code
    {
        Back,
        Tab,
        Clear,
        Return,
        Shift,
        Control,
        Menu,
        Pause,
        Capital,
        Escape,
        Convert,
        NonConvert,
        Accept,
        ModeChange,
        Space,
        Prior,
        Next,
        End,
        Home,
        Left,
        Right,
        Up,
        Down,
        Select,
        Print,
        Execute,
        Snapshot,
        Insert,
        Delete,
        Help,
        LeftWindows,
        RightWindows,
        Apps,
        Sleep,
        NumPad0,
        NumPad1,
        NumPad2,
        NumPad3,
        NumPad4,
        NumPad5,
        NumPad6,
        NumPad7,
        NumPad8,
        NumPad9,
        Multiply,
        Add,
        Separator,	//没有这个按键
        Subtract,
        Decimal,
        Divide,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        NumLock,
        Scroll,

        Semicolon,		// ';:'
        Slash,			// '/?'
        Tilde,			// '`~'
        LeftBracket,	// '[{'
        RightBracket,	// ']}'
        BackSlash,		// '\|'
        Quote,			// ''"'
        Comma,			// ' ,<'
        Underbar,		// '-_'
        Period,			// '.>'
        Equality,		// '=+'

        LeftShift,
        RightShift,
        LeftControl,
        RightControl,
        LeftMenu,
        RightMenu,
        BrowserBack,
        BrowserForward,
        BrowserRefresh,
        BrowserStop,
        BrowserSearch,
        BrowserFavorites,
        BrowserHome,
        VolumeMute,
        VolumeDown,
        VolumeUp,
        MediaNextTrack,
        MediaPrevTrack,
        MediaStop,
        MediaPlayPause,
        LaunchMail,
        LaunchMediaSelect,
        LaunchApp1,
        LaunchApp2,
        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
		
		System, // Alt Key

        NumKeyCodes,
        InvalidKey,
    }; 

#ifdef __ANDROID__
	///Android key codes
	enum AndroidKeyCode
	{
		KEYCODE_UNKNOWN = 0,
		KEYCODE_SOFT_LEFT       = 1,
		KEYCODE_SOFT_RIGHT      = 2,
		KEYCODE_HOME            = 3,
		KEYCODE_BACK            = 4,
		KEYCODE_CALL            = 5,
		KEYCODE_ENDCALL         = 6,
		KEYCODE_0               = 7,
		KEYCODE_1               = 8,
		KEYCODE_2               = 9,
		KEYCODE_3               = 10,
		KEYCODE_4               = 11,
		KEYCODE_5               = 12,
		KEYCODE_6               = 13,
		KEYCODE_7               = 14,
		KEYCODE_8               = 15,
		KEYCODE_9               = 16,
		KEYCODE_STAR            = 17,
		KEYCODE_POUND           = 18,
		KEYCODE_DPAD_UP         = 19,
		KEYCODE_DPAD_DOWN       = 20,
		KEYCODE_DPAD_LEFT       = 21,
		KEYCODE_DPAD_RIGHT      = 22,
		KEYCODE_DPAD_CENTER     = 23,
		KEYCODE_VOLUME_UP       = 24,
		KEYCODE_VOLUME_DOWN     = 25,
		KEYCODE_POWER           = 26,
		KEYCODE_CAMERA          = 27,
		KEYCODE_CLEAR           = 28,

		KEYCODE_A               = 29,
		KEYCODE_B               = 30,
		KEYCODE_C               = 31,
		KEYCODE_D               = 32,
		KEYCODE_E               = 33,
		KEYCODE_F               = 34,
		KEYCODE_G               = 35,
		KEYCODE_H               = 36,
		KEYCODE_I               = 37,
		KEYCODE_J               = 38,
		KEYCODE_K               = 39,
		KEYCODE_L               = 40,
		KEYCODE_M               = 41,
		KEYCODE_N               = 42,
		KEYCODE_O               = 43,
		KEYCODE_P               = 44,
		KEYCODE_Q               = 45,
		KEYCODE_R               = 46,
		KEYCODE_S               = 47,
		KEYCODE_T               = 48,
		KEYCODE_U               = 49,
		KEYCODE_V               = 50,
		KEYCODE_W               = 51,
		KEYCODE_X               = 52,
		KEYCODE_Y               = 53,
		KEYCODE_Z               = 54,

		KEYCODE_COMMA           = 55,
		KEYCODE_PERIOD          = 56,
		KEYCODE_ALT_LEFT        = 57,
		KEYCODE_ALT_RIGHT       = 58,
		KEYCODE_SHIFT_LEFT      = 59,
		KEYCODE_SHIFT_RIGHT     = 60,
		KEYCODE_TAB             = 61,
		KEYCODE_SPACE           = 62,
		KEYCODE_SYM             = 63,
		KEYCODE_EXPLORER        = 64,
		KEYCODE_ENVELOPE        = 65,
		KEYCODE_ENTER           = 66,
		KEYCODE_DEL             = 67,
		KEYCODE_GRAVE           = 68,
		KEYCODE_MINUS           = 69,
		KEYCODE_EQUALS          = 70,
		KEYCODE_LEFT_BRACKET    = 71,
		KEYCODE_RIGHT_BRACKET   = 72,
		KEYCODE_BACKSLASH       = 73,
		KEYCODE_SEMICOLON       = 74,
		KEYCODE_APOSTROPHE      = 75,
		KEYCODE_SLASH           = 76,
		KEYCODE_AT              = 77,
		KEYCODE_NUM             = 78,
		KEYCODE_HEADSETHOOK     = 79,
		KEYCODE_FOCUS           = 80,   
		KEYCODE_PLUS            = 81,
		KEYCODE_MENU            = 82,
		KEYCODE_NOTIFICATION    = 83,
		KEYCODE_SEARCH          = 84,
		KEYCODE_MEDIA_PLAY_PAUSE= 85,
		KEYCODE_MEDIA_STOP      = 86,
		KEYCODE_MEDIA_NEXT      = 87,
		KEYCODE_MEDIA_PREVIOUS  = 88,
		KEYCODE_MEDIA_REWIND    = 89,
		KEYCODE_MEDIA_FAST_FORWARD = 90,
		KEYCODE_MUTE             = 91,
		KEYCODE_PAGE_UP         =  92,
		KEYCODE_PAGE_DOWN       = 93,
		KEYCODE_PICTSYMBOLS     = 94, 
		KEYCODE_SWITCH_CHARSET  = 95,  

		KEYCODE_BUTTON_A        = 96,
		KEYCODE_BUTTON_B        = 97,
		KEYCODE_BUTTON_C        = 98,
		KEYCODE_BUTTON_X        = 99,
		KEYCODE_BUTTON_Y        = 100,
		KEYCODE_BUTTON_Z        = 101,
		KEYCODE_BUTTON_L1       = 102,
		KEYCODE_BUTTON_R1       = 103,
		KEYCODE_BUTTON_L2       = 104,
		KEYCODE_BUTTON_R2       = 105,
		KEYCODE_BUTTON_THUMBL   = 106,
		KEYCODE_BUTTON_THUMBR   = 107,
		KEYCODE_BUTTON_START    = 108,
		KEYCODE_BUTTON_SELECT   = 109,
		KEYCODE_BUTTON_MODE     = 110,

		KEYCODE_ESCAPE          = 111,
		KEYCODE_FORWARD_DEL     = 112,
		KEYCODE_CTRL_LEFT       = 113,
		KEYCODE_CTRL_RIGHT      = 114,
		KEYCODE_CAPS_LOCK       = 115,
		KEYCODE_SCROLL_LOCK     = 116,
		KEYCODE_META_LEFT       = 117,
		KEYCODE_META_RIGHT      = 118,
		KEYCODE_FUNCTION        = 119,
		KEYCODE_SYSRQ           = 120,
		KEYCODE_BREAK           = 121,
		KEYCODE_MOVE_HOME       = 122,
		KEYCODE_MOVE_END        = 123,
		KEYCODE_INSERT          = 124,
		KEYCODE_FORWARD         = 125,
		KEYCODE_MEDIA_PLAY      = 126,
		KEYCODE_MEDIA_PAUSE     = 127,
		KEYCODE_MEDIA_CLOSE     = 128,
		KEYCODE_MEDIA_EJECT     = 129,
		KEYCODE_MEDIA_RECORD    = 130,

		KEYCODE_F1              = 131,
		KEYCODE_F2              = 132,
		KEYCODE_F3              = 133,
		KEYCODE_F4              = 134,
		KEYCODE_F5              = 135,
		KEYCODE_F6              = 136,
		KEYCODE_F7              = 137,
		KEYCODE_F8              = 138,
		KEYCODE_F9              = 139,
		KEYCODE_F10             = 140,
		KEYCODE_F11             = 141,
		KEYCODE_F12             = 142,

		KEYCODE_NUM_LOCK        = 143,
		KEYCODE_NUMPAD_0        = 144,
		KEYCODE_NUMPAD_1        = 145,
		KEYCODE_NUMPAD_2        = 146,
		KEYCODE_NUMPAD_3        = 147,
		KEYCODE_NUMPAD_4        = 148,
		KEYCODE_NUMPAD_5        = 149,
		KEYCODE_NUMPAD_6        = 150,
		KEYCODE_NUMPAD_7        = 151,
		KEYCODE_NUMPAD_8        = 152,
		KEYCODE_NUMPAD_9        = 153,

		KEYCODE_NUMPAD_DIVIDE   = 154,
		KEYCODE_NUMPAD_MULTIPLY = 155,
		KEYCODE_NUMPAD_SUBTRACT = 156,
		KEYCODE_NUMPAD_ADD      = 157,
		KEYCODE_NUMPAD_DOT      = 158,
		KEYCODE_NUMPAD_COMMA    = 159,
		KEYCODE_NUMPAD_ENTER    = 160,
		KEYCODE_NUMPAD_EQUALS   = 161,
		KEYCODE_NUMPAD_LEFT_PAREN = 162,
		KEYCODE_NUMPAD_RIGHT_PAREN = 163,
		KEYCODE_VOLUME_MUTE     = 164,
		KEYCODE_INFO            = 165,
		KEYCODE_CHANNEL_UP      = 166,
		KEYCODE_CHANNEL_DOWN    = 167,
		KEYCODE_ZOOM_IN         = 168,
		KEYCODE_ZOOM_OUT        = 169,
		KEYCODE_TV              = 170,
		KEYCODE_WINDOW          = 171,
		KEYCODE_GUIDE           = 172,
		KEYCODE_DVR             = 173,
		KEYCODE_BOOKMARK        = 174,
		KEYCODE_CAPTIONS        = 175,
		KEYCODE_SETTINGS        = 176,
		KEYCODE_TV_POWER        = 177,
		KEYCODE_TV_INPUT        = 178,
		KEYCODE_STB_POWER       = 179,
		KEYCODE_STB_INPUT       = 180,
		KEYCODE_AVR_POWER       = 181,
		KEYCODE_AVR_INPUT       = 182,
		KEYCODE_PROG_RED        = 183,
		KEYCODE_PROG_GREEN      = 184,
		KEYCODE_PROG_YELLOW     = 185,
		KEYCODE_PROG_BLUE       = 186,
		KEYCODE_APP_SWITCH      = 187,
		KEYCODE_BUTTON_1        = 188,
		KEYCODE_BUTTON_2        = 189,
		KEYCODE_BUTTON_3        = 190,
		KEYCODE_BUTTON_4        = 191,
		KEYCODE_BUTTON_5        = 192,
		KEYCODE_BUTTON_6        = 193,
		KEYCODE_BUTTON_7        = 194,
		KEYCODE_BUTTON_8        = 195,
		KEYCODE_BUTTON_9        = 196,
		KEYCODE_BUTTON_10       = 197,
		KEYCODE_BUTTON_11       = 198,
		KEYCODE_BUTTON_12       = 199,
		KEYCODE_BUTTON_13       = 200,
		KEYCODE_BUTTON_14       = 201,
		KEYCODE_BUTTON_15       = 202,
		KEYCODE_BUTTON_16       = 203,
		KEYCODE_LANGUAGE_SWITCH = 204,
		KEYCODE_MANNER_MODE     = 205,
		KEYCODE_3D_MODE         = 206,
		KEYCODE_CONTACTS        = 207,
		KEYCODE_CALENDAR        = 208,
		KEYCODE_MUSIC           = 209,
		KEYCODE_CALCULATOR      = 210,
	}; 
#endif

    /// key group
    enum Group
    {
        Letters, // (A, B, ..., Z)
        Numbers, // (Key_0, ..., Key_9)
        NumPad, // (NumPad_0, ... NumPad9...+-*/)
        FunctionKeys, // (F1, ... , F24)
        CursorKeys,  // (->,... ,<-)
        HomeBlock // (Home, End, Prior, Next, Insert, Delete)
    };
    
    /// convert to string
    static Util::String ToString(Code code);
    /// convert from string
    static Code FromString(const Util::String& str);
    /// get key codes by group
    static Util::Array<InputKey::Code> KeyCodesByGroup(InputKey::Group group);

#ifdef __ANDROID__
	/// Translate Android key code to win32
	static Code TranslateAndoridToWin32(AndroidKeyCode androidCode);
#endif
	
private:
    static Util::Dictionary<Util::String,Code> dict;
};

};
//------------------------------------------------------------------------------





#endif // __inputkey_H__



