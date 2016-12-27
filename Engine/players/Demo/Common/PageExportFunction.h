/** \file: PageExportFunction.h
* \brief: 
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/11/21   1.0    SunHao   
*/

#ifndef __PAGEEXPORTFUNCTION_H__
#define __PAGEEXPORTFUNCTION_H__

namespace Demo
{
	void Toggle( int iNum );	
	char* GetCarmerPos( void );
	void LoadLogic(const char* pFilePath="logic:preBuild.glogic" );
}
#endif // __PAGEEXPORTFUNCTION_H__