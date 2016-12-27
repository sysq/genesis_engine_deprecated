/** \file defaultinputprocss.h
* \brief 文件内容说明
* Copyright (c) 2011,畅游天下WebJet事业部
* All rights reserved.
* Date        Ver    Who          Comment
* 2011:9:16   1.0    王兴博   
*/
#ifndef __gyrostinputprocss_H__
#define __gyrostinputprocss_H__

#include "app/appframework/manager.h"
#include "app/apputil/mayacamerautil.h"


namespace App
{
	/*
	系统提供的对用户输入的默认处理：根据输入消息操纵主相机。
	如果需要该处理，将该GyrostProcessInput(Manager）加入到InputFeature中。否则，可以使用自己的输入处理类
	*/
	class GyrostProcessInput: public App::Manager
	{
		__DeclareSubClass(GyrostProcessInput, App::Manager );
	public:
		GyrostProcessInput();
		~GyrostProcessInput();

		/// @App::Manager::OnActivate called when attached to game server
		virtual void OnActivate();

		/// @App::Manager::OnBeginFrame called before frame by the game server
		virtual void OnBeginFrame();
	public:
		AppUtil::MayaCameraUtil m_mayaCameraUtil;
	};



}







#endif // __gyrostinputprocss_H__
