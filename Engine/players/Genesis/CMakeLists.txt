#****************************************************************************
# Copyright (c) 2011-2013,WebJet Business Division,CYOU
#  
# http://www.genesis-3d.com.cn
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#****************************************************************************


#base lib for executable to link
MACRO( _MACRO_EXECUTABLE_BASE_LIB arg_TargetName )

TARGET_LINK_LIBRARIES(
#Project lib
	${arg_TargetName}
	App
	Animation
	Input
	Paticles
	FreeType
	GUIBase
	Serialization
	Resource
	GraphicSystem
	RenderSystem
	ProfileSystem
	TinyXML
	ZLib
	ShaderCompiler
	Sprite
	Vis
	MaterialMaker
	Foundation
	PackageTool
	Vegetation
	TerrainSystem
	SoundSystem

#mygui lib
	MyGUIEngine
	MyGUIPlatform

#system lib
	dbghelp.lib
	dxguid.lib
	wsock32.lib	
	rpcrt4.lib
	wininet.lib
	Wbemuuid.lib
	psapi.lib
	version.lib
#D3D lib
	d3d9.lib
	d3dx9.lib
	dinput8.lib
	xinput.lib
	dxerr9.lib
	Winmm.lib
#thirdpartylib
	${GENESIS_EXTERNAL_LIBS_DIR}/OpenGLES2.0/ARM/libEGL.lib
	${GENESIS_EXTERNAL_LIBS_DIR}/OpenGLES2.0/ARM/libGLESv2.lib
	${GENESIS_EXTERNAL_LIBS_DIR}/CG/Win/cg.lib
)

#lib
TARGET_LINK_LIBRARIES(
	${arg_TargetName}
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/mojoshader/win32/Debug/mojoshader.lib	
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/hlsl2glslfork/win32/Debug/hlsl2glsl.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/GlslOptimizer/win32/Debug/GLSL_Optimizer.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/FreeImage/win32/FreeImaged.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/OpCode/Debug/Opcode.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/libmpg123/Debug/libmpg123.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/Ogg/Debug/Ogg.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/Vorbis/Debug/vorbis.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/OpenAL/Debug/OpenAL.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/RepX3CHECKED.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PxTaskCHECKED.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3CommonCHECKED_x86.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3ExtensionsCHECKED.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3CookingCHECKED_x86.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3CHECKED_x86.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3VehicleCHECKED.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3CharacterKinematicCHECKED_x86.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysXProfileSDKCHECKED.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysXVisualDebuggerSDKCHECKED.lib	
)

TARGET_LINK_LIBRARIES(
	${arg_TargetName}
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/mojoshader/win32/Release/mojoshader.lib	
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/hlsl2glslfork/win32/Release/hlsl2glsl.lib	
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/GlslOptimizer/win32/Release/GLSL_Optimizer.lib	
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/FreeImage/win32/FreeImage.lib	
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/OpCode/Release/Opcode.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/libmpg123/Release/libmpg123.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/Ogg/Release/Ogg.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/Vorbis/Release/Vorbis.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/OpenAL/Release/OpenAL.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/RepX3.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PxTask.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3Common_x86.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3Extensions.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3Cooking_x86.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3_x86.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3Vehicle.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysX3CharacterKinematic_x86.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysXProfileSDK.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/PhysX3/Win32/PhysXVisualDebuggerSDK.lib	
)

########################file service temp code ########################################################################
TARGET_LINK_LIBRARIES(
	${arg_TargetName}
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/debug/libaxc.lib	
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/debug/libcurl.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/debug/libprotobuf.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/debug/libsqlite.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/debug/libwfs.lib
	debug ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/debug/libzmq.lib
)

TARGET_LINK_LIBRARIES(
	${arg_TargetName}
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/release/libaxc.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/release/libcurl.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/release/libprotobuf.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/release/libsqlite.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/release/libwfs.lib
	optimized ${GENESIS_EXTERNAL_LIBS_DIR}/fileservice/release/libzmq.lib
)
########################################## temp code ########################################################################

#ignore some libs
FOREACH( flag_var
		CMAKE_EXE_LINKER_FLAGS_DEBUG
		CMAKE_SHARED_LINKER_FLAGS_DEBUG
		CMAKE_EXE_LINKER_FLAGS_EDITOR_DEBUG
		CMAKE_SHARED_LINKER_FLAGS_EDITOR_DEBUG		
		CMAKE_EXE_LINKER_FLAGS_PROFILE_DEBUG
		CMAKE_SHARED_LINKER_FLAGS_PROFILE_DEBUG
)

IF( NOT ${flag_var} MATCHES "/NODEFAULTLIB:msvcrtd.lib" )
	#force update this on the CMAKE GUI
	SET( ${flag_var} "${${flag_var}} /NODEFAULTLIB:msvcrtd.lib" CACHE STRING "" FORCE)	
ENDIF( NOT ${flag_var} MATCHES "/NODEFAULTLIB:msvcrtd.lib" )

ENDFOREACH( flag_var )

#ignore some libs
FOREACH( flag_var
		CMAKE_EXE_LINKER_FLAGS_DEBUG
		CMAKE_SHARED_LINKER_FLAGS_DEBUG
		CMAKE_EXE_LINKER_FLAGS_EDITOR_DEBUG
		CMAKE_SHARED_LINKER_FLAGS_EDITOR_DEBUG	
		CMAKE_EXE_LINKER_FLAGS_PROFILE_DEBUG
		CMAKE_SHARED_LINKER_FLAGS_PROFILE_DEBUG	
)

IF( NOT ${flag_var} MATCHES "/NODEFAULTLIB:libcmt.lib" )
	#force update this on the CMAKE GUI
	SET( ${flag_var} "${${flag_var}} /NODEFAULTLIB:libcmt.lib" CACHE STRING "" FORCE)	
ENDIF( NOT ${flag_var} MATCHES "/NODEFAULTLIB:libcmt.lib" )

ENDFOREACH( flag_var )

#ignore some libs
FOREACH( flag_var
		CMAKE_EXE_LINKER_FLAGS_RELEASE
		CMAKE_SHARED_LINKER_FLAGS_RELEASE
		CMAKE_EXE_LINKER_FLAGS_EDITOR_RELEASE
		CMAKE_SHARED_LINKER_FLAGS_EDITOR_RELEASE	
		CMAKE_EXE_LINKER_FLAGS_PROFILE_RELEASE
		CMAKE_SHARED_LINKER_FLAGS_PROFILE_RELEASE
)

IF( NOT ${flag_var} MATCHES "/NODEFAULTLIB:msvcrt.lib" )
	#force update this on the CMAKE GUI
	SET( ${flag_var} "${${flag_var}} /NODEFAULTLIB:msvcrt.lib" CACHE STRING "" FORCE)	
ENDIF( NOT ${flag_var} MATCHES "/NODEFAULTLIB:msvcrt.lib" )

ENDFOREACH( flag_var )


ENDMACRO ( _MACRO_EXECUTABLE_BASE_LIB )

# folder
SET ( _HEADER_FILES 
	${CMAKE_SOURCE_DIR}/GenesisLoader/GenesisLoaderData.h
	${CMAKE_SOURCE_DIR}/GenesisLoader/GenesisLoaderInterface.h
	${CMAKE_SOURCE_DIR}/GenesisLoader/GenesisLoader.h
	simplegameapplication.h
	
)

# folder
SET ( _SOURCE_FILES
	#${CMAKE_SOURCE_DIR}/GenesisLoader/main.cc
	${CMAKE_SOURCE_DIR}/GenesisLoader/GenesisLoader.cc
	DemoMat_WinWebInterface.cc
	simplegameapplication.cc
	
)

#<-------- Additional Include Directories ------------------>
INCLUDE_DIRECTORIES(
	#TODO:Make this clear and simple
	${CMAKE_SOURCE_DIR}/foundation
	${CMAKE_SOURCE_DIR}/rendersystem
	${CMAKE_SOURCE_DIR}/players/Genesis
	${CMAKE_SOURCE_DIR}/extincludes	
	${CMAKE_SOURCE_DIR}/app
	${CMAKE_SOURCE_DIR}/tools/
	${CMAKE_SOURCE_DIR}/extincludes/freetype/include
	

	# should remove later
	${CMAKE_SOURCE_DIR}/graphicsystem
	${CMAKE_SOURCE_DIR}/addons/shadercompiler/win
	${CMAKE_SOURCE_DIR}/addons
	${CMAKE_SOURCE_DIR}/
	${CMAKE_SOURCE_DIR}/addons/myguiengine/include
)

ADD_EXECUTABLE( 
	Genesis 
	WIN32
	#head
	${_HEADER_FILES}
	#source
	${_SOURCE_FILES}
)

ADD_DEFINITIONS(
	-D_UNICODE
	-DUNICODE
)

#Set entry point 	
SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" CACHE STRING "" FORCE )

#generate debug information
FOREACH( flag_var
		CMAKE_EXE_LINKER_FLAGS
		CMAKE_EXE_LINKER_FLAGS_DEBUG
		CMAKE_EXE_LINKER_FLAGS_RELEASE
		CMAKE_EXE_LINKER_FLAGS_PROFILE_DEBUG
		CMAKE_EXE_LINKER_FLAGS_PROFILE_RELEASE
)

IF( NOT ${flag_var} MATCHES "/DEBUG" )	
	#force update this on the CMAKE GUI
	SET( ${flag_var} "${${flag_var}} /DEBUG" CACHE STRING "" FORCE)	
ENDIF( NOT ${flag_var} MATCHES "/DEBUG" )
ENDFOREACH( flag_var )

set_property(TARGET Genesis APPEND PROPERTY PROFILE_DEBUG_POSTFIX "_profile")
set_property(TARGET Genesis APPEND PROPERTY PROFILE_RELEASE_POSTFIX "_profile")

#Organize projects into folders
SET_PROPERTY(TARGET Genesis PROPERTY FOLDER "5.Players")

#set base link lib
_MACRO_EXECUTABLE_BASE_LIB( Genesis )
