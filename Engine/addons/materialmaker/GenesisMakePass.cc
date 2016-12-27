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
#include "GenesisMakePass.h"
#include "graphicsystem/GraphicSystem.h"
#include "rendersystem/gles/GPUProgramGLES.h"
#include "shadercompiler/ShaderMarcro.h"
namespace GenesisMaterialMaker
{

	void _parse_paramter(Util::String& param, bool& bArray, const Util::String& src, const SizeT nCount)
	{
		IndexT index = src.FindCharIndex('['); 

		if (InvalidIndex != index)
		{
			param = src.ExtractRange(0, index);
			bArray = true;
		}
		else
		{
			if (nCount > 1)
			{
				bArray = true;
			}


			param = src;
		}
	}

	bool IsGlobalParamter(const Util::String& param)
	{
		if (param == "g_LightProj"    || param == "g_LightDir" ||
			param == "g_LightDiffuse" || param == "g_LightPos" ||
			param == "g_LightAttenFactor")
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	GenesisMakePass::GenesisMakePass()
	{
		//empty
	}

	GenesisMakePass::~GenesisMakePass()
	{
		m_RenderObjectState = 0;
	}

	void GenesisMakePass::CreatePass(GPtr<Graphic::MaterialTech>& tech, const uint iPass)
	{
		const GenesisMakeGPUProgram& vsGpuProgram = m_ShaderProgramList[0];
		const GenesisMakeGPUProgram& psGpuProgram = m_ShaderProgramList[1];

		n_assert( GenesisMakeGPUProgram::IsVSGpuProgram(vsGpuProgram) );
		n_assert( GenesisMakeGPUProgram::IsPSGpuProgram(psGpuProgram) );

		const SizeT nVsCount = vsGpuProgram.GetSubGpuProgramCount();
		const SizeT nPsCount = psGpuProgram.GetSubGpuProgramCount();

		n_assert( nVsCount == nPsCount );

		nSubProgram = nVsCount;

		const SubGpuProgramList& vsSubProgramList = vsGpuProgram.GetSubGpuProgramList();
		const SubGpuProgramList& psSubProgramList = psGpuProgram.GetSubGpuProgramList();


		GPtr<Graphic::MaterialPass> pass = Graphic::MaterialPass::Create();

		GPtr<ShaderProgramCompiler::ShaderMarcro> pMarcro = ShaderProgramCompiler::ShaderMarcro::Create();

		SizeT nBuiltIn = m_BuiltInMacro.Size();
		for (IndexT i = 0; i < nBuiltIn; ++i)
		{
			pMarcro->Add(m_BuiltInMacro[i]);
		}

		SizeT nCustom = m_CustomMacro.Size();
		for (IndexT i = 0; i < nCustom; ++i)
		{
			pMarcro->Add(m_CustomMacro[i], true);
		}

		pass->SetShaderMarcro(pMarcro);

#if RENDERDEVICE_D3D9
		for (IndexT i = 0; i < nVsCount; ++i)
		{
			GPtr<RenderBase::GPUProgram> program = RenderBase::GPUProgram::Create();

			const GenesisSubGPUProgram& vsSubGpuProgram = vsSubProgramList.ValueAtIndex(i);
			const GenesisSubGPUProgram& psSubGpuProgram = psSubProgramList.ValueAtIndex(i);

			const uint vsMask = vsSubGpuProgram.GetShaderMask();
			const uint psMask = psSubGpuProgram.GetShaderMask();

			n_assert(vsMask == psMask);

			program->SetVertexCode(vsSubGpuProgram.GetShaderCode());
			program->SetPixelCode(psSubGpuProgram.GetShaderCode());

			pass->AddShaderParamBinding(vsMask, Graphic::SCT_VS, vsSubGpuProgram.GetParamList());
			pass->AddShaderParamBinding(vsMask, Graphic::SCT_PS, psSubGpuProgram.GetParamList());

			RenderBase::GPUProgramHandle gpuHandle = Graphic::GraphicSystem::Instance()->CreateShaderProgram(program);
			pass->AddGPUProgramHandle(vsMask, gpuHandle);
		}
#elif RENDERDEVICE_OPENGLES
		for (IndexT i = 0; i < nVsCount; ++i)
		{
			GPtr<RenderBase::GPUProgram> program = RenderBase::GPUProgram::Create();

			const GenesisSubGPUProgram& vsSubGpuProgram = vsSubProgramList.ValueAtIndex(i);
			const uint vsMask = vsSubGpuProgram.GetShaderMask();

			Util::String shaderCode;
			Util::String vsCode;
			Util::String psCode;
			static Util::String s_vsStart("VertexBegin");
			static Util::String s_vsEnd("VertexEnd");
			static Util::String s_psStart("PixelBegin");
			static Util::String s_psEnd("PixelEnd");

			shaderCode = vsSubGpuProgram.GetShaderCode();

			IndexT vertexStart   = shaderCode.FindStringIndex(s_vsStart);
			IndexT vertexEnd     = shaderCode.FindStringIndex(s_vsEnd);
			IndexT fragmentStart = shaderCode.FindStringIndex(s_psStart);
			IndexT fragmentEnd   = shaderCode.FindStringIndex(s_psEnd);

			if (vertexStart != InvalidIndex && vertexEnd != InvalidIndex)
			{
				vsCode = shaderCode.ExtractRange(vertexStart + s_vsStart.Length(), vertexEnd - vertexStart - s_vsStart.Length());
				program->SetVertexCode(vsCode);
			}

			if (fragmentStart != InvalidIndex && fragmentEnd != InvalidIndex)
			{
				psCode = shaderCode.ExtractRange(fragmentStart + s_psStart.Length(), fragmentEnd - fragmentStart - s_psStart.Length());
				program->SetPixelCode(psCode);
			}

			RenderBase::GPUProgramHandle gpuHandle = Graphic::GraphicSystem::Instance()->CreateShaderProgram(program);
			pass->AddGPUProgramHandle(vsMask, gpuHandle);

			_InitOpenGLes(gpuHandle, pass, &vsSubGpuProgram, vsMask);
		}
#endif


		/*for (SizeT i = 0; i < m_ShaderProgramList.Size(); ++i)
		{
			if (m_ShaderProgramList[i].GetDeviceType() == "D3D9")
			{
				if (m_ShaderProgramList[i].GetShaderType() == "vs")
				{
					program->SetVertexCode(m_ShaderProgramList[i].GetShaderCode());
					pass->AddShaderParamBinding(Graphic::SCT_VS,m_ShaderProgramList[i].GetParamList());
				}
				else if (m_ShaderProgramList[i].GetShaderType() == "ps")
				{
					program->SetPixelCode(m_ShaderProgramList[i].GetShaderCode());
					pass->AddShaderParamBinding(Graphic::SCT_PS,m_ShaderProgramList[i].GetParamList());
				}
			}
			else if (m_ShaderProgramList[i].GetDeviceType() == "OpenGL")
			{
				if (m_ShaderProgramList[i].GetShaderType() == "vs")
				{
					program->SetVertexCode(m_ShaderProgramList[i].GetShaderCode());
					pass->AddShaderParamBinding(Graphic::SCT_VS,m_ShaderProgramList[i].GetParamList());
				}
				else if (m_ShaderProgramList[i].GetShaderType() == "ps")
				{
					program->SetPixelCode(m_ShaderProgramList[i].GetShaderCode());
					pass->AddShaderParamBinding(Graphic::SCT_PS,m_ShaderProgramList[i].GetParamList());
				}
			}
			else if (m_ShaderProgramList[i].GetDeviceType() == "OpenGLES")
			{
				Util::String shaderCode;
				Util::String vsCode;
				Util::String psCode;
				Util::String s_vsStart("VertexBegin");
				Util::String s_vsEnd("VertexEnd");
				Util::String s_psStart("PixelBegin");
				Util::String s_psEnd("PixelEnd");
				if (m_ShaderProgramList[i].GetShaderType() == "vs")
				{
					shaderCode           = m_ShaderProgramList[i].GetShaderCode();
					IndexT vertexStart   = shaderCode.FindStringIndex(s_vsStart);
					IndexT vertexEnd     = shaderCode.FindStringIndex(s_vsEnd);
					IndexT fragmentStart = shaderCode.FindStringIndex(s_psStart);
					IndexT fragmentEnd   = shaderCode.FindStringIndex(s_psEnd);
					if (vertexStart != InvalidIndex && vertexEnd != InvalidIndex)
					{
						vsCode = shaderCode.ExtractRange(vertexStart + s_vsStart.Length(), vertexEnd - vertexStart - s_vsStart.Length());
						program->SetVertexCode(vsCode);
					}
					if (fragmentStart != InvalidIndex && fragmentEnd != InvalidIndex)
					{
						psCode = shaderCode.ExtractRange(fragmentStart + s_psStart.Length(), fragmentEnd - fragmentStart - s_psStart.Length());
						program->SetPixelCode(psCode);
					}

					m_bGles = true;
				}
			}

		}*/

		pass->SetRenderStateDesc(m_RenderObjectState);
		pass->SetName(m_name);
		


		tech->AddPass(pass);
	}

	void GenesisMakePass::_InitOpenGLes(RenderBase::GPUProgramHandle& gpuHandle, const GPtr<Graphic::MaterialPass>& pass, const GenesisSubGPUProgram* subProgram, const IndexT mask)
	{
		using namespace RenderBase;
		using namespace GLES;
		using namespace Graphic;

		GPtr<RenderCommandType> rcType( (RenderCommandType*)gpuHandle.AsObject() );
		GPtr<GPUProgram> program = rcType.downcast<GPUProgram>();

		GPtr<GPUProgramGLES> glesProgram = program.downcast<GPUProgramGLES>();
		n_assert(glesProgram.isvalid());

		Graphic::ShaderParamList shaderParamsList;

		const Util::Array<Uniform>& activeUniforms = glesProgram->GetActiveUniforms();

		Util::String paramName;
		
		for (IndexT iUniform = 0; iUniform < activeUniforms.Size(); ++iUniform)
		{
			bool bArray = false;
			const Uniform& uni = activeUniforms[iUniform];
			_parse_paramter(paramName, bArray, uni.name, uni.size);

			for (IndexT iParam = 0; iParam < uni.size; ++iParam)
			{
				ShaderParam shaderParam;

				if (bArray)
				{
					Util::String sNum;
					sNum.SetInt(iParam);
					Util::String name;
					name = paramName + sNum;
					shaderParam.SetName(name);
					shaderParam.SetRegister(uni.location + iParam);
				}
				else
				{
					if (IsGlobalParamter(uni.name))
					{
						Util::String sNum;
						sNum.SetInt(0);
						shaderParam.SetName(uni.name + sNum);
					}
					else
					{
						shaderParam.SetName(uni.name);
					}
					
					shaderParam.SetRegister(uni.location);
				}			

				switch (uni.type)
				{
				case GL_FLOAT:
				case GL_INT:
					shaderParam.SetParamType(eShaderParamFloat);
					break;

				case GL_FLOAT_VEC2:
				case GL_FLOAT_VEC3:
				case GL_FLOAT_VEC4:
				case GL_INT_VEC2:
				case GL_INT_VEC3:
				case GL_INT_VEC4:
					shaderParam.SetParamType(eShaderParamVector);
					break;

				case GL_FLOAT_MAT2:
				case GL_FLOAT_MAT3:
				case GL_FLOAT_MAT4:
					shaderParam.SetParamType(eShaderParamMatrix);
					break;

				case GL_SAMPLER_2D:
					shaderParam.SetParamType(eShaderParamTexture2D);
					break;

				case GL_SAMPLER_CUBE:
					shaderParam.SetParamType(eShaderParamTextureCUBE);
					break;
				}

				const_cast<GenesisSubGPUProgram*>(subProgram)->AddParam(shaderParam);
			}
			
		}
		
		pass->AddShaderParamBinding(mask, Graphic::SCT_VS, subProgram->GetParamList());
	}

	int GenesisMakePass::_FindRegister(const uint& sub, const Util::String& name)
	{
		const GenesisMakeGPUProgram& vsGpuProgram = m_ShaderProgramList[0];
		const GenesisMakeGPUProgram& psGpuProgram = m_ShaderProgramList[1];

		const SubGpuProgramList& vsSubProgramList = vsGpuProgram.GetSubGpuProgramList();
		const SubGpuProgramList& psSubProgramList = psGpuProgram.GetSubGpuProgramList();

		const GenesisSubGPUProgram& vsSubGpuProgram = vsSubProgramList.ValueAtIndex(sub);
		const GenesisSubGPUProgram& psSubGpuProgram = psSubProgramList.ValueAtIndex(sub);

		Util::Array<GenesisSubGPUProgram> subPrograms;
		subPrograms.Append(vsSubGpuProgram);
		subPrograms.Append(psSubGpuProgram);

		for (SizeT i = 0; i < subPrograms.Size(); ++i)
		{
			const Graphic::ShaderParamList& spl = subPrograms[i].GetParamList();
			for(SizeT spIndex = 0; spIndex < spl.Size(); ++spIndex )
			if(spl[spIndex].GetName() == name)
			{
				return spl[spIndex].GetRegister();
			}
		}
		return -1;
	}

	const uint GenesisMakePass::_GetSubProgramMask(const IndexT iSub) const
	{
		const GenesisMakeGPUProgram& vsGpuProgram = m_ShaderProgramList[0];
		const SubGpuProgramList& vsSubProgramList = vsGpuProgram.GetSubGpuProgramList();
		const GenesisSubGPUProgram& vsSubGpuProgram = vsSubProgramList.ValueAtIndex(iSub);

		const uint mask = vsSubGpuProgram.GetShaderMask();
		return mask;
	}

	void GenesisMakePass::AssignTextureSamplerStates(const GenesisTextureSamplerStateList& sslist)
	{
		if (!m_RenderObjectState.isvalid())
			return;

		for (IndexT iSub = 0; iSub < nSubProgram; ++iSub)
		{
			RenderBase::DeviceSamplerState rss;
			uint mask = _GetSubProgramMask(iSub);
			
			for (SizeT i = 0; i < sslist.Size(); ++i)
			{
				int reg = _FindRegister(iSub, sslist[i].m_name);
				if (reg != -1)
				{
					rss.m_textureIndexEnable[reg] = true;
					rss.m_addressU[reg] = sslist[i].m_tam;
					rss.m_addressV[reg] = sslist[i].m_tam;
					rss.m_addressW[reg] = sslist[i].m_tam;
					rss.m_Filter[reg] = sslist[i].m_tfo;
					rss.m_textureType[reg] = sslist[i].m_textureType;
				}
			}
			m_RenderObjectState->SetSamplerState(mask, rss);
		}

		
	}
}