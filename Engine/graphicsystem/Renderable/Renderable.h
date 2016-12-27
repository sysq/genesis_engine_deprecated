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
#ifndef RENDERABLE_H_
#define RENDERABLE_H_
#include "foundation/core/types.h"
#include "foundation/util/dictionary.h"
namespace Graphic
{
	class MaterialInstance;
	class Renderable
	{
	public:
		enum Mark
		{
			GenShadow = BIT_FLAG(0),
			CastShadow = BIT_FLAG(1),
			GenDepth = BIT_FLAG(2),
			MarkNone = BIT_FLAG_NONE,
			MarkAll = ~BIT_FLAG_NONE,			
		};

		virtual ~Renderable();

		MaterialInstance* GetMaterial() const;
		void SetMaterial(MaterialInstance* material);

		// renderable sort offset
		signed char GetSort() const;
		void SetSort( signed char sort);

		Mark GetMark() const;
		void SetMark(Mark mark);

		template<typename T> 
		T* cast_safe();  //safe, but slow

		template<typename T> 
		T* cast_fast();// unsafe, but fast

		template<typename T> 
		const T* cast_safe() const;  //safe, but slow

		template<typename T> 
		const T* cast_fast() const;// unsafe, but fast

		void SetShaderMask(const uint type, const uint& mask);

		const uint& GetShaderMask(const uint type) const;

		void ResetShaderMask();

		void SetAffectedLightCount(const SizeT num);

		const SizeT GetAffectedLightCount() const;

	protected:		
		Renderable();
		MaterialInstance* mMaterial;
		signed char mSort;
		Mark mMark;

		Util::Dictionary<uint, uint>  m_ShaderMask;
		static uint                   m_sDefaultShaderMask;
		SizeT                         m_nLights;
	};

	inline MaterialInstance* Renderable::GetMaterial() const
	{
		return mMaterial;
	}

	inline void Renderable::SetMaterial(MaterialInstance* material)
	{
		mMaterial = material;
	}

	inline signed char Renderable::GetSort() const
	{
		return mSort;
	}

	inline void Renderable::SetSort(signed char sort)
	{
		mSort = sort;
	}

	inline Renderable::Mark Renderable::GetMark() const
	{
		return mMark;
	}
	inline void Renderable::SetMark(Mark mark)
	{
		mMark = mark;
	}


	template<typename T> 
	inline T* Renderable::cast_safe()
	{
		return reinterpret_cast<T*>(this);
	}

	template<typename T>
	inline T* Renderable::cast_fast()
	{
		return static_cast<T*>(this);
	}

	template<typename T> 
	inline const T* Renderable::cast_safe() const
	{
		return reinterpret_cast<const T*>(this);
	}

	template<typename T>
	inline const T* Renderable::cast_fast() const
	{
		return static_cast<const T*>(this);
	}

	inline void Renderable::SetShaderMask(const uint type, const uint& mask)
	{
#if _DEBUG
		IndexT res = m_ShaderMask.FindIndex(type);

		if (res != InvalidIndex)
		{
			m_ShaderMask[type] = mask;
		}
		else
		{
			n_error("No Tis RenderPassType in RenderObject!!!!\n");
		}

#else
		m_ShaderMask[type] = mask;

#endif	

	}

	inline const uint& Renderable::GetShaderMask(const uint type) const
	{
#if _DEBUG
		IndexT res = m_ShaderMask.FindIndex(type);
		if (res != InvalidIndex)
		{
			return m_ShaderMask[type];
		}
		else
		{
			n_error("No Tis RenderPassType in RenderObject!!!!\n");
			return m_sDefaultShaderMask;
		}
#endif

		return m_ShaderMask[type];
	}

	inline void Renderable::SetAffectedLightCount(const SizeT num)
	{
		m_nLights = num;
	}

	inline const SizeT Renderable::GetAffectedLightCount() const
	{
		return m_nLights;
	}

}
#endif
