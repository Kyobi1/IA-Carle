#include "MyBotLogic.h"

#include "Globals.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"

#include <iostream>
#include <fstream>
#include "PathFinderAStar.h"
#include "Debug.h"

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
	Debug::log.Init(_configData.logpath, "Debug.log");
#endif

	BOT_LOGIC_LOG(mLogger, "Configure", true);

	//Write Code Here
}

void MyBotLogic::Init(const SInitData& _initData)
{
	BOT_LOGIC_LOG(mLogger, "Init", true);
	
	//Logger log;
	//log.Init("../Debug", "debug.txt");
	
	graph.init(_initData);
	//graph.debug(log);

	mLogger.Log("id NPC : " + std::to_string(_initData.npcInfoArray[0].uid));

	NPCMother::getInstance().init(_initData, &graph);
}

void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders)
{
	graph.update(_turnData);

	NPCMother& mother = NPCMother::getInstance();
	BOT_LOGIC_LOG(mLogger, "GetTurnOrders", true);

	mother.debug(mLogger);
	mother.nextTurn(_turnData.turnNb);
	mother.giveOrders(_orders);

	mLogger.Log("turn : " + std::to_string(_turnData.turnNb));
	mLogger.Log("NPC dir : " + std::to_string(_orders.front().direction));
}