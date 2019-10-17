#ifndef DEBUG_H
#define DEBUG_H
#include "Logger.h"
#include <string>
class Debug
{

public:
	static Logger log;
	static void Log(const std::string& _data) { log.Log(_data); };
	static void Log(const int& data) { log.Log(std::to_string(data)); };
};
#endif // !DEBUG_H
