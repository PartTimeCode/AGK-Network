#include "AGK_Common.h"
#include "Client_Main.h"
#include "AGK_Client.hpp"
#include "Client_Login.hpp"

// Namespace
using namespace AGK;


// Game states
constexpr int GAME_STATE_LOGIN = 1;
constexpr int GAME_STATE_PLAY = 2;

// Globals 
app App;
cAGKClient client;


void app::Begin(void* hWnd)
{
	client.StartTCP("PTC", "127.0.0.1", 60500);
	client.StartUDP("127.0.0.1", 60000);


	agk::SetVirtualResolution(1024, 768);
	agk::SetClearColor(151, 170, 204); // light blue
	agk::SetSyncRate(60, 0);
	agk::SetScissor(0, 0, 0, 0);
}

int app::Loop(void)
{
	agk::PrintC("AGK Client: FPS: ");
	agk::Print(agk::ScreenFPS());


	client.UpdateUDP();
	client.UpdateTCP();

	// client related UI code here

	// game code here (how to link to server messages!!??!!)

	// For now just print data to screen
	for (auto log : client.m_OutputLog)
		agk::Print(log.c_str());

	agk::Print(agk::ScreenFPS());
	agk::Sync();

	return 0; // return 1 to close app
}


void app::End(void)
{
	// clean up
	client.StopTCP();
	client.StopUDP();
}