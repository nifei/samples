#ifndef _XML_THREAD_H_
#define _XML_THREAD_H_

#include "stdafx.h"
#include "xl_lib/multithread/thread.h"
#include <vector>

typedef void (*funcThreadDataReadyCallback) (int row, int column);
class XmlParser;

class XmlThread : public xl::win32::multithread::thread
{
public:
	XmlThread(XmlParser *parser);
	~XmlThread();
protected:
    virtual xl::uint32  thread_proc(); // 1 ³É¹¦, 0 Ê§°Ü
private:
	XmlParser *m_parser;
};

#endif //_XML_THREAD_H_