#---------------------------------------------------------------------------------
# Copyright (c) 2011,WebJet Enterprise Department.ChangYou
# All rights reserved.
# Date        Ver    Who          Comment
# 2011-11-10   1.0    SunHao   
#---------------------------------------------------------------------------------

##################################################################################
# Build AnimationSample 
##################################################################################

# folder
SET ( _HEADER_FILES 
	simplegameapplication.h
	simplegamestatehandler.h
)

# folder
SET ( _SOURCE_FILES
	simplegame.cpp
	simplegameapplication.cpp
	simplegamestatehandler.cpp
)

#<-------- Additional Include Directories ------------------>
INCLUDE_DIRECTORIES(
	#TODO:Make this clear and simple
	${CMAKE_SOURCE_DIR}/foundation
	${CMAKE_SOURCE_DIR}/rendersystem
	${CMAKE_SOURCE_DIR}/extlibs
	${CMAKE_SOURCE_DIR}/extlibs/physX/Cooking/include
	${CMAKE_SOURCE_DIR}/extlibs/physX/Foundation/include
	${CMAKE_SOURCE_DIR}/extlibs/physX/Physics/include
	${CMAKE_SOURCE_DIR}/extlibs/physX/PhysXLoader/include	
	${CMAKE_SOURCE_DIR}/app	

	# should remove later
	${CMAKE_SOURCE_DIR}/graphicsystem
	${CMAKE_SOURCE_DIR}/addons/shadercompiler/win
	${CMAKE_SOURCE_DIR}/addons
	${CMAKE_SOURCE_DIR}/
)

ADD_EXECUTABLE( 
	PhyXSample
	WIN32
	#head
	${_HEADER_FILES}
	#source
	${_SOURCE_FILES}
)

#Organize projects into folders
SET_PROPERTY(TARGET PhyXSample PROPERTY FOLDER "5.Sample")


#Base lib
_MACRO_EXECUTABLE_BASE_LIB( PhyXSample )
