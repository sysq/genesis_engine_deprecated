/** \file: DemoFwdDecl.h
* \brief: Demo forward declare 
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/24   1.0    SunHao   
*/
#ifndef __DEMOFWDDECL_H__
#define __DEMOFWDDECL_H__

#include "core/ptr.h"

namespace Demo
{
	// pointer define
	/*
	etc 
	typedef Ptr<StateHandler> TStateHandlerPtr
	T + ClassName + Ptr

	typedef Ptr<const StateHandler> TStateHandlerConstPtr
	T + ClassName + ConstPtr
	*/

	// forward declare 
	class Estates;

	// typedef
	typedef Ptr<Estates> TEstatesPtr; 
	typedef int32 TEstateType;

	// const
	const int   GROUND_LAYER_ID	  = 0x02;
	const int   BUILDING_LAYER_ID = 0x04;	
	const int   NO_LOGIC_LAYER_ID = 0x00;

	const int   INVALID_ACTOR_FASTID = 0;

	// logic serialization
	const Util::String LOGIC_SERIALIZATION_SCENE_NAME		= "SceneName";
	const Util::String LOGIC_SERIALIZATION_BUILDING_COUNT	= "BuildingCount";
	const Util::String LOGIC_SERIALIZATION_GRID_COUNT		= "GridCount";
	const Util::String LOGIC_SERIALIZATION_BUILDING_TYPE	= "BuildingType";
	const Util::String LOGIC_SERIALIZATION_BUILDING_POS		= "BuildingPos";
	const Util::String LOGIC_SERIALIZATION_BUILDING_RADIAN	= "BuildingRadian";
}

#endif // __DEMOFWDDECL_H__
