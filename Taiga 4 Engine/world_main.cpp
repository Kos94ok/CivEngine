
#include "main.h"
#include "world.h"
#include "client.h"

vector<string> getBlueprintList()
{
	int filesFound = 0;
	vector<string> retArray;
	WIN32_FIND_DATAA dataStruct;
	HANDLE hFind;

	hFind = FindFirstFileA("Data//Blueprints//*", &dataStruct);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		if (strlen(dataStruct.cFileName) > 2) { retArray.push_back(dataStruct.cFileName); }
		while (FindNextFileA(hFind, &dataStruct))
		{
			if (strlen(dataStruct.cFileName) > 2) { retArray.push_back(dataStruct.cFileName); }
		}
	}
	return retArray;
}

void cWorld::analyzeBlueprints()
{
	blueprint.clear();
	int countCorrect = 0, countWrong = 0, value = -1;
	cBlueprint print;
	ifstream file;
	cBlueprintHeader header;
	vector<string> list = getBlueprintList();
	for (int i = 0; i < (int)list.size(); i++)
	{
		print.name = list[i];
		// Getting the header
		header = save.getHeaderFromFile("Data//Blueprints//" + list[i]);
		// Checking the header
		if (header.isValid && header.type > CHUNK_UNDEFINED && header.type <= CHUNK_MAXVALUE)
		{
			countCorrect += 1;
			print.type = header.type;
			blueprint.push_back(print);
		}
		else { countWrong += 1; }
	}
	console << "[MAIN] Blueprints analyzed: " << (int)list.size() << ". Correct: " << countCorrect << ", Wrong: " << countWrong << "\n";
}

void worldLoaderMain()
{
	int threadId = 7;
	console << "[WORLD_LOADER] Starting the world loader thread" << " [ID: " << threadId << "]" << "\n";
	sf::Packet data;
	bool isViable;
	while (!core.thread_shutdown[threadId])
	{
		mutex.worldMain.lock();
		if (!core.editorMode)
		{
			for (int y = 0; y < LIMIT_MAP; y++)
			{
				for (int i = 0; i < LIMIT_MAP; i++)
				{
					//if (world.map[i][y].type != CHUNK_UNDEFINED)
					//if (world.map[i][y].type == CHUNK_NORMAL)
					//{
						isViable = world.isChunkViable(vec2i(i, y));
						// Loading
						if (isViable && !world.isChunkLoaded(vec2i(i, y)))
						{
							// Server-side routine
							if (core.localServer || core.serverMode)
							{
								world.loadChunk(vec2i(i, y));
							}
							// Client-side routine
							else if (client.connected)
							{
								world.map[i][y].isLoaded = true;
								// Politely ask the server for chunk (i; y)
								data << MSG_REQUEST_CHUNKDATA << i << y;
								client.sendPacket(data);
								data.clear();
							}
 						}
						// Unloading
						else if (!isViable && world.isChunkLoaded(vec2i(i, y)))
						{
							if (core.localServer || core.serverMode) { world.saveChunk(vec2i(i, y)); }
							world.unloadChunk(vec2i(i, y));
						}
					//}
				}
			}
		}
		mutex.worldMain.unlock();
		Sleep(5);
		core.thread_antifreeze[threadId] = 0;
	}

	console << "[WORLD_LOADER] Cleaning up" << "\n";
}