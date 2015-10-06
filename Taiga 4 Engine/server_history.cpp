
#include "main.h"
#include "server.h"

void cServer::addToHistoryQueue(int targetPlayer, cPacketQueue data)
{
	mutex.serverHistory.lock();
	for (int i = 0; i < (int)historyQueue.size(); i++)
	{
		if (historyQueue[i].id == data.id) { mutex.serverHistory.unlock(); return; }
	}
	data.target = targetPlayer;
	historyQueueTimer.push_back(timeGetTime());
	historyQueue.push_back(data);
	mutex.serverHistory.unlock();
}

void cServer::removeFromHistoryQueue(int id)
{
	for (int i = 0; i < (int)historyQueue.size(); i++)
	{
		if (historyQueue[i].id == id)
		{
			mutex.serverHistory.lock();
			historyQueue.erase(historyQueue.begin() + i);
			historyQueueTimer.erase(historyQueueTimer.begin() + i);
			mutex.serverHistory.unlock();
			break;
		}
	}
}

void cServer::checkHistoryQueue()
{
	int timeLimit = 300;

	int myTime = timeGetTime();
	for (int i = 0; i < (int)historyQueue.size(); i++)
	{
		if (myTime - historyQueueTimer[i] > timeLimit && player[historyQueue[i].target].connected)
		{
			sendPacket(historyQueue[i].target, historyQueue[i].data, historyQueue[i].id);
			historyQueueTimer[i] = myTime;
		}
		else if (!player[historyQueue[i].target].connected)
		{
			removeFromHistoryQueue(historyQueue[i].id);
			i -= 1;
		}
	}
}