#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iosfwd>

class LogClass
{
public:
	LogClass();
	~LogClass();
	void Log(const char *log);
	void Warn(const char *log);
private:
	std::ofstream m_tOLogFile;
};