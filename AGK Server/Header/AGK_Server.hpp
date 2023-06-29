// Includes
#include "AGK_Common.h"
#include "Server_Main.h"
#include "cHashedList.h"
#include <string>
// Namespace
using namespace AGK;


class cClient
{
public:



private:
	UINT ID;
	std::string m_IPAddress;
	std::string m_Name;

	// other client data
	// password
	// in game items etc etc

};

class cAGKServer
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
		UINT iMessageID = agk::GetUDPNetworkMessage(m_iUDP_ID);
		if (iMessageID > 0)
		{
			m_OutputLog.push_back("UDP Message Incoming");
			int iMessageType = agk::GetNetworkMessageInteger(iMessageID);
			switch (iMessageType)
			{
			case CONNECT_TO_SERVER:
			{
				m_OutputLog.push_back("Client Connected");
			}
			break;

			case 999:
			{
				m_OutputLog.push_back("999 Sent from client");
				m_OutputLog.push_back(agk::GetNetworkMessageFromIP(iMessageID));
				m_OutputLog.push_back(std::to_string(agk::GetNetworkMessageFromPort(iMessageID)));
			}
			break;

			}

			agk::DeleteNetworkMessage(iMessageID);
		}

	}

	//############################
	// TCP Server
	bool StartTCP(const char* host, int port)
	{
		m_iTCP_ID = agk::HostNetwork(host, "Host", port);
		if (agk::IsNetworkActive(m_iTCP_ID))
		{
			m_OutputLog.push_back("TCP Server Started");
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
		if (bool(agk::GetNetworkExists(m_iTCP_ID)))
		{
			// TODO!
			// notify clients etc etc


			agk::CloseNetwork(m_iTCP_ID);
		}
	}

	void HandleTCPConnections()
	{
		UINT iClientID = agk::GetNetworkFirstClient(m_iTCP_ID);
		while (iClientID)
		{
			if (iClientID == agk::GetNetworkMyClientID(m_iTCP_ID))
			{// Client Is Server
				agk::PrintC("Client Is Server: ");
				agk::Print(iClientID);
			}
			else
			{

				// Check for Client connection
				if (agk::GetNetworkClientUserData(m_iTCP_ID, iClientID, CLIENT_CONNECT_STATE) == 0)
				{
					// set client connection flag
					agk::SetNetworkClientUserData(m_iTCP_ID, iClientID, CLIENT_CONNECT_STATE, CLIENT_CONNECT);
					
					// Check username and password


					// TODO: Notify clients of connection
					
					 
					// Debug output
					std::string client_name = agk::GetNetworkClientName(m_iTCP_ID, iClientID);
					std::string client_ip = agk::GetNetworkClientIP(m_iTCP_ID, iClientID);
					m_OutputLog.push_back("Client Connected: Name:" + client_name + " - IP: " + client_ip + " ID: " + std::to_string(iClientID));
				}


				// Check for Client disconnection
				if (agk::GetNetworkClientDisconnected(m_iTCP_ID, iClientID) == 1)
				{
					// get client connection flag
					if (agk::GetNetworkClientUserData(m_iTCP_ID, iClientID, CLIENT_CONNECT_STATE) == CLIENT_CONNECT)
					{
						// set client connection flag for disconnect
						agk::SetNetworkClientUserData(m_iTCP_ID, iClientID, CLIENT_CONNECT_STATE, CLIENT_DISCONNECT);

						// delete the client (this will not be instant)
						agk::DeleteNetworkClient(m_iTCP_ID, iClientID);


						// TODO: Notify clients of disconnection


						// Debug output
						std::string client_name = agk::GetNetworkClientName(m_iTCP_ID, iClientID);
						std::string client_ip = agk::GetNetworkClientIP(m_iTCP_ID, iClientID);
						m_OutputLog.push_back("Client Disconnected: Name:" + client_name + " - IP: " + client_ip + " ID: " + std::to_string(iClientID));
					}
				}
			}

			iClientID = agk::GetNetworkNextClient(m_iTCP_ID);
		}
	}

	void HandleTCPMessages()
	{
		// Handle incoming messages
		UINT iIncomingMessageID = agk::GetNetworkMessage(m_iTCP_ID);
		if (iIncomingMessageID > 0)
		{
			// Get message sender info
			static UINT iClientID = agk::GetNetworkMessageFromClient(iIncomingMessageID);
			static char* sClientIP = agk::GetNetworkMessageFromIP(iIncomingMessageID);
			static int iClinentPort = agk::GetNetworkMessageFromPort(iIncomingMessageID);

			// first int should always  be the message type
			static int iIncomingMessageType = agk::GetNetworkMessageInteger(iIncomingMessageID);
			switch (iIncomingMessageType)
			{
			case CONNECT_TO_SERVER:
			{
				// user connected to the TCP server, lets do a handshake to verify the connection

				//TODO!
				// add to client list
				// notify other clients
				//m_OutputLog.push_back("User sent connection handshake");
			}
			break;

			case PING_SERVER:
			{
				// use this for initial testing

			}
			break;
			}
		}
	}

	void UpdateTCP()
	{
		if (m_iTCP_ID > 0 && agk::IsNetworkActive(m_iTCP_ID))
		{
			// print details of the network, including client number (this number will include the host)
			agk::Print("Network Active");
			std::string output = "Number of Clients: " + std::to_string(agk::GetNetworkNumClients(m_iTCP_ID));
			agk::Print(output.c_str());

			HandleTCPConnections();
			HandleTCPMessages();
		}
	}


	std::vector<std::string> m_OutputLog;

private:
	
	UINT m_iUDP_ID; 
	UINT m_iTCP_ID;


    cHashedList<cClient>* cClient;
};


