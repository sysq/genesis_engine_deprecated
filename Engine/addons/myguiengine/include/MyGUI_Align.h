/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
#ifndef __MYGUI_ALIGN_H__
#define __MYGUI_ALIGN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_Diagnostic.h"
#include "MyGUI_StringUtility.h"
#include <map>

namespace MyGUI
{

	struct MYGUI_EXPORT Align
	{
		enum Enum
		{
			HCenter = MYGUI_FLAG_NONE, /**< center horizontally */
			VCenter = MYGUI_FLAG_NONE, /**< center vertically */
			Center = HCenter | VCenter, /**< center in the dead center */

			Left = MYGUI_FLAG(1), /**< value from the left (and center vertically) */
			Right = MYGUI_FLAG(2), /**< value from the right (and center vertically) */
			HStretch = Left | Right, /**< stretch horizontally proportionate to parent window (and center vertically) */

			Top = MYGUI_FLAG(3), /**< value from the top (and center horizontally) */
			Bottom = MYGUI_FLAG(4), /**< value from the bottom (and center horizontally) */
			VStretch = Top | Bottom, /**< stretch vertically proportionate to parent window (and center horizontally) */

			ProLeft = MYGUI_FLAG(5),
			ProRight = MYGUI_FLAG(6),
			ProHStretch = ProLeft | ProRight,

			ProTop = MYGUI_FLAG(7),
			ProBottom = MYGUI_FLAG(8),
			ProVStretch = ProTop | ProBottom,

			ProStretch = ProHStretch | ProVStretch,
			ProDefault = ProLeft | ProTop,

			Stretch = HStretch | VStretch, /**< stretch proportionate to parent window */
			Default = Left | Top /**< default value (value from left and top) */
		};

		Align(Enum _value = Default) :
			value(_value)
		{
		}

		bool isHCenter() const
		{
			return HCenter == (value & ((int)HStretch)) && HCenter == (value & ((int)ProHStretch));
		}

		bool isVCenter() const
		{
			return VCenter == (value & ((int)VStretch)) && VCenter == (value & ((int)ProVStretch));
		}

		bool isCenter() const
		{
			return Center == (value & ((int)Stretch)) && Center == (value & ((int)ProStretch));
		}

		bool isLeft() const
		{
			return Left == (value & ((int)HStretch));
		}

		bool isProLeft() const
		{
			return ProLeft == (value & ((int)ProHStretch));
		}

		bool isRight() const
		{
			return Right == (value & ((int)HStretch));
		}

		bool isProRight() const
		{
			return ProRight == (value & ((int)ProHStretch));
		}

		bool isHStretch() const
		{
			return HStretch == (value & ((int)HStretch));
		}

		bool isProHStretch() const
		{
			return ProHStretch == (value & ((int)ProHStretch));
		}

		bool isTop() const
		{
			return Top == (value & ((int)VStretch));
		}

		bool isProTop() const
		{
			return ProTop == (value & ((int)ProVStretch));
		}

		bool isBottom() const
		{
			return (Bottom == (value & ((int)VStretch)));
		}

		bool isProBottom() const
		{
			return (ProBottom == (value & ((int)ProVStretch)));
		}

		bool isVStretch() const
		{
			return (VStretch == (value & ((int)VStretch)));
		}

		bool isProVStretch() const
		{
			return (ProVStretch == (value & ((int)ProVStretch)));
		}

		bool isStretch() const
		{
			return (Stretch == (value & ((int)Stretch)));
		}

		bool isProStretch() const
		{
			return (ProStretch == (value & ((int)ProStretch)));
		}

		bool isDefault() const
		{
			return (Default == (value & ((int)Stretch)));
		}

		bool isProDefault() const
		{
			return (ProDefault == (value & ((int)ProStretch)));
		}

		Align& operator |= (Align const& _other)
		{
			value = Enum(int(value) | int(_other.value));
			return *this;
		}

		friend Align operator | (Enum const& a, Enum const& b)
		{
			return Align(Enum(int(a) | int(b)));
		}

		friend Align operator | (Align const& a, Align const& b)
		{
			return Align(Enum(int(a.value) | int(b.value)));
		}

		friend bool operator == (Align const& a, Align const& b)
		{
			return a.value == b.value;
		}

		friend bool operator != (Align const& a, Align const& b)
		{
			return a.value != b.value;
		}

		typedef std::map<std::string, int> MapAlign;

		static Align parse(const std::string& _value)
		{
			Align result(Enum(0));
			const MapAlign& map_names = result.getValueNames();
			const std::vector<std::string>& vec = utility::split(_value);
			for (size_t pos = 0; pos < vec.size(); pos++)
			{
				MapAlign::const_iterator iter = map_names.find(vec[pos]);
				if (iter != map_names.end())
				{
					result.value = Enum(int(result.value) | int(iter->second));
				}
			}
			return result;
		}

		std::string print() const
		{
			std::string result;

			if (value & Left)
			{
				if (value & Right)
					result = "HStretch";
				else
					result = "Left";
			}
			else if (value & Right)
				result = "Right";
			else
				result = "HCenter";

			if (value & ProLeft)
			{
				if (value & ProRight)
					result = "ProHStretch";
				else
					result = "ProLeft";
			}
			else if (value & ProRight)
				result = "ProRight";
			else
				result = "HCenter";

			if (value & Top)
			{
				if (value & Bottom)
					result += " VStretch";
				else
					result += " Top";
			}
			else if (value & Bottom)
				result += " Bottom";
			else
				result += " VCenter";

			if (value & ProTop)
			{
				if (value & ProBottom)
					result += " ProVStretch";
				else
					result += " ProTop";
			}
			else if (value & ProBottom)
				result += " ProBottom";
			else
				result += " VCenter";

			return result;
		}

		friend std::ostream& operator << ( std::ostream& _stream, const Align&  _value )
		{
			_stream << _value.print();
			return _stream;
		}

		friend std::istream& operator >> ( std::istream& _stream, Align&  _value )
		{
			_value.value = Enum(0);
			std::string value;
			_stream >> value;

			const MapAlign& map_names = _value.getValueNames();
			MapAlign::const_iterator iter = map_names.find(value);
			if (iter != map_names.end())
				_value.value = Enum(int(_value.value) | int(iter->second));

			if (!_stream.eof())
			{
				std::string value2;
				_stream >> value2;
				iter = map_names.find(value2);
				if (iter != map_names.end())
					_value.value = Enum(int(_value.value) | int(iter->second));
			}

			return _stream;
		}
		//- zhaoxiaohang add this function here for the script binding,I will write the reason below.
		//- because we need to use the enum (Align::Enum) as a param in the script function ICall_getAlign and ICall_setAlign
		//- in the function setAlign, because struct has only one member(Align::Enum value),the c plus plus can convert it form an int(enum) to the strunt Align aotomatically.
		//- but in the function getAlign,we use struct Align as the return value,but we use an int(enum) to get int in the c sharp code,this is dangource,because I don't know 
		//- what will happen(what I only know is that it caused a bad memory.I've got no more details).

		int GetValue()
		{
			return value;
		}
	private:
		const MapAlign& getValueNames() const
		{
			static MapAlign map_names;

			if (map_names.empty())
			{
				// OBSOLETE
				map_names["ALIGN_HCENTER"] = HCenter;
				map_names["ALIGN_VCENTER"] = VCenter;
				map_names["ALIGN_CENTER"] = Center;
				map_names["ALIGN_LEFT"] = Left;
				map_names["ALIGN_RIGHT"] = Right;
				map_names["ALIGN_HSTRETCH"] = HStretch;
				map_names["ALIGN_TOP"] = Top;
				map_names["ALIGN_BOTTOM"] = Bottom;
				map_names["ALIGN_VSTRETCH"] = VStretch;
				map_names["ALIGN_STRETCH"] = Stretch;
				map_names["ALIGN_DEFAULT"] = Default;

				map_names["ALIGN_PRO_LEFT"] = ProLeft;
				map_names["ALIGN_PRO_RIGHT"] = ProRight;
				map_names["ALIGN_PRO_HSTRETCH"] = ProHStretch;
				map_names["ALIGN_PRO_TOP"] = ProTop;
				map_names["ALIGN_PRO_BOTTOM"] = ProBottom;
				map_names["ALIGN_PRO_VSTRETCH"] = ProVStretch;
				map_names["ALIGN_PRO_STRETCH"] = ProStretch;
				map_names["ALIGN_PRO_DEFAULT"] = ProDefault;

				MYGUI_REGISTER_VALUE(map_names, HCenter);
				MYGUI_REGISTER_VALUE(map_names, VCenter);
				MYGUI_REGISTER_VALUE(map_names, Center);
				MYGUI_REGISTER_VALUE(map_names, Left);
				MYGUI_REGISTER_VALUE(map_names, Right);
				MYGUI_REGISTER_VALUE(map_names, HStretch);
				MYGUI_REGISTER_VALUE(map_names, Top);
				MYGUI_REGISTER_VALUE(map_names, Bottom);
				MYGUI_REGISTER_VALUE(map_names, VStretch);
				MYGUI_REGISTER_VALUE(map_names, Stretch);
				MYGUI_REGISTER_VALUE(map_names, Default);

				MYGUI_REGISTER_VALUE(map_names, ProLeft);
				MYGUI_REGISTER_VALUE(map_names, ProRight);
				MYGUI_REGISTER_VALUE(map_names, ProHStretch);
				MYGUI_REGISTER_VALUE(map_names, ProTop);
				MYGUI_REGISTER_VALUE(map_names, ProBottom);
				MYGUI_REGISTER_VALUE(map_names, ProVStretch);
				MYGUI_REGISTER_VALUE(map_names, ProStretch);
				MYGUI_REGISTER_VALUE(map_names, ProDefault);
			}

			return map_names;
		}

	private:
		Enum value;
	};

} // namespace MyGUI

#endif // __MYGUI_ALIGN_H__
