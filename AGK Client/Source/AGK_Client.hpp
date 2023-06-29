// Includes
#include "Client_Main.h"
#include "AGK_Common.h"

// Namespace
using namespace AGK;

class cAGKClient
{
public:

	//############################
	// UDP Server
	bool StartUDP(const char* host, int port)
	{
		m_iUDP_ID = agk::CreateUDPListener(host, port);
		if (m_iUDP_ID)
		{
			m_OutputLog.push_back("UDP Listener Started");
			return true;
		}
		else
		{
			m_OutputLog.push_back("Error Creating UDP Listener");
			return false;
		}

		//UINT iMessageID = agk::CreateNetworkMessage();
		//agk::AddNetworkMessageInteger(iMessageID, 999);
		//agk::SendUDPNetworkMessage(m_iNetworkID, iMessageID, "127.0.0.1", 60000);
	}

	void StopUDP()
	{
		if (m_iUDP_ID)
		{
			agk::DeleteUDPListener(m_iUDP_ID);
			m_OutputLog.push_back("UDP Listener Stopped");
			m_iUDP_ID = 0;
			// TODO!
			// notify clients etc etc
		}
	}

	void UpdateUDP()
	{

	}

	//############################
	// TCP Server
	bool StartTCP(const char* name, const char* host, int port)
	{
		m_iTCP_ID = agk::JoinNetwork(host, port, name);
		if (agk::IsNetworkActive(m_iTCP_ID))
		{
			m_OutputLog.push_back("TCP Server Started");

			// Send connection handshake to server
			static UINT iMessageID = agk::CreateNetworkMessage();
			agk::AddNetworkMessageInteger(iMessageID, CONNECT_TO_SERVER);
			agk::SendNetworkMessage(m_iTCP_ID, 1, iMessageID);
			return true;
		}
		else
		{
			m_OutputLog.push_back("Error Creating TCP Server");
			return false;
		}
	}

	void StopTCP()
	{

	}

	void UpdateTCP()
	{

	}


	std::vector<std::string> m_OutputLog;

private:

	UINT m_iUDP_ID;
	UINT m_iTCP_ID;

};


