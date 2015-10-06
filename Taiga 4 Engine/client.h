
#pragma once
#include "precomp.h"
#include "util.h"

class cClient
{
public:
	int dataQueueCounter;
	cPacketQueue dataQueue[LIMIT_SERVER_PACKETQUEUE];

	int unit;
	string playerName;
	float statCold;
	float statHunger;
	
	int packet;
	bool connected;
	sf::TcpSocket socket;
	int ping;
	int pingTimer;
	int lastPingTime;
	int lastPongTime;
	int mousePosTimer;
	int moveOrderTimer;
	sf::Vector2f lastMousePos;
	sf::Vector2f lastMoveOrderPos;

	void addCold(float value);
	void setCold(float value);
	void addHunger(float value);
	void setHunger(float value);

	void sendEcho(string str);
	void sendPacket(sf::Packet data);
	void connect(string ip, short port);
	void disconnect();

	void confirmPacket(int id);
	bool msgBig(sf::Packet data);
	bool msgUnit(sf::Packet data);
	bool msgOrder(sf::Packet data);
	bool msgGame(sf::Packet data);
	void pingServer();

	vector<int> receivedPackets;
	bool isPacketDuplicate(int id);

	cClient() {
		unit = -1;
		ping = -1;
		packet = 0;
		connected = false;
		socket.setBlocking(false);
		pingTimer = 0;
		playerName = "Player";

		dataQueueCounter = 0;
	}
};

extern cClient client;

void clientReceiveMain();
void clientSendMain();