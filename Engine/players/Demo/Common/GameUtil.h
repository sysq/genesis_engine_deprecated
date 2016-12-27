/** \file: GameUtil.h
* \brief: utility functions define here!
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/12/06   1.0    SunHao   
*/

#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__

#include "resource/meshres.h"
#include "appframework/app_fwd_decl.h"

namespace Demo
{
	App::TActorPtr DrawLines( Resources::PositionData posData );	
	Math::float3 CalcSplitZ(const Math::float3& one, const Math::float3& other, int iSplitCount, int iCount);
	Math::float3 CalcSplitX(const Math::float3& one, const Math::float3& other, int iSplitCount, int iCount);

}

#endif // __GAMEUTIL_H__