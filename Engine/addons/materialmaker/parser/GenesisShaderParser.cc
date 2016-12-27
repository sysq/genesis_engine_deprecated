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
#include "../GenesisMaterial.h"
#include "GenesisShaderParser.h"
#include "addons/shadercompiler/ShaderFactory.h"
#include "foundation/io/ioserver.h"
#include "foundation/io/textreader.h"
#include "io/memorystream.h"
#include "io/iointerfaceprotocol.h"
#include "io/iointerface.h"
#include "foundation/exception/exceptions.h"
#include "addons/shadercompiler/Utility/ShaderCompilerUtil.h"
#include "shadercompiler/ShadercompilerConfig.h"
#include "profilesystem/ProfileSystem.h"


void InitLexer (const char *buf, size_t length);
void EndLexer ();

int Genesisparse ();
extern GenesisMaterialMaker::GenesisMaterial* g_GenesisMaterial;
GPtr<Graphic::Material> Graphic::Material::s_defaultFallBackMat = 0;
namespace GenesisMaterialMaker
{
	bool CreateCompiledShaderDirectory(const IO::URI& outputFile)
	{
		
		return IO::IoServer::Instance()->CreateDirectory(outputFile);
	}
	void CompileShaderCode(const Util::StringAtom& shaderfile, Util::String& outputShader)
	{
		static IO::URI uriISpath("sys:");
		static IO::URI includeSysPath = IO::AssignRegistry::Instance()->ResolveAssigns(uriISpath);
		Util::String outputShaderfile;

		outputShaderfile = shaderfile.AsString();
		IO::URI outputPath("cmpileshd:" + outputShaderfile.ExtractFileName());

		outputShaderfile = outputPath.AsString().ExtractDirName() + "output_" + outputShaderfile.ExtractFileName();

#if __WIN32__ && _DEBUG
		if (!CreateCompiledShaderDirectory(outputPath.AsString().ExtractDirName()))
			n_warning("CompileShaderCode: can't create directory: %s",outputShaderfile.ExtractDirName().AsCharPtr());
#endif

#ifdef __GENESIS_EDITOR__
		ShaderProgramCompiler::ShaderCompilerConfig::usePreCompileShader = false;
		ShaderProgramCompiler::ShaderCompilerConfig::shaderCompileType = ShaderProgramCompiler::ShaderCompilerConfig::D3D9_SHADER_COMPILER;
#endif

#if __ANDROID__
		ShaderProgramCompiler::ShaderCompilerConfig::shaderCompileType = ShaderProgramCompiler::ShaderCompilerConfig::GLES_SHADER_COMPILER;
#endif
		if ( !ShaderProgramCompiler::ShaderCompilerConfig::usePreCompileShader )
		{
			 //editor,shaderPrecompileTool, use
			ShaderProgramCompiler::ShaderFactory::Instance()->Add(shaderfile.AsString());
			ShaderProgramCompiler::ShaderFactory::Instance()->Process();
			outputShader = ShaderProgramCompiler::ShaderFactory::Instance()->GetCompiledCode();
			ShaderProgramCompiler::ShaderFactory::Instance()->Finish();
		} 
		else
		{
			if ( !ShaderProgramCompiler::ShaderUtil::ReadText(shaderfile.AsString(),outputShader) )
			{
				n_warning("read compiled shader file failed,shader:%s \n",shaderfile.Value());
			}	
		}



	

		
	}

GPtr<Graphic::Material> MakeFromShader(const char* text,SizeT size)
{
	GenesisMaterial* genesismat = new GenesisMaterial();
	g_GenesisMaterial = genesismat;
	InitLexer(text,size);
	Genesisparse();
	EndLexer();
	GPtr<Graphic::Material> mat = genesismat->CreateRealMaterial();
	g_GenesisMaterial = NULL;
	delete genesismat;
	return mat;
}

void CreateDefaultFallBackMaterial()
{
	Util::StringAtom atom("sys:defaultFallBackShader.shader");
	Util::String text;

	CompileShaderCode(atom, text);
	Graphic::Material::s_defaultFallBackMat = GenesisMaterialMaker::MakeFromShader(text.AsCharPtr(),text.Length());	
}

bool VerifyGenesisShader(GPtr<Graphic::Material>& mat, const Util::StringAtom& shaderfile)
{
	if (mat->GetTechList().IsEmpty())
	{
		n_warning("MakeFromShader Warning:FileName: %s has technique error!\n rollback to default fallback shader!",shaderfile.AsString().AsCharPtr());
		return false;
	}

	for (SizeT i = 0; i < mat->GetTechList().Size(); ++i)
	{
		if (mat->GetTechList()[i]->GetPassList().IsEmpty())
		{
			n_warning("MakeFromShader Warning:FileName: %sTech: %d has pass error!\n Rollback to default fallback shader!",shaderfile.AsString().AsCharPtr(),i);
			return false;
		}
	}
	return true;
}
GPtr<Graphic::Material> MakeFromShader(const Util::StringAtom& shaderfile)
{

	if (!Graphic::Material::s_defaultFallBackMat.isvalid())
	{
		try
		{
			CreateDefaultFallBackMaterial();
		}
		catch(const Exceptions::Exception& e)
		{
			n_warning( "defaultfallback shader has error!\n", e.what() );
		}
	}

	

#ifdef __ANDROID__
	LOGI(shaderfile.Value());
#else
    n_printf( "shader %s\n", shaderfile.Value() );
#endif

	// check the empty file
	GPtr<IO::Stream> pStream = IO::MemoryStream::Create();
	n_assert( pStream );
	GPtr<IO::ReadStream> readStreamMsg = IO::ReadStream::Create();
	n_assert( readStreamMsg );

	readStreamMsg->SetFileName( shaderfile );
	readStreamMsg->SetStream( pStream );
	IO::IoInterface::Instance()->SendWait( readStreamMsg.upcast<Messaging::Message>() );

	if ( !readStreamMsg->GetResult() )
	{
		n_warning( "ShaderProcessor::ReadTextStream: can not open file: %s", shaderfile.Value() );
		return Graphic::Material::s_defaultFallBackMat;
	}
	

	Util::String text;
	try
	{
#ifdef __PROFILER_IO__ 
		//begint tick
		PROFILER_IO_TICKBEGIN(tickBegin);
		PROFILER_IO_LOGTIMES("begin,compile shader %s",shaderfile.Value());
#endif
		CompileShaderCode(shaderfile, text);

#ifdef __PROFILER_IO__
		//end tick
		PROFILER_IO_TICKEND(tickBegin,delta);
		PROFILER_IO_LOGTIMES2("end,compile shader %s,time consume: %f ms",shaderfile.Value(),__tick_delta_delta/1000.f);
		PROFILER_IO_READ_RES(mIoReadTexture,delta);
#endif
	}
	catch(const Exceptions::Exception& e)
	{
		n_warning( "Compile %s Error: %s!\n", shaderfile.AsString().AsCharPtr(), e.what() );
		return Graphic::Material::s_defaultFallBackMat;
	}

	GPtr<Graphic::Material> mat = GenesisMaterialMaker::MakeFromShader(text.AsCharPtr(),text.Length());

	return VerifyGenesisShader(mat,shaderfile) ? mat : Graphic::Material::s_defaultFallBackMat;
}
}