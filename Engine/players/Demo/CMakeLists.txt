#---------------------------------------------------------------------------------
# Copyright (c) 2011,WebJet Enterprise Department.ChangYou
# All rights reserved.
# Date        Ver    Who          Comment
# 2011-11-10   1.0    SunHao
# 2011-11-16	1.1		SunHao	   
#---------------------------------------------------------------------------------

##################################################################################
# Build Demo 
##################################################################################

# folder
SET ( COMMON_HEADER_FILES 
	Common/InputSystem.h
	Common/Model.h
	Common/DemoGameStateHandler.h
	Common/DemoFwdDecl.h
	Common/GameLogic.h
	Common/PageExportFunction.h
	Common/GameData.h
	Common/GameUtil.h
	Common/GameBuilding.h
)

SET ( COMMON_SOURCE_FILES
	Common/InputSystem.cc
	Common/Model.cc
	Common/DemoGameStateHandler.cc
	Common/GameLogic.cc
	Common/PageExportFunction.cc
	Common/GameData.cc
	Common/GameUtil.cc
	Common/GameBuilding.cc
)

SOURCE_GROUP(
	common
	FILES
	${COMMON_HEADER_FILES}
	${COMMON_SOURCE_FILES}
)

#<-------- Demo ------------------>
SET ( DEMO_HEADER_FILES 
	Demo_GameApplication.h
	simplegamestatehandler.h
)

# folder
SET ( DEMO_SOURCE_FILES
	Demo_GameApplication.cc
	Demo_Main.cc
	simplegamestatehandler.cc
)

#<-------- Additional Include Directories ------------------>
INCLUDE_DIRECTORIES(
	#TODO:Make this clear and simple
	${CMAKE_SOURCE_DIR}/foundation
	${CMAKE_SOURCE_DIR}/rendersystem
	${CMAKE_SOURCE_DIR}/extlibs	
	${CMAKE_SOURCE_DIR}/app
	${CMAKE_SOURCE_DIR}/samples/Demo/
	${CMAKE_SOURCE_DIR}/samples/Demo/Common


	# should remove later
	${CMAKE_SOURCE_DIR}/graphicsystem
	${CMAKE_SOURCE_DIR}/addons/shadercompiler/win
	${CMAKE_SOURCE_DIR}/addons
	${CMAKE_SOURCE_DIR}/
)

ADD_EXECUTABLE( 
	Demo
	WIN32
	#head
	${COMMON_HEADER_FILES}
	${DEMO_HEADER_FILES}
	#source
	${DEMO_SOURCE_FILES}
	${COMMON_SOURCE_FILES}
)

_MACRO_WEBJET_PROJECT_COMPILE_PROPERTIES( Demo )

#Organize projects into folders
SET_PROPERTY(TARGET Demo PROPERTY FOLDER "5.Sample")

_MACRO_EXECUTABLE_BASE_LIB( Demo )

TARGET_LINK_LIBRARIES(
	Demo
)

#<-------- DemoWebPublish ------------------>

SET ( WEBPLUGIN_REF
 ${CMAKE_SOURCE_DIR}/tools/WebJetPlugin/EngineRef
)


SET ( WEBPLUGIN_PACKETS
 ${WEBPLUGIN_REF}/Packets
)

SET ( DEMOPUBLISH_HEADER_FILES 	
	DemoPublish_GameApplication.h
	${WEBPLUGIN_REF}/WinWebInterface.h
	${WEBPLUGIN_REF}/WebJetWebData.h
)

# folder
SET ( DEMOPUBLISH_SOURCE_FILES
	DemoPublish_WinWebInterface.cc
	DemoPublish_GameApplication.cc			
)

SET ( PACKET_HEADER_FILES 
	${WEBPLUGIN_PACKETS}/WJCallUserFuncReturnPacket.h
	${WEBPLUGIN_PACKETS}/WJCallUserFuncSendPacket.h
	${WEBPLUGIN_PACKETS}/WJPacket.h
	${WEBPLUGIN_PACKETS}/WJPacketDefine.h
	${WEBPLUGIN_PACKETS}/WJPacketFactory.h
	${WEBPLUGIN_PACKETS}/WJPacketFactoryManager.h
	${WEBPLUGIN_PACKETS}/WJPacketRecevier.h
	${WEBPLUGIN_PACKETS}/WJPluginPacketFactoryManager.h
	${WEBPLUGIN_PACKETS}/Pipe.h
	${WEBPLUGIN_PACKETS}/ClientPipe.h
	${WEBPLUGIN_PACKETS}/WJCallFuncProxy.h
	${WEBPLUGIN_PACKETS}/WJFuncParameter.h
	${WEBPLUGIN_PACKETS}/LogFile.h
)

SET ( PACKET_SOURCE_FILES
	${WEBPLUGIN_PACKETS}/WJCallUserFuncReturnPacket.cc
	${WEBPLUGIN_PACKETS}/WJCallUserFuncSendPacket.cc
	${WEBPLUGIN_PACKETS}/WJPacketFactoryManager.cc
	${WEBPLUGIN_PACKETS}/WJPacketRecevier.cc
	${WEBPLUGIN_PACKETS}/WJPluginPacketFactoryManager.cc
	${WEBPLUGIN_PACKETS}/Pipe.cc
	${WEBPLUGIN_PACKETS}/ClientPipe.cc
	${WEBPLUGIN_PACKETS}/WJCallFuncProxy.cc
	${WEBPLUGIN_PACKETS}/WJFuncParameter.cc
	${WEBPLUGIN_PACKETS}/WJPacket.cc
	${WEBPLUGIN_PACKETS}/LogFile.cc
)

SOURCE_GROUP(
	Packets
	FILES	
	${PACKET_HEADER_FILES}
	${PACKET_SOURCE_FILES}
)

#<-------- Additional Include Directories ------------------>
INCLUDE_DIRECTORIES(
	#TODO:Make this clear and simple
	${CMAKE_SOURCE_DIR}/foundation
	${CMAKE_SOURCE_DIR}/rendersystem
	${CMAKE_SOURCE_DIR}/extlibs	
	${CMAKE_SOURCE_DIR}/app
	${CMAKE_SOURCE_DIR}/samples/Demo/
	${CMAKE_SOURCE_DIR}/samples/Demo/Common
	${WEBPLUGIN_REF}
	${WEBPLUGIN_PACKETS}
	

	# should remove later
	${CMAKE_SOURCE_DIR}/graphicsystem
	${CMAKE_SOURCE_DIR}/addons/shadercompiler/win
	${CMAKE_SOURCE_DIR}/addons
	${CMAKE_SOURCE_DIR}/
)

ADD_LIBRARY( 
	DemoWebPublish
	SHARED
	#head
	${DEMOPUBLISH_HEADER_FILES}
	${COMMON_HEADER_FILES}
	${PACKET_HEADER_FILES}
	#source
	${COMMON_SOURCE_FILES}
	${DEMOPUBLISH_SOURCE_FILES}
	${PACKET_SOURCE_FILES}
)

_MACRO_WEBJET_PROJECT_COMPILE_PROPERTIES( DemoWebPublish )

#Organize projects into folders
SET_PROPERTY(TARGET DemoWebPublish PROPERTY FOLDER "5.Sample")

#Compile properties
_MACRO_EXECUTABLE_BASE_LIB( DemoWebPublish )

