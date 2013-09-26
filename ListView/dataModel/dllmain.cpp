// dllmain.cpp : Defines the entry point for the DLL application.
#include "MainFrm.h"
#include "PostMessageToUIThread.h"

CAppModule   _Module;
static unsigned int s_uiThreadRef = 0;
static CMainFrame* s_wndMain = 0;

CMainFrame* MyMain()
{	
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));
	::DefWindowProc(NULL, 0, 0, 0L);
	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls
	hRes = _Module.Init(NULL, NULL);
	ATLASSERT(SUCCEEDED(hRes));
	CMessageLoop *theLoop = _Module.GetMessageLoop();
	if (!theLoop)
	{
		theLoop = new CMessageLoop();
		_Module.AddMessageLoop(theLoop);
	}
	CMainFrame* wndMain = new CMainFrame();
	if(wndMain->CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}
	theLoop->AddMessageFilter(wndMain);
	theLoop->AddIdleHandler(wndMain);
	wndMain->ShowWindow(SW_HIDE);
	return wndMain;
}

CMainFrame* GetUIThread()
{
	//一个隐藏的后台窗口, 创建的目的是得到操作UI线程事件队列的方法
	if (!s_wndMain)
		s_wndMain = MyMain();
	return s_wndMain;
}

void PostMessageToUIThread( void *userData, MainThreadCallbackFun ptrFun)
{
	if (CMainFrame* wndMain = GetUIThread())
	{
		WPARAM wParam = (WPARAM)userData;
		LPARAM	 lParam = (LPARAM)ptrFun;
		wndMain->PostMessage(WM_MY_MESSAGE, wParam, lParam);
	}
}

void InitUIThread()
{
	s_uiThreadRef++;
	GetUIThread();
}

void UnInitUIThread()
{
	s_uiThreadRef--;
	if (s_uiThreadRef == 0)
	{
		if (s_wndMain)
		{
			delete s_wndMain;
			s_wndMain = 0;
			_Module.RemoveMessageLoop();
			_Module.Term();
			::CoUninitialize();
		}
	}
}