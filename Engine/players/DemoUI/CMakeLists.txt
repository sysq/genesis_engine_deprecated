#---------------------------------------------------------------------------------
# Copyright (c) 2011,WebJet Enterprise Department.ChangYou
# All rights reserved.
# Date        Ver    Who          Comment
# 2011-11-23   1.0    libin   
#---------------------------------------------------------------------------------

##################################################################################
# Build GyrostatSample 
##################################################################################

# folder
SET ( _HEADER_FILES 
	gyrostat.h
	inputprocss.h
	simplegamestatehandler.h
	simplegameapplication.h
	
)

# folder
SET ( _SOURCE_FILES
	gyrostat.cc
	inputprocss.cc
	simplegamestatehandler.cc
	simplegameapplication.cc
	simplegame.cc
	
)

#<-------- Additional Include Directories ------------------>
INCLUDE_DIRECTORIES(
	#TODO:Make this clear and simple
	${CMAKE_SOURCE_DIR}/foundation
	${CMAKE_SOURCE_DIR}/rendersystem
	${CMAKE_SOURCE_DIR}/samples/DemoUI
	${CMAKE_SOURCE_DIR}/extlibs	
	${CMAKE_SOURCE_DIR}/extlibs/freetype/include
	
	${CMAKE_SOURCE_DIR}/app	

	# should remove later
	${CMAKE_SOURCE_DIR}/graphicsystem
	${CMAKE_SOURCE_DIR}/addons/shadercompiler/win
	${CMAKE_SOURCE_DIR}/addons
	${CMAKE_SOURCE_DIR}/
	${CMAKE_SOURCE_DIR}/addons/myguiengine/include
	
	
)

ADD_EXECUTABLE( 
	DemoUI 
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
#Organize projects into folders
SET_PROPERTY(TARGET DemoUI PROPERTY FOLDER "5.Sample")

_MACRO_COPY_T0_BINARY_DIR_AFTER_BUILD( DemoUI .exe )

_MACRO_EXECUTABLE_BASE_LIB( DemoUI)

