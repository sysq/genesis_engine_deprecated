#---------------------------------------------------------------------------------
# Copyright (c) 2011,WebJet Enterprise Department.ChangYou
# All rights reserved.
# Date        Ver    Who          Comment
# 2011-11-14   1.0    SunHao   
#---------------------------------------------------------------------------------

##################################################################################
# Build Paticles 
##################################################################################

#blur folder
SET ( BLUR_HEADER_FILES 
	blur/PostEffectBlurRenderer.h
)

#blur folder
SET ( BLUR_SOURCE_FILES
	blur/PostEffectBlurRenderer.cc
)

#simpleHDR folder
SET ( SIMPLEHDR_HEADER_FILES 
	simpleHDR/PostEffectSimpleHDRRenderer.h
)

#simpleHDR folder
SET ( SIMPLEHDR_SOURCE_FILES
	simpleHDR/PostEffectSimpleHDRRenderer.cc
)

#twirl folder
SET ( TWIRL_HEADER_FILES 
	twirl/PostEffectTwirlRenderer.h
)

#twirl folder
SET ( TWIRL_SOURCE_FILES
	twirl/PostEffectTwirlRenderer.cc
)

#showerdoor folder
SET ( SHOWERDOOR_HEADER_FILES 
	ShowerDoor/PostEffectShowerDoorRenderer.h
)

#showerdoor folder
SET ( SHOWERDOOR_SOURCE_FILES
	ShowerDoor/PostEffectShowerDoorRenderer.cc
)

#oldTV folder
SET ( OLDTV_HEADER_FILES 
	OldTV/PostEffectOldTVRenderer.h
)

#oldTV folder
SET ( OLDTV_SOURCE_FILES
	OldTV/PostEffectOldTVRenderer.cc
)

#goldenShine folder
SET ( GOLDENSHINE_HEADER_FILES 
	goldenShine/PostEffectGoldenShineRenderer.h
)

#goldenShine folder
SET ( GOLDENSHINE_SOURCE_FILES
	goldenShine/PostEffectGoldenShineRenderer.cc
)



#<-------- Source Group ------------------>
SOURCE_GROUP( 
	blur
	FILES 
	${BLUR_HEADER_FILES}
	${BLUR_SOURCE_FILES}	
)

#<-------- Source Group ------------------>
SOURCE_GROUP( 
	simpleHDR
	FILES 
	${SIMPLEHDR_HEADER_FILES}
	${SIMPLEHDR_SOURCE_FILES}	
)

#<-------- Source Group ------------------>
SOURCE_GROUP( 
	twirl
	FILES 
	${TWIRL_HEADER_FILES}
	${TWIRL_SOURCE_FILES}
)

#<-------- Source Group ------------------>
SOURCE_GROUP( 
	showerdoor
	FILES 
	${SHOWERDOOR_HEADER_FILES}
	${SHOWERDOOR_SOURCE_FILES}
)

#<-------- Source Group ------------------>
SOURCE_GROUP( 
	oldTV
	FILES 
	${OLDTV_HEADER_FILES}
	${OLDTV_SOURCE_FILES}
)

#<-------- Source Group ------------------>
SOURCE_GROUP( 
	goldenShine
	FILES 
	${GOLDENSHINE_HEADER_FILES}
	${GOLDENSHINE_SOURCE_FILES}
)


#<-------- Additional Include Directories ------------------>
INCLUDE_DIRECTORIES(
	#TODO:Make this clear and simple
	${CMAKE_SOURCE_DIR}/foundation
	${CMAKE_SOURCE_DIR}/rendersystem
	${CMAKE_SOURCE_DIR}/graphicsystem
	${CMAKE_SOURCE_DIR}/app
	${CMAKE_SOURCE_DIR}/extlibs	

	# should remove later
	${CMAKE_SOURCE_DIR}/addons/
	${CMAKE_SOURCE_DIR}/
)

ADD_LIBRARY(
	PostEffects
	STATIC 
	#header
	${BLUR_HEADER_FILES}
	${SIMPLEHDR_HEADER_FILES}
	${TWIRL_HEADER_FILES}
	${SHOWERDOOR_HEADER_FILES}
	${OLDTV_HEADER_FILES}
	${GOLDENSHINE_HEADER_FILES}
	#source
	${BLUR_SOURCE_FILES}
	${SIMPLEHDR_SOURCE_FILES}
	${TWIRL_SOURCE_FILES}
	${SHOWERDOOR_SOURCE_FILES}
	${OLDTV_SOURCE_FILES}
	${GOLDENSHINE_SOURCE_FILES}
 )

#Organize projects into folders
SET_PROPERTY(TARGET PostEffects PROPERTY FOLDER "3.Addons")

#Compile properties
_MACRO_TREAT_WARRINGS_AS_ERRORS( PostEffects )
