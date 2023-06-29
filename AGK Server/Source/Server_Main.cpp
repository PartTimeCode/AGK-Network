// Includes
#include "AGK_Common.h"
#include "Server_Main.h"
#include "AGK_Server.hpp"

// Namespace
using namespace AGK;

app App;



cAGKServer server;

void app::Begin(void* hWnd)
{

	server.StartUDP("127.0.0.1", 60000);
	server.StartTCP("127.0.0.1", 60500);


	agk::SetWindowAllowResize(0);
	agk::SetVirtualResolution(800, 600);
	agk::SetClearColor(0, 0, 0);
	agk::SetSyncRate(120, 0);
	agk::SetScissor(0, 0, 0, 0);
	agk::SetPrintSize(14);
}

int app::Loop(void)
{
	agk::PrintC("AGK Server: FPS: ");
	agk::Print(agk::ScreenFPS());

	server.UpdateUDP();
	server.UpdateTCP();

	// Server related UI code here



	// For now just print data to screen
	for (auto log : server.m_OutputLog)
		agk::Print(log.c_str());


	agk::Sync();
	return 0; // return 1 to close app
}


void app::End(void)
{
	// Call "Stop" functions for later clean up code 
	// (notify connected clients etc etc)
	server.StopUDP();
	server.StopTCP();
}
