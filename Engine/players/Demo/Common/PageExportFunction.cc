/** \file: PageExportFunction.cc
* \brief: 
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/11/21   1.0    SunHao   
*/
#include "stdneb.h"
#include "PageExportFunction.h"
#include "GameLogic.h"
#include "appframework/app_fwd_decl.h"
#include "app/graphicfeature/graphicsfeature.h"

namespace Demo
{
	void Toggle( int iNum )
	{
		GameLogic::Instance()->PrepareToBuild( iNum );
	}

	char* GetCarmerPos( void )
	{
		static char s_StrPos[64];
		App::TActorPtr pCamera = App::GraphicsFeature::Instance()->GetDefaultCameraActor();
		Math::vector pos = pCamera->GetPosition();

		memset( s_StrPos, 0, 64 );
		sprintf_s( s_StrPos, "%f %f %f", pos.x(), pos.y(), pos.z() );
		return s_StrPos;
	}

	void LoadLogic(const char* pFilePath/*="logic:preBuild.glogic"*/ )
	{
		Util::String sFilePath( pFilePath );
		GameLogic::Instance()->LoadLogic( sFilePath );

	}

}