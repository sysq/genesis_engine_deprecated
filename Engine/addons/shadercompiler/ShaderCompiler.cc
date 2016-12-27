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
#include "ShaderCompiler.h"
#include "util/array.h"

#include "ShaderElement.h"
#include "ShaderMarcro.h"
#include "ShadercompilerConfig.h"

namespace ShaderProgramCompiler
{
	
	__ImplementAbstractClass(GpuProgramCompiler,'GPCP',Core::RefCounted);

GpuProgramCompiler::GpuProgramCompiler()
{

}

GpuProgramCompiler::~GpuProgramCompiler()
{

}

void GpuProgramCompiler::InitCompiler()
{
	
}

void GpuProgramCompiler::_BeforeCompile(const ShaderPass* pPass)
{
	m_sResultCode.Clear();
	m_sRegisterBinds.Clear();
	m_sCompiledCode.Clear();
	m_Macros.Clear();

	const ShadingTemplateSetting& shadeSetting = pPass->GetShadingSetting();
	const CommonShaderSetting&   cs           = pPass->GetCommonSetting();


	m_sProgramStartString = "RenderAPI \"" + CommonShaderSetting::GpuSdkAsString(cs.GetGpuSDK()) + "\" {\n";

	m_sResultCode += m_sProgramStartString;
}

void GpuProgramCompiler::_AfterCompile(bool bForwardPass)
{

	SizeT nCount;
	
	if (bForwardPass)
	{
		nCount = m_SubShaderMarcroPermuation.Size();
	} 
	else
	{
		nCount = 1;
	}
	
	for (IndexT i = 0; i < nCount; ++i)
	{
		Util::String nSub;
		IndexT mask = m_SubShaderMarcroPermuation.KeyAtIndex(i);
		nSub.AppendInt(mask);
		m_sResultCode += "SubGpuProgram \"" + nSub + "\" {\n";

		if (!m_sRegisterBinds.IsEmpty())
		{
			m_sResultCode += m_sRegisterBinds[i];
		}
		
		
		Util::String sCompiledCode = m_sCompiledCode[i];

		SizeT maxLength = 5000;

		if (sCompiledCode.Length() > maxLength)
		{
			IndexT departs = sCompiledCode.Length()/ maxLength;
			Util::String tmpStr; 
			for (SizeT i = 0; i < departs; ++i)
			{
				if (i != 0)
				{
					tmpStr += "\"\nSetShaderCode \"";
				}
				tmpStr += sCompiledCode.ExtractRange(i * maxLength, maxLength);				
			}
			tmpStr += "\"\nSetShaderCode \"" + sCompiledCode.ExtractToEnd(departs * maxLength);
			sCompiledCode = tmpStr;

		}

		m_sResultCode += "SetShaderCode \"" + sCompiledCode + "\"\n\n";
		
		//right bracket of SubGpuProgram
		m_sResultCode += "}\n";
	}

	//right bracket of RenderAPI
	m_sResultCode += "}\n";
}

void GpuProgramCompiler::_CreatePermutation(const Util::Array<Util::String>& marcroName, bool bTemplate)
{
	SizeT nCount = marcroName.Size();

	SizeT nPermutation = (SizeT)Math::n_pow((float)2, (float)nCount);

	Util::Array<IndexT> permutation;
	permutation.Resize(nCount, 0);

	if (!nCount)
	{
		m_Permutation.Add(0, permutation);
		return;
	}

	for (IndexT n=1; n <= nPermutation; ++n)
	{
		int temp = n;

		for(IndexT i=0; i < nCount; ++i)

		{
			permutation[nCount-1-i] = temp % 2;

			temp /= 2 ;

		}

		if (bTemplate)
		{
			SizeT nLignt = permutation[0];

			IndexT nLightMap = marcroName.FindIndex("LIGHTMAP_ON");
			IndexT nShadow   = marcroName.FindIndex("SHADOW_ON");

			if (nShadow != InvalidIndex)
			{
				//剔除有阴影没有灯光的情况
				if (permutation[nShadow] && (!nLignt) && (!permutation[nLightMap]))
				{
					continue;
				}
			}


			if (nLightMap != InvalidIndex)
			{
				//开启了lightmap就不需要拼接lighting的代码
				if (permutation[nLightMap] + nLignt > 1)
				{
					continue;
				}
		

			}
		}

		
 		
		m_Permutation.Add(n-1, permutation);
	}
}

void GpuProgramCompiler::CreateAllShaderMacrosPermutation(const Util::Array<Util::String>& marcroName, const GPtr<ShaderMarcro>& pMarcro, bool bTemplate)
{
	_CreatePermutation(marcroName, bTemplate);

	SizeT nCount = m_Permutation.Size();

	for (IndexT i = 0; i < nCount; ++i)
	{
		const Util::Array<IndexT>& permutation = m_Permutation.ValueAtIndex(i);

		ShaderMarcros marcros;

		SizeT nPermu = permutation.Size();
		uint shaderMask = 0;

		for (IndexT j = 0; j < nPermu; ++j)
		{
			
			if (permutation[j])
			{
				const Util::String& name = marcroName[j];
				shaderMask |= pMarcro->CreateMask(name);
				ShaderMarcroInfo info;
				info.name  = name;
				info.value = "1";
				marcros.Append(info);
			}
		}
		
		m_SubShaderMarcroPermuation.Add(shaderMask, marcros);
	

	}
}

void GpuProgramCompiler::CreateBuiltInMarcro(const uint iPass, const Util::Array<Util::String>& customMacros, const ShadingTemplateSetting* pSetting)
{
	GPtr<ShaderMarcro> pMacro = ShaderMarcro::Create();
	pMacro->Init(customMacros, pSetting);

	bool bTemplate = true;
	if (!pSetting)
	{
		bTemplate = false;
	}

	if (iPass == 0)
	{
		const Util::Array<Util::String>& allNames = pMacro->GetAllMarcroName();
		CreateAllShaderMacrosPermutation(allNames, pMacro, bTemplate);
	}

	const Util::Array<Util::String>& builtIn = pMacro->GetBuiltInMacroName();

	Util::String sBuiltIn;
	sBuiltIn += "\"";

	if (iPass == 0 && bTemplate)
	{
		for (IndexT i = 0; i < builtIn.Size(); ++i)
		{
			sBuiltIn +=  builtIn[i] + " ";
		}
	}

	
	sBuiltIn += "\"";

	m_BuiltInMacro.Add(iPass, sBuiltIn);

	
}

}