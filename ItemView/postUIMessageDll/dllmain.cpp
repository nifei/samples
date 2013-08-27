// dllmain.cpp : Defines the entry point for the DLL application.
#include "MainFrm.h"
#include "UIThreadDll.h"

CAppModule   _Module;

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
	static CMainFrame* _wndMain = 0;
	if (!_wndMain)
		_wndMain = MyMain();
	return _wndMain;
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

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		if (CMainFrame* wndMain = GetUIThread())
		{
			delete wndMain;
			wndMain = 0;
			_Module.RemoveMessageLoop();
			_Module.Term();
			::CoUninitialize();
		}
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}