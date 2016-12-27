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
#ifndef MATERIALPASS_H_
#define MATERIALPASS_H_

#include "core/refcounted.h"
#include "ShaderParam.h"
#include "rendersystem/base/RenderStateDesc.h"
#include "graphicsystem/Material/GlobalShaderParam.h"
#include "graphicsystem/base/RenderBase.h"
#include "shadercompiler/ShaderMarcro.h"

namespace Graphic
{
	class MaterialPass;
	class Material;

	enum ShaderCodeType
	{
		SCT_VS = 0,
		SCT_PS,
		SCT_COUNT
	};

	typedef Util::FixedArray<ShaderParamList> ShaderParamBindingList;
	struct MatParamBinding
	{
		int Regiter;	//	index of gpu register.
		int bindIndex;	//	bind index. in global parameters means global index, in local parameters, means local index.
	};
	typedef Util::Array<MatParamBinding> MatParamBindings;
	typedef Util::FixedArray<MatParamBindings> BindingMap;

	struct GlobalBindingInfo
	{
		union{
			int MatCommonEnd;
			int MatCustomBegin;
		};
		union{
			int MatCustomEnd;
			int VecCommonBegin;
		};
		union{
			int VecCommonEnd;
			int VecCustomBegin;
		};
		union
		{
			int VecCustomEnd;
			int TexBegin;
		};

	};
	typedef Util::FixedArray<GlobalBindingInfo> GlobalBindingInfoMap;

	// binding map ,between shader register and material parameters index.
	class ShaderParamBindingMap: public Core::RefCounted
	{
		__DeclareClass(ShaderParamBindingMap);
	public:
		ShaderParamBindingMap();
		virtual ~ShaderParamBindingMap();

		const MatParamBindings& GetLocalBinding( ShaderCodeType t){
			n_assert( t >= SCT_VS && t < SCT_COUNT);
			return m_LocalBinding[t];
		}
		const MatParamBindings& GetGlobalBinding(ShaderCodeType t ){
			n_assert( t >= SCT_VS && t < SCT_COUNT);
			return m_GlobalBinding[t];
		}
		const GlobalBindingInfo& GetGlobalBindingInfo(ShaderCodeType t){
			n_assert( t >= SCT_VS && t < SCT_COUNT);
			return m_GlobaBingdingInfos[t];
		}
		const ShaderParamList& GetShaderParamList(ShaderCodeType t){
			n_assert( t >= SCT_VS && t < SCT_COUNT);
			return m_ShaderBindingList[t];
		}

		const ShaderParamBindingList& GetShaderParamBindingList(){
			return m_ShaderBindingList;
		}
		void Setup();

	protected:


		ShaderParamBindingList m_ShaderBindingList;	// all register index, type and name of pass
		BindingMap m_GlobalBinding;	//	used for global parameters.
		BindingMap m_LocalBinding;	//	used for local parameters.
		GlobalBindingInfoMap m_GlobaBingdingInfos;
		friend class MaterialPass;
	};


	class MaterialPass : public Core::RefCounted
	{
		__DeclareSubClass(MaterialPass,Core::RefCounted)
	public:
		MaterialPass();
		virtual	~MaterialPass();

		void AddGPUProgramHandle(const uint& mask, const RenderBase::GPUProgramHandle& handle);

		const RenderBase::GPUProgramHandle& GetGPUProgramHandle(const uint& mask) const;

		const Util::Dictionary<uint, RenderBase::GPUProgramHandle>& GetGPUProgramHandle() const;

		void Prepare(const SizeT count);

		const Util::Dictionary<uint, GPtr<ShaderParamBindingMap> >& GetShaderParamBindingMap() const;


		void SetRenderStateDesc(const GPtr<RenderBase::RenderStateDesc>& rso);
		GPtr<RenderBase::RenderStateDesc>& GetRenderStateObject();
		void SetName(const Util::String& name);
		const Util::String& GetName() const;

		void AddShaderParamBinding(const uint& mask, ShaderCodeType t, const ShaderParamList& value);
		const GPtr<ShaderParamBindingMap>& GetParamBindings(const uint& shaderMask);


		void Clone(const GPtr<MaterialPass>& from);

		void SetGlobalParamInfo(const uint& mask, const int& index, const bool value);

		bool isGlobalParamUsed( const uint& mask, const int& inedx ) const;

		void SetShaderMarcro(const GPtr<ShaderProgramCompiler::ShaderMarcro>& pMarcro);

		const GPtr<ShaderProgramCompiler::ShaderMarcro>& GetShaderMarcro() const;

		// internal call.
		// used for build a binding map for shader parameter and gpu register.
		void _BuildBindingMap(Material* mat);

	private:
		Util::Dictionary<uint, Util::Array<bool> > m_bGlobalParamUsage;
		GPtr<RenderBase::RenderStateDesc> m_renderStateObject;
		Util::Dictionary<uint, RenderBase::GPUProgramHandle> m_GPUProgramHandle;
		Util::Dictionary<uint, GPtr<ShaderParamBindingMap> > m_ShaderParamBindingMap;
		Util::String m_name;
		GPtr<ShaderProgramCompiler::ShaderMarcro>  m_pShaderMarcro;
	};

	inline void MaterialPass::AddGPUProgramHandle(const uint& mask, const RenderBase::GPUProgramHandle& handle)
	{
		IndexT res = m_GPUProgramHandle.FindIndex(mask);
		if (res == InvalidIndex)
		{
			m_GPUProgramHandle.Add(mask, handle);
		}
	}

	inline const RenderBase::GPUProgramHandle& MaterialPass::GetGPUProgramHandle(const uint& mask) const
	{
		return m_GPUProgramHandle[mask];
	}

	inline const Util::Dictionary<uint, RenderBase::GPUProgramHandle>& MaterialPass::GetGPUProgramHandle() const
	{
		return m_GPUProgramHandle;
	}

	inline void MaterialPass::AddShaderParamBinding(const uint& mask, ShaderCodeType t, const ShaderParamList& value)
	{
		n_assert( t >= SCT_VS && t < SCT_COUNT );
		
		IndexT res = m_ShaderParamBindingMap.FindIndex(mask);

		if (res == InvalidIndex)
		{
			GPtr<ShaderParamBindingMap> pBindMap = ShaderParamBindingMap::Create();
			pBindMap->m_ShaderBindingList[t] = value;
			m_ShaderParamBindingMap.Add(mask, pBindMap);
		}
		else
		{
			m_ShaderParamBindingMap.ValueAtIndex(res)->m_ShaderBindingList[t] = value;
		}

	}

	inline const Util::Dictionary<uint, GPtr<ShaderParamBindingMap> >& MaterialPass::GetShaderParamBindingMap() const
	{
		return m_ShaderParamBindingMap;
	}

	inline void MaterialPass::SetRenderStateDesc(const GPtr<RenderBase::RenderStateDesc>& rso)
	{
		m_renderStateObject = rso;
	}

	inline GPtr<RenderBase::RenderStateDesc>& MaterialPass::GetRenderStateObject()
	{
		return m_renderStateObject;
	}

	inline 
		const GPtr<ShaderParamBindingMap>& 
		MaterialPass::GetParamBindings(const uint& shaderMask)
	{
		return m_ShaderParamBindingMap[shaderMask];
	}

	inline
		void MaterialPass::SetName(const Util::String& name)
	{
		m_name = name;
	}

	inline
		const Util::String& MaterialPass::GetName() const
	{
		return m_name;
	}

	inline
		bool MaterialPass::isGlobalParamUsed( const uint& mask, const int& index ) const
	{
		n_assert( index >=0 && index < GlobalParamCount );
		IndexT idMask = m_bGlobalParamUsage.FindIndex(mask);
		if(idMask == InvalidIndex)
			return false;
		return m_bGlobalParamUsage[mask][index];
	}

	inline
		void MaterialPass::SetGlobalParamInfo(const uint& mask, const int& index, const bool value)
	{
		IndexT res = m_bGlobalParamUsage.FindIndex(mask);

		if (res == InvalidIndex)
		{
			Util::Array<bool> paramInfos;
			paramInfos.Resize(GlobalParamCount, false);
			m_bGlobalParamUsage.Add(mask, paramInfos);
		}

		m_bGlobalParamUsage[mask][index] = value;
	}

	inline 
		void MaterialPass::SetShaderMarcro(const GPtr<ShaderProgramCompiler::ShaderMarcro>& pMarcro)
	{
		m_pShaderMarcro = pMarcro;
	}

	inline 
		const GPtr<ShaderProgramCompiler::ShaderMarcro>& MaterialPass::GetShaderMarcro() const
	{
		return m_pShaderMarcro;
	}
}
#endif//MATERIALPASS_H_