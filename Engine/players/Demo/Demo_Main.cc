/** \file demo.cc
* \brief: make demo for the boss
* Copyright (c) 2011,畅游天下WebJet事业部
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/13   1.0    SunHao   
*/


#include "stdneb.h"

#include "system/appentry.h"
#include "Demo_GameApplication.h"

ImplementNebulaApplication()

/**
Demo 主要是用于方便的打开某个场景。 命令行参数如下
ProjectPath  指明要打开的路径  必须有
Scene        指明要打开的场景  如果没有，默认创建一个空场景

实例；
打开show工程的show.scene. 传入的命令行参数如下 
ProjectPath="E:\WebJet_CodeReview\trunk\test_project\ProjRes\show" Scene="asset:scene/show.scene"
E:\WebJet_CodeReview\trunk\test_project\ 替换成本地的工程路径
*/

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const Util::CommandLineArgs& args)
{
	Demo::DemoGameApplication app;
	app.SetCompanyName("Changyou");
	app.SetAppTitle("Demo");
	app.SetCmdLineArgs(args);

	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}
