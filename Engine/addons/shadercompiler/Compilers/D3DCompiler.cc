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

#if __WIN32__

#include "D3DCompiler.h"
#include "shadercompiler/ShaderElement.h"
#include "shadercompiler/ShaderCompilerSetting.h"
#include "CGTool.h"
#include "ShaderTemplate/ShaderJoint.h"
#include "ShaderMarcro.h"

namespace ShaderProgramCompiler
{
	__ImplementClass(D3DCompiler,'DDCP', GpuProgramCompiler);


D3DCompiler::D3DCompiler()
	: m_pTool(NULL)
{
	m_pTool = CGTool::Instance();
}

D3DCompiler::~D3DCompiler()
{

}

void D3DCompiler::InitCompiler()
{
	Super::InitCompiler();

	m_pTool->InitCG();


}

void D3DCompiler::Close()
{
	m_pTool->CloseCG();
}

void D3DCompiler::Compile(ShaderPass* pPass)
{
	SizeT nSubShader;
	const ShaderJoint::PassType& passType = pPass->GetPassType();
	bool bForwardPass = (passType == ShaderJoint::Forward);

	if (bForwardPass)
	{
		nSubShader = m_SubShaderMarcroPermuation.Size();
	}
	else
	{
		nSubShader = 1;
	}
	
	_BeforeCompile(pPass);

	for (IndexT i = 0; i < nSubShader; ++i)
	{
		m_Macros.Clear();

		ShaderMarcroInfo sdkInfo;
		sdkInfo.name  = "D3D9";
		sdkInfo.value = "1";

		m_Macros.Append(sdkInfo);


		if (bForwardPass)
		{
			const ShaderMarcros& macros = m_SubShaderMarcroPermuation.ValueAtIndex(i);
			SizeT nMarcros = macros.Size();
			for (IndexT i = 0; i < nMarcros; ++i)
			{
				if (!macros[i].IsDefault())
				{
					m_Macros.Append(macros[i]);
				}
				
			}
			
		}
		else
		{
			ShaderMarcroInfo macro;
			macro.name  = "SHADOW_ON";
			macro.value = "1"; 
			m_Macros.Append(macro);
		}


		m_pTool->Prepare(pPass, this);

		const Util::String& source = pPass->GetCode();

		const CommonShaderSetting& cs = pPass->GetCommonSetting();
		const CommonShaderSetting::ShaderType& st = cs.GetShaderType();
		const Util::String& entry                 = cs.GetEntryName(st);



		bool bRes = m_pTool->Compile(source, entry);

		if (bRes)
		{
			m_pTool->GetRegisterIndex();
		}

		const Util::String& sCompiled = m_pTool->GetCompiled();
		const Util::String& sBindings = m_pTool->GetBindings();

		pPass->SetCompiledCode(sCompiled);
		pPass->SetBingdings(sBindings);

		m_sCompiledCode.Add(i, sCompiled);
		m_sRegisterBinds.Add(i, sBindings);

		m_pTool->Clean();

		
	}

	_AfterCompile(bForwardPass);
}

}

#endif