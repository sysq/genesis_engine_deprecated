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
#pragma once
#ifndef GENESISMAKEGPUPROGRAM_H_
#define GENESISMAKEGPUPROGRAM_H_
#include "util/string.h"
#include "graphicsystem/Material/ShaderParam.h"
namespace GenesisMaterialMaker
{
class GenesisSubGPUProgram
{

public:

	GenesisSubGPUProgram();

	~GenesisSubGPUProgram();

public:

	void SetShaderCode(const Util::String& codeStr);

	Util::String GetShaderCode() const;

	void SetShaderType(const Util::String& type);

	const Util::String& GetShaderType() const;

	void AddParam(const Graphic::ShaderParam& param);

	const Graphic::ShaderParamList& GetParamList() const;

	void SetShaderMask(const Util::String& mask);

	const uint& GetShaderMask() const;

private:

	Util::String             m_codeStr;
	Util::String             m_shaderType;
	Graphic::ShaderParamList m_paramList;  

	uint                     m_shaderMask;
};

inline void GenesisSubGPUProgram::SetShaderCode(const Util::String& codeStr)
{
	m_codeStr += codeStr;
}

inline Util::String GenesisSubGPUProgram::GetShaderCode() const
{
	return m_codeStr;
}

inline void GenesisSubGPUProgram::SetShaderType(const Util::String& type)
{
	m_shaderType = type;
}

inline const Util::String& GenesisSubGPUProgram::GetShaderType() const
{
	return m_shaderType;
}

inline void GenesisSubGPUProgram::AddParam(const Graphic::ShaderParam& param)
{
	m_paramList.Append(param);
}

inline const Graphic::ShaderParamList& GenesisSubGPUProgram::GetParamList() const
{
	return m_paramList;
}

inline void GenesisSubGPUProgram::SetShaderMask(const Util::String& mask)
{
	m_shaderMask = (uint)mask.AsInt();
}

inline const uint& GenesisSubGPUProgram::GetShaderMask() const
{
	return m_shaderMask;
}


typedef Util::Dictionary<uint, GenesisSubGPUProgram> SubGpuProgramList;


class GenesisMakeGPUProgram
{
public:
	GenesisMakeGPUProgram();

	virtual ~GenesisMakeGPUProgram();

	
	void SetShaderType(const Util::String& type);
	const Util::String& GetShaderType() const;
	void SetDeviceType(const Util::String& api);
	const Util::String& GetDeviceType() const;
	
	void AddSubGpuProgram(GenesisSubGPUProgram& subProgram);

	const SubGpuProgramList& GetSubGpuProgramList() const;

	const SizeT GetSubGpuProgramCount() const;

	static bool IsVSGpuProgram(const GenesisMakeGPUProgram& gpu);

	static bool IsPSGpuProgram(const GenesisMakeGPUProgram& gpu);
	
private:
	Util::String m_codeStr;
	Util::String m_shaderType;
	Util::String m_renderAPI;
	Graphic::ShaderParamList m_paramList; 

	SubGpuProgramList m_subGpuProgramList;
};

typedef Util::Array<GenesisMakeGPUProgram> GenesisGPUProgramList;

inline void GenesisMakeGPUProgram::SetShaderType(const Util::String& type)
{
	m_shaderType = type;
}

inline const Util::String& GenesisMakeGPUProgram::GetShaderType() const
{
	return m_shaderType;
}

inline void GenesisMakeGPUProgram::SetDeviceType(const Util::String& api)
{
	m_renderAPI = api;
}

inline const Util::String& GenesisMakeGPUProgram::GetDeviceType() const
{
	return m_renderAPI;
}

inline void GenesisMakeGPUProgram::AddSubGpuProgram(GenesisSubGPUProgram& subProgram)
{
	subProgram.SetShaderType(m_shaderType);

	const uint nSub = subProgram.GetShaderMask();

	IndexT res = m_subGpuProgramList.FindIndex(nSub);

	if (res == InvalidIndex)
	{
		m_subGpuProgramList.Add(nSub, subProgram);
	}
	else
	{
		n_error("Duplicate SubGpuProgram!!!!!!!!!!!!\n");
	}
}

inline const SubGpuProgramList& GenesisMakeGPUProgram::GetSubGpuProgramList() const
{
	return m_subGpuProgramList;
}

inline const SizeT GenesisMakeGPUProgram::GetSubGpuProgramCount() const
{
	return m_subGpuProgramList.Size();
}

}

#endif//GENESISMAKEGPUPROGRAM_H_