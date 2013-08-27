#include "XmlThread.h"
#include "XmlParser.h"
#include <iostream>

XmlThread::XmlThread(XmlParser *parser)
:m_parser(parser)
{
}

XmlThread::~XmlThread()
{
}

xl::uint32 XmlThread::thread_proc()
{
	while(true)
	{
		std::cout<<"XmlThread is running"<<std::endl;
		::Sleep(10000);
	}
}