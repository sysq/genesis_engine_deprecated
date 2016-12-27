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

# folder
SET ( _HEADER_FILES 
	simplegameapplication.h
	
)

# folder
SET ( _SOURCE_FILES
	AndroidJNILoader.cc
	simplegameapplication.cc
	
)


#<-------- Additional Include Directories ------------------>
INCLUDE_DIRECTORIES(
	#TODO:Make this clear and simple
	${CMAKE_SOURCE_DIR}/foundation
	${CMAKE_SOURCE_DIR}/rendersystem
	${CMAKE_SOURCE_DIR}/players/Dash
	${CMAKE_SOURCE_DIR}/extlibs	
	${CMAKE_SOURCE_DIR}/app
	${CMAKE_SOURCE_DIR}/tools/
	${CMAKE_SOURCE_DIR}/extlibs/freetype/include
	${CMAKE_SOURCE_DIR}/extlibs/ShaderUtil
	${CMAKE_SOURCE_DIR}/extlibs/ShaderUtil/Cg/
	${CMAKE_SOURCE_DIR}/extlibs/ShaderUtil/mojoshader
	${CMAKE_SOURCE_DIR}/extlibs/ShaderUtil/hlsl2glslfork
	${CMAKE_SOURCE_DIR}/extlibs/ShaderUtil/GlslOptimizer
	${CMAKE_SOURCE_DIR}/extincludes

	# should remove later
	${CMAKE_SOURCE_DIR}/graphicsystem
	${CMAKE_SOURCE_DIR}/addons
	${CMAKE_SOURCE_DIR}/
	${CMAKE_SOURCE_DIR}/addons/myguiengine/include
)

#SET(CMAKE_BUILD_TYPE Debug)
SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -landroid -lGLESv2 -lEGL -llog -lc -lm -lOpenSLES" )
SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-soname,libGenesisA -Wl,--no-undefined  " )
SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -shared -Wl,-z,noexecstack " )
SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lgnustl_static -lgcc" )

ADD_LIBRARY( 
	GenesisA 
	SHARED
	#head
	${_HEADER_FILES}
	#source
	${_SOURCE_FILES}
)


TARGET_LINK_LIBRARIES(
#target
	GenesisA
#libs	
	App
	Sprite
	GraphicSystem
	MaterialMaker
	MaterialMaker
	GraphicSystem
	Animation
	GUIBase
	MyGUIEngine
	MyGUIPlatform
	Paticles
	Serialization
	TerrainSystem
	Vegetation
	Vis
	Input
	ShaderCompiler
	Sprite
	Resource
	Resource
	Sprite
	SoundSystem
	RenderSystem
	ProfileSystem
	PackageTool
	TinyXML
	Foundation
	FreeType
	ZLib
)


IF ( ANDROID_OPTIMIZED )
	TARGET_LINK_LIBRARIES(
		GenesisA
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Release/FreeImageLib.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Release/OpenEXR.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Release/LibTIFF4.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Release/LibRaw.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Release/LibOpenJPEG.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Release/LibJPEG.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Release/ZLib.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Release/LibPNG.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/OpenAL/Android/OpenAL32.a
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/mojoshader/android/Release/mojoshader.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/GlslOptimizer/android/Release/GLSL_Optimizer.a
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/hlslang/android/Release/hlslang.a
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Release/FreeImageLib.a	
		${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/Opcode/Android_Debug/Opcode.a

		)
ELSE( ANDROID_OPTIMIZED )
TARGET_LINK_LIBRARIES(
	GenesisA
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Debug/FreeImageLib.a	
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Debug/OpenEXR.a
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Debug/LibTIFF4.a	
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Debug/LibRaw.a	
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Debug/LibOpenJPEG.a	
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Debug/LibJPEG.a	
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Debug/ZLib.a	
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Debug/LibPNG.a	
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/OpenAL/Android/OpenAL32.a
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/mojoshader/android/Debug/mojoshader.a	
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/GlslOptimizer/android/Debug/GLSL_Optimizer.a
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/hlslang/android/Debug/hlslang.a
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/FreeImage/android/Debug/FreeImageLib.a	
	${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/Opcode/Android_Release/Opcode.a
	
)
ENDIF ( ANDROID_OPTIMIZED )

IF(${LINK_ANDRIOD_PHYSX})
			TARGET_LINK_LIBRARIES(
				GenesisA
				${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/PhysX3/Android/libPhysX.a
			)
ENDIF()


IF ( ANDROID_PROFILE )
TARGET_LINK_LIBRARIES(
	GenesisA
  ${CMAKE_SOURCE_DIR}/../../genesis_externals/Librarys/android-ndk-profiler/armeabi/libandroid-ndk-profiler.a
)
ENDIF ( ANDROID_PROFILE )

TARGET_LINK_LIBRARIES( GenesisA ${ANDROID_NDK_ROOT}/sources/cxx-stl/gnu-libstdc++/4.4.3/libs/armeabi/libgnustl_static.a )


ADD_DEFINITIONS(
	-D_UNICODE
	-DUNICODE
)

#Organize projects into folders
SET_PROPERTY(TARGET GenesisA PROPERTY FOLDER "5.Players")

#copy the target
GET_TARGET_PROPERTY( param_TargetFullPath GenesisA LOCATION )
ADD_CUSTOM_COMMAND(
	TARGET GenesisA
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${param_TargetFullPath} ${CMAKE_SOURCE_DIR}/Android/GenesisLoader/Genesis/obj/local/armeabi/libGenesisA.so )

ADD_CUSTOM_COMMAND(
	TARGET GenesisA
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${param_TargetFullPath} ${CMAKE_SOURCE_DIR}/Android/GenesisLoader/Genesis/libs/armeabi/libGenesisA.so )
#copy to editorasset
ADD_CUSTOM_COMMAND(
	TARGET GenesisA
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${param_TargetFullPath} ${CMAKE_SOURCE_DIR}/../../genesis_editor/bin/win32/editorasset/Build/Loader/proj.android/libs/armeabi/libGenesisA.so )

ADD_CUSTOM_COMMAND(
	TARGET GenesisA
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${param_TargetFullPath} ${CMAKE_SOURCE_DIR}/../../genesis_editor/bin/win32/editorasset/Build/Loader/proj.android/obj/local/armeabi/libGenesisA.so )
