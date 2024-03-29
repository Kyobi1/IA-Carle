cmake_minimum_required( VERSION 3.0 )
project( MyFirstAIBot C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Filters
include( Filters.cmake )


SET( AIBOT_BUILDFILES 
	
	CleanAll.bat
	CMakeLists.txt
	Filters.cmake
	GenSubmit.bat
	GenSubmit.py
	PlayLocalMatch.bat
	PlayLocalMatch.py
	rebuild_vis_sln_x64.bat
)

SET( AIBOT_HEADERS 

	#Framework
	AIBot.h
	BotLogicIF.h
	ConfigData.h
	Globals.h
	InitData.h
	Logger.h
	MyBotLogic.h	
	TurnData.h

	#Cl�ment
	Condition.h
	NPC.h
	NPCMother.h
	PathFinderAStar.h
	State.h
	StateMachine.h
	Transition.h

	#Victor
	Connection.h
	Graph.h
	HexCell.h
	Task.h

	#Judith
	PathFinder.h
	PriorityHeap.h
	NodeReccord.h
	Debug.h
	PoolService.h
	
	#DLL Stuff
	Interop_API.h	
	stdafx.h
	targetver.h
)

SET( AIBOT_SRC 

	#Framework
	AIBot.cpp
	Logger.cpp
	MyBotLogic.cpp
	
	#Cl�ment
	NPC.cpp
	NPCMother.cpp
	PathFinderAStar.cpp
	
	#Victor
	Connection.cpp
	Graph.cpp
	HexCell.cpp
	Task.cpp

	#Judith
	PathFinder.cpp
	Debug.cpp

	#DLL Stuff
	dllmain.cpp
	Interop_API.cpp
	stdafx.cpp
)

SET(jobfiles "${AIBOT_HEADERS};${AIBOT_SRC};${AIBOT_BUILDFILES}")
file(WRITE jobfiles.txt "${jobfiles}")

add_library(myfirstaibot SHARED ${AIBOT_SRC} ${AIBOT_HEADERS})

CREATE_VISUAL_STUDIO_FILTERS(AIBOT)

