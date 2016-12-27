#---------------------------------------------------------------------------------
# Copyright (c) 2011,WebJet Enterprise Department.ChangYou
# All rights reserved.
# Date        Ver    Who          Comment
# 2011-11-10   1.0    SunHao   
#---------------------------------------------------------------------------------

##################################################################################
# Build Sky
##################################################################################

# folder
SET ( _HEADER_FILES 
	Atmosphere.h
	Clouds.h
	ColorGradient.h
	MeshManager.h
	SkyRender.h
	SkySphere.h
	Moon.h
	SkyBox.h

)

# folder
SET ( _SOURCE_FILES
	Atmosphere.cc
	Clouds.cc
	ColorGradient.cc
	MeshManager.cc
	SkyRender.cc
	SkySphere.cc
	Moon.cc
	SkyBox.cc
)

#<-------- Additional Include Directories ------------------>
INCLUDE_DIRECTORIES(
	#TODO:Make this clear and simple
	${CMAKE_SOURCE_DIR}/foundation
	${CMAKE_SOURCE_DIR}/rendersystem
	${CMAKE_SOURCE_DIR}/extlibs	
	${CMAKE_SOURCE_DIR}/extlibs/boostWraper

	# should remove later
	${CMAKE_SOURCE_DIR}/graphicsystem
	${CMAKE_SOURCE_DIR}/addons
	${CMAKE_SOURCE_DIR}/
)

ADD_LIBRARY(
	Sky 
	STATIC 
	#header
	${_HEADER_FILES}	
	#SOURCE
	${_SOURCE_FILES}
 )

#Organize projects into folders
SET_PROPERTY(TARGET Sky PROPERTY FOLDER "3.Addons")

#Compile properties
_MACRO_TREAT_WARRINGS_AS_ERRORS( Sky )

