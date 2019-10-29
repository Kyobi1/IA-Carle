#include "MyBotLogic.h"

#include "Globals.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"

#include <iostream>
#include <fstream>
#include "PathFinderAStar.h"
#include "Debug.h"

#include <chrono>


MyBotLogic::MyBotLogic()
{
	//Write Code Here
}

MyBotLogic::~MyBotLogic()
{
	//Write Code Here
}

void MyBotLogic::Configure(const SConfigData& _configData)
{
#ifdef BOT_LOGIC_DEBUG
	mLogger.Init(_configData.logpath, "MyBotLogic.log");
#endif
	Debug::log.Init(_configData.logpath, "Debug.log");

	BOT_LOGIC_LOG(mLogger, "Configure", true);

	//Write Code Here
}

void MyBotLogic::Init(const SInitData& _initData)
{
	using namespace std::chrono;

	BOT_LOGIC_LOG(mLogger, "Init", true);
	
	//Logger log;
	//log.Init("../Debug", "debug.txt");
	auto Tstart = high_resolution_clock::now();

	graph.init(_initData);
	//graph.debug(log);

	//mLogger.Log("id NPC : " + std::to_string(_initData.npcInfoArray[0].uid));

	NPCMother::getInstance().init(_initData, &graph);
	auto Tstop = high_resolution_clock::now();
	duration<double> duration = Tstop - Tstart;
	auto s = std::to_string(duration.count());
	Debug::Log(s);
	Debug::Log("______________________________________________________");
}

void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders)
{
	using namespace std::chrono;
	auto Tstart = high_resolution_clock::now();
	graph.update(_turnData);

	NPCMother& mother = NPCMother::getInstance();
	//BOT_LOGIC_LOG(mLogger, "GetTurnOrders", true);

	mother.debug(mLogger);
	mother.nextTurn();
	mother.giveOrders(_orders);
	auto Tstop = high_resolution_clock::now();
	duration<double> duration = Tstop - Tstart;
	auto s = std::to_string(duration.count());
	Debug::Log(s);
	//mLogger.Log("id NPC turn : " + std::to_string(_orders.front().npcUID));
}