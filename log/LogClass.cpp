#include "stdafx.h"
#include "LogClass.h"
#include <iostream>

using namespace std;
LogClass::LogClass()
{
	if (m_tOLogFile.is_open() == false){
		m_tOLogFile.open("../log.txt", ios_base::out | ios_base::app);
		long pos = m_tOLogFile.tellp();
		m_tOLogFile.seekp(pos, ios_base::end);
	}
}

LogClass::~LogClass(void)
{
	if (m_tOLogFile.is_open() == true)
	{
		m_tOLogFile.close();
	}
}

void LogClass::Log(const char* log)
{
	if (m_tOLogFile)
	{
		if(log)
			m_tOLogFile<<log<<endl;
		else 
			m_tOLogFile<<"nil log"<<endl;
	}
}

void LogClass::Warn(const char* warning)
{
	::MessageBoxA(0, warning, "¾¯¸æ", 0);
}