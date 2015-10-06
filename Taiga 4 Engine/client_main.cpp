
#include "main.h"
#include "client.h"
#include "math.h"

void clientReceiveMain()
{
	int threadId = 5;
	if (core.serverMode) { return; }
	console << "[CLIENT_RECEIVE] Starting the client receive thread" << " [ID: " << threadId << "]" << "\n";
	sf::Packet data;
	int retVal;
	string cmd, type;
	bool parsed = false;
	int id;

	while (!core.thread_shutdown[threadId])
	{
		if (client.connected && !core.serverMode)
		{
			client.socket.setBlocking(false);
			retVal = client.socket.receive(data);
			if (retVal == sf::Socket::Done)
			{
				data >> id;
				if (!client.isPacketDuplicate(id))
				{
					client.confirmPacket(id);
					parsed = client.msgBig(data);
					if (!parsed) { parsed = client.msgUnit(data); }
					if (!parsed) { parsed = client.msgOrder(data); }
					if (!parsed) { parsed = client.msgGame(data); }
				}

				data.clear();
			}
			else if (retVal == sf::Socket::Disconnected)
			{
				console << "[CLIENT_RECEIVE] Connection to server lost!" << "\n";
				client.disconnect();
			}
			else { Sleep(10); }
		}
		else { Sleep(10); }
		core.thread_antifreeze[threadId] = 0;
	}

	console << "[CLIENT_RECEIVE] Cleaning up\n";
}

void clientSendMain()
{
	int threadId = 6;
	if (core.serverMode) { return; }
	console << "[CLIENT_SEND] Starting the client send thread" << " [ID: " << threadId << "]" << "\n";

	while (!core.thread_shutdown[threadId])
	{
		if (client.connected && client.dataQueueCounter > 0)
		{
			//console << "[CLIENT_SEND] Packet sent!" << "\n";
			// Sending the data
			client.socket.send(client.dataQueue[0].data);
			// Moving the queue
			for (int i = 0; i < client.dataQueueCounter - 1; i++)
			{
				client.dataQueue[i] = client.dataQueue[i + 1];
			}
			client.dataQueueCounter -= 1;
		}
		else if (client.pingTimer > 500) {
			client.pingServer();
			client.pingTimer = 0;
		}
		else { Sleep(10); }
		core.thread_antifreeze[threadId] = 0;
	}

	console << "[CLIENT_SEND] Cleaning up\n";
}