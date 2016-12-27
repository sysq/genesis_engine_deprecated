/** \file: DrawModel.h
* \brief: Draw Model by code
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/25   1.0    SunHao   
*/
#ifndef __DRAWMODEL_H__
#define __DRAWMODEL_H__

#include "appframework/app_fwd_decl.h"
#include "util/string.h"
#include "math/float3.h"
#include "resource/resource_fwd_decl.h"

namespace Demo 
{
	App::TActorPtr DrawABunker( const Util::String& sActorName="Bunker" );
	App::TActorPtr DrawATree( const Util::String& sActorName="Tree" );
	App::TActorPtr DrawABmw7( const Util::String& sActorName="BMW7Series" );
	App::TActorPtr DrawACube( const Util::String& sAcotrName="Cube" );
	App::TActorPtr DrawAVarietyShop( const Util::String& sActorName="VarietyShop" );
	App::TActorPtr DrawHouse1( const Util::String& sActorName="House1" );
	App::TActorPtr DrawHouse2( const Util::String& sActorName="House2" );
	App::TActorPtr DrawTree01( const Util::String& sActorName="Tree_01" );
	App::TActorPtr DrawMengGuBao01( const Util::String& sActorName="MengGuBao01" );
	App::TActorPtr DrawStreetLight01( const Util::String& sActorName="StreetLight01" );
	App::TActorPtr DrawCJ_chitang001( const Util::String& sActorName="CJ_chitang001" );
	App::TActorPtr DrawCJ_diji2( const Util::String& sActorName="CJ_diji2" );
	App::TActorPtr DrawCJ_diji4( const Util::String& sActorName="CJ_diji4" );
	App::TActorPtr DrawCJ_juanxincai002( const Util::String& sActorName="CJ_juanxincai002" );
	App::TActorPtr DrawCJ_jiulou001( const Util::String& sActorName="CJ_jiulou001" );
	App::TActorPtr DrawCJ_jzz2( const Util::String& sActorName="CJ_jzz2" );
	App::TActorPtr DrawCJ_jzz4( const Util::String& sActorName="CJ_jzz4" );
	App::TActorPtr DrawCJ_matou001( const Util::String& sActorName="CJ_matou001" );
	App::TActorPtr DrawCJ_daozi002( const Util::String& sActorName="CJ_daozi002" );
	App::TActorPtr DrawCJ_huanggua001( const Util::String& sActorName="CJ_huanggua001" );
	App::TActorPtr DrawCJ_huanggua002( const Util::String& sActorName="CJ_huanggua002" );
	App::TActorPtr DrawCJ_kuwei001( const Util::String& sActorName="CJ_kuwei001" );
	App::TActorPtr DrawCJ_tiandi( const Util::String& sActorName="CJ_tiandi" );
	App::TActorPtr DrawCJ_nangua( const Util::String& sActorName="CJ_nangua" );
	App::TActorPtr DrawCJ_minju001( const Util::String& sActorName="CJ_minju001" );
	App::TActorPtr DrawCJ_mxzgh001( const Util::String& sActorName="CJ_mxzgh001" );
	App::TActorPtr DrawCJ_simiao002( const Util::String& sActorName="CJ_simiao002" );
	App::TActorPtr DrawCJ_zhuzi( const Util::String& sActorName="CJ_zhuzi" );
	//App::TActorPtr ( const Util::String& sActorName="" );
}

#endif // __DRAWMODEL_H__
