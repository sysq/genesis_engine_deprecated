#include "stdneb.h"

#include "simplegameapplication.h"
#include "system/appentry.h"

ImplementNebulaApplication()

using namespace Sample;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const Util::CommandLineArgs& args)
{

	SimpleGameApplication app;
	app.SetCompanyName("Radon Labs GmbH");
	app.SetAppTitle("Test Game");
	app.SetCmdLineArgs(args);
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
}