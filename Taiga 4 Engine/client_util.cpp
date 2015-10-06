
#include "main.h"
#include "client.h"
#include "camera.h"
#include "game.h"
#include "util.h"

void cClient::sendPacket(sf::Packet data)
{
	dataQueue[dataQueueCounter].data = data;
	dataQueueCounter += 1;
}

void cClient::connect(string ip, short port)
{
	if (core.serverMode) { return; }

	disconnect();

	/*
	Connecting to 'localserver' means that we are connecting to the same instance
	of the game. At the same time, '127.0.0.1' or 'localhost' will mean other
	instance running on the same machine.
	*/
	bool localServer = false;
	if (ip == "localserver") { localServer = true; ip = "127.0.0.1"; }

	sf::IpAddress address(ip);
	sf::Time timeout(sf::milliseconds(5000));

	console << "[cClient::connect] Connecting to " << ip << ":" << port << "..." << "\n";
	client.socket.setBlocking(true);
	if (client.socket.connect(address, port, timeout) == sf::Socket::Done)
	{
		client.connected = true;
		core.localServer = localServer;
		console << "[cClient::connect] Server found!" << "\n";
	}
	else
	{
		console << "[cClient::connect] Can't reach the server!" << "\n";
	}
	client.socket.setBlocking(false);

	// Sending client info
	sf::Packet data;
	data << MSG_PLAYER_NAME << client.playerName;
	client.sendPacket(data);
	data.clear();
	data << MSG_INFO_CAMRES << camera.res.x << camera.res.y;
	client.sendPacket(data);
	data.clear();
}

void cClient::disconnect()
{
	if (core.serverMode) { return; }

	if (!core.localServer) { game.unitCounter = 0; }
	core.localServer = true;
	client.connected = false;
	client.socket.disconnect();
}

void cClient::sendEcho(string str)
{
	sf::Packet data;
	data << MSG_ECHO << str;
	sendPacket(data);
	data.clear();
}

void cClient::pingServer()
{
	sf::Packet data;
	data << MSG_PING;
	sendPacket(data);
	lastPingTime = timeGetTime();
	data.clear();
}

void cClient::confirmPacket(int id)
{
	sf::Packet data;
	data << MSG_CONFIRM << id;
	sendPacket(data);
	data.clear();
}

bool cClient::isPacketDuplicate(int id)
{
	for (int i = 0; i < (int)receivedPackets.size(); i++)
	{
		if (receivedPackets[i] == id) { return true; }
	}

	receivedPackets.push_back(id);
	// Check packet history size
	if (receivedPackets.size() > 11000)
	{
		receivedPackets.erase(receivedPackets.begin(), receivedPackets.begin() + 1000);
	}
	return false;
}

void cClient::addCold(float val) {
	setCold(statCold + val);
}

void cClient::setCold(float val) {
	statCold = val;
	if (statCold < value.getMinColdLocal()) { statCold = value.getMinColdLocal(); }
	else if (statCold > value.getMaxColdLocal()) { statCold = value.getMaxColdLocal(); }
}

void cClient::addHunger(float val) {
	setHunger(statHunger + val);
}

void cClient::setHunger(float val) {
	statHunger = val;
	if (statHunger < value.getMinHungerLocal()) { statHunger = value.getMinHungerLocal(); }
	else if (statHunger > value.getMaxHungerLocal()) { statHunger = value.getMaxHungerLocal(); }
}